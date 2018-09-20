/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X USART0 用户配置文件
 * \sa am_hwconf_lpc82x_usart0.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_usart.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_usart0
 * \copydoc am_hwconf_lpc82x_usart0.c
 * @{
 */

/**
 * \brief 基本输入频率设置（基本输入频率必须小于系统时钟频率且应为波特率的整数倍）
 *
 * 为了设置波特率为 115200，故设置串口基本输入频率为：
 * 11059200Hz(11059200 = 115200 * 96)
 * 串口基本输入频率设置为 11.059200MHz，可满足大多数波特率的设置 (9600,4800,115200)
 *
 * \note USART0/1/2 共用一个基本输入频率，不要随意改动
 */
#ifndef  __LPC82X_UASART_BASE_RATE
#define  __LPC82X_UASART_BASE_RATE  11059200
#endif

/**
 * \brief USART0 平台初始化
 */
am_local void __lpc82x_usart0_plfm_init (void)
{

    /* 设置串口基础时钟 */
    amhw_lpc82x_clk_usart_baseclkrate_set(__LPC82X_UASART_BASE_RATE);

    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_UART0);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_UART0);

    am_gpio_pin_cfg(PIO0_4, PIO_FUNC_U0_TXD);
    am_gpio_pin_cfg(PIO0_0, PIO_FUNC_U0_RXD);
}

/**
 * \brief USART0 平台解初始化
 */
am_local void __lpc82x_usart0_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_UART0);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_UART0);

    am_gpio_pin_cfg(PIO0_4, PIO0_4_GPIO);
    am_gpio_pin_cfg(PIO0_0, PIO0_0_GPIO);
}

/** \brief USART0 设备信息 */
am_local am_const am_lpc_usart_devinfo_t __g_lpc82x_usart0_devinfo = {
    LPC82X_USART0_BASE,             /* USART0 寄存器块基地址 */
    INUM_USART0,                    /* USART0 中断号 */
    CLK_UART0,                      /* USART0 时钟号 */
    __lpc82x_usart0_plfm_init,      /* 平台初始化函数 */
    __lpc82x_usart0_plfm_deinit,    /* 平台解初始化函数 */
    NULL                            /* 无 RS485 方向控制函数 */
};

/** \brief USART0 设备实例 */
am_local am_lpc_usart_dev_t __g_lpc82x_usart0_dev;

/**
 * \brief USART0 实例初始化
 */
am_uart_handle_t am_lpc82x_usart0_inst_init (void)
{
    return am_lpc_usart_init(&__g_lpc82x_usart0_dev,
                             &__g_lpc82x_usart0_devinfo);
}

/**
 * \brief USART0 实例解初始化
 */
void am_lpc82x_usart0_inst_deinit (am_uart_handle_t handle)
{
    am_lpc_usart_deinit(handle);
}

/**
 * @}
 */

/* end of file */
