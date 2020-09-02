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
 * \brief UART 环形缓冲区例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "UART interrupt mode(Add ring buffer) test:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_3 引脚连接 PC 串口的 TXD，
 *       PIOA_2 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zlg_std_uart_ringbuf.c src_zlg_std_uart_ringbuf
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-18  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg_std_uart_ringbuf
 * \copydoc demo_zlg_std_uart_ringbuf.c
 */

/** [src_zlg_std_uart_ringbuf] */
#include "ametal.h"
#include "am_zlg237_inst_init.h"
#include "demo_std_entries.h"
#include "am_vdebug.h"
#include "demo_am237_core_entries.h"
/**
 * \brief 例程入口
 */
void demo_zlg237_core_std_usart_ringbuf_entry (void)
{
    AM_DBG_INFO("demo am237_core std usart ringbuf!\r\n");

     /* 等待发送数据完成 */
    am_mdelay(100);

    demo_std_uart_ringbuf_entry(am_zlg237_usart2_inst_init());
}
/** [src_zlg_std_uart_ringbuf] */

/* end of file */
