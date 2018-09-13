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
 * \brief HC595标准设备实现（GPIO驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-23  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_hc595_gpio.h"
#include "am_gpio.h"
#include "am_delay.h"

/*******************************************************************************
  Local functions
*******************************************************************************/
static void __hc595_data_shift (am_hc595_gpio_dev_t *p_dev,
                                uint8_t              data)
{
    int     i;

    for (i = 0; i < 8; i++) {

        am_gpio_set(p_dev->p_info->clk_pin, 0);

        if (p_dev->p_info->lsb_first) {

            am_gpio_set(p_dev->p_info->data_pin, !!(data & (1 << i)));

        } else {

            am_gpio_set(p_dev->p_info->data_pin, !!(data & (1 << (7 - i))));
        }

        if ( p_dev->p_info->clk_low_udelay ) {
            am_udelay( p_dev->p_info->clk_low_udelay);
        }

        am_gpio_set(p_dev->p_info->clk_pin, 1);

        if ( p_dev->p_info->clk_high_udelay) {
            am_udelay( p_dev->p_info->clk_high_udelay);
        }
    }
}

/*******************************************************************************
  standard driver functions
*******************************************************************************/

static int __hc595_gpio_enable (void *p_drv)
{
    am_hc595_gpio_dev_t *p_dev = (am_hc595_gpio_dev_t *)p_drv;

    if (p_dev->p_info->oe_pin == -1) {
        return AM_OK;                      /* always is enabled */
    }

    return am_gpio_set(p_dev->p_info->oe_pin, 0);
}

/******************************************************************************/
static int __hc595_gpio_disable (void *p_drv)
{
    am_hc595_gpio_dev_t *p_dev = (am_hc595_gpio_dev_t *)p_drv;

    if (p_dev->p_info->oe_pin == -1) {
        return -AM_ENOTSUP;
    }

    return am_gpio_set(p_dev->p_info->oe_pin, 1);
}

/******************************************************************************/
static int __hc595_gpio_data_send (void       *p_cookie,
                                   const void *p_data,
                                   size_t      nbytes)
{
    am_hc595_gpio_dev_t *p_dev = (am_hc595_gpio_dev_t *)p_cookie;

    const uint8_t *p_buf = (const uint8_t *)p_data;
    int            i;

    am_gpio_set(p_dev->p_info->lock_pin, 0);

    for (i = 0; i < nbytes; i++) {
        __hc595_data_shift(p_dev, p_buf[i]);
    }

    am_gpio_set(p_dev->p_info->lock_pin, 1);

    return AM_OK;
}

/******************************************************************************/
static const struct am_hc595_drv_funcs __g_hc595_gpio_drv_funcs = {
    __hc595_gpio_enable,
    __hc595_gpio_disable,
    __hc595_gpio_data_send,
};

/*******************************************************************************
  Public functions
*******************************************************************************/

am_hc595_handle_t am_hc595_gpio_init (am_hc595_gpio_dev_t        *p_dev,
                                      const am_hc595_gpio_info_t *p_info)
{
    if ((p_dev == NULL) || (p_info == NULL)) {
        return NULL;
    }

    p_dev->p_info = p_info;

    am_gpio_pin_cfg(p_info->clk_pin, AM_GPIO_OUTPUT_INIT_HIGH);
    am_gpio_pin_cfg(p_info->data_pin, AM_GPIO_OUTPUT_INIT_HIGH);
    am_gpio_pin_cfg(p_info->lock_pin, AM_GPIO_OUTPUT_INIT_HIGH);
    if ( p_info->oe_pin  != -1 ) {
       am_gpio_pin_cfg(p_info->oe_pin, AM_GPIO_OUTPUT_INIT_LOW);
    }


    p_dev->isa.p_funcs  = &__g_hc595_gpio_drv_funcs;
    p_dev->isa.p_cookie = p_dev;

    return &p_dev->isa;
}

/******************************************************************************/
int am_hc595_gpio_deinit (am_hc595_gpio_dev_t *p_dev)
{
    return AM_OK;
}

/* end of file */
