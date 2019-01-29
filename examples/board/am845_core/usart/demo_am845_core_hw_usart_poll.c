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
 * \brief USART 例程（轮询方式），通过硬件层接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "USART hw_poll mode test:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *    1. 例程使用 USART0，与 DEBUG 调试使用串口相同；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *       PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_usart_poll.c src_am845_core_hw_usart_poll
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_usart_poll
 * \copydoc demo_am845_core_hw_usart_poll.c
 */

/** [src_am845_core_hw_usart_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "demo_nxp_entries.h"

/**
 * \brief 基本输入频率设置（基本输入频率必须小于系统时钟频率且应为波特率的整数倍）
 *
 * 为了设置波特率为 115200，故设置串口基本输入频率为：
 * 11059200Hz(11059200 = 115200 * 96)。
 * 串口基本输入频率设置为 11.059200MHz，可满足大多数波特率的设置 (9600,4800,115200)
 *
 * \note  USART0\1\2 共用一个基本输入频率,不要轻易改动
 */
#define  __LPC84X_UASART_BASE_RATE  11059200
  
/**
 * \brief 例程入口
 */
void demo_am845_core_hw_usart_poll_entry (void)
{

    am_kprintf("demo am845 hw usart poll !\r\n");
    amhw_lpc84x_clk_uart0_clk_sel_set(AMHW_LPC84X_CLK_DEVICE_FRG0CLK);

    /* 使能串口 0 时钟并复位 */
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_UART0);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_UART0);

    /* 配置引脚复用 */
    am_gpio_pin_cfg(PIO0_19, PIO_FUNC_U0_TXD);
    am_gpio_pin_cfg(PIO0_18, PIO_FUNC_U0_RXD);

    demo_lpc_hw_usart_poll_entry(LPC84X_USART0,
    		amhw_lpc84x_clk_periph_freq_get(LPC84X_USART0),
            115200);
}
/** [src_am845_core_hw_usart_poll] */

/* end of file */
