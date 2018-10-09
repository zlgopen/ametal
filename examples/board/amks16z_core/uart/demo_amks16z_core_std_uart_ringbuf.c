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
 * \brief UART环形缓冲区方式下接收发送数据例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. PIOC_3 引脚连接PC串口的TXD;
 *   2. PIOC_4 引脚连接PC串口的RXD。
 *   3. 配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *
 * - 实验现象：
 *   1. 串口输出"UART interrupt mode(Add ring buffer) test:"；
 *   2. 串口输出接收到的字符串。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_uart_ringbuf.c src_amks16z_core_std_uart_ringbuf
 *
 * \internal
 * \par Modification history
 * - 1.01 16-09-19  nwt, change interface function name.
 * - 1.00 15-10-27  xym, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_std_uart_ringbuf
 * \copydoc demo_amks16z_core_std_uart_ringbuf.c
 */

/** [src_amks16z_core_std_uart_ringbuf] */
#include "ametal.h"
#include "am_fsl_uart.h"
#include "am_uart_rngbuf.h"
#include "am_kl26_inst_init.h"
#include "am_board.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_std_uart_ringbuf_entry (void)
{
    am_uart_handle_t uart_handle;
    /* UART初始化 */
    uart_handle = am_kl26_uart1_inst_init();

    demo_std_uart_ringbuf_entry(uart_handle);
}

/** [src_amks16z_core_std_uart_ringbuf] */

/* end of file */
