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
 * \brief UART DMA 发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1.串口打印提示字符 "DMA tx transfer start:"；
 *   2. 然后串口打印一次内存定义的字符，分别为：
 *      uart tx dma test running
 *      uart tx dma test done
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）；
 *    3. 在 DMA 操作期间不要对串口数据寄存器进行操作。
 *
 * \par 源代码
 * \snippet demo_hc32f17x_hw_uart_tx_dma.c src_hc32f17x_hw_uart_tx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-23  zp, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f17x_hw_uart_tx_dma
 * \copydoc demo_hc32f17x_hw_uart_tx_dma.c
 */

/** [src_hc32f17x_hw_uart_tx_dma] */
#include "ametal.h"
#include "am_hc32.h"
#include "demo_hc32_entries.h"
#include "am_hc32f17x_inst_init.h"
#include "demo_amf17x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f17x_core_hw_uart_tx_dma_entry (void)
{
    AM_DBG_INFO("demo amf17x_core hw uart tx dma!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_UART1_TXD | PIOA_2_OUT_PP );
    am_gpio_pin_cfg(PIOA_3, PIOA_3_UART1_RXD | PIOA_10_INPUT_FLOAT);

    /* 使能时钟 */
    am_clk_enable(CLK_UART1);

    /* 等待发送数据完成 */
    am_mdelay(100);

    demo_hc32_hw_uart_tx_dma_entry(HC32_UART1,
                                     am_clk_rate_get(CLK_UART1),
                                     DMA_CHAN_1,
                                     HC32_DMA_SRC_TYPE_UART1_TX);
}

/** [src_hc32f17x_hw_uart_tx_dma] */

/* end of file */
