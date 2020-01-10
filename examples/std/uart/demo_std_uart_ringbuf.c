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
 * \brief UART 环形缓冲区方式下接收发送数据例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "UART interrupt mode(Add ring buffer) test:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note： 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *       （如：AM_DBG_INFO()）
 *
 * \par 源代码
 * \snippet demo_std_uart_ringbuf.c src_std_uart_ringbuf
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-4  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_uart_ringbuf
 * \copydoc demo_std_uart_ringbuf.c
 */

/** [src_std_uart_ringbuf] */
#include "ametal.h"
#include "am_uart.h"
#include "am_uart_rngbuf.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define UART_RX_BUF_SIZE  128  /**< \brief 接收环形缓冲区大小，应该为2^n  */
#define UART_TX_BUF_SIZE  128  /**< \brief 发送环形缓冲区大小，应该为2^n  */

/*******************************************************************************
  全局变量
*******************************************************************************/

/** \brief UART 接收环形缓冲区  */
static uint8_t __uart_rxbuf[UART_RX_BUF_SIZE];

/** \brief UART 发送环形缓冲区  */
static uint8_t __uart_txbuf[UART_TX_BUF_SIZE];

/** \brief 初始化显示字符  */
static const uint8_t __ch[] = {"UART interrupt mode(Add ring buffer) test:\r\n"};

/** \brief 串口缓冲区设备 */
static am_uart_rngbuf_dev_t __g_uart_ringbuf_dev;

/**
 * \brief 例程入口
 */
void demo_std_uart_ringbuf_entry (am_uart_handle_t uart_handle)
{
    uint8_t                 uart1_buf[5];   /* 数据缓冲区 */
    am_uart_rngbuf_handle_t handle = NULL;  /* 串口环形缓冲区服务句柄 */

    /* UART 初始化为环形缓冲区模式 */
    handle = am_uart_rngbuf_init(&__g_uart_ringbuf_dev,
                                  uart_handle,
                                  __uart_rxbuf,
                                  UART_RX_BUF_SIZE,
                                  __uart_txbuf,
                                  UART_TX_BUF_SIZE);

//    am_uart_rngbuf_send(handle, __ch, sizeof(__ch));

    while (1) {

        /* 从接收缓冲区取出一个数据到 buf，接收数据 */
        am_uart_rngbuf_receive(handle, uart1_buf, 1);

        /* 将 buf 的一个数据放入环形缓冲区，发送数据 */
        am_uart_rngbuf_send(handle, uart1_buf, 1);
    }
}
/** [src_std_uart_ringbuf] */

/* end of file */
