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
 * \brief HC32F460 WDT 用户配置文件
 * \sa am_hwconf_hc32f460_wdt.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-12
 * \endinternal
 */

#include "am_hc32f460_wdt.h"
#include "am_hc32f460.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_wdt
 * \copydoc am_hwconf_hc32f460_wdt.c
 * @{
 */

/** \brief WDT 平台初始化 */
static void __hc32f460_wdt_plfm_init (void)
{

}

/** \brief 解除WDT 平台初始化 */
static void __hc32f460_wdt_plfm_deinit (void)
{

}

/** \brief WDT 设备信息 */
static const am_hc32f460_wdt_devinfo_t  __g_wdt_devinfo = {
    HC32F460_WDT_BASE,          /**< \brief WDT寄存器基址 */
    CLK_PCLK3,                  /**< \brief 时钟ID */
    INUM_WDT,                   /**< \brief 中断ID */

    /** \brief 刷新错误/溢出后产生的事件类型（中断/复位） */
    AM_HC32F460_WDT_TRIGGER_EVENT_REQUEST_RESET,

    /** \brief WDT在sleep模式下计数禁止/许可 */
    AM_HC32F460_WDT_SLEEPOFF,

    /** \brief 刷新允许区域 */
    AM_HC32F460_WDT_REFRESH_RANGE_100,
    __hc32f460_wdt_plfm_init,   /**< \brief WDT 平台初始化 */
    __hc32f460_wdt_plfm_deinit, /**< \brief 解除WDT 平台初始化 */
};

/** \brief WDT设备实例 */
am_hc32f460_wdt_dev_t __g_wdt_dev;

/**
 * \brief WDT 实例初始化，获得WDT标准服务句柄
 */
am_wdt_handle_t am_hc32f460_wdt_inst_init (void)
{
    return am_hc32f460_wdt_init(&__g_wdt_dev, &__g_wdt_devinfo);
}

/**
 * \brief WDT 实例解初始化
 */
void am_hc32f460_wdt_inst_deinit (am_wdt_handle_t handle)
{
    am_hc32f460_wdt_deinit(handle);
}

/**
 * @}
 */

/* end of file */
