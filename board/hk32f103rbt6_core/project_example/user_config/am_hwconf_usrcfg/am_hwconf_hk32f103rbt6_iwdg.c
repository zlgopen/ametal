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
 * \brief HK32F103RBT6 IWDG 用户配置文件
 * \sa am_hwconf_hk32f103rbt6_iwdg.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-24  zcb, first implementation
 * \endinternal
 */

#include "am_hk32f103rbt6.h"
#include "am_hk32f103rbt6_iwdg.h"
#include "amhw_hk32f103rbt6_rcc.h"

/**
 * \addtogroup am_if_src_hwconf_hk32f103rbt6_iwdg
 * \copydoc am_hwconf_hk32f103rbt6_iwdg.c
 * @{
 */

/** \brief IDWG 平台初始化 */
static void __hk32f103rbt6_iwdg_plfm_init (void)
{

    /* 检查复位是否由看门狗造成的 */
    if (amhw_hk32f103rbt6_rcc_reset_flag() & AMHW_HK32F103RBT6_RCC_CSR_IWDGRSTF) {
        amhw_hk32f103rbt6_rcc_reset_flag_clear();
    }

    amhw_hk32f103rbt6_rcc_lsi_enable();

    while (amhw_hk32f103rbt6_rcc_lsirdy_read() == AM_FALSE);
}

/** \brief 解除 IWDG 平台初始化 */
static void __hk32f103rbt6_iwdg_plfm_deinit (void)
{
}

/** \brief IWDG 设备信息 */
static const am_hk32f103rbt6_iwdg_devinfo_t __g_iwdg_devinfo = {
    HK32F103RBT6_IWDG_BASE,          /**< \brief IWDG 寄存器基址 */
    __hk32f103rbt6_iwdg_plfm_init,    /**< \brief IWDG 平台初始化 */
    __hk32f103rbt6_iwdg_plfm_deinit,  /**< \brief 解除IWDG 平台初始化 */
};

/** \brief IWDG设备实例 */
static am_hk32f103rbt6_iwdg_dev_t __g_iwdg_dev;

/**
 * \brief IWDG 实例初始化，获得IWDG标准服务句柄
 */
am_wdt_handle_t am_hk32f103rbt6_iwdg_inst_init (void)
{
    return am_hk32f103rbt6_iwdg_init(&__g_iwdg_dev, &__g_iwdg_devinfo);
}

/**
 * \brief IWDG 实例解初始化
 */
void am_hk32f103rbt6_iwdg_inst_deinit (am_wdt_handle_t handle)
{
    am_hk32f103rbt6_iwdg_deinit(handle);
}

/**
 * @}
 */

/* end of file */
