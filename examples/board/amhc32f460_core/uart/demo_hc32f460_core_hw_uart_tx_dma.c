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
 *    1. 如需观察串口打印的调试信息，需要将 PIOC_13 引脚连接 PC 串口的 TXD，
 *       PIOH_2 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）；
 *    3. 在 DMA 操作期间不要对串口数据寄存器进行操作。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_uart_tx_dma.c src_hc32f460_hw_uart_tx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-10  cds, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_uart_tx_dma
 * \copydoc demo_hc32f460_hw_uart_tx_dma.c
 */

/** [src_hc32f460_hw_uart_tx_dma] */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_entries.h"
#include "am_hc32f460_dma.h"

extern am_hc32f460_dma_dev_t __g_dma1_dev;

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_uart_tx_dma_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core hw uart tx dma!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOC_13, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_RX));
    am_gpio_pin_cfg(PIOH_2, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_TX));

    /* 使能时钟 */
    am_clk_enable(CLK_USART4);
    am_clk_enable(CLK_PTDIS);

    /* 等待发送数据完成 */
    am_mdelay(100);

    demo_hc32f460_hw_uart_tx_dma_entry(HC32F460_UART4,
                                       am_clk_rate_get(CLK_USART4),
                                       &__g_dma1_dev,
                                       DMA_CHAN_2);
}

/** [src_hc32f460_hw_uart_tx_dma] */

/* end of file */
