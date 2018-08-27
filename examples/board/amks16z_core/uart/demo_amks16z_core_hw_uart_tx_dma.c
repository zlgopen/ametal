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
 * \brief UART0通过DMA方式接收数据，通过HW层的接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1 引脚连接PC串口的TXD;
 *   2. PIOA_2 引脚连接PC串口的RXD;
 *   3. 配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *
 * - 实验现象：
 *   1.串口打印提示字符 DMA tx transfer start:
 *   2. 然后串口交替打印一次内存定义的字符，分别为：
 *      uart tx dma test running
 *      uart tx dma test done
 *      
 *
 * \note 在DMA操作期间不要对串口数据寄存器UART_D进行操作
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_uart_tx_dma.c src_amks16z_core_hw_uart_tx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-21  nwt, make some changes.
 * - 1.00 15-07-10  xym, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_uart_tx_dma
 * \copydoc demo_amks16z_core_hw_uart_tx_dma.c
 */

/** [src_amks16z_core_hw_uart_tx_dma] */
#include "ametal.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_uart_tx_dma_entry (void)
{
    demo_kl26_hw_uart_tx_dma_entry();
}

/** [src_amks16z_core_hw_uart_tx_dma] */

/* end of file */
