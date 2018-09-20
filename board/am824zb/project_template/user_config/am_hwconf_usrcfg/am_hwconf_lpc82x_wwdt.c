/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
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
 * - 1.00 15-07-07  sss, first implementation.
 * \endinternal
 */

#include "am_lpc82x.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"
#include "am_lpc_wwdt.h"

/**
 * \addtogroup am_if_src_hwconf_wwdt
 * \copydoc am_hwconf_lpc82x_wwdt.c
 * @{
 */

/** \brief WWDT 平台初始化 */
static void __lpc82x_wwdt_plfm_init (void)
{
    /* 使能WWDT时钟 */
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_WWDT);

    /* 设置WDT时钟，0.6MHz，64分频，时钟频率9.375KHz */
    amhw_lpc82x_clk_wdtoscc_cfg(AMHW_LPC82X_CLK_WDTOSC_RATE_0_6MHZ, 64);

    /* 使能WDTOSC */
    amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_WDT_OSC);
}

/** \brief 解除WWDT 平台初始化 */
static void __lpc82x_wwdt_plfm_deinit (void)
{
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_WWDT);
}

/** \brief WWDT 设备信息 */
static const am_lpc_wwdt_devinfo_t  __g_wwdt_devinfo = {
    LPC82X_WWDT_BASE,          /**< \brief WWDT寄存器基址 */
    CLK_WWDT,                  /**< \brief WWDT时钟ID */
    __lpc82x_wwdt_plfm_init,   /**< \brief WWDT 平台初始化 */
    __lpc82x_wwdt_plfm_deinit, /**< \brief 解除WWDT 平台初始化 */
};

/** \brief WWDT设备实例 */
static am_lpc_wwdt_dev_t __g_wwdt_dev;

/**
 * \brief  WWDT 实例初始化，获得WDT标准服务句柄
 */
am_wdt_handle_t am_lpc82x_wwdt_inst_init (void)
{
    return am_lpc_wwdt_init(&__g_wwdt_dev, &__g_wwdt_devinfo);
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
