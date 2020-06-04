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
 * \brief LED 配置文件（GPIO直接驱动）
 * \sa am_hwconf_led_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_led_gpio.h"
#include "zml165_pin.h"
#include "am_input.h"

/**
 * \addtogroup am_if_src_hwconf_led_gpio
 * \copydoc am_hwconf_led_gpio.c
 * @{
 */

/* 定义GPIO LED 实例 */
static am_led_gpio_dev_t  __g_led_gpio;

static const int __g_led_pins[] = {PIOB_4, PIOB_3};

/* 定义GPIO按键实例信息 */
static const am_led_gpio_info_t __g_led_gpio_info = {
    {
        0,                            /* 起始编号0 */
        1                             /* 结束编号1，共计2个LED */
    },
    __g_led_pins,
    AM_TRUE
};

int am_led_gpio_inst_init (void)
{
    return am_led_gpio_init(&__g_led_gpio, &__g_led_gpio_info);
}

/**
 * @}
 */

/* end of file */
