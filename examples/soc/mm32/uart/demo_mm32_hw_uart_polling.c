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
 * \brief UART 轮询方式例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定串口输出 "HW example---UART test in polling mode:"；
 *   2. 指定串口输出接收到的字符串。
 *
 * \par 源代码
 * \snippet demo_mm32_hw_uart_polling.c src_mm32_hw_uart_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-17  ari, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_mm32_hw_uart_polling
 * \copydoc demo_mm32_hw_uart_polling.c
 */

/** [src_mm32_hw_uart_polling] */
#include "ametal.h"
//#include "am_mm32l073.h"
#include "hw/amhw_mm32_uart.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define UART_BAUDRATE     (115200)              /** <\brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static const uint8_t uart_hw_str[] =
                                {"HW example---UART test in polling mode:\r\n"};

/**
 * \brief UART HW 查询收发初始化
 */
static void uart_polling_init (amhw_mm32_uart_t *p_hw_uart, uint32_t clk_rate)
{

    /* 设置串口波特率 */
    amhw_mm32_uart_baudrate_set(p_hw_uart, clk_rate, UART_BAUDRATE);

    amhw_mm32_uart_stop_bit_sel(p_hw_uart, AMHW_MM32_UART_STOP_1BIT);
    amhw_mm32_uart_data_length(p_hw_uart, AMHW_MM32_UART_DATA_8BIT);
    amhw_mm32_uart_parity_bit_sel(p_hw_uart,  AMHW_MM32_UART_PARITY_NO);

    /* 使能串口 */
    amhw_mm32_uart_rx_enable(p_hw_uart,AM_TRUE);
    amhw_mm32_uart_tx_enable(p_hw_uart,AM_TRUE);
    amhw_mm32_uart_enable(p_hw_uart);
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_uart_polling_entry (amhw_mm32_uart_t *p_hw_uart,
                                     uint32_t         clk_rate)
{

    /* 数据缓冲区 */
    uint8_t buf_src[256];

    /* UART查询初始化 */
    uart_polling_init(p_hw_uart, clk_rate);

    amhw_mm32_uart_poll_send(p_hw_uart, uart_hw_str, sizeof(uart_hw_str));

    while (1) {

        /* 接收字符 */
        amhw_mm32_uart_poll_receive(p_hw_uart, buf_src, 1);

        /* 发送刚刚接收的字符 */
        amhw_mm32_uart_poll_send(p_hw_uart, buf_src, 1);
    }
}
/** [src_mm32l073_hw_uart_polling] */

/* end of file */
