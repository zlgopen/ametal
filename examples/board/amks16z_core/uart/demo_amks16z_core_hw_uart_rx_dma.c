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
 *   1. PIOC_3 引脚连接PC串口的TXD;
 *   2. PIOC_4 引脚连接PC串口的RXD;
 *   3. 配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *   4. 通过上位机串口一次性向MCU发送5个字符。
 *
 * - 实验现象：
 *   1. 串口打印出接收到的数据;
 *   2. 然后串口打印出“DMA transfer done!”。

 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_uart_rx_dma.c src_amks16z_core_hw_uart_rx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-21  nwt, make some changes.
 * - 1.00 15-07-10  xym, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_uart_rx_dma
 * \copydoc demo_amks16z_core_hw_uart_rx_dma.c
 */

/** [src_amks16z_core_hw_uart_rx_dma] */
#include "ametal.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_uart_rx_dma_entry (void)
{

    demo_kl26_hw_uart_rx_dma_entry();
}

/** [src_amks16z_core_hw_uart_rx_dma] */

/* end of file */
