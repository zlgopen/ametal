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
 * \brief LED 用户配置文件（GPIO直接驱动）
 * \sa am_hwconf_hc32l17x_led_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  tee, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_led_gpio.h"
#include "hc32_pin.h"
#include "am_input.h"

/**
 * \addtogroup am_if_src_hwconf_hc32l17x_led_gpio
 * \copydoc am_hwconf_hc32l17x_led_gpio.c
 * @{
 */

/** \brief 定义 LED 相关的 GPIO 管脚信息 */
static const int __g_hc32l17x_led_pins[] = {PIOD_14};

/** \brief 设备信息 */
static const am_led_gpio_info_t __g_hc32l17x_led_gpio_info = {
    {
        0,                             /* 起始编号 */
        AM_NELEMENTS(__g_hc32l17x_led_pins) - 1 /* 结束编号 */
    },
    __g_hc32l17x_led_pins,
    AM_TRUE
};

/** \brief 设备实例 */
static am_led_gpio_dev_t  __g_hc32l17x_led_gpio;

/** \brief 实例初始化函数 */
int am_hc32_led_gpio_inst_init (void)
{
    return am_led_gpio_init(&__g_hc32l17x_led_gpio, &__g_hc32l17x_led_gpio_info);
}

/**
 * @}
 */

/* end of file */
