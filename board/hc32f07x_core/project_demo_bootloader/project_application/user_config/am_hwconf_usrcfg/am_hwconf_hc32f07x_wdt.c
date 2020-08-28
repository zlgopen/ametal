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
 * \brief HC32 WDT 用户配置文件
 * \sa am_hwconf_hc32f07x_wdt.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10
 * \endinternal
 */

#include "am_hc32_wdt.h"
#include "am_hc32f07x.h"
#include "am_hc32_clk.h"
#include "hw/amhw_hc32_rcc.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f07x_wdt
 * \copydoc am_hwconf_hc32f07x_wdt.c
 * @{
 */

/** \brief WDT 平台初始化 */
static void __hc32f07x_wdt_plfm_init (void)
{
    am_clk_enable(CLK_WDT);
}

/** \brief 解除WDT 平台初始化 */
static void __hc32f07x_wdt_plfm_deinit (void)
{
    am_clk_disable(CLK_WDT);
}

/** \brief WDT 设备信息 */
static const am_hc32_wdt_devinfo_t  __g_wdt_devinfo = {
    HC32_WDT_BASE,          /**< \brief WDT寄存器基址 */
    CLK_WDT,                  /**< \brief 时钟ID */
    __hc32f07x_wdt_plfm_init,   /**< \brief WDT 平台初始化 */
    __hc32f07x_wdt_plfm_deinit, /**< \brief 解除WDT 平台初始化 */
};

/** \brief WDT设备实例 */
static am_hc32_wdt_dev_t __g_wdt_dev;

/**
 * \brief WDT 实例初始化，获得WDT标准服务句柄
 */
am_wdt_handle_t am_hc32_wdt_inst_init (void)
{
    return am_hc32_wdt_init(&__g_wdt_dev, &__g_wdt_devinfo);
}

/**
 * \brief WDT 实例解初始化
 */
void am_hc32_wdt_inst_deinit (am_wdt_handle_t handle)
{
    am_hc32_wdt_deinit(handle);
}

/**
 * @}
 */

/* end of file */
