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
 * \brief SPI 主机发送数据例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 主机与 SPI 从机进行物理连接。
 *
 * - 实验现象：
 *   1. 本例程每 500 毫秒通过 SPI，向从机发送 "nihao" 字符串；
 *   2. 从机接收到字符串，通过串口将字符串打印出来，且 LED0 闪烁。
 *
 * \note
 *    1. 本例程需要与 demo_lpc_hw_spi_slave.c 一同测试；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_spi_master.c src_lpc_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.01 15-11-26  sky, modified
 * - 1.00 15-07-15  aii, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_spi_master
 * \copydoc demo_lpc_hw_spi_master.c
 */

/** [src_lpc_hw_spi_master] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc_spi.h"

#define __SPI_SPEED        100000  /**< \brief SPI 速率宏定义 */
#define __SPI_PRE_DELAY    0       /**< \brief SSEL 置位与起始帧间延时 */
#define __SPI_POST_DELAY   0       /**< \brief 帧末尾与 SSEL 解除置位间延时 */
#define __SPI_FRAME_DELAY  0       /**< \brief SPI 相邻数据帧间延时 */
#define __SPI_TRANS_DELAY  0       /**< \brief 两次传输 SSEL 解除置位间延时 */

/**
 * \brief SPI0 硬件初始化
 *
 * \param[in] p_hw_spi 指向 SPI 寄存器块的指针
 *
 * \return 无
 */
am_local void __spi_master_hard_init (amhw_lpc_spi_t *p_hw_spi,
                                      uint32_t        clk)
{

    /* 配置 SPI 分频值 */
    amhw_lpc_spi_div_set(
        p_hw_spi,
        ((clk / __SPI_SPEED)));

    /* 配置 SPI 传输延时 */
    amhw_lpc_spi_pre_delay(p_hw_spi, __SPI_PRE_DELAY);
    amhw_lpc_spi_post_delay(p_hw_spi, __SPI_POST_DELAY);
    amhw_lpc_spi_frame_delay(p_hw_spi, __SPI_FRAME_DELAY);
    amhw_lpc_spi_trans_delay(p_hw_spi, __SPI_TRANS_DELAY);

    /* 配置 SPI 传输参数 */
    amhw_lpc_spi_cfg_set(p_hw_spi,
                         AMHW_LPC_SPI_CFG_MASTER | AMHW_LPC_SPI_CFG_CHANGE);
    amhw_lpc_spi_enable(p_hw_spi);

    /* 设置 SPI 一个数据长度为 8 位 */
    amhw_lpc_spi_data_flen_set(p_hw_spi, 8);
}

/**
 * \brief SPI0 发送字节
 *
 * \param[in] p_hw_spi 指向 SPI 寄存器块的指针
 * \param[in] dat      要发送的字节数据
 *
 * \return 无
 */
am_local void __spi_sent_byte (amhw_lpc_spi_t *p_hw_spi, uint16_t dat)
{

    /* 等待发送空闲寄存器置位 */
    while (!(amhw_lpc_spi_stat_get(p_hw_spi) & AMHW_LPC_SPI_STAT_IDLE));

    /* 发送数据 */
    amhw_lpc_spi_txdatctl(p_hw_spi,
                          dat,
                          AMHW_LPC_SPI_TXDATCTL_EOT |
                          AMHW_LPC_SPI_TXDATCTL_RX_IGNORE,
                          8);
}

/**
 * \brief SPI0 发送字符串
 *
 * \param[in] p_hw_spi 指向 SPI 寄存器块的指针
 * \param[in] p_str    要发送字符串地址
 *
 * \return 无
 */
am_local void __spi_sent_str(amhw_lpc_spi_t *p_hw_spi, am_const uint8_t *p_str)
{
    while (*p_str != 0) {
        __spi_sent_byte(p_hw_spi, *p_str++);
    }
}

void demo_lpc_hw_spi_master_entry(amhw_lpc_spi_t *p_hw_spi,
                                     uint32_t        clk)
{
    /* SPI0 硬件初始化 */
    __spi_master_hard_init(p_hw_spi, clk);

    AM_FOREVER {

        /* SPI 发送字符串 */
        __spi_sent_str(p_hw_spi, (am_const uint8_t *)"nihao");

        am_mdelay(500);

    }
}
/** [src_lpc_hw_spi_master] */

/* end of file */
