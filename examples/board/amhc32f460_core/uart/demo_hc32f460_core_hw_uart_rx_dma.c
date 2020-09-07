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
 * \brief UART DMA 接收例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 通过上位机串口一次性向 MCU 发送 5 个字符。
 *
 * - 实验现象：
 *   1. 串口打印出接收到的数据；
 *   2. 然后串口打印出 "DMA transfer done!"。
 *
 * \note
 *    1. 将 PIOC_13 引脚连接 PC 串口的 TXD，PIOH_2 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_uart_rx_dma.c src_hc32f460_hw_uart_rx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-11  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_uart_rx_dma
 * \copydoc demo_hc32f460_hw_uart_rx_dma.c
 */

/** [src_hc32f460_hw_uart_rx_dma] */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_entries.h"
#include "am_hc32f460_dma.h"

extern am_hc32f460_dma_dev_t __g_dma1_dev;

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_uart_rx_dma_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core hw uart rx dma!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOC_13, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_RX));
    am_gpio_pin_cfg(PIOH_2, GPIO_AFIO(AMHW_HC32F460_AFIO_USART4_TX));

    /* 使能时钟 */
    am_clk_enable(CLK_USART4);
    am_clk_enable(CLK_PTDIS);

    /* 等待发送数据完成 */
    am_mdelay(100);

    demo_hc32f460_hw_uart_rx_dma_entry(HC32F460_UART4,
                                       am_clk_rate_get(CLK_USART4),
                                       &__g_dma1_dev,
                                       DMA_CHAN_2);
}

/** [src_hc32f460_hw_uart_rx_dma] */

/* end of file */
