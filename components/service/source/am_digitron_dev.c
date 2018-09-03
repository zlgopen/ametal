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
 * \brief 通用数码管设备管理
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_digitron_disp.h"
#include "am_digitron_dev.h"
#include "am_int.h"

/*******************************************************************************
  Global variables
*******************************************************************************/

static am_digitron_dev_t *__gp_head;

/*******************************************************************************
  Local functions
*******************************************************************************/
static am_digitron_dev_t * __digitron_dev_find_with_id (int id)
{
    am_digitron_dev_t *p_cur   = __gp_head;

    int key = am_int_cpu_lock();

    while (p_cur != NULL) {

        if (p_cur->p_info->id == id) {
            break;
        }

        p_cur = p_cur->p_next;
    }

    am_int_cpu_unlock(key);

    return p_cur;
}

/******************************************************************************/
static int __digitron_dev_add (am_digitron_dev_t *p_dd)
{
    int key = am_int_cpu_lock();

    /* add the handler to event handler list */
    p_dd->p_next = __gp_head;
    __gp_head    = p_dd;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
static int __digitron_dev_del (am_digitron_dev_t *p_dd)
{
    int                 ret     = -AM_ENODEV;
    am_digitron_dev_t **pp_head = &__gp_head;
    am_digitron_dev_t  *p_prev  = AM_CONTAINER_OF(pp_head, am_digitron_dev_t, p_next);
    am_digitron_dev_t  *p_cur   = __gp_head;
     
    int key = am_int_cpu_lock();
  
    while (p_cur != NULL) {
        if (p_cur == p_dd) {

            p_prev->p_next = p_dd->p_next;
            p_dd->p_next   = NULL;
            ret            = AM_OK;
            break;
        }

        p_prev = p_cur;
        p_cur  = p_cur->p_next;
    }

    am_int_cpu_unlock(key);

    return ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/

int am_digitron_dev_lib_init (void)
{
    __gp_head = NULL;

    return AM_OK;
}

/******************************************************************************/
int am_digitron_dev_add (am_digitron_dev_t            *p_dd,
                         const am_digitron_devinfo_t  *p_info,
                         const am_digitron_disp_ops_t *p_ops,
                         void                         *p_cookie)
{
    if ((p_dd == NULL) || (p_ops == NULL) || (p_info == NULL)) {
        return -AM_EINVAL;
    }

    if (__digitron_dev_find_with_id(p_info->id) != NULL) {
        return -AM_EPERM;
    }

    /* try to delete it if exist */
    __digitron_dev_del(p_dd);

    p_dd->p_info   = p_info;
    p_dd->p_ops    = p_ops;
    p_dd->p_next   = NULL;
    p_dd->p_cookie = p_cookie;

    return __digitron_dev_add(p_dd);
}

/******************************************************************************/
int am_digitron_dev_del (am_digitron_dev_t *p_dd)
{
    if (p_dd == NULL) {
        return -AM_EINVAL;
    }

    return __digitron_dev_del(p_dd);
}

/*******************************************************************************
  The implement for standard interface (am_digitron_disp.h)
*******************************************************************************/

/******************************************************************************/
int am_digitron_disp_decode_set (int        id,
                                 uint16_t (*pfn_decode) (uint16_t ch))
{
    am_digitron_dev_t *p_dd = __digitron_dev_find_with_id(id);

    if (p_dd == NULL) {
        return -AM_ENODEV;
    }

    if (p_dd->p_ops->pfn_decode_set) {
        return p_dd->p_ops->pfn_decode_set(p_dd->p_cookie, pfn_decode);
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
int am_digitron_disp_blink_set (int id, int index, am_bool_t blink)
{
    am_digitron_dev_t *p_dd = __digitron_dev_find_with_id(id);

    if (p_dd == NULL) {
        return -AM_ENODEV;
    }

    if (p_dd->p_ops->pfn_blink_set) {
        return p_dd->p_ops->pfn_blink_set(p_dd->p_cookie, index, blink);
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
int am_digitron_disp_at (int id, int index, uint16_t seg)
{
    am_digitron_dev_t *p_dd = __digitron_dev_find_with_id(id);

    if (p_dd == NULL) {
        return -AM_ENODEV;
    }

    if (p_dd->p_ops->pfn_disp_at) {
        return p_dd->p_ops->pfn_disp_at(p_dd->p_cookie, index, seg);
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
int am_digitron_disp_char_at (int id, int index, const char ch)
{
    am_digitron_dev_t *p_dd = __digitron_dev_find_with_id(id);

    if (p_dd == NULL) {
        return -AM_ENODEV;
    }

    if (p_dd->p_ops->pfn_disp_char_at) {
        return p_dd->p_ops->pfn_disp_char_at(p_dd->p_cookie, index, ch);
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
int am_digitron_disp_str (int id, int index, int len, const char *p_str)
{
    am_digitron_dev_t *p_dd = __digitron_dev_find_with_id(id);

    if (p_dd == NULL) {
        return -AM_ENODEV;
    }

    if (p_dd->p_ops->pfn_disp_str) {
        return p_dd->p_ops->pfn_disp_str(p_dd->p_cookie, index, len, p_str);
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
int am_digitron_disp_clr (int id)
{
    am_digitron_dev_t *p_dd = __digitron_dev_find_with_id(id);

    if (p_dd == NULL) {
        return -AM_ENODEV;
    }

    if (p_dd->p_ops->pfn_clr) {
        return p_dd->p_ops->pfn_clr(p_dd->p_cookie);
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
int am_digitron_disp_enable (int id)
{
    am_digitron_dev_t *p_dd = __digitron_dev_find_with_id(id);

    if (p_dd == NULL) {
        return -AM_ENODEV;
    }

    if (p_dd->p_ops->pfn_enable) {
        return p_dd->p_ops->pfn_enable(p_dd->p_cookie);
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
int am_digitron_disp_disable (int id)
{
    am_digitron_dev_t *p_dd = __digitron_dev_find_with_id(id);

    if (p_dd == NULL) {
        return -AM_ENODEV;
    }

    if (p_dd->p_ops->pfn_disable) {
        return p_dd->p_ops->pfn_disable(p_dd->p_cookie);
    }

    return -AM_ENOTSUP;
}

/* end of file */
