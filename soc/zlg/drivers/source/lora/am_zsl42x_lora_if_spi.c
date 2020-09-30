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
 * \file zsl42x lora spi
 *
 * \internal
 * \par Modification history
 * - 1.0.0 20-03-25  enf,first implementation.
 * - 1.0.1 20-09-23  yg, adaptation zsl42x.
 * \endinternal
 */

#include "ametal.h"
#include "am_gpio.h"
#include "am_spi.h"
#include "stddef.h"
#include "am_types.h"
#include "am_int.h"
#include "am_delay.h"

#include "am_zsl42x_lora_if_spi.h"

#include "am_zsl42x.h"


#if 0

/** \brief 用于控制IO模拟的SPI速率，根据不同的MCU速度来调整；当前为MCU主频为xMHz */
#define __ZSL42X_LORA_SPI_TIME        48

am_local void __delay_us (unsigned int i)
{
    unsigned int j, k;

    for (j = 0; j < 1; j++) {
        for (k = 0; k < i; k++);
    }
}

am_local void __zsl42x_lora_sw_spi_write (uint8_t send_data)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {

        __delay_us(__ZSL42X_LORA_SPI_TIME);
        am_gpio_set(PIOA_5, 0);  //SCK:0

        if((send_data << i) & 0x80){
            __delay_us(__ZSL42X_LORA_SPI_TIME);
            am_gpio_set(PIOA_6, 1);  //MOSI:1
        } else {
            __delay_us(__ZSL42X_LORA_SPI_TIME);
            am_gpio_set(PIOA_6, 0);  //MOSI:0
        }

        __delay_us(__ZSL42X_LORA_SPI_TIME);
        am_gpio_set(PIOA_5, 1);  //SCK:1
    }

    __delay_us(__ZSL42X_LORA_SPI_TIME);

    am_gpio_set(PIOA_5, 0);  //SCK:0
    am_gpio_set(PIOA_6, 0);  //MOSI:0

}

am_local uint8_t __zsl42x_lora_sw_spi_read (void)
{
    uint8_t   i, temp = 0;

    for (i = 0; i < 8; i++) {

        am_gpio_set(PIOA_5, 0);  //SCK:0
        __delay_us(__ZSL42X_LORA_SPI_TIME);

        temp = (temp << 1);

        am_gpio_set(PIOA_5, 1);  //SCK:1
        __delay_us(__ZSL42X_LORA_SPI_TIME);

        if (am_gpio_get(PIOA_7)) {  //MISO
            temp++;
        } else {
            temp = temp + 0;
        }

        __delay_us(__ZSL42X_LORA_SPI_TIME);
    }

    am_gpio_set(PIOA_5, 0);  //SCK:0
    __delay_us(__ZSL42X_LORA_SPI_TIME);

    return temp;
}

/**
 * \brief ZSL42X LORA SPI写寄存器
 */
am_local int __zsl42x_lora_if_spi_write (void           *p_drv,
                                         uint8_t        *p_opcode,
                                         uint32_t        opcode_size,
                                         const uint8_t  *p_buffer,
                                         uint32_t        size)
{
    am_zsl42x_lora_dev_t  *p_zsl42x_lora_dev = (am_zsl42x_lora_dev_t *)p_drv;
    uint8_t                i                 = 0;

    am_gpio_set(p_zsl42x_lora_dev->p_devinfo->nss_pin,
                ZSL42X_LORA_SPI_CS_ENABLE);

    for(i = 0; i < opcode_size; i++) {
        __zsl42x_lora_sw_spi_write(p_opcode[i]);
    }

    for(i = 0; i < size; i++) {
        __zsl42x_lora_sw_spi_write(p_buffer[i]);
    }

    am_gpio_set(p_zsl42x_lora_dev->p_devinfo->nss_pin,
                ZSL42X_LORA_SPI_CS_DISABLE);

    return AM_ZSL42X_LORA_RET_OK;
}

/**
 * \brief ZSL42X LORA SPI读寄存器
 */
