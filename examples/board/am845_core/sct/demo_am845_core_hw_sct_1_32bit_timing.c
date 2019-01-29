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
 * \brief SCT 32 位定时例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 以 10Hz 的频率闪烁。
 *
 * \note
 *    1. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器；
 *    2. 本例程使用了 SCT 事件 0，用于产生周期性中断，使用匹配通道 0，事件发生计数器
 *       清零。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_sct_1_32bit_timing.c src_am845_core_hw_sct_1_32bit_timing
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-08  hgo, modified
 * - 1.00 15-07-13  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_sct_1_32bit_timing
 * \copydoc demo_am845_core_hw_sct_1_32bit_timing.c
 */

/** [src_am845_core_hw_sct_1_32bit_timing] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_sct_1_32bit_timing_entry (void)
{
    uint32_t frq = 0;
  
    AM_DBG_INFO("demo am845_core hw sct 1_32bit timing\r\n");
    /* 平台初始化 */
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SCT);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SCT);

    AM_DBG_INFO("The demo for SCT timing:\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");

    frq = amhw_lpc84x_clk_system_clkrate_get();
  
    demo_lpc_hw_sct_1_32bit_timing_entry(LPC84X_SCT0, INUM_SCT0, frq);
}
/** [src_am845_core_hw_sct_1_32bit_timing] */

/* end of file */
