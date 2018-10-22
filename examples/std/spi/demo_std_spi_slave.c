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
 * \brief SPI 从机例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 主机与从机连接。
 *
 * - 实验现象：
 *   1. 传输完成之后从机打印接收缓冲区的数据。
 *
 * \par 源代码
 * \snippet demo_std_spi_slave.c src_std_spi_slave
 *
 * \internal
 * \par History
 * - 1.00 17-09-19 fra, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_spi_slave
 * \copydoc demo_std_spi_slave.c
 */

/** [src_std_spi_slave] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_spi_slv.h"

#define __REG_SIZE 40 /**< \brief 缓冲区大小 */

uint8_t g_spi_slve_rxbuf[__REG_SIZE]; /**< \brief 从机接收缓冲区 */
uint8_t g_spi_slve_txbuf[__REG_SIZE]; /**< \brief 从机发送缓冲区 */

/**
 * \brief 传输开始回调
 */
static int __cs_active (void *p_arg)
{
    return AM_OK;
}

static int __transfer_data (void *p_arg, am_spi_slv_transfer_t *p_trans)
{
    p_trans->p_rx_buf = g_spi_slve_rxbuf;
    p_trans->p_tx_buf = g_spi_slve_txbuf;
    p_trans->nbytes   = __REG_SIZE;

    p_trans->bits_per_word = 0;

    return AM_OK;
}

/**
 * \brief 传输结束回调
 */
static int __cs_inactive (void *p_arg, size_t n)
{
    uint32_t i;

    AM_DBG_INFO("rx_buf:\r\n");

    for (i = 0; i < __REG_SIZE; i++) {
        AM_DBG_INFO("0x%02x  ", g_spi_slve_rxbuf[i]);

        if (((i + 1) % 4) == 0) {
            AM_DBG_INFO("\r\n");
        }
    }
    AM_DBG_INFO("\r\n");

    return AM_OK;
}

/**
 * \brief SPI从设备回调函数类型
 */
static am_spi_slv_cb_funs_t __g_spi_slv_cb_funs = {
    __cs_active,
    __cs_inactive,
    __transfer_data,
};

/**
 * \brief 例程入口
 */
void demo_std_spi_slave_entry (am_spi_slv_handle_t handle)
{
    uint8_t             i;
    am_spi_slv_device_t spi_slv_dev;

    /* 构造发送的数据 */
    for (i = 0; i < __REG_SIZE; i++) {
        g_spi_slve_txbuf[i] = i + 4;
    }

    am_spi_slv_mkdev(&spi_slv_dev,
                      handle,
                      8,
                      AM_SPI_SLV_MODE_1,
                     &__g_spi_slv_cb_funs,
                     &spi_slv_dev);

    if (am_spi_slv_setup(&spi_slv_dev) != AM_OK) {
        AM_DBG_INFO("fail to setup!\r\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_spi_slave] */

/* end of file */
