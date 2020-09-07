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
 * \brief UART 轮询方式例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "STD-UART test in polling mode:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_3 引脚连接 PC 串口的 TXD，
 *       PIOA_2 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_std_uart_polling.c src_hk32f103rbt6_std_uart_polling
 *
 * \internal
 * \par Modification History
 * - 1.01 17-04-17  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hk32f103rbt6_std_uart_polling
 * \copydoc demo_hk32f103rbt6_std_uart_polling.c
 */

/** [src_hk32f103rbt6_std_uart_polling] */
#include "ametal.h"
#include "am_board.h"
#include "am_hk32f103rbt6_inst_init.h"
#include "demo_std_entries.h"
#include "demo_hk32f103rbt6_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_core_std_usart_polling_entry (void)
{
    AM_DBG_INFO("demo hk32f103rbt6_core std usart polling!\r\n");

     /* 等待发送数据完成 */
    am_mdelay(100);

    demo_std_uart_polling_entry(am_hk32f103rbt6_usart2_inst_init());
}
/** [src_hk32f103rbt6_std_uart_polling] */

/* end of file */
