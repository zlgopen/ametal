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
 * \brief MicroPort-RS485 用户配置文件
 * \sa am_hwconf_microport_rs485.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-16  pea, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_usart.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"

/**
 * \addtogroup am_if_src_hwconf_microport_rs485
 * \copydoc am_hwconf_microport_rs485.c
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
#define  __LPC82X_UASART_BASE_RATE    11059200
#endif

/** \brief MicroPort-RS485 方向控制引脚 */
#define  __MICROPORT_RS485_DIR_PIN    PIO0_1

/**
 * \brief MicroPort-RS485 方向控制函数
 *
 * \param[in] is_txmode AM_TRUE: 发送模式， AM_FALSE: 接收模式
 */
void __microport_rs485_dir (am_bool_t is_txmode)
{
    am_gpio_set(__MICROPORT_RS485_DIR_PIN, (int)is_txmode);
}

/**
 * \brief MicroPort-RS485 平台初始化
 */
am_local void __microport_rs485_plfm_init (void)
{
    amhw_lpc82x_clk_usart_baseclkrate_set(__LPC82X_UASART_BASE_RATE);

    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_UART0);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_UART0);

    am_gpio_pin_cfg(PIO0_4, PIO_FUNC_U0_TXD);
    am_gpio_pin_cfg(PIO0_0, PIO_FUNC_U0_RXD);

    am_gpio_pin_cfg(__MICROPORT_RS485_DIR_PIN, AM_GPIO_OUTPUT_INIT_LOW);
}

/**
 * \brief MicroPort-RS485 平台解初始化
 */
am_local void __microport_rs485_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_UART0);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_UART0);

    am_gpio_pin_cfg(PIO0_4, PIO0_4_GPIO);
    am_gpio_pin_cfg(PIO0_0, PIO0_0_GPIO);

    am_gpio_pin_cfg(__MICROPORT_RS485_DIR_PIN, AM_GPIO_INPUT);
}

/** \brief MicroPort-RS485 设备信息 */
am_local am_const am_lpc_usart_devinfo_t __g_microport_rs485_devinfo = {
    LPC82X_USART0_BASE,               /* USART0 寄存器基址 */
    INUM_USART0,                      /* USART0 的中断号 */
    CLK_UART0,                        /* USART0 的时钟号 */
    __microport_rs485_plfm_init,      /* 平台初始化函数 */
    __microport_rs485_plfm_deinit,    /* 平台解初始化函数 */
    __microport_rs485_dir             /* RS485 方向切换函数 */
};

/** \brief MicroPort-RS485 设备实例 */
am_local am_lpc_usart_dev_t __g_microport_rs485_dev;

/**
 * \brief MicroPort-RS485 实例初始化
 */
am_uart_handle_t am_microport_rs485_inst_init (void)
{
    am_uart_handle_t handle = NULL;

    handle = am_lpc_usart_init(&__g_microport_rs485_dev,
                               &__g_microport_rs485_devinfo);

    if (NULL != handle) {
        am_uart_ioctl(handle, AM_UART_RS485_SET, (void *)AM_TRUE);
    }

    return handle;
}

/**
 * @}
 */

/* end of file */
