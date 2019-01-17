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
 * \brief LPC84X USART2 用户配置文件
 * \sa am_hwconf_lpc84x_usart2.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */

#include <am_lpc84x.h>
#include "ametal.h"
#include "am_lpc_usart.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"

/**
 * \addtogroup am_if_src_hwconf_lpc84x_usart2
 * \copydoc am_hwconf_lpc84x_usart2.c
 * @{
 */

/**
 * \brief 基本输入频率设置（基本输入频率必须小于系统时钟频率且应为波特率的整数倍）
 *
 * 为了设置波特率为 115200，故设置串口基本输入频率为：
 * 11059200Hz(11059200 = 115200 * 96)
 * 串口基本输入频率设置为 11.059200MHz，可满足大多数波特率的设置 (9600,4800,115200)
 *
 * \note USART2/1/2 共用一个基本输入频率，不要随意改动
 */
#ifndef  __LPC84X_FRG0_BASE_RATE
#define  __LPC84X_FRG0_BASE_RATE  11059200
#endif

/**
 * \brief USART2 平台初始化
 */
am_local void __lpc84x_usart4_plfm_init (void)
{

	amhw_lpc84x_clk_uart4_clk_sel_set(AMHW_LPC84X_CLK_DEVICE_FRG0CLK);

    /* 设置串口基础时钟 */
    amhw_lpc84x_clk_frg0_baseclkrate_set(__LPC84X_FRG0_BASE_RATE);

    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_UART4);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_UART4);

    am_gpio_pin_cfg(PIO0_1, PIO_FUNC_U4_TXD);
    am_gpio_pin_cfg(PIO0_23,  PIO_FUNC_U4_RXD);
}

/**
 * \brief USART2 平台解初始化
 */
am_local void __lpc84x_usart4_plfm_deinit (void)
{
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_UART4);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_UART4);

    am_gpio_pin_cfg(PIO0_27, PIO0_27_GPIO);
    am_gpio_pin_cfg(PIO0_26, PIO0_26_GPIO);
}

/** \brief USART2 设备信息 */
am_local am_const am_lpc_usart_devinfo_t __g_lpc84x_usart4_devinfo = {
    LPC84X_USART4_BASE,             /* USART3 寄存器块基地址 */
	INUM_PIN_INT7,                  /* USART3 中断号 */
	CLK_UART4,                      /* USART3 时钟号 */
    __lpc84x_usart4_plfm_init,      /* 平台初始化函数 */
    __lpc84x_usart4_plfm_deinit,    /* 平台解初始化函数 */
    NULL                            /* 无 RS485 方向控制函数 */
};

/** \brief USART2 设备实例 */
am_local am_lpc_usart_dev_t __g_lpc84x_usart4_dev;

/**
 * \brief USART2 实例初始化
 */
am_uart_handle_t am_lpc84x_usart4_inst_init (void)
{
    return am_lpc_usart_init(&__g_lpc84x_usart4_dev,
                             &__g_lpc84x_usart4_devinfo);
}

/**
 * \brief USART2 实例解初始化
 */
void am_lpc84x_usart4_inst_deinit (am_uart_handle_t handle)
{
    am_lpc_usart_deinit(handle);
}

/**
 * @}
 */

/* end of file */
