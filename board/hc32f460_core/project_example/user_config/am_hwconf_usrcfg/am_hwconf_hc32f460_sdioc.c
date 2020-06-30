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
 * \brief HC32F460 SDIOC 用户配置文件
 * \sa am_hwconf_hc32f460_sdioc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-29  cds, first implementation
 * \endinternal
 */

#include "am_hc32f460_sdioc.h"
#include "hw/amhw_hc32f460_sdioc.h"
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_hc32f460_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_sdioc
 * \copydoc am_hwconf_hc32f460_sdioc.c
 * @{
 */

/** \brief SDIO 平台初始化函数 */
static void __hc32f460_sdioc1_plfm_init (void)
{

    am_gpio_pin_cfg(PIOC_8,  GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
    am_gpio_pin_cfg(PIOC_9,  GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
    am_gpio_pin_cfg(PIOC_10, GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
    am_gpio_pin_cfg(PIOC_11, GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
    am_gpio_pin_cfg(PIOC_12, GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
    am_gpio_pin_cfg(PIOD_2,  GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));

    am_clk_enable(CLK_SDIOC1);
}

/** \brief 解除SDIO 平台初始化函数 */
static void __hc32f460_sdioc1_plfm_deinit (void)
{
    am_clk_disable(CLK_SDIOC1);
}

/**
 * \brief SDIOC1 设备信息
 */
static const am_hc32f460_sdioc_devinfo_t __g_sdioc1_devinfo = {

    HC32F460_SDIOC1_BASE,               /**< \brief 寄存器块基址 */
    1,
    INUM_SDIOC1,                        /**< \brief 中断编号 */
    CLK_SDIOC1,                         /**< \brief 时钟ID值 */

    AMHW_HC32F460_SDIO_SPEED_MODE_LOW,  /**< \brief SDIO速率模式 */
    __hc32f460_sdioc1_plfm_init,        /**< \brief 平台初始化 */
    __hc32f460_sdioc1_plfm_deinit       /**< \brief 平台去初始化 */
};

am_hc32f460_sdioc_dev_t __g_sdioc1_dev;          /**< \brief  设备实例 */
static am_sdio_handle_t  __g_sdio_handle = NULL; /**< \brief  标准服务句柄 */

/** \brief SDIO 实例初始化，获得SDIO标准服务句柄 */
am_sdio_handle_t am_hc32f460_sdioc1_inst_init (void)
{
    if (NULL == __g_sdio_handle) {
        __g_sdio_handle = am_hc32f460_sdioc_init(&__g_sdioc1_dev, &__g_sdioc1_devinfo);
    }

    return __g_sdio_handle;
}

/** \brief SDIOC 实例解初始化 */
void am_hc32f460_sdioc1_inst_deinit (am_sdio_handle_t handle)
{
    am_hc32f460_sdioc_deinit(handle);
    __g_sdio_handle = NULL;
}




/** \brief SDIO 平台初始化函数 */
static void __hc32f460_sdioc2_plfm_init (void)
{

//    am_gpio_pin_cfg(PIOC_8,  GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
//    am_gpio_pin_cfg(PIOC_9,  GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
//    am_gpio_pin_cfg(PIOC_10, GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
//    am_gpio_pin_cfg(PIOC_11, GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
//    am_gpio_pin_cfg(PIOC_12, GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));
//    am_gpio_pin_cfg(PIOD_2,  GPIO_AFIO(AMHW_HC32F460_AFIO_SDIO));

    am_clk_enable(CLK_SDIOC2);

}

/** \brief 解除SDIO 平台初始化函数 */
static void __hc32f460_sdioc2_plfm_deinit (void)
{
    am_clk_disable(CLK_SDIOC2);
}

/**
 * \brief SDIOC2 设备信息
 */
static const am_hc32f460_sdioc_devinfo_t __g_sdioc2_devinfo = {

    HC32F460_SDIOC2_BASE,               /**< \brief 寄存器块基址 */
    2,
    INUM_SDIOC2,                        /**< \brief 中断编号 */
    CLK_SDIOC2,                         /**< \brief 时钟ID值 */

    AMHW_HC32F460_SDIO_SPEED_MODE_LOW,  /** \brief SDIO速率模式 */
    __hc32f460_sdioc2_plfm_init,        /**< \brief 平台初始化 */
    __hc32f460_sdioc2_plfm_deinit       /**< \brief 平台去初始化 */
};

am_hc32f460_sdioc_dev_t __g_sdioc2_dev;   /**< \brief  设备实例 */
static am_sdio_handle_t  __g_sdio2_handle = NULL; /**< \brief  标准服务句柄 */

/** \brief SDIO 实例初始化，获得SDIO标准服务句柄 */
am_sdio_handle_t am_hc32f460_sdioc2_inst_init (void)
{
    if (NULL == __g_sdio2_handle) {
        __g_sdio2_handle = am_hc32f460_sdioc_init(&__g_sdioc2_dev, &__g_sdioc2_devinfo);
    }

    return __g_sdio2_handle;
}

/** \brief SDIOC 实例解初始化 */
void am_hc32f460_sdioc2_inst_deinit (am_sdio_handle_t handle)
{
    am_hc32f460_sdioc_deinit(handle);
    __g_sdio2_handle = NULL;
}


/** @} */

/* end of file */