am_local int __zsl42x_lora_if_spi_read (void     *p_drv,
                                        uint8_t  *p_opcode,
                                        uint32_t  opcode_size,
                                        uint8_t  *p_buffer,
                                        uint32_t  size)
{
    am_zsl42x_lora_dev_t *p_zsl42x_lora_dev = (am_zsl42x_lora_dev_t *)p_drv;
    uint8_t               i                 = 0;

    am_gpio_set(p_zsl42x_lora_dev->p_devinfo->nss_pin,
                ZSL42X_LORA_SPI_CS_ENABLE);

    for(i = 0; i < opcode_size; i++) {
        __zsl42x_lora_sw_spi_write(p_opcode[i]);
    }

    for(i = 0; i < size; i++) {
        p_buffer[i] = __zsl42x_lora_sw_spi_read();
    }

    am_gpio_set(p_zsl42x_lora_dev->p_devinfo->nss_pin,
                ZSL42X_LORA_SPI_CS_DISABLE);

    return AM_ZSL42X_LORA_RET_OK;
}

am_local int __zsl42x_lora_buffer_read (am_zsl42x_lora_dev_t  *p_dev,
                                        uint8_t               *p_addr,
                                        uint32_t               addr_len,
                                        uint8_t               *p_buffer,
                                        uint32_t               size)
{

    return __zsl42x_lora_if_spi_read((void*)p_dev,
                                     p_addr,
                                     addr_len,
                                     p_buffer,
                                     size);
}

am_local int __zsl42x_lora_buffer_write (am_zsl42x_lora_dev_t  *p_dev,
                                         uint8_t               *p_addr,
                                         uint32_t               addr_len,
                                         const uint8_t         *p_buffer,
                                         uint32_t               size)
{
    return __zsl42x_lora_if_spi_write((void *)p_dev,
                                      p_addr,
                                      addr_len,
                                      p_buffer,
                                      size);
}

am_local int __zsl42x_lora_drv_reset (void *p_drv)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;
    am_mdelay(10);
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->reset_pin, 0);
    am_mdelay(20);
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->reset_pin, 1);
    am_mdelay(10);

    return AM_OK;
}

/** \brief zsl42x lora drv_funcs */
am_local am_zsl42x_lora_drv_funcs_t __g_drv_funcs = {
    NULL,
    NULL,
    __zsl42x_lora_drv_reset,
    __zsl42x_lora_buffer_read,
    __zsl42x_lora_buffer_write
};

/**
 * \brief ZSL42X LORA SPI适配初始化
 */
am_zsl42x_lora_server_handle_t
am_zsl42x_lora_if_spi_init (am_zsl42x_lora_if_spi_dev_t        *p_dev,
                      const am_zsl42x_lora_if_spi_devinfo_t    *p_devinfo)
{
    if ((p_dev == NULL) || (p_devinfo == NULL)) {
        return NULL;
    }

    p_dev->p_spi_devinfo = p_devinfo;

    if (p_dev->p_spi_devinfo->pfn_plfm_init) {
        p_dev->p_spi_devinfo->pfn_plfm_init();
    }

    /* 模拟spi 方式 */
    am_gpio_pin_cfg(PIOA_6,
                    PIOA_6_GPIO | AM_GPIO_PULLDOWN | AM_GPIO_OUTPUT_INIT_LOW);

    am_gpio_pin_cfg(PIOA_7, PIOA_7_GPIO | AM_GPIO_PULLDOWN | AM_GPIO_INPUT);

    am_gpio_pin_cfg(PIOA_5,
                    PIOA_5_GPIO | AM_GPIO_PULLDOWN | AM_GPIO_OUTPUT_INIT_LOW);

    am_gpio_pin_cfg(PIOA_15,
                    PIOA_15_GPIO | AM_GPIO_PULLUP | AM_GPIO_OUTPUT_INIT_HIGH);

    am_gpio_pin_cfg(PIOC_10,
                    PIOC_10_GPIO | AM_GPIO_PULLUP | AM_GPIO_OUTPUT_INIT_HIGH);

    am_gpio_pin_cfg(PIOC_11,
                    PIOC_11_GPIO | AM_GPIO_PULLDOWN | AM_GPIO_INPUT);

    am_gpio_pin_cfg(PIOC_12,
                    PIOC_12_GPIO | AM_GPIO_INPUT  | AM_GPIO_PULLDOWN);

    return am_zsl42x_lora_init(&p_dev->zsl42x_lora_dev,
                               &p_devinfo->zsl42x_lora_devinfo,
                               &__g_drv_funcs);
}

/**
 * \brief ZSL42X LORA SPI适配去初始化
 */
