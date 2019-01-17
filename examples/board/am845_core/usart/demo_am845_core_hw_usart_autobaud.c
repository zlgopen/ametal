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
 * \brief USART 自动匹配波特率例程，通过硬件层接口实现
 *
 * - 操作步骤：
 *   1. 当上位机接收到 "please set a new baudrate and then input '5'"时，改变
 *      上位机的波特率为 4800 或 9600,然后输入字符 'a' 或 '5'。
 *
 * - 实验现象：
 *   1. 自动波特率匹配成功后，串口打印 "The usart baudrate is xxx"，否则乱码；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *       PIO0_23 引脚连接 PC 串口的 RXD。
 *    2. 例程原理，MCU 硬件通过测量 RXD 引脚接受数据时上升沿与下降沿之间的时钟
 *       宽度自动计算波特率，所以要求上位机发送的数据（'a' 或 '5'）。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_usart_autobaud.c src_am845_core_hw_usart_autobaud
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_usart_autobaud
 * \copydoc demo_am845_core_hw_usart_autobaud.c
 */

 /** [src_am845_core_hw_usart_autobaud] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_usart_autobaud_entry (void)
{
    am_kprintf("demo am845 hw usart autobaud!\r\n");
    demo_lpc824_hw_usart_autobaud_entry(LPC84X_USART0,
    amhw_lpc84x_clk_periph_freq_get(LPC84X_USART0));
}
/** [src_am845_core_hw_usart_autobaud] */

/* end of file */
