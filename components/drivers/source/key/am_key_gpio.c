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
 * \brief 独立按键驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-15  tee, first implementation.
 * \endinternal
 */
#include "am_key_gpio.h"
#include "am_gpio.h"
#include "am_input.h"

/*******************************************************************************
   Local Functions
*******************************************************************************/

static uint32_t __key_val_read (am_key_gpio_t *p_dev)
{
    int       i;
    uint32_t  val = 0x00;

    for (i = 0; i < p_dev->p_info->pin_num; i++) {
        if (am_gpio_get(p_dev->p_info->p_pins[i])) {
            val |= (1 << i);
        }
    }

    return val;
}

/******************************************************************************/
static void __key_gpio_timer_cb (void *p_arg)
{
    am_key_gpio_t *p_dev     = (am_key_gpio_t *)p_arg;
    uint32_t       key_value = __key_val_read(p_dev);
    uint32_t       key_change;
    int            i;

    if (p_dev->key_prev == key_value) {

        if (key_value != p_dev->key_press) {

            key_change = p_dev->key_press ^ key_value;

            for (i = 0; i < p_dev->p_info->pin_num; i++) {

                if (key_change & (1u << i)) {

                    if (!!(key_value & (1u << i)) == !!p_dev->p_info->active_low) {
                        am_input_key_report(p_dev->p_info->p_codes[i],
                                            AM_INPUT_KEY_STATE_RELEASED);
                    } else {
                        am_input_key_report(p_dev->p_info->p_codes[i],
                                            AM_INPUT_KEY_STATE_PRESSED);
                    }
                }
            }

            p_dev->key_press = key_value;
        }
    }

    p_dev->key_prev = key_value;
}

/*******************************************************************************/
int am_key_gpio_init (am_key_gpio_t *p_dev, const am_key_gpio_info_t *p_info)
{
    int i;

    if ((p_dev == NULL) || (p_info == NULL)) {
        return -AM_EINVAL;
    }

    if (p_info->pin_num > 32) {
        return -AM_ENOTSUP;
    }

    if (p_info->active_low) {

        p_dev->key_prev = p_dev->key_press = 0xFFFFFFFF;

        for (i = 0; i < p_info->pin_num; i++) {
            am_gpio_pin_cfg(p_info->p_pins[i], AM_GPIO_INPUT | AM_GPIO_PULLUP);
        }
    } else {

        p_dev->key_prev = p_dev->key_press = 0x00000000;

        for (i = 0; i < p_info->pin_num; i++) {
            am_gpio_pin_cfg(p_info->p_pins[i], AM_GPIO_INPUT | AM_GPIO_PULLDOWN);
        }
    }

    p_dev->p_info = p_info;

    am_softimer_init(&p_dev->timer, __key_gpio_timer_cb, p_dev);
    am_softimer_start(&p_dev->timer, p_info->scan_interval_ms);

    return AM_OK;
}

/* end of file */
