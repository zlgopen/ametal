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
 * \brief SCT 定时例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED0 的状态以 3s 为周期在定时翻转和停止翻转两个状态间切换。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_lpc82x_std_sct_timer.c src_lpc82x_std_sct_timer
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-07-15  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc82x_std_sct_timer
 * \copydoc demo_lpc82x_std_sct_timer.c
 */

/** [src_lpc82x_std_sct_timer] */
#include "ametal.h"
#include "am_timer.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x_inst_init.h"

/**
 * \brief 定时器回调函数
 */
am_local void __src_timing_callback (void *p_arg)
{
    am_led_toggle(LED0);
}

/**
 * \brief 例程入口
 */
void demo_lpc82x_std_sct_timer_entry (void)
{
//    am_timer_info_t   sct_timing_info;
    am_timer_handle_t sct_timing_handle;

    sct_timing_handle = am_lpc82x_sct0_timing_inst_init();
    am_timer_info_get(sct_timing_handle);

    /* 设置回调函数 */
    am_timer_callback_set(sct_timing_handle, 0, __src_timing_callback, NULL);

    AM_FOREVER
    {

//        /* 设置定时中断频率为 20Hz */
//        am_timer_enable(sct_timing_handle,
//                        0,
//                        sct_timing_info.clk_frequency / 20);

        am_mdelay(3000);
        am_timer_disable(sct_timing_handle, 0);
        am_mdelay(3000);
    }
}
/** [src_lpc82x_std_sct_timer] */

/* end of file */
