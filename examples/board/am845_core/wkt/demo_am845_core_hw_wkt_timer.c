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
 * \brief WKT 定时器例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 以 1s 间隔闪烁。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_wkt_timer.c src_am845_core_hw_wkt_timer
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-02  sky, modified
 * - 1.00 15-07-13  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_wkt_timer
 * \copydoc demo_am845_core_hw_wkt_timer.c
 */

/** [src_am845_core_hw_wkt_timer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_wkt_timer_entry (void)
{
    am_kprintf("demo am845 hw wkt timer!\r\n");

    /* 初始化 WKT AHB 时钟 */
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_WKT);

    /* 复位 WKT */
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_WKT);

    /* 使能低功耗振荡器 */
    amhw_lpc82x_pmu_lposcen_enable(LPC84X_PMU);

    /* 使用低功耗时钟 */
    amhw_lpc_wkt_clksel_cfg(LPC84X_WKT, AMHW_LPC_WKT_LOW_POWER_CLOCK);

    demo_lpc_hw_wkt_timer(LPC84X_WKT, 1000);
}
/** [src_am845_core_hw_wkt_timer] */

/* end of file */
