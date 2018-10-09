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
 * \brief UART查询方式下接收发送数据例程，通过HW层的接口实现
 *
 * - 操作步骤：
 *   1. PIOC_3 引脚连接PC串口的TXD;
 *   2. PIOC_4 引脚连接PC串口的RXD。
 *   3. 配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *
 * - 实验现象：
 *   1. 串口输出"HW example---UART test in polling mode:"；
 *   2. 串口输出接收到的字符串。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_uart_polling.c src_fsl_hw_uart_polling
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-19  nwt, make some changes.
 * - 1.00 15-07-07  xym, first implementation.
 * \endinternal
 */
 
 /**
 * \addtogroup demo_fsl_if_hw_uart_polling
 * \copydoc demo_fsl_hw_uart_polling.c
 */

/** [src_fsl_hw_uart_polling] */
#include "ametal.h"
#include "hw/amhw_fsl_uart.h"
#include "am_gpio.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define UART_BAUDRATE     (115200)              /** <\brief 波特率.  */

/**
 * \brief UART hw 查询收发初始化
 */
static void uart_polling_init (amhw_fsl_uart_t *p_hw_uart,
                               uint32_t         uart_clk,
                               am_bool_t        uart0_flog)
{
    /* 设置串口波特率 */
    if(uart0_flog){
        amhw_fsl_uart_ver0_baudrate_set(p_hw_uart,
                                        uart_clk,
                                        UART_BAUDRATE);
    } else {
        amhw_fsl_uart_ver1_baudrate_set(p_hw_uart,
                                        uart_clk,
                                        UART_BAUDRATE);
    }
    amhw_fsl_uart_stop_bit_set (p_hw_uart, AMHW_FSL_UART_BDH_SBNS_STOP_1);
    amhw_fsl_uart_data_mode_set(p_hw_uart, AMHW_FSL_UART_C1_M_8BIT);
    amhw_fsl_uart_parity_set(p_hw_uart,  AMHW_FSL_UART_C1_PARITY_NO);

    /* 使能串口 */
    amhw_fsl_uart_enable(p_hw_uart);
}

/**
 * \brief 例程入口
 */
void demo_fsl_hw_uart_polling_entry (amhw_fsl_uart_t *p_hw_uart,
                                     uint32_t         uart_clk,
                                     am_bool_t        uart0_flog)
{
    const uint8_t uart_hw_str[]={"HW example---UART test in polling mode:\r\n"};
    
    /* 数据缓冲区       */
    uint8_t buf_src[256];

    /* UART查询初始化 */
    uart_polling_init(p_hw_uart, uart_clk, uart0_flog);

    amhw_fsl_uart_poll_send(p_hw_uart, uart_hw_str, sizeof(uart_hw_str));

    while (1) {
        /* 接收字符 */
        amhw_fsl_uart_poll_receive(p_hw_uart, buf_src, 1);
        /* 发送刚刚接收的字符 */
        amhw_fsl_uart_poll_send(p_hw_uart, buf_src, 1);
    }
}
/** [src_fsl_hw_uart_polling] */

/* end of file */
