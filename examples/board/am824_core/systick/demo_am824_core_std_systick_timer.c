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
 * \brief Systick 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口 以 10Hz 的频率打印。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 由于 Systick 默认作为系统延时使用，使用测试本 Demo 前需要将
 *       am_prj_config.h 中的 AM_CFG_DELAY_ENABLE 定义为 0，不使用系统延时。
 *
 * \par 源代码
 * \snippet demo_am824_std_systick_timer.c src_am824_std_systick_timer
 *
 * \internal
 * \par History
 * - 1.00 15-09-23  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_systick_timer
 * \copydoc demo_am824_std_systick_timer.c
 */

/** [src_am824_std_systick_timer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_std_systick_timer_entry (void)
{
    am_timer_handle_t systick_handle;

    AM_DBG_INFO("demo am824 std systick timer\r\n");

    systick_handle = am_arm_systick_inst_init();
    demo_std_timer_timing_entry(systick_handle, 0);
}
/** [src_am824_std_systick_timer] */

/* end of file */
