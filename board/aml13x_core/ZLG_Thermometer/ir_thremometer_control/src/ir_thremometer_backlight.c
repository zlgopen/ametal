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
 * \brief 背光控制
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "ir_thremometer_backlight.h"

#define BACKLIGHT_1_PIN PIOC_12
#define BACKLIGHT_2_PIN PIOC_11
/** \brief 背光灯初始化*/
void ir_thremometer_backlight_init(void)
{
	am_gpio_pin_cfg(BACKLIGHT_1_PIN, AM_GPIO_OUTPUT_INIT_LOW);
	am_gpio_pin_cfg(BACKLIGHT_2_PIN, AM_GPIO_OUTPUT_INIT_LOW);
	ir_thremometer_backlight_on(BACKLIGNT_GREEN);
}
/** \brief 开启对应颜色背光灯*/
void ir_thremometer_backlight_on(backlight_mode_t mode)
{
	if (mode == BACKLIGNT_RED) {
		am_gpio_set(BACKLIGHT_1_PIN, 0);
		am_gpio_set(BACKLIGHT_2_PIN, 1);
	} else if (mode == BACKLIGNT_GREEN) {
		am_gpio_set(BACKLIGHT_1_PIN, 1);
		am_gpio_set(BACKLIGHT_2_PIN, 0);
	} else {
		am_gpio_set(BACKLIGHT_1_PIN, 1);
		am_gpio_set(BACKLIGHT_2_PIN, 1);
	}
}
/** \brief 关闭所有背光灯*/
void ir_thremometer_backlight_off(void)
{
	am_gpio_set(BACKLIGHT_1_PIN, 0);
	am_gpio_set(BACKLIGHT_2_PIN, 0);
}
/* end of file */
