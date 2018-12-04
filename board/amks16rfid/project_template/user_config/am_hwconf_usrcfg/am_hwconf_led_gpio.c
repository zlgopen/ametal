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
 * \brief GPIO LED配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-06-01  nwt, first implementation.
 * \endinternal
 */ 
#include "ametal.h"
#include "am_led_gpio.h"
#include "kl26_pin.h"
#include "am_input.h"
#include "am_board.h"

/**
 * \addtogroup am_if_src_hwconf_led_gpio
 * \copydoc am_hwconf_led_gpio.c
 * @{
 */

/*******************************************************************************
  板上LED相关信息
*******************************************************************************/

/** \brief 板上LED对应的led引脚  */
static const int __g_led_pin[ ] = {PIOC_4, PIOC_8};

/** \brief 板上LED的GPIO管脚和是否低电平点亮 */
static const am_led_gpio_info_t __g_led_gpio_info = {
     {LED0, LED1},
	 __g_led_pin,
     AM_TRUE
};

/* 定义GPIO LED 实例        */
static am_led_gpio_dev_t __g_led_gpio_dev;

/** \brief GPIO LED instance init */
int am_led_gpio_inst_init (void)
{
    return am_led_gpio_init(&__g_led_gpio_dev, &__g_led_gpio_info);
}

/**
 * @}
 */

/* end of file */
