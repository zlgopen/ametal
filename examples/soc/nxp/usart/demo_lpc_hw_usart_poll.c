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
 * \brief USART 例程（轮询方式），通过硬件层接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "USART hw_poll mode test:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *    1. 例程使用 USART0，与 DEBUG 调试使用串口相同；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_usart_poll.c src_lpc824_hw_usart_poll
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_usart_poll
 * \copydoc demo_lpc824_hw_usart_poll.c
 */

/** [src_lpc824_hw_usart_poll] */
#include "ametal.h"
#include "hw/amhw_lpc_usart.h"
//#include "hw/amhw_lpc82x_clk.h"

/**
 * \brief 基本输入频率设置（基本输入频率必须小于系统时钟频率且应为波特率的整数倍）
 *
 * 为了设置波特率为 115200，故设置串口基本输入频率为：
 * 11059200Hz(11059200 = 115200 * 96)。
 * 串口基本输入频率设置为 11.059200MHz，可满足大多数波特率的设置 (9600,4800,115200)
 *
 * \note  USART0\1\2 共用一个基本输入频率,不要轻易改动
 */
#define  __LPC84X_UASART_BASE_RATE  11059200


/** \brief 波特率 */
#define __USART_BAUDRATE            115200

void demo_lpc_hw_usart_poll_entry(amhw_lpc_usart_t *p_hw_usart,
                                  uint32_t          u_clk,
                                  uint32_t          baudrate)
{
     uint8_t buf[5];
  
    /* USART0 8 位数据长度 1 位停止位 无奇偶校验 */
    amhw_lpc_usart_config(p_hw_usart,
                          AMHW_LPC_USART_CFG_8BIT |
                          AMHW_LPC_USART_CFG_STOP_1 |
                          AMHW_LPC_USART_CFG_PARITY_NO);

   /* 波特率 115200 */
    amhw_lpc_usart_baudrate_set(p_hw_usart,
                                u_clk,
                                baudrate);

    /* 使能串口 */
    amhw_lpc_usart_enable(p_hw_usart);

    amhw_lpc_usart_poll_send(p_hw_usart,
                             (am_const uint8_t *)"USART hw_poll mode test:\r\n",
                             sizeof("USART hw_poll mode test:\r\n") - 1);

    AM_FOREVER {

        /* 接收数据 */
        amhw_lpc_usart_poll_receive (p_hw_usart, &buf[0], 1);

        /* 发送数据 */
        amhw_lpc_usart_poll_send(p_hw_usart, &buf[0], 1);
    }
}
/** [src_lpc824_hw_usart_poll] */

/* end of file */
