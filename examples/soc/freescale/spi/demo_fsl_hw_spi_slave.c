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
 * \brief SPI从机演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI从机接口和主机接口对应连接起来;
 *   2. 将程序下载到demo板，重新上电或者复位。
 *
 * - 实验现象:
 *   1. 从机接收主机数据，并通过串口将接收到的数据打印出来；
 *   2. 从机发送数据到主机，并通过串口将发送的数据打印出来；
 *
 * - 注意：
 *   1. 需要使用其他设备充当主机。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_spi_slave.c src_fsl_hw_spi_slave
 *
 * \internal
 * \par History
 * - 1.00 16-09-27  sdy, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_fsl_if_hw_spi_slave
 * \copydoc demo_fsl_hw_spi_slave.c
 */
 
/** [src_fsl_hw_spi_slave] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_fsl_spi.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"


/**
 * \brief SPI从机初始化（8bit数据宽度 SPI_MODE1）
 */
static void __spi_slv_init (amhw_fsl_spi_t *p_hw_spi)
{
    /* 配置SPI为从机 */
    amhw_fsl_spi_workmode_cfg(p_hw_spi, AMHW_FSL_SPI_WMODE_SLAVE);

    /* 配置8bit数据宽度 */
    amhw_fsl_spi_feature_cfg(p_hw_spi, AMHW_FSL_SPI_CFG_8BIT_WIDTH);

    /* 配置时钟相位和极性 */
    amhw_fsl_spi_mode_cfg(p_hw_spi, AMHW_FSL_SPI_MODE_1);

    /* 使能SPI模块 */
    amhw_fsl_spi_enable(p_hw_spi);
}

/**
 * \brief SPI从机发送数据
 */
static void __spi_slv_send (amhw_fsl_spi_t  *p_hw_spi,
                            uint8_t         *p_buf,
                            uint32_t         count)
{
    uint32_t i = 0;
    
    for (i = 0; i < count ; i++) {

        while ((amhw_fsl_spi_stat_get(p_hw_spi) & AMHW_FSL_SPI_STAT_T_EMPTY) == 0);
        amhw_fsl_spi_data8_wirte(p_hw_spi, p_buf[i]);
    }
    
}

/**
 * \brief SPI从机接收数据
 */
static void __spi_slv_recv (amhw_fsl_spi_t  *p_hw_spi,
                            uint8_t         *p_buf,
                            uint32_t         count)
{
    uint32_t i = 0;

    for (i = 0; i < count; i++) {
        while ( (amhw_fsl_spi_stat_get(p_hw_spi) & AMHW_FSL_SPI_STAT_R_FULL) == 0 );

        p_buf[i] = amhw_fsl_spi_data8_read(p_hw_spi);
    }
}


/**
 * \brief 例程入口
 */
void demo_fsl_hw_spi_slave_entry (amhw_fsl_spi_t *p_hw_spi)
{
    uint8_t spi_slv_txbuf[32] = {0};
    uint8_t spi_slv_rxbuf[32] = {0};
    uint8_t length =32;
    uint8_t i;

    /* SPI 从机初始化 */
    __spi_slv_init(p_hw_spi);

    /* 构造发送的数据 */
    for (i = 0; i < length; i++) {
        spi_slv_txbuf[i] = i;
    }


    while (1) {
        /* 从机发送数据到主机 */
        __spi_slv_send(p_hw_spi, spi_slv_txbuf, length);
        AM_DBG_INFO("\r\n--Slave transmit Data:\r\n");
        for (i = 0; i < length; i++) {
            AM_DBG_INFO("The %2dst data is %2d \r\n", i, spi_slv_txbuf[i]);
        }

        __spi_slv_recv(p_hw_spi, spi_slv_rxbuf, length);
        AM_DBG_INFO("\r\n--Slave receive Data:\r\n");
        for (i = 0; i<length; i++) {
            AM_DBG_INFO("The %2dst data is %2d \r\n", i, spi_slv_rxbuf[i]);
        }

    }
}

/** [src_fsl_hw_spi_slave] */

/* end of file */

