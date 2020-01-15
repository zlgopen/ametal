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
 * \brief MM32L073 WWDT 用户配置文件
 * \sa am_hwconf_mm32l073_iwdg.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-07-07  sss, first implementation.
 * \endinternal
 */

#include "am_mm32l073.h"
#include "hw/amhw_mm32l073_rcc.h"
#include "am_mm32_iwdg.h"

/**
 * \addtogroup am_if_src_hwconf_mm32l073_iwdg
 * \copydoc am_hwconf_mm32l073_iwdg.c
 * @{
 */

/** \brief WWDT 平台初始化 */
static void __mm32_iwdg_plfm_init (void)
{

    /* 检查复位是否由看门狗造成的 */
    if (amhw_mm32l073_rcc_reset_flag() & AMHW_MM32L073_RCC_AHB_IWDGRSTF) {
        amhw_mm32l073_rcc_reset_flag_clear();
    }

    amhw_mm32l073_rcc_lsi_enable();

    while (amhw_mm32l073_rcc_lsirdy_read() == AM_FALSE);
}

/** \brief 解除WWDT 平台初始化 */
static void __mm32_iwdg_plfm_deinit (void)
{

}

/** \brief WWDT 设备信息 */
static const am_mm32_iwdg_devinfo_t  __g_iwdg_devinfo = {
    MM32L073_IWDG_BASE,       /**< \brief WWDT 寄存器基址 */
    __mm32_iwdg_plfm_init,   /**< \brief WWDT 平台初始化 */
    __mm32_iwdg_plfm_deinit, /**< \brief 解除WWDT 平台初始化 */
};

/** \brief WWDT 设备实例 */
static am_mm32_iwdg_dev_t __g_iwdg_dev;

/**
 * \brief WWDT 实例初始化，获得WDT标准服务句柄
 */
am_wdt_handle_t am_mm32l073_iwdg_inst_init (void)
{
    return am_mm32_iwdg_init(&__g_iwdg_dev, &__g_iwdg_devinfo);
}

/**
 * \brief WWDT 实例解初始化
 */
void am_mm32l073_iwdg_inst_deinit (am_wdt_handle_t handle)
{
    am_mm32_iwdg_deinit(handle);
}

/**
 * @}
 */

/* end of file */
