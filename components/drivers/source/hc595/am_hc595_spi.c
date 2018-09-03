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
 * \brief HC595标准设备实现（SPI驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-23  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_hc595_spi.h"
#include "am_gpio.h"
#include "am_spi.h"

/*******************************************************************************
  standard driver functions
*******************************************************************************/

static int __hc595_spi_enable (void *p_cookie)
{
    am_hc595_spi_dev_t *p_dev = (am_hc595_spi_dev_t *)p_cookie;

    if (p_dev->p_info->pin_oe == -1) {
        return AM_OK;                      /* always is enabled */
    }

    return am_gpio_set(p_dev->p_info->pin_oe, 0);
}

/******************************************************************************/
static int __hc595_spi_disable (void *p_cookie)
{
    am_hc595_spi_dev_t *p_dev = (am_hc595_spi_dev_t *)p_cookie;

    if (p_dev->p_info->pin_oe == -1) {
        return -AM_ENOTSUP;
    }

    return am_gpio_set(p_dev->p_info->pin_oe, 1);
}

/******************************************************************************/
static int __hc595_spi_data_send (void       *p_cookie,
                                  const void *p_data,
                                  size_t      nbytes)
{
    am_hc595_spi_dev_t *p_dev = (am_hc595_spi_dev_t *)p_cookie;

    return am_spi_write_then_write(&(p_dev->spi_dev), p_data, nbytes, NULL, 0);
}

/******************************************************************************/
static const struct am_hc595_drv_funcs __g_hc595_spi_drv_funcs = {
    __hc595_spi_enable,
    __hc595_spi_disable,
    __hc595_spi_data_send,
};

/*******************************************************************************
  Public functions
*******************************************************************************/
am_hc595_handle_t am_hc595_spi_init (am_hc595_spi_dev_t        *p_dev,
                                     const am_hc595_spi_info_t *p_info,
                                     am_spi_handle_t            handle)
{
    if ((p_dev == NULL) || (handle == NULL) || (p_info == NULL)) {
        return NULL;
    }

    am_spi_mkdev(&(p_dev->spi_dev),
                 handle,
                 8,
                 AM_SPI_MODE_3 | ((p_info->lsb_first) ? AM_SPI_LSB_FIRST : 0),
                 p_info->clk_speed,
                 p_info->pin_lock,
                 NULL);


    if (am_spi_setup(&(p_dev->spi_dev)) < 0) {
        return NULL;
    }

    p_dev->p_info       = p_info;
    p_dev->isa.p_funcs  = &__g_hc595_spi_drv_funcs;
    p_dev->isa.p_cookie = p_dev;

    return &p_dev->isa;
}

/******************************************************************************/
int am_hc595_spi_deinit (am_hc595_spi_dev_t *p_dev)
{
    return AM_OK;
}

/* end of file */
