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
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 的 MOSI 引脚和 MISO 引脚用线相连，模拟从机设备，回环测试。
 *
 * - 实验现象：
 *   1. 主机通过 MISO 发送数据，发出的数据从 MOSI 读回；
 *   2. 调试串口打印测试结果。
 *
 * \par 源代码
 * \snippet demo_mm32_hw_spi_master.c src_mm32_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 17-04-27  ari, first implementation
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_mm32_hw_spi_master
 * \copydoc demo_mm32_hw_spi_master.c
 */
 
/** [src_mm32_hw_spi_master] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
//#include "am_mm32l073.h"
#include "hw/amhw_mm32_spi.h"

/**
 * \name SPI传输结构体配置参数
 */
#define SPI_CFG_LSB         AM_BIT(0)   /**< \brief 低位优先发送 */
#define SPI_CFG_16BIT       AM_BIT(1)   /**< \brief 传输数据宽度为16位 */

#define SPI_CFG_MODE_0      0           /**< \brief 模式0 */
#define SPI_CFG_MODE_1      1           /**< \brief 模式1 */
#define SPI_CFG_MODE_2      2           /**< \brief 模式2 */
#define SPI_CFG_MODE_3      3           /**< \brief 模式3 */

/** @} */

/** 
 * \brief SPI传输结构体
 */
typedef struct spi_transfer {
    const void  *p_txbuf;           /**< \brief 指向发送数据缓存的指针 */
    void        *p_rxbuf;           /**< \brief 指向接收数据缓存的指针 */
    uint32_t     nbytes;            /**< \brief 数据长度 */
    uint32_t     flags;             /**< \brief SPI传输控制标志位 */
} spi_transfer_t;

spi_transfer_t g_spi_transfer;      /* 定义一个SPI传输结构体实例 */

static void spi_speed_cfg (amhw_mm32_spi_t *p_hw_spi,
                           uint32_t        target_speed,
                           uint32_t        clk_rate)
{
    uint32_t best_pdiv;              /* 计算出的速度 */

    best_pdiv = clk_rate / target_speed;

    amhw_mm32_spi_baud_rate_set(p_hw_spi, best_pdiv);
}

static void spi_master_cfg (amhw_mm32_spi_t *p_hw_spi)
{
    amhw_mm32_spi_first_bit_sel(p_hw_spi, AMHW_MM32_SPI_DATA_MSB);
    amhw_mm32_spi_data_len_sel(p_hw_spi, AMHW_MM32_SPI_DATA_LEN_8BIT);
    amhw_mm32_spi_valid_data_sel(p_hw_spi,AMHW_MM32_SPI_VALID_DATA_8BIT);

    /* 配置时钟相位和极性 */
    amhw_mm32_spi_clk_mode_set(p_hw_spi, SPI_CFG_MODE_1 ^ 0x01);
}

static void spi_master_cs_ctr (amhw_mm32_spi_t *p_hw_spi,
                               int                pin,
                               uint8_t            active_level,
                               am_bool_t          state)
{

    /* 片选有效 */
    if (state) {
        amhw_mm32_spi_cs_sel(p_hw_spi,AM_TRUE);

    /* 片选无效 */
    } else {
        amhw_mm32_spi_cs_sel(p_hw_spi, AM_FALSE);
    }
}

/**
 * \brief SPI回环传输测试
 */
void spi_loop_trans (amhw_mm32_spi_t *p_hw_spi,
                     spi_transfer_t *p_trans,
                     int32_t         cs_pin)
{
    uint32_t pos = 0;

    spi_master_cfg(p_hw_spi);

    spi_master_cs_ctr(p_hw_spi, cs_pin, AM_GPIO_LEVEL_LOW, AM_TRUE);

    while(pos < p_trans->nbytes) {

        /* 等待可以发送 */
        while ((amhw_mm32_spi_reg_cstat_get(p_hw_spi) &
                AMHW_MM32_SPI_CSTAT_TX_EMPTY) == 0);

            amhw_mm32_spi_tx_data8_put(p_hw_spi,
                                        *((uint8_t*)(p_trans->p_txbuf) + pos));

        /* 等待可以接收 */
        while ((amhw_mm32_spi_reg_cstat_get(p_hw_spi) &
                AMHW_MM32_SPI_CSTAT_RXVAL) == 0);

            *(uint8_t*)((uint8_t*)p_trans->p_rxbuf + pos) =
                                         amhw_mm32_spi_rx_data8_get(p_hw_spi);
            pos += 1;
    }

    /* 处理片选 */
    spi_master_cs_ctr(p_hw_spi, cs_pin, AM_GPIO_LEVEL_LOW, AM_FALSE);
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_spi_master_entry (amhw_mm32_spi_t *p_hw_spi,
                                   int32_t         cs_pin,
                                   uint32_t        clk_rate)
{
    uint8_t         i;
    uint8_t         spi_send_buf[16];
    uint8_t         spi_recv_buf[16];
    const uint32_t  length     = 16;
    am_bool_t       error_ocur = AM_FALSE;
    spi_transfer_t *p_trans    = &g_spi_transfer;

    p_trans->p_txbuf = spi_send_buf;
    p_trans->p_rxbuf = spi_recv_buf;
    p_trans->nbytes  = length;

    amhw_mm32_spi_mode_sel(p_hw_spi, AMHW_MM32_SPI_MODE_MASTER);
    spi_speed_cfg(p_hw_spi, 100000, clk_rate);
    amhw_mm32_spi_tx_enable(p_hw_spi, AM_TRUE);
    amhw_mm32_spi_rx_enable(p_hw_spi, AM_TRUE);
    amhw_mm32_spi_module_enable(p_hw_spi, AM_TRUE);

    /* 构造数据 */
    for (i = 0; i < length; i++) {
        spi_send_buf[i] = i;
    }

    /* 数据传输 */
    spi_loop_trans(p_hw_spi, p_trans, cs_pin);

    /* 数据检验 */
    for (i = 0; i < length; i++) {
        if (spi_recv_buf[i] != spi_send_buf[i]) {
            error_ocur = AM_TRUE;
            break;
        }
    }

    if (!error_ocur) {
        am_kprintf("trans done!\n");
    } else {
        am_kprintf("trans error!\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_mm32_hw_spi_master] */

/* end of file */
