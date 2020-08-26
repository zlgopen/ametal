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
 * \brief ZSN700 WDT 用户配置文件
 * \sa am_hwconf_zsn700_wdt.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10
 * \endinternal
 */

#include "am_zsn700_wdt.h"
#include "am_zsn700.h"
#include "am_zsn700_clk.h"
#include "hw/amhw_zsn700_rcc.h"

/**
 * \addtogroup am_if_src_hwconf_zsn700_wdt
 * \copydoc am_hwconf_zsn700_wdt.c
 * @{
 */

/** \brief WDT 平台初始化 */
static void __zsn700_wdt_plfm_init (void)
{
    am_clk_enable(CLK_WDT);
}

/** \brief 解除WDT 平台初始化 */
static void __zsn700_wdt_plfm_deinit (void)
{
    am_clk_disable(CLK_WDT);
}

/** \brief WDT 设备信息 */
static const am_zsn700_wdt_devinfo_t  __g_wdt_devinfo = {
    ZSN700_WDT_BASE,          /**< \brief WDT寄存器基址 */
    CLK_WDT,                  /**< \brief 时钟ID */
    __zsn700_wdt_plfm_init,   /**< \brief WDT 平台初始化 */
    __zsn700_wdt_plfm_deinit, /**< \brief 解除WDT 平台初始化 */
};

/** \brief WDT设备实例 */
static am_zsn700_wdt_dev_t __g_wdt_dev;

/**
 * \brief WDT 实例初始化，获得WDT标准服务句柄
 */
am_wdt_handle_t am_zsn700_wdt_inst_init (void)
{
    return am_zsn700_wdt_init(&__g_wdt_dev, &__g_wdt_devinfo);
}

/**
 * \brief WDT 实例解初始化
 */
void am_zsn700_wdt_inst_deinit (am_wdt_handle_t handle)
{
    am_zsn700_wdt_deinit(handle);
}

/**
 * @}
 */

/* end of file */
