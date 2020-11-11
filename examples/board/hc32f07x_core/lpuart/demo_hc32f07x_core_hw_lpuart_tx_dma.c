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
 * \brief LPUART DMA 发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1.串口打印提示字符 "DMA tx transfer start:"；
 *   2. 然后串口打印一次内存定义的字符，分别为：
 *      lpuart tx dma test running
 *      lpuart tx dma test done
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_1 引脚连接 PC 串口的 TXD，
 *       PIOA_0 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）；
 *    3. 在 DMA 操作期间不要对串口数据寄存器进行操作。
 *
 * \par 源代码
 * \snippet demo_hc32f07x_hw_lpuart_tx_dma.c src_hc32f07x_hw_lpuart_tx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-26  zp, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f07x_hw_lpuart_tx_dma
 * \copydoc demo_hc32f07x_hw_lpuart_tx_dma.c
 */

/** [src_hc32f07x_hw_lpuart_tx_dma] */
#include "ametal.h"
#include "am_hc32.h"
#include "demo_hc32_entries.h"
#include "am_hc32f07x_inst_init.h"
#include "demo_amf07x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f07x_core_hw_lpuart_tx_dma_entry (void)
{
    AM_DBG_INFO("demo amf07x_core hw lpuart tx dma!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_LPUART1_RXD | PIOA_1_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOA_0, PIOA_0_LPUART1_TXD | PIOA_0_OUT_PP );

    /* 使能时钟 */
    am_clk_enable(CLK_LPUART1);

    /* 等待发送数据完成 */
    am_mdelay(100);

    demo_hc32_hw_lpuart_tx_dma_entry(HC32_LPUART1,
                                       AMHW_HC32_LPUART_SCLK_SRC_PCLK,
                                       24000000,
                                       115200,
                                       DMA_CHAN_1,
                                       HC32_DMA_SRC_TYPE_LPUART1_TX);
}

/** [src_hc32f07x_hw_lpuart_tx_dma] */

/* end of file */
