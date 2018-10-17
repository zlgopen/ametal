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
 * \brief 按键输入事件（实现了标准接口文件 am_input.h）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "am_input.h"
#include "am_event.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include <string.h>
#include "am_event_category_input.h"
#include "am_event_input_key.h"

/*******************************************************************************
  Local defines
*******************************************************************************/
#define  __KEEP_TIME_INVALID   -1   /**< \brief  无效计时值                   */

/*******************************************************************************
  Local type defines
*******************************************************************************/

/** \brief 按键数据 */
typedef struct am_event_input_key_data {
    int     key_code;              /**< \brief  按键类型码                    */
    int     key_state;             /**< \brief  按键状态, DOWN : 0, UP : 非0  */
    int     keep_time;             /**< \brief  按键保持时间                  */
} am_event_input_key_data_t;

/*******************************************************************************
  Global variables
*******************************************************************************/

static am_event_type_t                   __g_event_input_key;

static const am_event_input_key_info_t  *__gp_event_input_key_info = NULL;

/*******************************************************************************
  Local functions
*******************************************************************************/

/* 按键输入事件回调函数，分发键值 */
static void __event_input_key_handle (am_event_type_t *p_evt_type,
                                      void            *p_evt_data,
                                      void            *p_hdl_data)
{
    am_input_key_handler_t    *p_hdlr = (am_input_key_handler_t *)p_hdl_data;
    am_event_input_key_data_t *p_data = (am_event_input_key_data_t *)p_evt_data;

    p_hdlr->pfn_cb(p_hdlr->p_usr_data,
                   p_data->key_code,
                   p_data->key_state,
                   p_data->keep_time);
}

/******************************************************************************/

/* 键值的上报 */
static int __event_input_key_report (int key_code, int key_state, int keep_time)
{
    am_event_input_key_data_t  data;

    (void)memset(&data, 0, sizeof(data));

    data.key_code   = key_code;
    data.key_state  = key_state;
    data.keep_time  = keep_time;

    return am_event_raise(&__g_event_input_key, &data, 0);
}

/******************************************************************************/

/* 长按定时器回调函数 */
static void __key_long_press_softimer_cb (void *p_arg)
{
    const am_event_input_key_info_t *p_info       = __gp_event_input_key_info;
    am_event_input_key_long_press_t *p_long_press = (am_event_input_key_long_press_t *)p_arg;

    if (p_long_press->keep_time == 0) {

        p_long_press->keep_time = p_info->key_long_press_time_start_ms;

        /* 达到上报长按事件的启动时间，定时时间切换为周期性上报时间 */
        am_softimer_start(&p_long_press->timer,
                           p_info->key_long_press_time_period_ms);

    } else {

        /* 增加值前，避免数据溢出 */
        if ((p_long_press->keep_time + p_info->key_long_press_time_period_ms)
             > p_long_press->keep_time) {

            p_long_press->keep_time += p_info->key_long_press_time_period_ms;
        }
    }

    /* 上报按键(长按)事件，按键时间由 keep_time 传递 */
    __event_input_key_report(p_long_press->key_code,
                             AM_INPUT_KEY_STATE_PRESSED,
                             p_long_press->keep_time);
}

/******************************************************************************/

/* 为指定按键启动长按计时 */
static int __key_long_press_timing_start (int key_code)
{
    const am_event_input_key_info_t *p_info       = __gp_event_input_key_info;
    am_event_input_key_long_press_t *p_long_press = NULL;

    int i;

    if ((p_info == NULL) || (p_info->key_long_press_max_num == 0)) {
        return -AM_EINVAL;
    }

    for (i = 0; i < p_info->key_long_press_max_num; i++) {

        if (p_info->p_buf[i].keep_time == __KEEP_TIME_INVALID) {

            p_long_press = &p_info->p_buf[i];                /* 空闲缓存      */

        } else if (p_info->p_buf[i].key_code == key_code) {  /* 已经开始计时  */

            return AM_OK;
        }
    }

    if (p_long_press != NULL) {

        p_long_press->key_code  = key_code;
        p_long_press->keep_time = 0;

        am_softimer_start(&p_long_press->timer,
                          p_info->key_long_press_time_start_ms);

        return AM_OK;
    }

    return -AM_EAGAIN;                                      /* 暂无可用资源   */
}

