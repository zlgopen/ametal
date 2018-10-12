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
 * \brief 程序清单5.106
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_uart.h"
#include "am_lpc82x_inst_init.h"
static int __uart_txchar_get (void *p_arg, char *p_outchar)
{
    // *p_outchar = ? 赋值为需要发送的数据，如发送 0x55，*p_outchar = 0x55;
    *p_outchar = 0x55;
    return AM_OK;
}

int am_main (void)
{
    am_uart_handle_t uart_handle = am_lpc82x_usart0_inst_init();
    am_uart_callback_set(
        uart_handle, AM_UART_CALLBACK_TXCHAR_GET, __uart_txchar_get, NULL);
    am_uart_tx_startup (uart_handle);
    while(1) {
    }
}


/* end of file */

