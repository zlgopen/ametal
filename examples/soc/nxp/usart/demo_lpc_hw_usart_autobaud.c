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
 *    1. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *    2. 例程原理，MCU 硬件通过测量 RXD 引脚接受数据时上升沿与下降沿之间的时钟
 *       宽度自动计算波特率，所以要求上位机发送的数据（'a' 或 '5'）。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_usart_autobaud.c src_lpc824_hw_usart_autobaud
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_usart_autobaud
 * \copydoc demo_lpc824_hw_usart_autobaud.c
 */

 /** [src_lpc824_hw_usart_autobaud] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc_usart.h"
#include "hw/amhw_lpc82x_clk.h"

/**
 * \brief 基本输入频率设置（基本输入频率必须小于系统时钟频率且应为波特率的整数倍）
 *
 * 为了设置波特率为 115200，故设置串口基本输入频率为：
 * 11059200Hz(11059200 = 115200 * 96)。
 * 串口基本输入频率设置为 11.059200MHz，可满足大多数波特率的设置 (9600,4800,115200)
 *
 * \note  USART0\1\2 共用一个基本输入频率,不要轻易改动
 */

void demo_lpc824_hw_usart_autobaud_entry(amhw_lpc_usart_t *p_hw_usart,
                                         uint32_t          uclk)
{
    uint8_t  tbuf[5];
    uint32_t baudrate;

    AM_DBG_INFO("Please set a new baudrate and then input '5'...\r\n");

    /* 自动波特率模式 */
    amhw_lpc_usart_ctl_set(p_hw_usart,
                           AMHW_LPC_USART_CTL_AUTOBAUD);

    /* 等待 PC 端发送字符，匹配波特率 */
    while (amhw_lpc_usart_ctl_get(p_hw_usart) &
           AMHW_LPC_USART_CTL_AUTOBAUD);

    /* 获取实际波特率 */
    baudrate = amhw_lpc_usart_baudrate_get(
                   p_hw_usart,
                   uclk);

    AM_DBG_INFO("\r\nThe usart baudrate is %d \r\n", baudrate);

    AM_FOREVER {

        /* 接收数据 */
        amhw_lpc_usart_poll_receive (p_hw_usart, &tbuf[0], 1);

        /* 发送数据 */
        amhw_lpc_usart_poll_send(p_hw_usart , &tbuf[0], 1);
    }
}
/** [src_lpc824_hw_usart_autobaud] */

/* end of file */
