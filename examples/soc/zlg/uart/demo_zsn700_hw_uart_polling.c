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
 * \snippet demo_zsn700_hw_uart_polling.c src_zsn700_hw_uart_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-23  zp, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_zsn700_hw_uart_polling
 * \copydoc demo_zsn700_hw_uart_polling.c
 */

/** [src_zsn700_hw_uart_polling] */
#include "ametal.h"
#include "hw/amhw_zsn700_uart.h"

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
static void uart_polling_init (amhw_zsn700_uart_t *p_hw_uart, uint32_t clk_rate)
{
    /* 工作模式设置  */
    amhw_zsn700_uart_mode_sel(p_hw_uart, AMHW_ZSN700_UART_WORK_MODE_1);

    /* 分频系数设置  */
    amhw_zsn700_uart_clk_div_sel(p_hw_uart, AMHW_ZSN700_UART_CLK_DIV_MODE13_8);

    /* 等待发送空完成 */
    while (amhw_zsn700_uart_flag_check(p_hw_uart,
                                       AMHW_ZSN700_UART_FLAG_TX_EMPTY) ==
                                       AM_FALSE);

    /* 停止位设置 */
    amhw_zsn700_uart_stop_bit_sel(p_hw_uart  , AMHW_ZSN700_UART_STOP_1_0_BIT);

    /* 校验设置 */
    amhw_zsn700_uart_parity_bit_sel(p_hw_uart, AMHW_ZSN700_UART_PARITY_NO);

    /* 波特率设置 */
    amhw_zsn700_uart_baudrate_set( p_hw_uart, clk_rate, UART_BAUDRATE);

    /* 接收使能
     * Mode0:     0：发送; 1：接收
     * Mode1/2/3: 0：发送; 1： 接收/发送
     */
    amhw_zsn700_uart_enable(p_hw_uart,AMHW_ZSN700_UART_RX);
}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_uart_polling_entry (void     *p_hw_uart,
                                        uint32_t  clk_rate)
{

    /* 数据缓冲区 */
    uint8_t buf_src[256];
    amhw_zsn700_uart_t * p_uart = (amhw_zsn700_uart_t * )p_hw_uart;

    /* UART查询初始化 */
    uart_polling_init(p_uart, clk_rate);

    amhw_zsn700_uart_poll_send(p_uart, uart_hw_str, sizeof(uart_hw_str));

    while (1) {

        /* 接收字符 */
        amhw_zsn700_uart_poll_receive(p_uart, buf_src, 1);

        /* 发送刚刚接收的字符 */
        amhw_zsn700_uart_poll_send(p_uart, buf_src, 1);
    }
}
/** [src_zsn700116_hw_uart_polling] */

/* end of file */
