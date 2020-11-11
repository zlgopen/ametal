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
 * \brief 低功耗模式（深度睡眠）低功耗串口唤醒例程，通过驱动层接口实现
 *
 * - 实现现象
 *   1.串口发送数据提示进入深度睡眠模式。
 *   2.PC端通过串口发送数据，MCU收到数据唤醒，退出中断后继续进入深度睡眠模式。
 *
 * \note
 *   1.将 PIOB_11 引脚连接 PC 串口的 TXD，PIOB_12 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zsl42x_lpuart_wake_up.c src_zsl42x_lpuart_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 20-07-15  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_lpuart_wake_up
 * \copydoc demo_zsl42x_lpuart_wake_up.c
 */

/** [src_zsl42x_lpuart_wake_up] */
#include "ametal.h"
#include "am_board.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "zsl42x_pin.h"
#include "am_zsl42x_inst_init.h"
#include "demo_am42x_core_entries.h"
#include "am_zsl42x_lpmode.h"

/* 低功耗配置 */
static void lowerpower_cfg (void)
{
    int i = 0;

    amhw_zsl42x_rcc_set_start(0x5A5A);
    amhw_zsl42x_rcc_set_start(0xA5A5);
    amhw_zsl42x_rcc_swdio_gpio(); 

    /* 配置没有用到的引脚 */
    for (i = PIOA_0; i < PIN_NUM; i++) {

        am_gpio_pin_cfg(i, AM_GPIO_INPUT | AM_GPIO_PULLDOWN);
    }

    /* 低功耗模式初始化 */
    am_zsl42x_lpmode_init();
}

/* 低功耗串口接收函数 */
static void lpuart_recv_handler (void *p_arg, char  ch)
{
    am_uart_handle_t handle = (am_uart_handle_t)(p_arg);

    am_uart_poll_send(handle, (const uint8_t*)&ch, 1);
}

/**
 * \brief 例程入口
 */
void demo_zsl42x_lpuart_wake_up_entry (void)
{
    const uint8_t tx_string[] = "deepsleep mode lpuart wake up test!\r\n";

    am_uart_handle_t handle;

    /* 防止进入低功耗无法烧录 */
    am_mdelay(3000);

    lowerpower_cfg();

    handle = am_zsl42x_lpuart0_inst_init();
    
    am_uart_ioctl(handle, AM_UART_MODE_SET, (void*)AM_UART_MODE_INT);
    am_uart_callback_set(handle,
                         AM_UART_CALLBACK_RXCHAR_PUT,
                         lpuart_recv_handler,
                         handle);

    am_uart_poll_send(handle, tx_string, sizeof(tx_string) - 1);
    am_mdelay(100);

    /* 进入深度睡眠模式 */
    am_zsl42x_lpmode_deepsleep(AM_TRUE);

    while (1) {

    }
}
/** [src_zsl42x_lpuart_wake_up] */

/* end of file */
