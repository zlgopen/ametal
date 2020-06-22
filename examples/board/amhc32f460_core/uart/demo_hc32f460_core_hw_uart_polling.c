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
 * \brief UART 轮询方式例程，通过 HW 层接口实现
 *
 *
 * - 实验现象：
 *   1. 串口输出 "HW example---UART test in polling mode:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *    1. 将 PIOC_13 引脚连接 PC 串口的 TXD，PIOH_2 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_uart_polling.c src_hc32f460_hw_uart_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-11  cds, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_hc32f460_hw_uart_polling
 * \copydoc demo_hc32f460_hw_uart_polling.c
 */

/** [src_hc32f460_hw_uart_polling] */
#include "ametal.h"
#include "am_board.h"
#include "am_hc32f460.h"
#include "am_gpio.h"
#include "demo_hc32f460_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_uart_polling_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core hw uart polling!\r\n");

     /* 等待发送数据完成 */
    am_mdelay(100);

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOC_13, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_RX));
    am_gpio_pin_cfg(PIOH_2, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_TX));

    /* 使能时钟 */
    am_clk_enable(CLK_USART4);

    demo_hc32f460_hw_uart_polling_entry(HC32F460_UART4, am_clk_rate_get(CLK_USART4));
}
/** [src_hc32f460_hw_uart_polling] */

/* end of file */
