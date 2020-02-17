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
 * - 1.00 17-04-07  ari, first implementation
 * \endinternal
 */
#include "hw/amhw_mm32_uart.h"

/**
 * \brief UART数据发送(查询模式)
 */
uint32_t amhw_mm32_uart_poll_send (amhw_mm32_uart_t     *p_hw_uart,
                                  const uint8_t       *p_txbuf,
                                  uint32_t             nbytes)
{
    uint32_t len = nbytes;

    while (len--) {

         while(amhw_mm32_uart_status_flag_check(
                                                 p_hw_uart,
                                                 AMHW_MM32_UART_TX_EMPTY_FLAG)
                                                 == AM_FALSE);
         amhw_mm32_uart_data_write(p_hw_uart, (uint8_t)(*p_txbuf++));
    }

    return nbytes;
}

/**
 * \brief UART数据接收(查询模式)
 */
uint32_t amhw_mm32_uart_poll_receive (amhw_mm32_uart_t    *p_hw_uart,
                                     uint8_t            *p_rxbuf,
                                     uint32_t            nbytes)
{
    uint32_t len = nbytes;

    while (len--) {
        while(amhw_mm32_uart_status_flag_check(p_hw_uart,
                                              AMHW_MM32_UART_RX_VAL_FLAG)
                                              == AM_FALSE);
        *p_rxbuf++ = (uint8_t)amhw_mm32_uart_data_read(p_hw_uart);
    }

    return nbytes;
}

/**
 * \brief LPUART(Version 0) baud ratio set
 */
int amhw_mm32_uart_baudrate_set (amhw_mm32_uart_t    *p_hw_uart,
                                uint32_t            uart_clk,
                                uint32_t            baud)
{

    uint32_t int_val   = 0;
    uint32_t deci_val  = 0;

    /* 确保有效的 clock value */
    if ((uart_clk > 96000000)) { //todo
        uart_clk = 0;

        /* 初始化异常，由于期望的系统时钟值无效 */
        return AM_ERROR;
    }

    int_val = (uart_clk /baud)/16;
    deci_val = (uart_clk /baud)%16;

    amhw_mm32_uart_br_div_mantissa_set(p_hw_uart, int_val);
    amhw_mm32_uart_br_div_fraction_set(p_hw_uart, deci_val);

    /* FRA 寄存器需要重新使能 UART 才能生效 */
    if (amhw_mm32_uart_uarten_get(p_hw_uart)) {
        amhw_mm32_uart_disable(p_hw_uart);
        amhw_mm32_uart_enable(p_hw_uart);
    }

    return baud;
}

/* end of file */
