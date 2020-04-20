/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zsn700.cn/
*******************************************************************************/

/**
 * \file
 * \brief LPUART操作接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-18  yrz, first implementation
 * \endinternal
 */
#include "hw/amhw_zsn700_lpuart.h"

/**
 * \brief LPUART数据发送(查询模式)
 */
uint32_t amhw_zsn700_lpuart_poll_send (amhw_zsn700_lpuart_t *p_hw_lpuart,
                                       const uint8_t        *p_txbuf,
                                       uint32_t              nbytes)
{
    uint32_t len = nbytes;

    while (len--) {

         while(amhw_zsn700_lpuart_flag_check(p_hw_lpuart,
                                           AMHW_ZSN700_LPUART_FLAG_TX_EMPTY)==
                                           AM_FALSE);

         amhw_zsn700_lpuart_data_write(p_hw_lpuart, (uint8_t)(*p_txbuf++));
    }

    return nbytes;
}

/**
 * \brief LPUART数据接收(查询模式)
 */
uint32_t amhw_zsn700_lpuart_poll_receive (amhw_zsn700_lpuart_t    *p_hw_lpuart,
                                        uint8_t            *p_rxbuf,
                                        uint32_t            nbytes)
{
    uint32_t len = nbytes;

    while (len--) {

        while(amhw_zsn700_lpuart_flag_check(
                  p_hw_lpuart,
                  AMHW_ZSN700_LPUART_FLAG_RX_COMPLETE)==
                  AM_FALSE);

        *p_rxbuf++ = (uint8_t)amhw_zsn700_lpuart_data_read(p_hw_lpuart);

        amhw_zsn700_lpuart_flag_clr(p_hw_lpuart,
                                    AMHW_ZSN700_LPUART_FLAG_RX_COMPLETE);
    }

    return nbytes;
}

/**
 * \brief LPLPUART(Version 0) baud ratio set
 */
int amhw_zsn700_lpuart_baudrate_set (amhw_zsn700_lpuart_t *p_hw_lpuart,
                                     uint32_t              lpuart_clk,
                                     uint32_t              baud)
{
    uint8_t                        clk_bits = 0;
    uint8_t                        clk_div  = 0;
    uint32_t                       val_set  = 0;
    amhw_zsn700_lpuart_work_mode_t mode     = AMHW_ZSN700_LPUART_WORK_MODE_0;

    /* 确保有效的 clock value */
    if ((lpuart_clk > 48000000)) {
        lpuart_clk = 0;

        /* 初始化异常，由于期望的系统时钟值无效 */
        return AM_ERROR;
    }

    /* 获取时钟模式 */
    mode = amhw_zsn700_lpuart_mode_get(p_hw_lpuart);

    /* 模式0情况下，波特率固定为 PCLK/12 */
    if(mode == AMHW_ZSN700_LPUART_WORK_MODE_0) {

    	baud = (uint32_t)((float)lpuart_clk / 12.0);

    /* 模式2情况下，波特率 = 时钟 / 分频系数 */
    } else if (mode == AMHW_ZSN700_LPUART_WORK_MODE_2) {

        /* 获取分频系数设置位的数字量 */
        clk_bits = amhw_zsn700_lpuart_clk_div_get(p_hw_lpuart);

        /* 解析数字量对应的分频值 */
        switch(clk_bits) {
        case 0:
            clk_div = 32;
            break;

        case 1:
            clk_div = 16;
            break;

        case 2:
            clk_div = 8;
            break;
        }

        baud = (uint32_t)((float)lpuart_clk / (float)clk_div);

    /* 模式1/3情况下，波特率 = 时钟 / （分频系数*波特率寄存器设定值） */
    } else if ((mode == AMHW_ZSN700_LPUART_WORK_MODE_1) ||
               (mode == AMHW_ZSN700_LPUART_WORK_MODE_3) ||
               (mode == AMHW_ZSN700_LPUART_WORK_MODE_4)) {

        /* 获取分频系数设置位的数字量 */
        clk_bits = amhw_zsn700_lpuart_clk_div_get(p_hw_lpuart);

        /* 解析数字量对应的分频值 */
        switch(clk_bits) {
        case 0:
            clk_div = 16;
            break;

        case 1:
            clk_div = 8;
            break;

        case 2:
            clk_div = 4;
            break;
        }

        /* 计算存入波特率寄存器的值 */
        val_set = (uint32_t)((float)lpuart_clk / (float)baud / (float)clk_div);

        /* 超出最大值，直接填充所允许的最大值 */
        if (val_set > 0xFFFF) {
            p_hw_lpuart->scnt = 0xFFFF;

            /* 重新计算实际波特率，返回*/
            baud = (uint32_t)((float)lpuart_clk / (float)clk_div / (float)0xFFFF);

        /* 在允许范围内，填充设定值 */
        } else {
            p_hw_lpuart->scnt = val_set;

        }
    }

    return baud;
}

/* end of file */
