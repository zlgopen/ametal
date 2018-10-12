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
 * \brief 程序清单5.66 
 *
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_spi.h"
#include "am_lpc82x_inst_init.h"
#include "am_lpc82x.h"
#include "am_vdebug.h"

static void __spi_msg_complete_callback (void *p_arg)
{
    *(uint8_t *) p_arg = 1;                        // 设置完成标志为1
}

int am_main (void)
{
    am_spi_message_t    spi_msg;                  // 定义一个SPI消息结构体
    uint8_t             tx_buf[8];
    uint8_t             rx_buf[8];
    am_spi_transfer_t   spi_trans;                // 定义一个SPI传输结构体
    volatile  uint8_t   complete_flag = 0;        // 完成标志
    am_spi_device_t     spi_dev; 
    am_spi_handle_t     spi0_handle   = am_lpc82x_spi0_dma_inst_init();
    am_spi_mkdev(&spi_dev, spi0_handle, 8, AM_SPI_MODE_0, 3000000, PIO0_13, NULL); 
    am_spi_setup(&spi_dev);                       // 设置SPI从设备
    am_spi_mktrans(&spi_trans, tx_buf, rx_buf, 8, 0, 0, 0, 0, 0);
    am_spi_msg_init (&spi_msg, __spi_msg_complete_callback, (void *)&complete_flag);
    am_spi_trans_add_tail(&spi_msg, &spi_trans);
    am_spi_msg_start(&spi_dev, &spi_msg);
    while (complete_flag == 0);                   // 等待消息处理结束
    while(1){
    }
}



/* end of file */

