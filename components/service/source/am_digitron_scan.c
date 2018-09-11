/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief 通用动态扫描类数码管
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "string.h"
#include "am_int.h"
#include "am_digitron_scan.h"

/*******************************************************************************
  Local macro defines
*******************************************************************************/

#define __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_cookie) \
    am_digitron_scan_dev_t *p_dev = (am_digitron_scan_dev_t *)p_cookie

/*******************************************************************************
  buffer access (according to the buffer type)
*******************************************************************************/

typedef struct __buf_access {
    void (*pfn_set)(void *, int, int);
    int  (*pfn_get)(void *, int);
} __buf_access_t;

static void __buf_uint8_set (uint8_t *p_buf, int i, int data)
{
    p_buf[i] = data;
}

/******************************************************************************/
static int __buf_uint8_get (uint8_t *p_buf, int i)
{
    return p_buf[i];
}

/******************************************************************************/
static void __buf_uint16_set (uint16_t *p_buf, int i, int data)
{
    p_buf[i] = data;
}

/******************************************************************************/
static int __buf_uint16_get (uint16_t *p_buf, int i)
{
    return p_buf[i];
}

/******************************************************************************/
static const __buf_access_t __g_buf_uint8_access = {
    (void (*)(void *, int, int))__buf_uint8_set,
    (int  (*)(void *, int))__buf_uint8_get,
};

/******************************************************************************/
static const __buf_access_t __g_buf_uint16_access = {
    (void (*)(void *, int, int))__buf_uint16_set,
    (int  (*)(void *, int))__buf_uint16_get,
};

/*******************************************************************************
  Local Functions
*******************************************************************************/

static void __seg_disp_buf_set (am_digitron_scan_dev_t *p_dev, int i, int data)
{
    ((const __buf_access_t *)(p_dev->pfn_buf_access))->\
            pfn_set(p_dev->p_info->p_disp_buf, i, data);
}

/******************************************************************************/
static int  __seg_disp_buf_get (am_digitron_scan_dev_t *p_dev, int i)
{
    return ((const __buf_access_t *)(p_dev->pfn_buf_access))->\
            pfn_get(p_dev->p_info->p_disp_buf, i);
}

/******************************************************************************/
static void __seg_scan_buf_set (am_digitron_scan_dev_t *p_dev, int i, int data)
{
    ((const __buf_access_t *)(p_dev->pfn_buf_access))->\
            pfn_set(p_dev->p_info->p_scan_buf, i, data);
}

/******************************************************************************/
static void __scan_seg_send (am_digitron_scan_dev_t *p_dev)
{
    am_digitron_scan_ops_separate_t *p_ops = \
            (am_digitron_scan_ops_separate_t *)p_dev->p_ops;

    am_digitron_seg_send(p_ops->p_send_seg,
                         p_dev->p_info->p_scan_buf,
                         am_digitron_num_segment_get(p_dev->p_baseinfo),
                         p_dev->num_scan);
}

/******************************************************************************/
static void __scan_com_send (am_digitron_scan_dev_t *p_dev)
{
    am_digitron_scan_ops_separate_t *p_ops = \
            (am_digitron_scan_ops_separate_t *)p_dev->p_ops;

    am_digitron_com_sel(p_ops->p_sel_com, p_dev->scan_idx);
}

/******************************************************************************/
static void __scan_data_send (am_digitron_scan_dev_t *p_dev)
{
    am_digitron_scan_ops_unite_t *p_ops = \
            (am_digitron_scan_ops_unite_t *)p_dev->p_ops;

    am_digitron_data_send(p_ops->p_send_data,
                          p_dev->p_info->p_disp_buf,
                          am_digitron_num_segment_get(p_dev->p_baseinfo),
                          p_dev->num_scan,
                          p_dev->scan_idx);
}

/******************************************************************************/
static void __scan_black_proc (am_digitron_scan_dev_t *p_dev)
{
    if (p_dev->pfn_black_cb) {
        p_dev->pfn_black_cb(p_dev->p_arg, p_dev->scan_idx);
    }
}

