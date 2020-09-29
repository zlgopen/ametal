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
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 使输入引脚为低电平。
 *
 * - 实验现象：
 *   2. 输入引脚为低电平时，输出引脚状态翻转。
 *
 * \par 源代码
 * \snippet demo_zsl42x_hw_gpio.c src_zsl42x_hw_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-20  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_gpio
 * \copydoc demo_zsl42x_hw_gpio.c
 */

/** [src_zsl42x_hw_gpio] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "hw/amhw_zsl42x_gpio.h"

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 */
void demo_zsl42x_hw_gpio_entry (void    *p_hw_gpio,
                              int32_t  input_pin,
                              int32_t  output_pin)
{
	amhw_zsl42x_gpio_t *p_gpio = (amhw_zsl42x_gpio_t *)p_hw_gpio;

    uint32_t dir_input = 0;

    /* 取消引脚的复用功能 */
    amhw_zsl42x_gpio_afio_set(p_gpio, AMHW_ZSL42x_AFIO_NO, input_pin);

    /* 数字端口设置*/
    amhw_zsl42x_gpio_pin_digital_set(p_gpio, input_pin);

    /* 引脚输入 */
    amhw_zsl42x_gpio_pin_dir_input(p_gpio, input_pin);

    /* 使能引脚的上拉电阻 */
    amhw_zsl42x_gpio_pin_pu_enable(p_gpio, input_pin);

    /* 取消引脚的复用功能 */
    amhw_zsl42x_gpio_afio_set(p_gpio, AMHW_ZSL42x_AFIO_NO, output_pin);

    /* 数字端口设置*/
    amhw_zsl42x_gpio_pin_digital_set(p_gpio, output_pin);

    /* 引脚输出 */
    amhw_zsl42x_gpio_pin_dir_output(p_gpio, output_pin);

    /* 驱动能力强弱设置 */
    amhw_zsl42x_gpio_pin_driver_high(p_gpio, output_pin);

    /* 使能引脚的上拉电阻 */
    amhw_zsl42x_gpio_pin_pu_enable(p_gpio, output_pin);

    while (1) {

        dir_input = amhw_zsl42x_gpio_pin_input_get(p_gpio, input_pin);

        if (dir_input == 0) {
            am_gpio_toggle(output_pin);
            am_mdelay(500);
        }
    }
}
/** [src_zsl42x_hw_gpio] */

/* end of file */
