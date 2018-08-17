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
 * \brief SPI 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 MOSI 和 MISO 短接，虚拟从机设备连接。
 *
 * - 实验现象：
 *   1. 主机通过 MOSI 发送数据，发出的数据从 MOSI 读回；
 *   2. 调试串口打印测试结果。
 *
 * \par 源代码
 * \snippet demo_std_spi_master.c src_std_spi_master
 *
 * \internal
 * \par History
 * - 1.00 17-04-25  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_spi_master
 * \copydoc demo_std_spi_master.c
 */

/** [src_std_spi_master] */
#include "ametal.h"
#include "am_spi.h"
#include "am_wait.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"

#define BUF_LEN  20 /**< \brief 测试缓冲区大小 */

static uint8_t __g_tx_buf[BUF_LEN]; /**< \brief 写数据缓存     */
static uint8_t __g_rx_buf[BUF_LEN]; /**< \brief 读数据缓存     */

static am_spi_device_t __g_dev; /**< \brief 设备      */

/**
 * \brief SPI 消息完成回调函数
 */
static void __message_complete (void *p_arg)
{
    am_wait_t *p_wait = (am_wait_t *)p_arg;

    /* 结束等待序列 */
    am_wait_done(p_wait);
}

/**
 * \brief SPI 传输函数
 */
static int __trans_once (am_spi_handle_t spi_handle,
                         int             cs_pin)
{
    uint8_t           i;
    uint8_t           error_ocur = 0;
    am_wait_t         msg_wait;
    am_spi_message_t  spi_msg;
    am_spi_transfer_t trans;

    /* 初始化 SPI 设备 */
    am_spi_mkdev (&__g_dev,
                   spi_handle,
                   8,
                   AM_SPI_MODE_1,
                   3000000,
                   cs_pin,  /* 使用引脚控制片选 */
                   NULL);

    /* 数据传输之前应使用 am_spi_setup (am_spi_device_t *p_dev) 检验设备是否支持 */
    if (am_spi_setup(&__g_dev) != AM_OK) {
        return AM_ERROR;
    }

    for(i = 0; i < BUF_LEN; i++) {
        __g_tx_buf[i] = i + 10;
    }

    for(i = 0; i < BUF_LEN; i++) {
        __g_rx_buf[i] = 0;
    }

    am_wait_init(&msg_wait);

    am_spi_msg_init(&spi_msg,
                     __message_complete,
                     (void *)&msg_wait);

    am_spi_mktrans(&trans,
                    __g_tx_buf,
                    __g_rx_buf,
                    BUF_LEN,
                    0,
                    0,
                    0,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans);

    am_spi_msg_start(&__g_dev, &spi_msg);

    am_wait_on(&msg_wait);
    
    for (i = 0; i < BUF_LEN; i++) {
        if (__g_tx_buf[i] != __g_rx_buf[i]) {
            error_ocur = 1;
            break;
        }
    }

    if (!error_ocur) {
        am_kprintf("trans done!\n");
        return AM_OK;
    } else {
        am_kprintf("trans error!\n");
    }
    return AM_ERROR;
}

/**
 * \brief 例程入口
 */
void demo_std_spi_master_entry (am_spi_handle_t spi_handle,
                                int             cs_pin)
{
    __trans_once(spi_handle, cs_pin);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_spi_master] */

/* end of file */
