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
 * \brief UART操作接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-19  zp, first implementation
 * \endinternal
 */
#include "hw/amhw_zsl42x_uart.h"

/**
 * \brief UART数据发送(查询模式)
 */
uint32_t amhw_zsl42x_uart_poll_send (amhw_zsl42x_uart_t *p_hw_uart,
                                     const uint8_t      *p_txbuf,
                                     uint32_t            nbytes)
{
    uint32_t len = nbytes;

    while (len--) {

         while(amhw_zsl42x_uart_flag_check(p_hw_uart,
                                           AMHW_ZSL42x_UART_FLAG_TX_EMPTY)==
                                           AM_FALSE);

         amhw_zsl42x_uart_data_write(p_hw_uart, (uint8_t)(*p_txbuf++));
    }

    return nbytes;
}

/**
 * \brief UART数据接收(查询模式)
 */
uint32_t amhw_zsl42x_uart_poll_receive (amhw_zsl42x_uart_t    *p_hw_uart,
                                        uint8_t            *p_rxbuf,
                                        uint32_t            nbytes)
{
    uint32_t len = nbytes;

    while (len--) {

        while(amhw_zsl42x_uart_flag_check(p_hw_uart,
                                          AMHW_ZSL42x_UART_FLAG_RX_COMPLETE)==
                                          AM_FALSE);

        *p_rxbuf++ = (uint8_t)amhw_zsl42x_uart_data_read(p_hw_uart);\

        amhw_zsl42x_uart_flag_clr(p_hw_uart,AMHW_ZSL42x_UART_FLAG_RX_COMPLETE);
    }

    return nbytes;
}

/**
 * \brief LPUART(Version 0) baud ratio set
 */
int amhw_zsl42x_uart_baudrate_set (amhw_zsl42x_uart_t *p_hw_uart,
                                   uint32_t            uart_clk,
                                   uint32_t            baud)
{
    uint8_t  clk_bit = 0;
    uint8_t  clk_div = 0;
    uint32_t val_set = 0;
    volatile uint8_t waittime = 0;
    amhw_zsl42x_uart_work_mode_t mode = AMHW_ZSL42x_UART_WORK_MODE_0;

    /* 确保有效的 clock value */
    if ((uart_clk > 48000000)) {
        uart_clk = 0;

        /* 初始化异常，由于期望的系统时钟值无效 */
        return AM_ERROR;
    }

    /* 获取时钟模式 */
    mode = amhw_zsl42x_uart_mode_get(p_hw_uart);

    /* 模式0情况下，波特率固定为 PCLK/12 */
    if(mode == AMHW_ZSL42x_UART_WORK_MODE_0) {

    	baud = (uint32_t)((float)uart_clk / 12.0);

    /* 模式2情况下，波特率 = 时钟 / 分频系数 */
    } else if (mode == AMHW_ZSL42x_UART_WORK_MODE_2) {
        /* 获取分频系数设置位的数字量 */
        clk_bit = amhw_zsl42x_uart_clk_div_get(p_hw_uart);

        /* 解析数字量对应的分频值 */
        if(clk_bit == 0) {
            clk_div = 32;
        } else {
            clk_div = 16;
        }

        baud = (uint32_t)((float)uart_clk / (float)clk_div);

    /* 模式1/3情况下，波特率 = 时钟 / （分频系数*波特率寄存器设定值） */
    } else if ((mode == AMHW_ZSL42x_UART_WORK_MODE_1) ||
               (mode == AMHW_ZSL42x_UART_WORK_MODE_3)) {

        /* 获取分频系数设置位的数字量 */
        clk_bit = amhw_zsl42x_uart_clk_div_get(p_hw_uart);

        /* 解析数字量对应的分频值 */
        if(clk_bit == 0) {
            clk_div = 16;
        } else {
            clk_div = 8;
        }

        /* 计算存入波特率寄存器的值 */
        val_set = (uint32_t)((float)uart_clk / (float)(baud * clk_div) + 0.5);

        /* 超出最大值，直接填充所允许的最大值 */
        if (val_set > 0xFFFF) {
            p_hw_uart->scnt = 0xFFFF;

            /* 重新计算实际波特率，返回*/
            baud = (uint32_t)((float)uart_clk / (float)clk_div / (float)0xFFFF);

        /* 在允许范围内，填充设定值 */
        } else {
            p_hw_uart->scnt = val_set;

        }
    }

		waittime = 100;
    while(waittime--);
		
    return baud;
}

/* end of file */
