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
 * \brief 高光谱传感器 AS7421 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 在中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

#include "am_gpio.h"
#include "am_delay.h"
#include "am_stm32f103rbt6.h"
#include "am_sensor.h"
#include "am_hwconf_sensor_as7421.h"
#include "demo_std_entries.h"
#include "demo_stm32f103rbt6_core_entries.h"

/* 3.3V_LED_EN */
#define PIN_3P3_LED_EN      PIOA_8

/**
 * \brief 传感器例程入口
 */
void demo_stm32f103rbt6_core_sensor_as7421_int_entry (void)
{
    /* 使能 3.3V_LED_EN 引脚 */
    am_gpio_pin_cfg(PIN_3P3_LED_EN, AM_GPIO_OUTPUT_INIT_HIGH);

    /* 等待供电稳定 */
    am_mdelay(2);

    am_sensor_handle_t handle = am_sensor_as7421_inst_init();

    demo_std_as7421_int_entry(handle);
}


/* end of file */
