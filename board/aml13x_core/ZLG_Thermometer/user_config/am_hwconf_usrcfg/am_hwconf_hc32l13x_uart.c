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
 * \brief HC32 UART 用户配置文件
 * \sa am_hc32l_hwconfig_uart.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-19  zp, first implementation
 * \endinternal
 */

#include "am_gpio.h"
#include "am_hc32_uart.h"
#include "hw/amhw_hc32_uart.h"
#include "am_clk.h"
#include "am_hc32.h"
#include "hw/amhw_hc32_gpio.h"
#include "hw/amhw_hc32_rcc.h"

/**
 * \addtogroup am_if_src_hwconf_hc32_uart
 * \copydoc am_hwconf_hc32_uart.c
 * @{
 */
/** \brief 串口0平台初始化 */
static void __hc32_plfm_uart0_init (void)
{
    am_clk_enable(CLK_UART0);
    am_gpio_pin_cfg(PIOA_9, PIOA_9_UART0_TXD | PIOA_9_OUT_PP);
    am_gpio_pin_cfg(PIOA_10, PIOA_10_UART0_RXD | PIOA_10_INPUT_FLOAT);

//    am_gpio_pin_cfg(PIOB_8,  PIOB_8_UART0_TXD  | PIOB_8_OUT_PP );
//    am_gpio_pin_cfg(PIOB_9, PIOB_9_UART0_RXD | PIOB_9_INPUT_FLOAT);
}

/** \brief 解除串口0平台初始化 */
static void __hc32_plfm_uart0_deinit (void)
{
    am_clk_disable(CLK_UART0);

	am_gpio_pin_cfg(PIOA_9, PIOA_9_GPIO | PIOA_9_INPUT_PU);
    am_gpio_pin_cfg(PIOA_10, PIOA_10_GPIO | PIOA_10_INPUT_PU);

//    am_gpio_pin_cfg(PIOB_8, PIOB_8_GPIO | PIOB_8_INPUT_PU);
//    am_gpio_pin_cfg(PIOB_9, PIOB_9_GPIO | PIOB_9_INPUT_PU);
}

/** \brief 串口0设备信息 */
static const am_hc32_uart_devinfo_t __g_uart0_devinfo = {

    HC32_UART0_BASE,                /**< \brief 串口0 */
    INUM_UART0_2,                   /**< \brief 串口0的中断编号 */
    CLK_UART0,                      /**< \brief 串口时钟ID */

    AMHW_HC32_UART_PARITY_NO |      /**< \brief 无极性 */
    AMHW_HC32_UART_STOP_1_0_BIT,    /**< \brief 1个停止位 */

    AM_FALSE,                       /**< \brief 不使用异步半双工（单线）模式 */

    115200,                         /**< \brief 设置的波特率 */

    0,                              /**< \brief 无其他中断 */

    NULL,                           /**< \brief 使用RS485 */
    __hc32_plfm_uart0_init,         /**< \brief UART0的平台初始化 */
    __hc32_plfm_uart0_deinit,       /**< \brief UART0的平台去初始化 */
};

/**< \brief 定义串口0 设备 */
static am_hc32_uart_dev_t  __g_uart0_dev;

/** \brief UART0实例初始化，获得uart0标准服务句柄 */
am_uart_handle_t am_hc32_uart0_inst_init (void)
{
    return am_hc32_uart_init(&__g_uart0_dev, &__g_uart0_devinfo);
}

/** \brief UART0实例解初始化 */
void am_hc32_uart0_inst_deinit (am_uart_handle_t handle)
{
    am_hc32_uart_deinit((am_hc32_uart_dev_t *)handle);
}

/** \brief uart1平台初始化 */
static void __hc32_plfm_uart1_init (void)
{
    am_clk_enable(CLK_UART1);

    am_gpio_pin_cfg(PIOD_0, PIOD_0_UART1_TXD | PIOD_0_OUT_PP );
    am_gpio_pin_cfg(PIOD_1, PIOD_1_UART1_RXD | PIOD_1_INPUT_FLOAT);
}

/** \brief 解除串口1平台初始化 */
static void __hc32_plfm_uart1_deinit (void)
{
    am_clk_disable(CLK_UART1);

    am_gpio_pin_cfg(PIOD_0, PIOD_0_GPIO | PIOD_0_INPUT_PU);
    am_gpio_pin_cfg(PIOD_1, PIOD_1_GPIO | PIOD_1_INPUT_PU);
}

/** \brief 串口1设备信息 */
static const am_hc32_uart_devinfo_t __g_uart1_devinfo = {

    HC32_UART1_BASE,                /**< \brief 串口1 */
    INUM_UART1_3,                   /**< \brief 串口1的中断编号 */
    CLK_UART1,                      /**< \brief 串口时钟ID */

    AMHW_HC32_UART_PARITY_NO |      /**< \brief 无极性 */
    AMHW_HC32_UART_STOP_1_0_BIT,    /**< \brief 1个停止位 */

    AM_FALSE,                       /**< \brief 不使用异步半双工（单线）模式 */

    115200,                         /**< \brief 设置的波特率 */

    0,                              /**< \brief 无其他中断 */

    NULL,                           /**< \brief 使用RS485 */
    __hc32_plfm_uart1_init,         /**< \brief UART1的平台初始化 */
    __hc32_plfm_uart1_deinit,       /**< \brief UART1的平台去初始化 */
};

/**< \brief 定义串口1 设备 */
static am_hc32_uart_dev_t  __g_uart1_dev;

/** \brief UART1实例初始化，获得uart1标准服务句柄 */
am_uart_handle_t am_hc32_uart1_inst_init (void)
{
    return am_hc32_uart_init(&__g_uart1_dev, &__g_uart1_devinfo);
}

/** \brief UART1实例解初始化 */
void am_hc32_uart1_inst_deinit (am_uart_handle_t handle)
{
    am_hc32_uart_deinit((am_hc32_uart_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
