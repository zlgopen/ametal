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
 * \brief HC32F460 SWDT 用户配置文件
 * \sa am_hwconf_hc32f460_swdt.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-12
 * \endinternal
 */

#include "am_hc32f460.h"
#include "hc32f460_icg.h"
#include "am_hc32f460_swdt.h"
/**
 * \addtogroup am_if_src_hwconf_hc32f460_swdt
 * \copydoc am_hwconf_hc32f460_swdt.c
 * @{
 */


/** \brief SWDT 平台初始化 */
static void __hc32f460_swdt_plfm_init (void)
{

}

/** \brief 解除SWDT 平台初始化 */
static void __hc32f460_swdt_plfm_deinit (void)
{

}

/** \brief SWDT 设备信息 */
static const am_hc32f460_swdt_devinfo_t  __g_swdt_devinfo = {
    HC32F460_SWDT_BASE,          /**< \brief SWDT寄存器基址 */
    CLK_SWDTCLK,                 /**< \brief 时钟ID */
    INUM_SWDT,                   /**< \brief 中断ID */
    HC32F460_ICG0_SWDT_ITS,      /**< \brief 触发事件类型（中断/复位） */
    __hc32f460_swdt_plfm_init,   /**< \brief SWDT 平台初始化 */
    __hc32f460_swdt_plfm_deinit, /**< \brief 解除SWDT 平台初始化 */
};

/** \brief SWDT设备实例 */
am_hc32f460_swdt_dev_t __g_swdt_dev;

/**
 * \brief SWDT 实例初始化，获得WDT标准服务句柄
 */
am_wdt_handle_t am_hc32f460_swdt_inst_init (void)
{
    return am_hc32f460_swdt_init(&__g_swdt_dev, &__g_swdt_devinfo);
}

/**
 * \brief SWDT 实例解初始化
 */
void am_hc32f460_swdt_inst_deinit (am_wdt_handle_t handle)
{
    am_hc32f460_swdt_deinit(handle);
}

/**
 * @}
 */

/* end of file */