int am_zsl42x_lora_if_spi_deinit (am_zsl42x_lora_server_handle_t handle)
{
    am_zsl42x_lora_if_spi_dev_t *p_dev = (am_zsl42x_lora_if_spi_dev_t *)handle;

    if (p_dev->p_spi_devinfo->pfn_plfm_deinit) {
        p_dev->p_spi_devinfo->pfn_plfm_deinit();
    }

    /* 模拟spi 方式 */
    am_gpio_pin_cfg(PIOA_5,  AM_GPIO_INPUT | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(PIOA_7,  AM_GPIO_INPUT | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(PIOA_6,  AM_GPIO_OUTPUT_INIT_HIGH);
    am_gpio_pin_cfg(PIOA_15, AM_GPIO_OUTPUT_INIT_HIGH);

    am_zsl42x_lora_deinit(handle);

    return AM_ZSL42X_LORA_RET_OK;
}

#else

am_local void __spi_poll_strans (amhw_zsl42x_spi_t *p_hw_spi,
                                 uint32_t         nbtyes,
                                 const uint8_t   *p_txbuf,
                                 uint8_t         *p_rxbuf)
{
    uint8_t  cur_recv   = 0;
    uint32_t cur_nbtyes = 0;

    while (cur_nbtyes < nbtyes) {

        while(amhw_zsl42x_spi_flag_check (p_hw_spi,
                                          AMHW_ZSL42x_SPI_FLAG_TX_EMPTY) ==
                                          AM_FALSE);

        amhw_zsl42x_spi_tx_data8_put(p_hw_spi,
                                    (p_txbuf ? (p_txbuf[cur_nbtyes]) : (0X00)),
                                     AM_FALSE);

        while (amhw_zsl42x_spi_flag_check (p_hw_spi,
                                           AMHW_ZSL42x_SPI_FLAG_RX_NOT_EMPTY) ==
                                           AM_FALSE);

        cur_recv = amhw_zsl42x_spi_rx_data8_get(p_hw_spi, AM_FALSE);

        if (p_rxbuf) {
            p_rxbuf[cur_nbtyes] = cur_recv;
        }
        cur_nbtyes++;
    }
}

/**
 * \brief ZSL42X LORA SPI写寄存器
 */
am_local int __zsl42x_lora_if_spi_write (am_zsl42x_lora_dev_t  *p_drv,
                                         uint8_t               *p_opcode,
                                         uint32_t               opcode_size,
                                         const uint8_t         *p_buffer,
                                         uint32_t               size)
{
    int                     key;
    am_zsl42x_lora_if_spi_dev_t *p_dev    = \
        AM_ZSL42X_LORA_CONTAINER_OF(p_drv,
                                    am_zsl42x_lora_if_spi_dev_t,
                                    zsl42x_lora_dev);

    amhw_zsl42x_spi_t        *p_hw_spi = \
        (amhw_zsl42x_spi_t *)p_dev->p_spi_devinfo->spi_rebase;

    if (NULL == p_dev) {
        return AM_ZSL42X_LORA_RET_EINVAL;
    }

    key = am_int_cpu_lock();

    am_gpio_set(p_dev->p_spi_devinfo->zsl42x_lora_devinfo.nss_pin,
                ZSL42X_LORA_SPI_CS_ENABLE);

    amhw_zsl42x_spi_cs_sel(p_hw_spi, AM_TRUE);

    __spi_poll_strans(p_hw_spi, opcode_size, p_opcode, NULL);

    /* 防止只有操作码，没有数据的命令漏写  */
    if ( NULL != p_buffer || 0 != size) {
        __spi_poll_strans(p_hw_spi, size, p_buffer, NULL);
    }

    am_gpio_set(p_dev->p_spi_devinfo->zsl42x_lora_devinfo.nss_pin,
                ZSL42X_LORA_SPI_CS_DISABLE);

    amhw_zsl42x_spi_cs_sel(p_hw_spi, AM_FALSE);

    am_int_cpu_unlock(key);

    return AM_ZSL42X_LORA_RET_OK;
}

/**
 * \brief ZSL42X LORA SPI读寄存器
 */
int __zsl42x_lora_if_spi_read (am_zsl42x_lora_dev_t  *p_drv,
                               uint8_t               *p_opcode,
                               uint32_t               opcode_size,
                               uint8_t               *p_buffer,
                               uint32_t               size)
{
    int                     key;
    am_zsl42x_lora_if_spi_dev_t *p_dev    = \
        AM_ZSL42X_LORA_CONTAINER_OF(p_drv,
                                    am_zsl42x_lora_if_spi_dev_t,
                                    zsl42x_lora_dev);

    amhw_zsl42x_spi_t        *p_hw_spi = \
        (amhw_zsl42x_spi_t *)p_dev->p_spi_devinfo->spi_rebase;

    if (NULL == p_dev || NULL == p_buffer || 0 == size) {
        return AM_ZSL42X_LORA_RET_EINVAL;
    }

    key = am_int_cpu_lock();

    am_gpio_set(p_dev->p_spi_devinfo->zsl42x_lora_devinfo.nss_pin,
                ZSL42X_LORA_SPI_CS_ENABLE);

    amhw_zsl42x_spi_cs_sel(p_hw_spi,AM_TRUE);

    __spi_poll_strans(p_hw_spi, opcode_size, p_opcode, NULL);
    __spi_poll_strans(p_hw_spi, size, NULL, p_buffer);

    am_gpio_set(p_dev->p_spi_devinfo->zsl42x_lora_devinfo.nss_pin,
                ZSL42X_LORA_SPI_CS_DISABLE);

    amhw_zsl42x_spi_cs_sel(p_hw_spi, AM_FALSE);

    am_int_cpu_unlock(key);

    return AM_ZSL42X_LORA_RET_OK;
}

am_local int __zsl42x_lora_drv_reset (void *p_drv)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    am_mdelay(10);
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->reset_pin, 0);
    am_mdelay(20);
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->reset_pin, 1);
    am_mdelay(10);

    return AM_OK;
}

