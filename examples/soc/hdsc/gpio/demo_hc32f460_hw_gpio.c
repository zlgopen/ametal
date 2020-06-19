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
 * \snippet demo_zlg_hw_gpio.c src_zlg_hw_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg_hw_gpio
 * \copydoc demo_zlg_hw_gpio.c
 */

/** [src_zlg_hw_gpio] */
#include <hw/amhw_hc32f460_gpio.h>
#include "ametal.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"

enum
{
    LEVEL_LOW  = 0,
    LEVEL_HIGH
};

/**
 * \brief 例程入口
 */
void demo_hc32f460_hw_gpio_entry (amhw_hc32f460_gpio_t *p_hw_gpio,
                                  int32_t          input_pin,
                                  int32_t          output_pin)
{
    uint32_t level = 0;

    /* 取消引脚的复用功能 */
    amhw_hc32f460_gpio_pin_afr_set(p_hw_gpio, AMHW_HC32F460_AFIO_GPIO, input_pin);

    /* 数字端口设置*/
    amhw_hc32f460_gpio_pin_digital_set(p_hw_gpio, input_pin);

    /* 引脚输入 */
    amhw_hc32f460_gpio_pin_dir_input(p_hw_gpio, input_pin);

    /* 使能引脚的上拉电阻 */
    amhw_hc32f460_gpio_pin_pu_enable(p_hw_gpio, input_pin);

    /* 取消引脚的复用功能 */
    amhw_hc32f460_gpio_pin_afr_set(p_hw_gpio, AMHW_HC32F460_AFIO_GPIO, output_pin);

    /* 数字端口设置*/
    amhw_hc32f460_gpio_pin_digital_set(p_hw_gpio, output_pin);

    /* 引脚输出 */
    amhw_hc32f460_gpio_pin_dir_output(p_hw_gpio, output_pin);

    /* 驱动能力强弱设置 */
    amhw_hc32f460_gpio_pin_driver_capability(p_hw_gpio, AMHW_HC32F460_GPIO_SPEED_LOW, output_pin);

    /* 使能引脚的上拉电阻 */
    amhw_hc32f460_gpio_pin_pu_enable(p_hw_gpio, output_pin);

    amhw_hc32f460_gpio_pin_out_high(p_hw_gpio, output_pin);
    amhw_hc32f460_gpio_pin_out_low(p_hw_gpio, output_pin);

    while (1) {
        level = amhw_hc32f460_gpio_pin_get(p_hw_gpio, input_pin);
        if (LEVEL_LOW == level) {
            amhw_hc32f460_gpio_pin_toggle(p_hw_gpio, output_pin);
            am_mdelay(500);
        }
    }
}
/** [src_zlg_hw_gpio] */

/* end of file */