/******************************************************************************/
static void __digitron_dynamic_scan_timer_cb (void *p_arg)
{
    __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_arg);

    am_bool_t  turn_on  = AM_FALSE;
    int        i;
    int        index;
    uint16_t   code;
    int        num_cols;
    int        num_rows;
    int        scan_mode;

    if (p_dev == NULL) {
        return;
    }

    num_cols  = am_digitron_num_col_get(p_dev->p_baseinfo);
    num_rows  = am_digitron_num_row_get(p_dev->p_baseinfo);
    scan_mode = am_digitron_scan_mode_get(p_dev->p_baseinfo);

    /* no digitron for blinking */
    if (p_dev->blink_flags == 0) {

        p_dev->blink_cnt = 0;
        turn_on          = AM_TRUE;

    } else {

        p_dev->blink_cnt += p_dev->scan_interval;

        if (p_dev->blink_cnt >= p_dev->p_info->blink_on_time +
                                p_dev->p_info->blink_off_time) {
            p_dev->blink_cnt = 0;
        }

        if (p_dev->blink_cnt >= p_dev->p_info->blink_on_time) {
            turn_on = AM_FALSE;          /* 闪烁熄灭阶段 */
        } else {
            turn_on = AM_TRUE;           /* 闪烁点亮阶段 */
        }
    }

    /* 需要消影  */
    if (p_dev->is_separate) {

        code = am_digitron_seg_active_low_get(p_dev->p_baseinfo) ?
                0xFFFF :
                0x0000;

        for (i = 0; i < p_dev->num_scan; i++) {
            __seg_scan_buf_set(p_dev, i, code);
        }
        __scan_seg_send(p_dev);
    }

    /* 设置扫描段码 */
    for (i = 0; i < p_dev->num_scan; i++) {

        if (scan_mode == AM_DIGITRON_SCAN_MODE_ROW) {
            index = i + p_dev->scan_idx * num_cols;
        } else {
            index = i * num_cols + p_dev->scan_idx;
        }

        /* 不是闪烁位 或者 处于闪烁位的点亮阶段 */
        if ((!(p_dev->blink_flags & AM_BIT(index))) || (turn_on == AM_TRUE)) {
            code = __seg_disp_buf_get(p_dev, index);
        } else {
            code = am_digitron_seg_active_low_get(p_dev->p_baseinfo) ?
                    0xFFFF :
                    0x0000;
        }

        __seg_scan_buf_set(p_dev, i, code);
    }

    /* 执行扫描，发送新的位码和段码 */
    if (p_dev->is_separate) {

        __scan_com_send(p_dev);
        __scan_black_proc(p_dev);
        __scan_seg_send(p_dev);

    } else {

        __scan_data_send(p_dev);
        __scan_black_proc(p_dev);
    }

    if (scan_mode == AM_DIGITRON_SCAN_MODE_ROW) {
        p_dev->scan_idx = (p_dev->scan_idx + 1) % num_rows;
    } else {
        p_dev->scan_idx = (p_dev->scan_idx + 1) % num_cols;
    }
}

/******************************************************************************/
/* 显示段码设置 */
static void __digitron_disp_buf_set (am_digitron_scan_dev_t *p_dev,
                                     int                     index,
                                     uint16_t                data)
{
    if (am_digitron_seg_active_low_get(p_dev->p_baseinfo)) {
        data ^= 0xFFFFu;
    }

    __seg_disp_buf_set(p_dev, index, data);
}

/******************************************************************************/
/* 显示段码叠加（或） */
static void __digitron_disp_buf_xor (am_digitron_scan_dev_t *p_dev,
                                     int                     index,
                                     uint16_t                data)
{

    uint16_t temp = __seg_disp_buf_get(p_dev, index);

    if (am_digitron_seg_active_low_get(p_dev->p_baseinfo)) {
        temp &= ~data;
    } else {
        temp |= data;
    }

    __seg_disp_buf_set(p_dev, index, temp);
}

/*******************************************************************************
  Standard digitron driver functions
*******************************************************************************/

static int __digitron_decode_set (void      *p_cookie,
                                  uint16_t (*pfn_decode) (uint16_t ch))
{
    __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_cookie);

    if ((p_dev == NULL) || (pfn_decode == NULL)) {
        return -AM_EINVAL;
    }

    p_dev->pfn_decode = pfn_decode;

    return AM_OK;
}

/******************************************************************************/
static int __digitron_blink_set (void *p_cookie, int index, am_bool_t blink)
{
    __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_cookie);

    if ((p_dev == NULL) || (index >= p_dev->num_digitron)) {
        return -AM_EINVAL;
    }

    AM_BIT_MODIFY(p_dev->blink_flags, index, blink);

    return AM_OK;
}

/******************************************************************************/
static int __digitron_disp_at (void *p_cookie, int index, uint16_t seg)
{
    __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_cookie);

    if ((p_dev == NULL) || (index >= p_dev->num_digitron)) {
        return -AM_EINVAL;
    }

    __digitron_disp_buf_set(p_dev, index, seg);

    return AM_OK;
}

/******************************************************************************/
static int __digitron_disp_char_at (void *p_cookie, int index, const char ch)
{
    __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_cookie);

    uint16_t seg = 0x00;

    if ((p_dev == NULL) || (index >= p_dev->num_digitron)) {
        return -AM_EINVAL;
    }

    if (p_dev->pfn_decode) {
        seg = p_dev->pfn_decode(ch);
    }

    if ('.' == ch) {

        __digitron_disp_buf_xor(p_dev, index, seg);

    } else {

        __digitron_disp_buf_set(p_dev, index, seg);
    }

    return AM_OK;
}

