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
 * \brief UART 中断发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "HW example---UART test in interrupt mode:"；
 *   2. 串口输出接收到的字符。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOE_4 引脚连接 PC 串口的 TXD，
 *       PIOE_5 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_uart_int.c src_hc32f460_hw_uart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-06  cds, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_hc32f460_hw_uart_int
 * \copydoc demo_hc32f460_hw_uart_int.c
 */

/** [src_hc32f460_hw_uart_int] */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_board.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_clk.h"
#include "demo_hc32f460_entries.h"

static void __hc32f460_uart_pins_intit (void)
{
    am_gpio_pin_cfg(PIOC_13, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_RX));
    am_gpio_pin_cfg(PIOH_2, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_TX));
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_uart_int_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core hw uart int!\r\n");

    am_mdelay(100);

    /* 使能时钟 */
    am_clk_enable(CLK_USART4);

    demo_hc32f460_hw_uart_int_entry(HC32F460_UART4,
                                    __hc32f460_uart_pins_intit,
                                    am_clk_rate_get(CLK_USART4),
                                    HC32F460_UART4_BASE);
}
/** [src_zlg118_hw_uart_int] */

/* end of file */
