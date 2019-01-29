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
 * \brief WKT 定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED0 以 1s 间隔闪烁。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_lpc_std_wkt_timer.c src_lpc_std_wkt_timer
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-02  sky, modified
 * - 1.00 15-09-22  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_std_wkt_timer
 * \copydoc demo_lpc_std_wkt_timer.c
 */

/** [src_lpc_std_wkt_timer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"

am_local am_timer_handle_t __g_wkt_handle; /**< \brief 定时器标准服务操作句柄类型变量 */

/**
 * \brief WKT 回调函数
 */
am_local void __wkt_callback (void *p_arg)
{
    uint32_t  ckin = (uint32_t)p_arg;
    /* LED0 翻转 */
    am_led_toggle(LED0);

    /**
     * 使能定时器 1Hz 中断，注意，最大值为：0xFFFFFFFF
     * 因为 WKT 定时器没有自动重载，所以需要再次开启
     */
    am_timer_enable(__g_wkt_handle, 0, ckin);
}

void demo_lpc_std_wkt_timer_entry (am_timer_handle_t timer_handle,
                                   uint8_t           chan,
                                   uint32_t          clkin)
{
    uint32_t               wkt_timer_count;

    /* 传入 handle */
    __g_wkt_handle = timer_handle;
  
    /* 使用通道 0(只有通道 0) */
    am_timer_callback_set(timer_handle,
                          0,
                          __wkt_callback, 
                          (void *)clkin);

    /* 使能定时器 1Hz 中断，注意，最大值为：0xFFFFFFFF */
    am_timer_enable(timer_handle, 0, clkin);

    AM_FOREVER {
        am_timer_count_get(timer_handle, 0, &wkt_timer_count);
        am_mdelay(200);
    }
}

/** [src_lpc_std_wkt_timer] */

/* end of file */
