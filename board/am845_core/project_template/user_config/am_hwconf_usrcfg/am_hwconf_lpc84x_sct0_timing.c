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
 * \brief LPC84X SCT0 TIMING 用户配置文件
 * \sa am_hwconf_lpc84x_sct0_timing.c
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-09  hgo, modified
 * - 1.00 15-07-07  oce, first implementation
 * \endinternal
 */

#include <am_lpc84x.h>
#include "ametal.h"
#include "am_lpc_sct_timing.h"
#include "hw/amhw_lpc84x_clk.h"

/**
 * \addtogroup am_if_src_hwconf_lpc84x_sct0_timing
 * \copydoc am_hwconf_lpc84x_sct0_timing.c
 * @{
 */

/**
 * \brief SCT0 TIMING 平台初始化
 */
am_local void __lpc84x_sct0_timing_plfm_init (void)
{
	amhw_lpc84x_sct_clk_sel_set( AMHW_LPC84X_CLK_SCT_MAIN_CLK, 2);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SCT);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SCT);
}

/**
 * \brief SCT0 TIMING 平台解初始化
 */
am_local void __lpc84x_sct0_timing_plfm_deinit (void)
{
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SCT);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_SCT);
}

/** \brief SCT TIMING 设备信息 */
am_local am_const am_lpc_sct_timing_devinfo_t __g_lpc84x_sct0_timing_devinfo = {
    LPC84X_SCT0_BASE,                    /* SCT0 寄存器块基地址 */
    INUM_SCT0,                           /* SCT0 中断号 */
    CLK_SCT,                             /* SCT0 时钟号 */
    AM_LPC_SCT_TIMING_2_16BIT,           /* SCT 用于 2 个 16 位定时器 */
    __lpc84x_sct0_timing_plfm_init,      /* 平台初始化函数 */
    __lpc84x_sct0_timing_plfm_deinit,    /* 平台解初始化函数 */
};

/** \brief SCT TIMING 设备实例 */
am_local am_lpc_sct_timing_dev_t __g_lpc84x_sct0_timing_dev;

/**
 * \brief SCT0 TIMING 实例初始化
 */
am_timer_handle_t am_lpc84x_sct0_timing_inst_init (void)
{
    return am_lpc_sct_timing_init(&__g_lpc84x_sct0_timing_dev,
                                  &__g_lpc84x_sct0_timing_devinfo);
}

/**
 * \brief SCT0 TIMING 实例解初始化
 */
void am_lpc84x_sct0_timing_inst_deinit (am_timer_handle_t handle)
{
    am_lpc_sct_timing_deinit((am_lpc_sct_timing_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
