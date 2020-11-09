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
 * \brief LPUART 轮询方式例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "STD-UART test in polling mode:"；
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
 * \snippet demo_zsl42x_std_uart_polling.c src_zsl42x_std_uart_polling
 *
 * \internal
 * \par Modification History
 * - 1.01 19-09-26  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_std_uart_polling
 * \copydoc demo_zsl42x_std_uart_polling.c
 */

/** [src_zsl42x_std_uart_polling] */
#include "ametal.h"
#include "am_board.h"
#include "am_zsl42x_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am42x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zsl42x_core_std_lpuart_polling_entry (void)
{
    AM_DBG_INFO("demo am42x_core std lpuart polling!\r\n");

     /* 等待发送数据完成 */
    am_mdelay(100);

    demo_std_uart_polling_entry(am_zsl42x_lpuart0_inst_init());
}
/** [src_zsl42x_std_uart_polling] */

/* end of file */
