/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X WWDT 用户配置文件
 * \sa am_hwconf_lpc82x_wwdt.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-07  sss, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_wwdt.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_wwdt
 * \copydoc am_hwconf_lpc82x_wwdt.c
 * @{
 */

/**
 * \brief WWDT 平台初始化
 */
am_local void __lpc82x_wwdt_plfm_init (void)
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_WWDT);

    /* 设置 WDT 时钟，0.6MHz，64 分频，时钟频率 9.375KHz */
    amhw_lpc82x_clk_wdtoscc_cfg(AMHW_LPC82X_CLK_WDTOSC_RATE_0_6MHZ, 64);

    /* 使能 WDTOSC */
    amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_WDT_OSC);
}

/**
 * \brief WWDT 平台解初始化
 */
am_local void __lpc82x_wwdt_plfm_deinit (void)
{
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_WWDT);
}

/** \brief WWDT 设备信息 */
am_local am_const am_lpc_wwdt_devinfo_t __g_lpc82x_wwdt_devinfo = {
    LPC82X_WWDT_BASE,          /* WWDT 寄存器基址 */
    CLK_WWDT,                  /* WWDT 时钟号 */
    __lpc82x_wwdt_plfm_init,   /* 平台初始化函数 */
    __lpc82x_wwdt_plfm_deinit, /* 平台解初始化函数 */
};

/** \brief WWDT 设备实例 */
am_local am_lpc_wwdt_dev_t __g_lpc82x_wwdt_dev;

/**
 * \brief WWDT 实例初始化
 */
am_wdt_handle_t am_lpc82x_wwdt_inst_init (void)
{
    return am_lpc_wwdt_init(&__g_lpc82x_wwdt_dev, &__g_lpc82x_wwdt_devinfo);
}

/**
 * \brief WWDT 实例解初始化
 */
void am_lpc82x_wwdt_inst_deinit (am_wdt_handle_t handle)
{
    am_lpc_wwdt_deinit((am_lpc_wwdt_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