/******************************************************************************/

/* 停止指定按键的长按计时 */
static int __key_long_press_timing_stop (int key_code)
{
    const am_event_input_key_info_t *p_info       = __gp_event_input_key_info;
    am_event_input_key_long_press_t *p_long_press = NULL;

    int i;

    if ((p_info == NULL) || (p_info->key_long_press_max_num == 0)) {
        return -AM_EINVAL;
    }

    for (i = 0; i < p_info->key_long_press_max_num; i++) {

        if ((p_info->p_buf[i].keep_time != __KEEP_TIME_INVALID) &&
            (p_info->p_buf[i].key_code  == key_code)) {

            p_long_press = &p_info->p_buf[i];               /* 空闲缓存       */

            am_softimer_stop(&p_long_press->timer);

            p_long_press->keep_time = __KEEP_TIME_INVALID;

            return AM_OK;
        }
    }

    return -AM_ENOENT;                                      /* 该按键不存在   */
}

/*******************************************************************************
  Public functions
*******************************************************************************/

void am_event_input_key_init (const am_event_input_key_info_t *p_info)
{
    int i;

    am_event_init(&__g_event_input_key);
    am_event_category_intput_event_register(&__g_event_input_key);

    if ((p_info                         != NULL) &&
        (p_info->key_long_press_max_num != 0) &&
        (p_info->p_buf                  != NULL)) {


        for (i = 0; i < p_info->key_long_press_max_num; i++) {

            p_info->p_buf[i].keep_time = __KEEP_TIME_INVALID;   /* 未计时     */

            am_softimer_init(&p_info->p_buf[i].timer,
                             __key_long_press_softimer_cb,
                             (void *)&p_info->p_buf[i]);

        }

        __gp_event_input_key_info = p_info;
    }
}

/******************************************************************************/
int am_input_key_handler_register (am_input_key_handler_t  *p_handler,
                                   am_input_key_cb_t        pfn_cb,
                                   void                    *p_arg)
{
    if ((NULL == p_handler) || (NULL == pfn_cb)) {
        return -AM_EINVAL;
    }

    p_handler->pfn_cb      = pfn_cb;
    p_handler->p_usr_data  = p_arg;

    am_event_handler_init(&p_handler->ev_handler,
                          __event_input_key_handle,
                          (void *)p_handler,          /* private date */
                          0);

    am_event_handler_register(&__g_event_input_key,
                              &p_handler->ev_handler);

    return AM_OK;
}

/******************************************************************************/
int am_input_key_handler_unregister (am_input_key_handler_t *p_handler)
{
    if (NULL == p_handler) {
        return -AM_EINVAL;
    }

    return am_event_handler_unregister(&__g_event_input_key,
                                       &p_handler->ev_handler);
}

/*******************************************************************************
  key event report
*******************************************************************************/
int am_event_input_key_pressed  (int key_code)
{
    /* 开始长按计时 */
    int ret = __key_long_press_timing_start(key_code);

    /* 若返回值小于0，则表明无法支持该按键的长按计时，keep_time设定为-1   */
    return __event_input_key_report(key_code,
                                    AM_INPUT_KEY_STATE_PRESSED,
                                    (ret < 0) ? -1 : 0);
}

/******************************************************************************/
int am_event_input_key_released (int key_code)
{
    /*
     * 停止长按计时
     *
     * 如使用 am_event_input_key_pressed_without_timing() 上报的按键按下事件，
     * 则该按键可能并未启动长按计时，内部处理应该考虑到这种情况。
     */
    __key_long_press_timing_stop(key_code);

    /* 上报按键释放事件 */
    return __event_input_key_report(key_code, AM_INPUT_KEY_STATE_RELEASED, 0);
}

/******************************************************************************/
int am_event_input_key_pressed_without_timing (int key_code, int keep_time)
{
    /* 简单上报事件即可 */
    return __event_input_key_report(key_code, AM_INPUT_KEY_STATE_PRESSED, keep_time);
}

/* end of file */
