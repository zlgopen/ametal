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
 * \brief HC32 LPUART 用户配置文件
 * \sa am_hc32l_hwconfig_lpuart.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */

#include "am_gpio.h"
#include "am_hc32_lpuart.h"
#include "hw/amhw_hc32_lpuart.h"
#include "am_clk.h"
#include "am_hc32.h"
#include "hw/amhw_hc32_gpio.h"
#include "hw/amhw_hc32_rcc.h"

/**
 * \addtogroup am_if_src_hwconf_hc32_lpuart
 * \copydoc am_hwconf_hc32_lpuart.c
 * @{
 */
/** \brief LPUART0平台初始化 */
static void __hc32_plfm_lpuart0_init (void)
{
    am_clk_enable(CLK_LPUART0);

    am_gpio_pin_cfg(PIOC_11, PIOC_11_LPUART0_RXD | PIOC_11_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOC_10, PIOC_10_LPUART0_TXD | PIOC_10_OUT_PP );
}

/** \brief 解除LPUART0平台初始化 */
static void __hc32_plfm_lpuart0_deinit (void)
{
    am_clk_disable(CLK_LPUART0);

    am_gpio_pin_cfg(PIOB_11, PIOB_11_GPIO | PIOB_11_INPUT_PU);
    am_gpio_pin_cfg(PIOB_12, PIOB_12_GPIO | PIOB_12_INPUT_PU);
}

/** \brief LPUART0设备信息 */
static const am_hc32_lpuart_devinfo_t __g_lpuart0_devinfo = {

    HC32_LPUART0_BASE,              /**< \brief LPUART0 */
    INUM_LPUART0,                   /**< \brief LPUART0的中断编号 */
    AMHW_HC32_LPUART_SCLK_SRC_PCLK, /**< \brief 通信传输时钟  */

    AMHW_HC32_LPUART_PARITY_NO |    /**< \brief 无极性 */
    AMHW_HC32_LPUART_STOP_1_0_BIT,  /**< \brief 1个停止位 */

    AM_FALSE,

    115200,                         /**< \brief 设置的波特率 */

    0,                              /**< \brief 无其他中断 */

    NULL,                           /**< \brief 使用RS485 */
    __hc32_plfm_lpuart0_init,       /**< \brief LPUART0的平台初始化 */
    __hc32_plfm_lpuart0_deinit,     /**< \brief LPUART0的平台去初始化 */
};

/**< \brief 定义LPUART0 设备 */
static am_hc32_lpuart_dev_t  __g_lpuart0_dev;

/** \brief LPUART0实例初始化，获得lpuart0标准服务句柄 */
am_uart_handle_t am_hc32_lpuart0_inst_init (void)
{
    return am_hc32_lpuart_init(&__g_lpuart0_dev, &__g_lpuart0_devinfo);
}

/** \brief LPUART0实例解初始化 */
void am_hc32_lpuart0_inst_deinit (am_uart_handle_t handle)
{
    am_hc32_lpuart_deinit((am_hc32_lpuart_dev_t *)handle);
}

/** \brief lpuart2平台初始化 */
static void __hc32_plfm_lpuart1_init (void)
{
    am_clk_enable(CLK_LPUART1);

    am_gpio_pin_cfg(PIOC_11, PIOC_11_LPUART1_RXD | PIOC_11_INPUT_FLOAT );
    am_gpio_pin_cfg(PIOC_10, PIOC_10_LPUART1_TXD | PIOC_10_OUT_PP);
}

/** \brief 解除LPUART1平台初始化 */
static void __hc32_plfm_lpuart1_deinit (void)
{
    am_clk_disable(CLK_LPUART1);

    am_gpio_pin_cfg(PIOC_11, PIOC_11_GPIO | PIOC_11_INPUT_PU);
    am_gpio_pin_cfg(PIOC_12, PIOC_12_GPIO | PIOC_12_INPUT_PU);
}

/** \brief LPUART1设备信息 */
static const am_hc32_lpuart_devinfo_t __g_lpuart1_devinfo = {

    HC32_LPUART1_BASE,              /**< \brief LPUART1 */
    INUM_LPUART1,                   /**< \brief LPUART1的中断编号 */
    AMHW_HC32_LPUART_SCLK_SRC_PCLK, /**< \brief 通信传输时钟  */

    AMHW_HC32_LPUART_PARITY_NO |    /**< \brief 无极性 */
    AMHW_HC32_LPUART_STOP_1_0_BIT,  /**< \brief 1个停止位 */

    AM_FALSE,

    115200,                         /**< \brief 设置的波特率 */

    0,                              /**< \brief 无其他中断 */

    NULL,                           /**< \brief 使用RS485 */
    __hc32_plfm_lpuart1_init,       /**< \brief LPUART1的平台初始化 */
    __hc32_plfm_lpuart1_deinit,     /**< \brief LPUART1的平台去初始化 */
};

/**< \brief 定义LPUART1 设备 */
static am_hc32_lpuart_dev_t  __g_lpuart1_dev;

/** \brief LPUART1实例初始化，获得lpuart1标准服务句柄 */
am_uart_handle_t am_hc32_lpuart1_inst_init (void)
{
    return am_hc32_lpuart_init(&__g_lpuart1_dev, &__g_lpuart1_devinfo);
}

/** \brief LPUART1实例解初始化 */
void am_hc32_lpuart1_inst_deinit (am_uart_handle_t handle)
{
    am_hc32_lpuart_deinit((am_hc32_lpuart_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
