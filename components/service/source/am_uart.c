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
 * \brief UART implementation
 * 
 * \internal
 * \par Modification history
 * - 1.01 14-12-03  jon, add UART interrupt mode
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */
#include "am_uart.h"


/*******************************************************************************
  public functions
*******************************************************************************/ 

/**
 * \brief USART send data(poll mode)
 *
 * \param[in] handle  : The UART handle
 * \param[in] p_txbuf : The transmit buffer
 * \param[in] nbytes  : The number bytes to transmit
 * 
 * \return The number bytes transmit successful
 */
int am_uart_poll_send (am_uart_handle_t  handle,
                       const uint8_t    *p_txbuf,
                       uint32_t          nbytes)
{
    uint32_t len = nbytes; 
    
    while (len) {
        if (am_uart_poll_putchar(handle, *p_txbuf) != AM_OK) {
             continue;
        }
        
        len--;
        p_txbuf++;
    }
    
    return nbytes;
}
                            
/**
 * \brief USART receive data(poll mode)
 *
 * \param[in] p_usart : Pointer to USART register block
 * \param[in] p_rxbuf : The receive buffer
 * \param[in] nbytes  : The number bytes to receive
 * 
 * \return The number bytes received successful
 */
int am_uart_poll_receive (am_uart_handle_t  handle,
                          uint8_t          *p_rxbuf,
                          uint32_t          nbytes)
{
    uint32_t len = nbytes; 
    
    while (len){
        if (am_uart_poll_getchar(handle, (char *)p_rxbuf) != AM_OK) {
             continue;
        }
        
        len--;
        p_rxbuf++;
    }
    
    return nbytes;
}

/* end of file */
