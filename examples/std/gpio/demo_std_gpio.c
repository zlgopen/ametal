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
 * \brief GPIO 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 使输入引脚为低电平。
 *
 * - 实验现象：
 *   2. 输入引脚为低电平时，输出引脚状态翻转。
 *
 * \par 源代码
 * \snippet demo_std_gpio.c src_std_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_gpio
 * \copydoc demo_std_gpio.c
 */

/** [src_std_gpio] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_gpio_entry (int input_pin, int output_pin)
{
    uint8_t dir_input = 0;

    /* 一般默认按键对应的 GPIO 引脚作为输入 */
    am_gpio_pin_cfg(input_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);

    /* 一般默认 LED 对应的 GPIO 引脚作为输出 */
    am_gpio_pin_cfg(output_pin, AM_GPIO_OUTPUT_INIT_LOW | AM_GPIO_PULLUP);

    while (1) {

         dir_input = am_gpio_get(input_pin);
         if (dir_input == 0) {

             /*  消除抖动 */
             while (am_gpio_get(input_pin) == 0);
            
             /* 翻转output_pin */
             am_gpio_toggle(output_pin);
         }
    }
}
/** [src_std_gpio] */

/* end of file */
