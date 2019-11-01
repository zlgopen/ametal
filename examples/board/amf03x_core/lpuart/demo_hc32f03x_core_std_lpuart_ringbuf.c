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
 * \brief LPUART 环形缓冲区例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "UART interrupt mode(Add ring buffer) test:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 将 PIOB_12 引脚连接 PC 串口的 RXD，PIOB_11 引脚连接 PC 串口的 TXD；
 *    3. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f03x_std_uart_ringbuf.c src_hc32f03x_std_uart_ringbuf
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f03x_std_uart_ringbuf
 * \copydoc demo_hc32f03x_std_uart_ringbuf.c
 */

/** [src_hc32f03x_std_uart_ringbuf] */
#include "ametal.h"
#include "am_hc32f03x_inst_init.h"
#include "demo_std_entries.h"
#include "am_vdebug.h"
#include "demo_amf03x_core_entries.h"
/**
 * \brief 例程入口
 */
void demo_hc32f03x_core_std_lpuart_ringbuf_entry (void)
{
    AM_DBG_INFO("demo amf03x_core std lpuart ringbuf!\r\n");

     /* 等待发送数据完成 */
    am_mdelay(100);

    demo_std_uart_ringbuf_entry(am_hc32_lpuart0_inst_init());
}
/** [src_hc32f03x_std_uart_ringbuf] */

/* end of file */
