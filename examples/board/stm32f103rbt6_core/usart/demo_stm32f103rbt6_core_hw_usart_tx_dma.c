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
 *      usart tx dma test running
 *      usart tx dma test done
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_3 引脚连接 PC 串口的 TXD，
 *       PIOA_2 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）；
 *    3. 在 DMA 操作期间不要对串口数据寄存器进行操作。
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_hw_usart_tx_dma.c src_stm32f103rbt6_hw_usart_tx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-10  ari, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_stm32f103rbt6_hw_usart_tx_dma
 * \copydoc demo_stm32f103rbt6_hw_usart_tx_dma.c
 */

/** [src_stm32f103rbt6_hw_usart_tx_dma] */
#include "ametal.h"
#include "am_stm32f103rbt6.h"
#include "demo_stm32f103rbt6_entries.h"
#include "am_stm32f103rbt6_inst_init.h"
#include "demo_stm32f103rbt6_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_core_hw_usart_tx_dma_entry (void)
{
    AM_DBG_INFO("demo stm32f103rbt6_core hw usart tx dma!\r\n");

    /* 等待调试串口发送数据完成 */
    am_mdelay(100);

    /* 使能串口时钟*/
    am_clk_enable(CLK_USART2);

    /* 使能串口相关引脚*/
    am_gpio_pin_cfg(PIOA_2, PIOA_2_NO_REMAP | PIOA_2_AF_PP );
    am_gpio_pin_cfg(PIOA_3, PIOA_3_NO_REMAP | PIOA_3_INPUT_FLOAT);

    demo_stm32f103rbt6_hw_usart_tx_dma_entry((void *)STM32F103RBT6_USART2,
                                      am_clk_rate_get(CLK_USART2),
                                      DMA_CHAN_USART2_TX);
}
/** [src_stm32f103rbt6_hw_usart_tx_dma] */

/* end of file */
