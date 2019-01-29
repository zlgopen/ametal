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
 *    1. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_am845_core_std_sct_timer.c src_am845_core_std_sct_timer
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-07-15  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_sct_timer
 * \copydoc demo_am845_core_std_sct_timer.c
 */

/** [src_am845_core_std_sct_timer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_std_sct_timer_entry (void)
{
    AM_DBG_INFO("The SCT demo for std timer service\r\n");
    demo_std_timer_timing_entry(am_lpc84x_sct0_timing_inst_init(), 0);
}
/** [src_am845_core_std_sct_timer] */

/* end of file */
