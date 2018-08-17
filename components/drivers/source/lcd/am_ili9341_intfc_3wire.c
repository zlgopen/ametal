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
 * \brief ili9341 三线接口驱动函数实现
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-27  vir, first implementation.
 * \endinternal
 */

#include "am_ili9341_intfc_3wire.h"
#include "am_gpio.h"

/**
 * \brief 在标准的SPI接口中不用片选信号，我们自己在调用时拉低片选即可
 */
static void __ili9341_no_use_cs(am_spi_device_t *p_dev, int pin)
{

}

/**
 * \brief 发送8位命令
 */
static  int __ili9341_3wire_cmd_write(void *p_drv, uint8_t cmd)
{
    int ret = AM_OK;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    am_ili9341_intfc_3wire_dev_t *p_dev = (am_ili9341_intfc_3wire_dev_t *)p_drv;
    const am_ili9341_intfc_3wire_devinfo_t *p_devinfo = p_dev->p_devinfo;

    /* 拉低片选  cs*/
    am_gpio_set( p_devinfo->cs_pin,AM_GPIO_LEVEL_LOW);

    /* 拉低mosi引脚 表示发送 命令 */
    am_gpio_pin_cfg(p_devinfo->mosi_pin,AM_GPIO_OUTPUT_INIT_LOW );

    /* 发送一个时钟沿 */
    am_gpio_pin_cfg(p_devinfo->sck_pin,AM_GPIO_OUTPUT_INIT_LOW | AM_GPIO_PUSH_PULL);
    am_gpio_set(p_devinfo->sck_pin, AM_GPIO_LEVEL_HIGH);
    am_gpio_set(p_devinfo->sck_pin, AM_GPIO_LEVEL_LOW);

    /* 将命令发送出去 */
    am_gpio_pin_cfg(p_devinfo->sck_pin, p_devinfo->sck_func );
    am_gpio_pin_cfg(p_devinfo->mosi_pin, p_devinfo->mosi_func );
    ret = am_spi_write_then_read (&(p_dev->spi_dev),
                                  &cmd,
                                  1,
                                  NULL,
                                  0);

    /* 拉高片选  cs*/
    am_gpio_set(p_devinfo->cs_pin, AM_GPIO_LEVEL_HIGH);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;

}

/**
 * \brief 发送8位数据
 */
static int __ili9341_3wire_data_u8_write(void *p_drv,uint8_t data)
{
    int ret = AM_OK;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    am_ili9341_intfc_3wire_dev_t *p_dev = (am_ili9341_intfc_3wire_dev_t *)p_drv;
    const am_ili9341_intfc_3wire_devinfo_t *p_devinfo = p_dev->p_devinfo;

    /* 拉低片选  cs*/
    am_gpio_set( p_devinfo->cs_pin,AM_GPIO_LEVEL_LOW);

    /* 拉高 mosi引脚 表示发送 数据 */
    am_gpio_pin_cfg(p_devinfo->mosi_pin,AM_GPIO_OUTPUT_INIT_HIGH );

    /* 发送一个时钟沿 */
    am_gpio_pin_cfg(p_devinfo->sck_pin,AM_GPIO_OUTPUT_INIT_LOW | AM_GPIO_PUSH_PULL);
    am_gpio_set(p_devinfo->sck_pin, AM_GPIO_LEVEL_HIGH);
    am_gpio_set(p_devinfo->sck_pin, AM_GPIO_LEVEL_LOW);

    /* 将数据发送出去 */
    am_gpio_pin_cfg(p_devinfo->sck_pin, p_devinfo->sck_func  );
    am_gpio_pin_cfg(p_devinfo->mosi_pin, p_devinfo->mosi_func);
    ret = am_spi_write_then_read (&(p_dev->spi_dev),
                                  &data,
                                  1,
                                  NULL,
                                  0);

    /* 拉高片选  cs*/
    am_gpio_set(p_devinfo->cs_pin, AM_GPIO_LEVEL_HIGH);

    if (AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}

/**
 * \brief 发送16位数据
 */
static int __ili9341_3wire_data_u16_write(void *p_drv,uint16_t data)
{
    uint8_t data1 = (uint8_t)(data >> 8);
    uint8_t data2 = (uint8_t) data;

    __ili9341_3wire_data_u8_write(p_drv, data1);
    __ili9341_3wire_data_u8_write(p_drv, data2);
    return AM_OK;
}

static struct am_ili9341_drv_funcs __g_ili9341_funcs = {
        __ili9341_3wire_cmd_write,
        __ili9341_3wire_data_u8_write,
        __ili9341_3wire_data_u16_write,
        NULL,
        NULL,
};


am_ili9341_serv_t * am_ili9341_intfc_3wire_init(am_ili9341_intfc_3wire_dev_t *p_dev,
                             const am_ili9341_intfc_3wire_devinfo_t *p_devinfo,
                             am_spi_handle_t                         spi_handle)
{
    int ret = AM_OK;

    p_dev->serv.p_funcs = &__g_ili9341_funcs;
    p_dev->serv.p_drv   = p_dev;

    p_dev->p_devinfo = p_devinfo;

    /* 构造spi设备 */
    am_spi_mkdev(&p_dev->spi_dev,
                  spi_handle,
                  8,                    /* 字大小为8-bit */
                  AM_SPI_MODE_0,
                  p_devinfo->max_speed_hz,
                  p_dev->p_devinfo->cs_pin,
                  __ili9341_no_use_cs);
    /* 设置设备 */
    ret = am_spi_setup( &(p_dev->spi_dev));
    if ( ret != AM_OK ) {
        return NULL;
    }

    return &p_dev->serv;
}

/* end of file */
