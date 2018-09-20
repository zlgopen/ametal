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
 * \brief LPC82X USART1 用户配置文件
 * \sa am_hwconf_lpc82x_usart1.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-03  bob, first implementation.
 * \endinternal
 */

#include "am_lpc82x.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"
#include "am_lpc_usart.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_usart1
 * \copydoc am_hwconf_lpc82x_usart1.c
 * @{
 */

/**
 * \brief 基本输入频率设置（基本输入频率必须小于系统时钟频率且应为波特率的整数倍）
 *
 * 为了设置波特率为115200,故设置串口基本输入频率为：
 * 11059200Hz(11059200 = 115200 * 96)。
 * 串口基本输入频率设置为11.059200MHz，可满足大多数波特率的设置(9600,4800,115200)
 * \note  USART0\1\2共用一个基本输入频率,不要随意改动。
 *
 */
#ifndef  __LPC82X_UASART_BASE_RATE

#define  __LPC82X_UASART_BASE_RATE  11059200

#endif

/** \brief USART1平台初始化 */
static void __lpc82x_plfm_usart1_init (void)
{
    /* 设置串口基础时钟 */
    amhw_lpc82x_clk_usart_baseclkrate_set(__LPC82X_UASART_BASE_RATE);

    /* 使能USART1时钟并复位 */
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_UART1);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_UART1);

    am_gpio_pin_cfg(PIO0_26, PIO_FUNC_U1_RXD);
    am_gpio_pin_cfg(PIO0_27, PIO_FUNC_U1_TXD);
}

/** \brief 解除USART1 平台初始化 */
static void __lpc82x_plfm_usart1_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_UART1);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_UART1);

    am_gpio_pin_cfg(PIO0_26, PIO0_26_GPIO);
    am_gpio_pin_cfg(PIO0_27, PIO0_27_GPIO);
}

/** \brief USART1 设备信息 */
static const am_lpc_usart_devinfo_t __g_usart1_devinfo = {
    LPC82X_USART1_BASE,                 /**< \brief USART1寄存器基址 */
    INUM_USART1,                        /**< \brief USART1的中断编号 */
    CLK_UART0,                          /**< \brief USART0的时钟编号 */
    __lpc82x_plfm_usart1_init,          /**< \brief USART1的平台初始化 */
    __lpc82x_plfm_usart1_deinit,        /**< \brief USART1的平台去初始化 */
    NULL                                /**< \brief USART1的平台无RS485管脚 */
};

static am_lpc_usart_dev_t  __g_usart1_dev;    /**< \brief 定义USART1 设备 */

/** \brief UART0 实例初始化，获得ADC标准服务句柄 */
am_uart_handle_t am_lpc82x_usart1_inst_init (void)
{
    return am_lpc_usart_init(&__g_usart1_dev, &__g_usart1_devinfo);
}

/** \brief USART1 实例解初始化 */
void am_lpc82x_usart1_inst_deinit (am_uart_handle_t handle)
{
    am_lpc_usart_deinit(handle);
}

/**
 * @}
 */

/* end of file */
