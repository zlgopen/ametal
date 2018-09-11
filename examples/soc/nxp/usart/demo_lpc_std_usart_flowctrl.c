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
 * \brief USART 流控制例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 配置上位机串口波特率为 115200，8 位数据长度 1 位停止位 无奇偶校验；
 *   2. 打开 PC 端上位机的软件流控，设置 XON = 0x11， XOFF = 0x13。
 *
 * - 实验现象：
 *   1. 串口输出 "USART flowctrl test:\r\n"；
 *   2. 串口输出接收到的字符串；
 *   3. 例程中下位机每 100 毫秒只能处理 5 个字符（回送），因为开启了软件流控，所以
 *      PC 端大规模(使用带软件流控的上位机)发送数据也不会丢失数据，
 *      PC 发送的数据量等于接收的数据量。
 *
 * \note
 *    1. 例程使用 USART0，与 DEBUG 调试使用串口相同；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc_std_usart_flowctrl.c src_lpc_std_usart_flowctrl
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_std_usart_flowctrl
 * \copydoc demo_lpc_std_usart_flowctrl.c
 */

/** [src_lpc_std_usart_flowctrl] */
#include "ametal.h"
#include "am_delay.h"
#include "am_uart_rngbuf.h"
#include "am_lpc_usart.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

#define __UART0_RX_BUF_SIZE  128  /**< \brief 接收环形缓冲区大小，应该为 2^n */
#define __UART0_TX_BUF_SIZE  128  /**< \brief 发送环形缓冲区大小，应该为 2^n */

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

/** \brief USART 接收环形缓冲区 */
am_local uint8_t __g_usart0_rxbuf[__UART0_RX_BUF_SIZE];

/** \brief USART 发送环形缓冲区 */
am_local uint8_t __g_usart0_txbuf[__UART0_TX_BUF_SIZE];

/** \brief 串口缓冲区设备 */
am_local am_uart_rngbuf_dev_t  __g_uart0_rngbuf_dev;

void demo_lpc_std_usart_flowctrl_entry (am_uart_handle_t  uart_handle)
{
    uint8_t                 usart0_buf[5];
    am_uart_rngbuf_handle_t handle;
  
    am_uart_poll_send(uart_handle,
                      (am_const uint8_t *)"USART flowctrl test:\r\n",
                      sizeof("USART flowctrl test:\r\n") - 1);

    /* USART 初始化为环形缓冲区模式 */
    handle = am_uart_rngbuf_init(&__g_uart0_rngbuf_dev,
                                  uart_handle,
                                  __g_usart0_rxbuf,
                                  __UART0_RX_BUF_SIZE,
                                  __g_usart0_txbuf,
                                  __UART0_TX_BUF_SIZE);

    /* 开启软件流控 */
    am_uart_rngbuf_ioctl(handle,
                         AM_UART_FLOWMODE_SET,
                         (void *)AM_UART_FLOWCTL_SW);

    /* 设置流关阀值，50 个字节 */
    am_uart_rngbuf_ioctl(handle,
                         AM_UART_RNGBUF_RX_FLOW_OFF_THR,
                         (void *)50);

    /* 设置流开阀值，10 个字节 */
    am_uart_rngbuf_ioctl(handle,
                         AM_UART_RNGBUF_RX_FLOW_OFF_THR,
                         (void *)10);

    AM_FOREVER {

       /* 每 100 ms 从接收环形缓冲区中取出 5 个数据发回到 PC 端
        * 如果 PC 端下发的数据过多，当环形缓冲区达到流关阀值时，
        * MCU 会向 PC 发送流控信号 XOFF（0x13），PC 端停止发送。
        * 当接收环形缓冲区内的数据被取出减少到流空阀值时，MCU
        * 会向 PC 发送流控信号 XON （0x11），PC 端继续发送数据。
        */

       /* 取出接收缓冲区内的数据 */
       am_uart_rngbuf_receive(handle, usart0_buf, 5);

       /* 写数据到发送缓冲区 */
       am_uart_rngbuf_send(handle, usart0_buf, 5);

       am_mdelay(100);
    }
}
/** [src_lpc_std_usart_flowctrl] */

/* end of file */
