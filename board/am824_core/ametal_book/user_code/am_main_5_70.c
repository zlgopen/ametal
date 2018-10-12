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
 * \brief 程序清单5.70 
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
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
int am_main(void)
{
    am_spi_handle_t  spi1_handle  = am_lpc82x_spi1_dma_inst_init(); 
    am_spi_device_t  hc595_dev;                // 定义与HC595关联的从机设备
    uint8_t          tx_buf[1] = {0x55};       // 假定发送一个8位数据0x55

    // 初始化一个与74HC595对应的SPI从机实例
    am_spi_mkdev(
        &hc595_dev,                    // 传递从机设备地址
        spi1_handle,                // SPI0操作句柄
        8,                             // 数据宽度为8-bit
        AM_SPI_MODE_3,                 // 选择模式3
        3000000,                       // 最大频率3MHz
        PIO0_14,                       // 片选引脚PIO0_1 4
        NULL);                         // 无自定义片选控制函数NULL
        am_spi_setup(&hc595_dev);      // 设置SPI从设备
        am_spi_write_then_write(&hc595_dev,    // 传递从机设备
        tx_buf,                        // 发送数据缓冲区0的数据
        1,                             // 发送1个数据
        NULL,                          // 发送数据缓冲区1无数据
        0);                            // 无数据，则长度为0
    while(1) {
    }
}


/* end of file */

