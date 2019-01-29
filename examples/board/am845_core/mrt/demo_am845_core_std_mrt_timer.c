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
 * \brief 多频率定时器（MRT）例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 调试串口输出定时器的相关信息
 *   2. 定时周期到达(默认为 10Hz)，调试串口会输出 The timing frq is 10Hz;
 *
 * \note
 *    1. 由于 MRT 默认作为系统滴答使用，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_SOFTIMER_ENABLE、 AM_CFG_SYSTEM_TICK_ENABLE 以及
 *       AM_CFG_KEY_GPIO_ENABLE 定义为 0，不使用软件定时器、系统嘀嗒、板载独立按键。
 *
 * \par 源代码
 * \snippet demo_am845_core_std_mrt_timer.c src_am845_core_std_mrt_timer
 *
 * \internal
 * \par History
 * - 1.00 15-07-15  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_mrt_timer
 * \copydoc demo_am845_core_std_mrt_timer.c
 */

/** [src_am845_core_std_mrt_timer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc84x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_std_mrt_timer_entry (void)
{
    AM_DBG_INFO("The MRT demo for standard timing service\r\n");

    demo_std_timer_timing_entry(am_lpc84x_mrt_inst_init(), 0);

}
/** [src_am845_core_std_mrt_timer] */

/* end of file */
