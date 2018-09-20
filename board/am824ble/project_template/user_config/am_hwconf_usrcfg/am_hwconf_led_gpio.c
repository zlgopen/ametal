/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LED 用户配置文件（GPIO 驱动）
 * \sa am_hwconf_led_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  tee, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_input.h"
#include "am_led_gpio.h"
#include "am_lpc82x.h"

/**
 * \addtogroup am_if_src_hwconf_led_gpio
 * \copydoc am_hwconf_led_gpio.c
 * @{
 */

/** \brief LED 引脚号 */
am_local am_const int __g_led_pins[] = {
    PIO0_8,    /* LED0 引脚 */
    PIO0_9     /* LED1 引脚 */
};

/** \brief LED 设备信息 */
am_local am_const am_led_gpio_info_t __g_led_gpio_devinfo = {
    {
        0,                                /* LED 起始编号 */
        AM_NELEMENTS(__g_led_pins) - 1    /* LED 结束编号 */
    },
    __g_led_pins,
    AM_TRUE
};

/** \brief LED 设备实例 */
am_local am_led_gpio_dev_t __g_led_gpio_dev;

/**
 * \brief LED 实例初始化（GPIO 驱动）
 */
int am_led_gpio_inst_init (void)
{
    return am_led_gpio_init(&__g_led_gpio_dev, &__g_led_gpio_devinfo);
}

/**
 * @}
 */

/* end of file */
