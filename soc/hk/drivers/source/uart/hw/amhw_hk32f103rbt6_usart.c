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
 * \brief USART操作接口
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-07  ari, first implementation
 * \endinternal
 */
#include "amhw_hk32f103rbt6_usart.h"

/**
 * \brief UART数据发送(查询模式)
 */
uint32_t amhw_hk32f103rbt6_usart_poll_send (amhw_hk32f103rbt6_usart_t     *p_hw_usart,
                                      const uint8_t       *p_txbuf,
                                      uint32_t             nbytes)
{
    uint32_t len = nbytes;

    while (len--) {

         while(amhw_hk32f103rbt6_usart_status_flag_check(p_hw_usart,
                                                   AMHW_HK32F103RBT6_USART_TX_EMPTY_FLAG)
                                                   == AM_FALSE);
         amhw_hk32f103rbt6_usart_data_write(p_hw_usart, (*p_txbuf++));
    }

    return nbytes;
}

/**
 * \brief UART数据接收(查询模式)
 */
uint32_t amhw_hk32f103rbt6_usart_poll_receive (amhw_hk32f103rbt6_usart_t    *p_hw_usart,
                                         uint8_t                *p_rxbuf,
                                         uint32_t                nbytes)
{
    uint32_t len = nbytes;

    while (len--) {
        while(amhw_hk32f103rbt6_usart_status_flag_check(p_hw_usart,
                                                  AMHW_HK32F103RBT6_USART_RX_NOT_EMPTY_FLAG)
                                                   == AM_FALSE);
        *p_rxbuf++ = (uint8_t)amhw_hk32f103rbt6_usart_data_read(p_hw_usart);
    }

    return nbytes;
}

/**
 * \brief LPUSART(Version 0) baud ratio set
 */
int amhw_hk32f103rbt6_usart_baudrate_set (amhw_hk32f103rbt6_usart_t *p_hw_usart,
                                    uint32_t             usart_clk,
                                    uint32_t             baud)
{

    uint32_t int_val   = 0;
    uint32_t deci_val  = 0;

    /* 确保有效的 clock value */
    if ((usart_clk > 96000000)) { //todo
        usart_clk = 0;

        /* 初始化异常，由于期望的系统时钟值无效 */
        return AM_ERROR;
    }

    int_val  = (usart_clk /baud)/16;
    deci_val = (usart_clk /baud)%16;

    amhw_hk32f103rbt6_usart_br_div_mantissa_set(p_hw_usart, int_val);
    amhw_hk32f103rbt6_usart_br_div_fraction_set(p_hw_usart, deci_val);

    return baud;
}

/* end of file */
