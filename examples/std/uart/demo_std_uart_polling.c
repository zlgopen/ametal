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
 * \brief UART 查询方式下接收发送数据例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "STD-UART test in polling mode:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note： 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *       （如：AM_DBG_INFO()）
 *
 * \par 源代码
 * \snippet demo_std_uart_polling.c src_std_uart_polling
 *
 * \internal
 * \par Modification History
 * - 1.01 17-05-04  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_uart_polling
 * \copydoc demo_std_uart_polling.c
 */

/** [src_std_uart_polling] */
#include "ametal.h"
#include "am_uart.h"

/** \brief 需要发送的字符串 */
static const uint8_t __ch[] = {"STD-UART test in polling mode:\r\n"};

/**
 * \brief 例程入口
 */
void demo_std_uart_polling_entry (am_uart_handle_t handle)
{
    uint8_t uart1_buf[5];    /* 数据缓冲区 */

    am_uart_poll_send(handle, __ch, sizeof(__ch));

    while (1) {
        /* 接收字符 */
        am_uart_poll_receive(handle, uart1_buf, 1);

        /* 发送刚刚接收的字符 */
        am_uart_poll_send(handle, uart1_buf, 1);

    }
}
/** [src_std_uart_polling] */

/* end of file */
