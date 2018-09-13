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
 * \brief LED标准接口实现（GPIO驱动）
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-25  fft, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_led_gpio.h"
#include "am_gpio.h"

/*******************************************************************************
  Local functions
*******************************************************************************/ 

static int __led_gpio_set (void *p_cookie, int led_id, am_bool_t state)
{
    am_led_gpio_dev_t *p_dev = (am_led_gpio_dev_t *)p_cookie;

    led_id = led_id - p_dev->p_info->serv_info.start_id;

    am_gpio_set(p_dev->p_info->p_pins[led_id], state ^ p_dev->p_info->active_low);

    return AM_OK;
}

/******************************************************************************/
static int __led_gpio_toggle (void *p_cookie, int led_id)
{
    am_led_gpio_dev_t *p_dev = (am_led_gpio_dev_t *)p_cookie;

    led_id = led_id - p_dev->p_info->serv_info.start_id;

    am_gpio_toggle(p_dev->p_info->p_pins[led_id]);

    return AM_OK;
}

/******************************************************************************/

static const am_led_drv_funcs_t __g_led_gpio_drv_funcs = {
    __led_gpio_set,
    __led_gpio_toggle
};

/*******************************************************************************
  Public functions
*******************************************************************************/
int am_led_gpio_init (am_led_gpio_dev_t         *p_dev,
                      const am_led_gpio_info_t  *p_info)
{
    int i;
    int num;

    if ((p_dev == NULL) || (p_info == NULL)) {
        return -AM_EINVAL;
    }

    num = p_info->serv_info.end_id - p_info->serv_info.start_id + 1;

    if (num <= 0) {
        return -AM_EINVAL;
    }

    p_dev->p_info = p_info;

    for (i = 0; i < num; i++) {
        if (p_info->active_low) {
            am_gpio_pin_cfg(p_info->p_pins[i], AM_GPIO_OUTPUT_INIT_HIGH);
        } else {
            am_gpio_pin_cfg(p_info->p_pins[i], AM_GPIO_OUTPUT_INIT_LOW);
        }
    }

    return am_led_dev_add(&p_dev->isa,
                          &p_info->serv_info,
                           &__g_led_gpio_drv_funcs,
                           p_dev);
}

/******************************************************************************/
int am_led_gpio_deinit (am_led_gpio_dev_t *p_dev)
{
    int i;
    int num;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    num = p_dev->p_info->serv_info.end_id - p_dev->p_info->serv_info.start_id + 1;

    if (num <= 0) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {
        am_gpio_pin_cfg(p_dev->p_info->p_pins[i], AM_GPIO_INPUT | AM_GPIO_FLOAT);
    }

    return am_led_dev_del(&p_dev->isa);
}

/* end of file */
