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
 * \brief 程序清单5.108
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_lpc82x_inst_init.h"
#include "am_uart.h"

static uint8_t     g_buf0[10] = {0};
static uint8_t     g_buf1[10] = {0};
static int         g_rx_index = 0;
static int         g_tx_index = -1;
static int         g_rx_cnt = 0;
static int         g_tx_cnt = 0;

static int __uart_txchar_get (void *p_arg, char *p_outchar)
{
    if (g_tx_index == -1)        return -AM_EEMPTY;
    if (g_tx_cnt == 10) {      // 当前缓冲区中的数据发送完毕
        g_tx_cnt = 0;
        return -AM_EEMPTY;
    }
    if (g_tx_index == 0) {
        *p_outchar = g_buf0[g_tx_cnt++];
    } else {
        *p_outchar = g_buf1[g_tx_cnt++];
    }
    return AM_OK;
}

static int __uart_rxchar_put (void *p_arg, char inchar)
{
    if (g_rx_index == 0) {                   // 将数据存放到缓冲区0
        g_buf0[g_rx_cnt++] = inchar;
    } else {                                 // 将数据存放到缓冲区1
        g_buf1[g_rx_cnt++] = inchar;
    }
    if (g_rx_cnt == 10) {                    // 缓冲区存满，发送缓冲区中的数据
        g_rx_cnt   = 0;
        g_tx_index = g_rx_index;             // 发送该缓冲区中的数据
        g_rx_index = !g_rx_index;            // 接下来的数据存放到另外一个缓冲区中
        am_uart_tx_startup(p_arg);           // 启动串口发送
    }
    return AM_OK;
}

int am_main (void)
{
    am_uart_handle_t uart_handle = am_lpc82x_usart0_inst_init();
    am_uart_callback_set(
        uart_handle, AM_UART_CALLBACK_TXCHAR_GET, __uart_txchar_get, uart_handle);
    am_uart_callback_set(
        uart_handle, AM_UART_CALLBACK_RXCHAR_PUT, __uart_rxchar_put, uart_handle);
    // 设置为中断模式
    am_uart_ioctl(uart_handle,AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);
    while(1) {
    }
}


/* end of file */