/** \brief zsl42x lora drv_funcs */
am_local am_zsl42x_lora_drv_funcs_t __g_drv_funcs = {
    NULL,
    NULL,
    __zsl42x_lora_drv_reset,
    __zsl42x_lora_if_spi_read,
    __zsl42x_lora_if_spi_write
};

am_zsl42x_lora_handle_t
am_zsl42x_lora_if_spi_init (am_zsl42x_lora_if_spi_dev_t       *p_dev,
                      const am_zsl42x_lora_if_spi_devinfo_t   *p_devinfo)
{
    amhw_zsl42x_spi_t *p_hw_spi = NULL;

    if (NULL == p_dev || NULL == p_devinfo) {
        return NULL;
    }

    p_hw_spi = (amhw_zsl42x_spi_t *) p_devinfo->spi_rebase;

    p_dev->p_spi_devinfo = p_devinfo;

    if (p_dev->p_spi_devinfo->pfn_plfm_init) {
        p_dev->p_spi_devinfo->pfn_plfm_init();
    }

    /* 配置为主机模式 */
    amhw_zsl42x_spi_mode_sel(p_hw_spi, AMHW_ZSL42x_SPI_MODE_TYPE_MASTER);

    /* 配置SPI模式 */
    amhw_zsl42x_spi_clk_mode_set(p_hw_spi, AMHW_ZSL42x_SPI_CLK_MODE_0);

    /* 设置SPI速率 */
    amhw_zsl42x_spi_baud_rate_set(p_hw_spi, AMHW_ZSL42x_SPI_BAUD_RATE_PCLK_DIV4);

    /* SPI使能 */
    amhw_zsl42x_spi_enable(p_hw_spi, AM_TRUE);

    return am_zsl42x_lora_init(&p_dev->zsl42x_lora_dev,
                               &p_devinfo->zsl42x_lora_devinfo,
                               &__g_drv_funcs);
}

/**
 * \brief ZSL42X LORA SPI适配去初始化
 */
int am_zsl42x_lora_if_spi_deinit (am_zsl42x_lora_handle_t handle)
{
    am_zsl42x_lora_if_spi_dev_t *p_dev = (am_zsl42x_lora_if_spi_dev_t *)handle;

    if (p_dev->p_spi_devinfo->pfn_plfm_deinit) {
        p_dev->p_spi_devinfo->pfn_plfm_deinit();
    }

    am_zsl42x_lora_deinit(handle);

    return AM_ZSL42X_LORA_RET_OK;
}

#endif

/* end of file */