/******************************************************************************/
static int __digitron_disp_str (void       *p_cookie, 
                                int         index, 
                                int         len, 
                                const char *p_str)
{
	  uint16_t last_ch = 0, ch;
    int      idx     = index - 1;
    uint8_t  str_len = 0;
	
    __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_cookie);

    if ((p_dev == NULL)                ||
        (index >= p_dev->num_digitron) ||
        (p_str == NULL)) {

        return -AM_EINVAL;
    }

    str_len = strlen(p_str);

    if (len > str_len) {
        len = str_len;               /* 最大长度不会超过数码管长度  */
    }

    if ((index + len) > p_dev->num_digitron) {
        len = p_dev->num_digitron - index;
    }

    while ((len) && ((ch = *p_str++) != '\0')) {

        if (('.' == ch) && ('.' != last_ch)) {

            /* 小数点显示在起始位置，要占用位置，否则，不单独占用位置  */
            if (idx < 0) {
                len--;
                ++idx;
            }
            __digitron_disp_char_at(p_dev, idx, ch);

        } else {
            __digitron_disp_char_at(p_dev, ++idx, ch);
            len--;
        }
        last_ch = ch;
    }

    return AM_OK;
}

/******************************************************************************/
static int __digitron_clr (void *p_cookie)
{
    __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_cookie);

    int i;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < (p_dev->num_digitron); i++) {
        __digitron_disp_buf_set(p_dev, i, 0x00);
    }

    return AM_OK;
}

/******************************************************************************/
static int __digitron_enable (void *p_cookie)
{
    __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_cookie);

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    am_softimer_start(&p_dev->timer, p_dev->scan_interval);

    return AM_OK;
}

/******************************************************************************/
static int __digitron_disable (void *p_cookie)
{
    __DIGITRON_DYNAMIC_SCAN_DEV_DECL(p_dev, p_cookie);

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    am_softimer_stop(&p_dev->timer);

    return AM_OK;
}

/*******************************************************************************
  Global variables
*******************************************************************************/

static const am_digitron_disp_ops_t __g_digitron_dev_ops = {
    __digitron_decode_set,
    __digitron_blink_set,
    __digitron_disp_at,
    __digitron_disp_char_at,
    __digitron_disp_str,
    __digitron_clr,
    __digitron_enable,
    __digitron_disable,
};

/*******************************************************************************
  Public functions
*******************************************************************************/

int am_digitron_scan_init (am_digitron_scan_dev_t           *p_dev,
                           const am_digitron_scan_devinfo_t *p_info,
                           const am_digitron_base_info_t    *p_baseinfo,
                           am_bool_t                         is_separate,
                           void                             *p_ops)
{
    if ((p_dev == NULL) || (p_info == NULL) || (p_ops == NULL)) {
        return -AM_EINVAL;
    }

    p_dev->p_baseinfo    = p_baseinfo;
    p_dev->p_info        = p_info;
    p_dev->pfn_decode    = NULL;
    p_dev->blink_flags   = 0x00;
    p_dev->blink_cnt     = 0;

    p_dev->num_digitron  = am_digitron_num_col_get(p_baseinfo) *
                           am_digitron_num_row_get(p_baseinfo);

    p_dev->scan_idx      = 0;
    p_dev->is_separate   = is_separate;
    p_dev->p_ops         = p_ops;
    p_dev->pfn_black_cb  = NULL;
    p_dev->p_arg         = NULL;

    if (am_digitron_num_segment_get(p_baseinfo) <= 8) {
        p_dev->pfn_buf_access = &__g_buf_uint8_access;
    } else {
        p_dev->pfn_buf_access = &__g_buf_uint16_access;
    }

    if (am_digitron_scan_mode_get(p_baseinfo) == AM_DIGITRON_SCAN_MODE_ROW) {

        /* 一次扫描一行，一行的数码管个数与列数相等 */
        p_dev->num_scan = am_digitron_num_col_get(p_baseinfo);

    } else {

        /* 一次扫描一列，一列的数码管个数与行数相等 */
        p_dev->num_scan = am_digitron_num_row_get(p_baseinfo);
    }

    p_dev->scan_interval = 1000 / p_info->scan_freq / (p_dev->num_digitron / p_dev->num_scan);

    am_softimer_init(&p_dev->timer, __digitron_dynamic_scan_timer_cb, p_dev);
    am_softimer_start(&p_dev->timer, p_dev->scan_interval);

    /* 添加标准的数码管设备  */
    return am_digitron_dev_add(&p_dev->isa,
                               &p_info->devinfo,
                               &__g_digitron_dev_ops,
                               p_dev);
}

/******************************************************************************/
int am_digitron_scan_cb_set (am_digitron_scan_dev_t *p_dev,
                             am_digitron_scan_cb_t   pfn_cb,
                             void                   *p_arg)
{
    int key;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    key = am_int_cpu_lock();

    p_dev->pfn_black_cb = pfn_cb;
    p_dev->p_arg        = p_arg;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
int am_digitron_scan_deinit (am_digitron_scan_dev_t *p_dev)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    am_softimer_stop(&p_dev->timer);

    /* 删除标准的数码管设备  */
    return am_digitron_dev_del(&p_dev->isa);
}

/* end of file */
