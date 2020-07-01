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
#include "hw/amhw_hc32f460_uart.h"

/**
 * \brief UART数据发送(查询模式)
 */
uint32_t amhw_hc32f460_uart_poll_send(amhw_hc32f460_uart_t *p_hw_uart,
        const uint8_t *p_txbuf, uint32_t nbytes)
{
    uint32_t len = nbytes;

    while (len--) {

        while (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
        AMHW_HC32F460_UART_TX_EMPTY_FLAG) == AM_FALSE)
            ;
        amhw_hc32f460_uart_data_write(p_hw_uart, (uint8_t) (*p_txbuf++));
    }

    return nbytes;
}

/**
 * \brief UART数据接收(查询模式)
 */
uint32_t amhw_hc32f460_uart_poll_receive(amhw_hc32f460_uart_t *p_hw_uart,
        uint8_t *p_rxbuf, uint32_t nbytes)
{
    uint32_t len = nbytes;

    while (len--) {
        while (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
        AMHW_HC32F460_UART_RX_VAL_FLAG) == AM_FALSE)
            ;
        *p_rxbuf++ = (uint8_t) amhw_hc32f460_uart_data_read(p_hw_uart);
    }

    return nbytes;
}

/**
 * \brief LPUART(Version 0) baud ratio set
 */
int amhw_hc32f460_uart_baudrate_set(amhw_hc32f460_uart_t *p_hw_uart,
                                    int                   uart_clk,
                                    uint32_t              baud)
{
    uint32_t B = 0ul;
    uint32_t C = 0ul;
    uint32_t OVER8 = 0ul;
    float DIV = 0.0f;
    uint64_t u64Tmp = 0u;
    uint32_t DIV_Integer = 0ul;
    uint32_t DIV_Fraction = 0xFFFFFFFFul;
    int enRet = AM_OK;


    /* 确保有效的 clock value */
    if ((uart_clk > 168000000)) { //todo
        uart_clk = 0;

        /* 初始化异常，由于期望的系统时钟值无效 */
        return AM_ERROR;
    }

    uint8_t reg_pr_psc = 0;
    uint8_t i = 0;

    /* 寻找合适的内部时钟分频（1 << 2 * reg_pr_psc） */
    for (i = 0; i < 4; i++) {
        C = uart_clk / (1 << 2 * reg_pr_psc);
        if (C > 0ul) {
            B = baud;
            OVER8 = p_hw_uart->CR1_f.OVER8;
            /* FBME = 0 Calculation formula */
            /* B = C / (8 * (2 - OVER8) * (DIV_Integer + 1)) */
            /* DIV_Integer = (C / (B * 8 * (2 - OVER8))) - 1 */
            DIV = ((float) C / ((float) B * 8.0f * (2.0f - (float) OVER8))) - 1.0f;
            DIV_Integer = (uint32_t) (DIV);
            if (!((DIV < 0.0f) || (DIV_Integer > 0xFFul))) {
                break;
            }
        }
        reg_pr_psc++;
    }

    if (reg_pr_psc > 3) {
        return AM_ERROR;
    } else {
        p_hw_uart->PR_f.PSC = reg_pr_psc;
    }

    if ((DIV - (float) DIV_Integer) > 0.00001f) {
        /* FBME = 1 Calculation formula */
        /* B = C * (128 + DIV_Fraction) / (8 * (2 - OVER8) * (DIV_Integer + 1) * 256) */
        /* DIV_Fraction = ((8 * (2 - OVER8) * (DIV_Integer + 1) * 256 * B) / C) - 128 */
        /* E = (C * (128 + DIV_Fraction) / (8 * (2 - OVER8) * (DIV_Integer + 1) * 256 * B)) - 1 */
        /* DIV_Fraction = (((2 - OVER8) * (DIV_Integer + 1) * 2048 * B) / C) - 128 */
        u64Tmp = (uint64_t) (((uint64_t) 2ul - (uint64_t) OVER8)
                * ((uint64_t) DIV_Integer + 1ul) * (uint64_t) B);
        DIV_Fraction = (uint32_t) (2048ul * u64Tmp / C - 128ul);
        if (DIV_Fraction > 0x7Ful) {
            enRet = AM_ERROR;
        }
    }

    if (AM_OK == enRet) {
        p_hw_uart->CR1_f.FBME = (DIV_Fraction > 0x7f) ? 0ul : 1ul;
        p_hw_uart->BRR_f.DIV_FRACTION = DIV_Fraction;
        p_hw_uart->BRR_f.DIV_INTEGER = DIV_Integer;
    } else {
        return AM_ERROR;
    }
    
    return baud;
}


/* end of file */
