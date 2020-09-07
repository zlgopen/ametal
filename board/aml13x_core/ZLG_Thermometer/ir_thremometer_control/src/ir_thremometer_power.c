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
 * \brief 电源控制
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "ir_thremometer_power.h"

#define IR_THREM_POWER_PIN  PIOA_7
/** \brief 电源接管IO初始化*/
void ir_thremometer_power_init(void)
{
	am_gpio_pin_cfg(IR_THREM_POWER_PIN, AM_GPIO_OUTPUT_INIT_HIGH);
}
/** \brief 打开电源*/
void ir_thremometer_power_on(void)
{
	am_gpio_set(IR_THREM_POWER_PIN, 1);
}
/** \brief 关闭电源*/
void ir_thremometer_power_off(void)
{
	am_gpio_set(IR_THREM_POWER_PIN, 0);
}
/* end of file */
