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
 * \brief 程序清单5.65  该程序是没有任何效果的
 *
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_spi.h"
static void __spi_msg_complete_callback (void *p_arg)
{
     // 消息处理完毕
}

int am_main (void)
{
    am_spi_message_t     spi_msg;                  // 定义一个SPI消息结构体
    uint8_t              tx_buf[8];
    uint8_t              rx_buf[8];
    am_spi_transfer_t    spi_trans;                // 定义一个SPI传输结构体

    am_spi_mktrans(&spi_trans, tx_buf, rx_buf, 8, 0, 0, 0, 0, 0);
    am_spi_msg_init (&spi_msg,     __spi_msg_complete_callback, NULL);
    am_spi_trans_add_tail(&spi_msg, &spi_trans);

    return 0;
}




/* end of file */

