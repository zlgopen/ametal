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
 * \snippet demo_mm32_hw_gpio.c src_mm32_hw_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32_hw_gpio
 * \copydoc demo_mm32_hw_gpio.c
 */

/** [src_mm32_hw_gpio] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "hw/amhw_mm32_gpio.h"

/**
 * \brief 例程入口
 */
void demo_mm32_hw_gpio_entry (amhw_mm32_gpio_t *p_hw_gpio,
                             int32_t          input_pin,
                             int32_t          output_pin)
{
    uint32_t dir_input = 0;

    /* 取消引脚的复用功能 */
    amhw_mm32_gpio_pin_afr_set(p_hw_gpio, AMHW_MM32_GPIO_AF_DEFAULT, input_pin);

    /* 设置引脚的方向为输入及设置引脚的模式 */
    amhw_mm32_gpio_pin_set(p_hw_gpio, AMHW_MM32_GPIO_MODE_IPU << 2 | 0x00, input_pin);

    /* 使能引脚的上拉电阻 */
    amhw_mm32_gpio_pin_out_high(p_hw_gpio, input_pin);

    /* 取消引脚的复用功能 */
    amhw_mm32_gpio_pin_afr_set(p_hw_gpio, AMHW_MM32_GPIO_AF_DEFAULT, output_pin);

    /* 设置引脚的方向为输出及设置引脚的模式 */
    amhw_mm32_gpio_pin_set(p_hw_gpio, AMHW_MM32_GPIO_MODE_OUT_PP << 2 | 0x00, output_pin);

    /* 设置引脚的输出速度 */
    amhw_mm32_gpio_pin_dir_output(p_hw_gpio, AMHW_MM32_GPIO_SPEED_10MHz, output_pin);

    /* 使能引脚的上拉电阻 */
    amhw_mm32_gpio_pin_out_high(p_hw_gpio, output_pin);

    while (1) {

        dir_input = amhw_mm32_gpio_pin_get(p_hw_gpio, input_pin);

        if (dir_input == 0) {
            am_gpio_toggle(output_pin);
            am_mdelay(500);
        }
    }
}
/** [src_mm32_hw_gpio] */

/* end of file */
