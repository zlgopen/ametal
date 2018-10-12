/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s hc595 driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_spi.h"
#include "am_lpc82x_inst_init.h"
#include "hc595.h"
#include "am_lpc82x.h"

static am_spi_device_t  g_hc595_dev;
void hc595_init(void)        // 板级初始化
{
    am_spi_handle_t   spi1_handle  = am_lpc82x_spi1_dma_inst_init();
    // 初始化一个与74HC595对应的SPI从机实例
    am_spi_mkdev( &g_hc595_dev,           // 传递从机设备地址
                  spi1_handle,            // SPI0操作句柄
                  8,                      // 数据宽度为8-bit
                  AM_SPI_MODE_3,          // 选择模式3
                  3000000,                // 最大频率3MHz
                  PIO0_14,                // 片选引脚PIO0_14
                  NULL);                  // 无自定义片选控制函数NULL
    am_spi_setup(&g_hc595_dev);           // 设置SPI从设备
}

void hc595_send_data (uint8_t data)       // 串行输入并行输出数据
{  // 使用连续两次写函数进行数据发送
    am_spi_write_then_write(&g_hc595_dev,
                            &data,        // 发送数据缓冲区0的数据
                            1,            // 发送一个数据
                            NULL,         // 发送数据缓冲区1无数据
                            0);           // 无数据，长度为0
}                                         


static    am_spi_transfer_t   g_spi_trans;     // 仅需要传输一次数据
static    am_spi_message_t    g_spi_msg;       // 消息
static    uint8_t             g_tx_buf[1];     // 数据缓冲区，一个8位数据

void hc595_send_data_async (uint8_t         data,         // 发送的数据
                            am_pfnvoid_t    pfn_complete, // 消息处理完成回调函数
                            void           *p_arg)        // 回调函数的参数
{
    g_tx_buf[0] =  data;             // 将数据加载至缓冲区中
    am_spi_mktrans(&g_spi_trans,
                    g_tx_buf,        // 发送数据缓冲区
                    NULL,            // 无需接收数据，置接收数据缓冲区为NULL
                    1,               // 传输数据个数为1
                    0,               // 本次传输不影响片选
                    8,               // 位宽为8
                    0,               // 传输后无需延时
                    3000000,         // 时钟频率，3MHz
                    0);              // 无特殊控制标志
    am_spi_msg_init (&g_spi_msg,
                      pfn_complete,  // 消息处理完毕后，调用用户回调函数
                      p_arg);        // 用户指定的回调函数参数
    am_spi_trans_add_tail(&g_spi_msg, &g_spi_trans);
    am_spi_msg_start(&g_hc595_dev, &g_spi_msg);
}


/* end of file */
