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
 *    1. 如需观察串口打印的调试信息，需要将 PIOB_3 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_aml165_core_hw_uart_int.c src_aml165_core_hw_uart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-01-20  ari, first implementation
 * \endinternal
 */
 
 /**
 * \addtogroup demo_if_aml165_core_hw_uart_int
 * \copydoc demo_aml165_core_hw_uart_int.c
 */ 

/** [src_aml165_core_hw_uart_int] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zml165.h"
#include "demo_zlg_entries.h"
#include "demo_aml165_core_entries.h"

static void __zlg165_uart_pins_intit (void)
{
    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOB_3, PIOB_3_UART1_TX | PIOB_3_AF_PP);
    am_gpio_pin_cfg(PIOB_4, PIOB_4_UART1_RX | PIOB_4_INPUT_FLOAT);
}

/**
 * \brief 例程入口
 */
void demo_aml165_core_hw_uart_int_entry (void)
{
    AM_DBG_INFO("demo aml165_core hw uart int!\r\n");

    /* 使能时钟 */
    am_clk_enable(CLK_UART1);

    demo_zlg_hw_uart_int_entry(ZML165_UART1, 
                               __zlg165_uart_pins_intit, 
                               am_clk_rate_get(CLK_UART1), 
                               ZML165_UART1_BASE, 
                               INUM_UART1);
}
/** [src_aml165_core_hw_uart_int] */

/* end of file */
