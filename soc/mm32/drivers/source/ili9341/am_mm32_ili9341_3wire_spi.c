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
 * \brief zlg 三线接口驱动函数实现
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-27  vir, first implementation
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_mm32_ili9341_3wire_spi.h"

/**
 * \brief 发送8位命令
 */
static  int __mm32_ili9341_3wire_cmd_write (void *p_drv, uint8_t cmd)
{
    amhw_mm32_spi_t *p_spi_reg_base  =
      (( am_mm32_ili9341_intfc_3wire_dev_t *) p_drv)->p_devinfo->spi_reg_base;

    uint32_t temp = 0;

    /* 蝶式交换 */
    cmd = ( cmd >> 4) | ( cmd << 4 );
    cmd = ((cmd >> 2) & 0X33) | ( (cmd << 2) & 0XCC);
    cmd = ((cmd >> 1) & 0X55) | ( (cmd << 1) & 0XAA);

    temp = cmd;
    /* 代表发送命令 */
    temp <<=1 ;

    /* 写入数据 */
    p_spi_reg_base->txreg = temp;
    p_spi_reg_base->txreg = temp >> 8;
    while ( 0 == ( p_spi_reg_base->cstat & 0x01) );

    return AM_OK;
}

/**
 * \brief 发送8位数据
 */
static int __mm32_ili9341_3wire_data_u8_write (void *p_drv,uint8_t data)
{
    amhw_mm32_spi_t *p_spi_reg_base = (( am_mm32_ili9341_intfc_3wire_dev_t *) \
                       p_drv)->p_devinfo->spi_reg_base;

   uint32_t temp = 0;

   /* 蝶式交换 */
   data = ( data >> 4) | ( data << 4 );
   data = ((data >> 2) & 0X33) | ( (data << 2) & 0XCC);
   data = ((data >> 1) & 0X55) | ( (data << 1) & 0XAA);

   temp = data;
   /* 代表发送命令 */
    temp = ( temp << 1 ) | 0X1 ;

    /* 写入数据 */
    p_spi_reg_base->txreg = temp;
    p_spi_reg_base->txreg = temp >> 8;
    while ( 0 == ( p_spi_reg_base->cstat & 0x01) );

    return AM_OK;
}

/**
 * \brief 发送16位数据
 */
static int __mm32_ili9341_3wire_data_u16_write(void *p_drv,uint16_t data)
{
    amhw_mm32_spi_t *p_spi_reg_base = (( am_mm32_ili9341_intfc_3wire_dev_t *) \
                       p_drv)->p_devinfo->spi_reg_base;

    uint32_t temp = 0;
    uint8_t data1 = (uint8_t)(data >> 8);
    uint8_t data2 = (uint8_t) data;

    /* 蝶式交换 */
    data1 = ( data1 >> 4) | ( data1 << 4 );
    data1 = ((data1 >> 2) & 0X33) | ( (data1 << 2) & 0XCC);
    data1 = ((data1 >> 1) & 0X55) | ( (data1 << 1) & 0XAA);

    temp = data1;
    /* 代表发送命令 */
    temp = ( temp << 1 ) | 0X1 ;

    /* 写入数据 */
    p_spi_reg_base->txreg = temp;
    p_spi_reg_base->txreg = temp >> 8;
    while ( 0 == ( p_spi_reg_base->cstat & 0x01) );

    /* 蝶式交换 */
    data2 = ( data2 >> 4) | ( data2 << 4 );
    data2 = ((data2 >> 2) & 0X33) | ( (data2 << 2) & 0XCC);
    data2 = ((data2 >> 1) & 0X55) | ( (data2 << 1) & 0XAA);

    temp = data2; /* 不需要清除temp */
    /* 代表发送命令 */
    temp = ( temp << 1 ) | 0X1 ;

    /* 写入数据 */
    p_spi_reg_base->txreg = temp;
    p_spi_reg_base->txreg = temp >> 8;
    while ( 0 == ( p_spi_reg_base->cstat & 0x01) );

    return AM_OK;
}

static struct am_ili9341_drv_funcs __g_ili9341_funcs = {
        __mm32_ili9341_3wire_cmd_write,
        __mm32_ili9341_3wire_data_u8_write,
        __mm32_ili9341_3wire_data_u16_write,
        NULL,
        NULL,
};

/**
 * \brief 初始化
 */

am_ili9341_serv_t *am_mm32_ili9341_intfc_3wire_init (
                          am_mm32_ili9341_intfc_3wire_dev_t           *p_dev,
                          const am_mm32_ili9341_intfc_3wire_devinfo_t *p_devinfo)
{
    p_dev->serv.p_funcs = &__g_ili9341_funcs;
    p_dev->serv.p_drv   = p_dev;

    p_dev->p_devinfo = p_devinfo;

    uint32_t best_pdiv;

    am_gpio_pin_cfg(p_devinfo->cs_pin,   p_devinfo->cs_func);

    /**
     *  通过寄存器配置SPI
     */
    /* 开启时钟 */
    am_clk_enable(p_devinfo->clk_spi);

    /* 配置引脚  ,默认50M输出 */
    am_gpio_pin_cfg(p_devinfo->miso_pin, p_devinfo->miso_func);
    am_gpio_pin_cfg(p_devinfo->mosi_pin, p_devinfo->mosi_func);
    am_gpio_pin_cfg(p_devinfo->sck_pin,  p_devinfo->sck_func);

    amhw_mm32_spi_mode_sel(p_devinfo->spi_reg_base, AMHW_MM32_SPI_MODE_MASTER);
    /* 设置速度 */
    best_pdiv = am_clk_rate_get(p_devinfo->clk_spi) / p_devinfo->speed_hz;
    amhw_mm32_spi_baud_rate_set(p_devinfo->spi_reg_base, best_pdiv);

    amhw_mm32_spi_tx_enable(p_devinfo->spi_reg_base, AM_TRUE);
    amhw_mm32_spi_rx_enable(p_devinfo->spi_reg_base, AM_TRUE);
    amhw_mm32_spi_module_enable(p_devinfo->spi_reg_base, AM_TRUE);

    amhw_mm32_spi_first_bit_sel(p_devinfo->spi_reg_base, AMHW_MM32_SPI_DATA_LSB);
    amhw_mm32_spi_data_len_sel(p_devinfo->spi_reg_base, AMHW_MM32_SPI_DATA_LEN_8BIT);
    amhw_mm32_spi_valid_data_sel(p_devinfo->spi_reg_base,AMHW_MM32_SPI_VALID_DATA_8BIT);

    /* 设置9位 */
    amhw_mm32_spi_extlen_set(p_devinfo->spi_reg_base, 9);

    /* 配置时钟相位和极性 模式1 */
    amhw_mm32_spi_clk_mode_set(p_devinfo->spi_reg_base, 1);

    /* 设置 片选有效 */
    amhw_mm32_spi_cs_sel(p_devinfo->spi_reg_base, AM_TRUE);
    amhw_mm32_spi_nss_ctrl_sel(p_devinfo->spi_reg_base,AMHW_MM32_SPI_NSS_CTRL_BT_HD);

    return &p_dev->serv;
}

/* end of file */
