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
 * \brief RCC
 *
 * 驱动直接使用了 ZLG116 外设寄存器基地址， 因此，该驱动仅适用于 ZLG116
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-06  zp, first implementation
 * \endinternal
 */
#include "hw/amhw_zlg118_gpio.h"
#include "am_gpio.h"

/**
 * \brief 设置GPIO引脚的模式
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] gpio_mode : 引脚的模式，值为 amhw_zlg217_gpiomode_t 这个枚举类型
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
void amhw_zlg118_gpio_pin_mode_set (amhw_zlg118_gpio_t     *p_hw_gpio,
                                    amhw_zlg118_gpiomode_t  gpio_mode,
                                    int                     pin)
{

    switch(gpio_mode) {

    /* 上拉输入 */
    case AMHW_ZLG118_GPIO_MODE_IPU:

        /* 数字端口/模拟端口设置*/
        amhw_zlg118_gpio_pin_digital_set(p_hw_gpio, pin);

        /* 输入输出方向设置 */
        amhw_zlg118_gpio_pin_dir_input(p_hw_gpio, pin);

        /* 驱动能力强弱设置 */
        amhw_zlg118_gpio_pin_driver_high(p_hw_gpio, pin);

        /* 上拉设置 */
        amhw_zlg118_gpio_pin_pu_enable(p_hw_gpio, pin);

        /* 下拉设置 */
        amhw_zlg118_gpio_pin_pd_disable(p_hw_gpio, pin);

        /* 开漏输出设置*/
        amhw_zlg118_gpio_pin_od_disable(p_hw_gpio, pin);
        break;

    /* 下拉输入 */
    case AMHW_ZLG118_GPIO_MODE_IPD:

        /* 数字端口/模拟端口设置*/
        amhw_zlg118_gpio_pin_digital_set(p_hw_gpio, pin);

        /* 输入输出方向设置 */
        amhw_zlg118_gpio_pin_dir_input(p_hw_gpio, pin);

        /* 驱动能力强弱设置 */
        amhw_zlg118_gpio_pin_driver_high(p_hw_gpio, pin);

        /* 上拉设置 */
        amhw_zlg118_gpio_pin_pu_disable(p_hw_gpio, pin);

        /* 下拉设置 */
        amhw_zlg118_gpio_pin_pd_enable(p_hw_gpio, pin);

        /* 开漏输出设置*/
        amhw_zlg118_gpio_pin_od_disable(p_hw_gpio, pin);
        break;

    /* 模拟输入 */
    case AMHW_ZLG118_GPIO_MODE_AIN:

        /* 数字端口/模拟端口设置*/
        amhw_zlg118_gpio_pin_analog_set(p_hw_gpio, pin);

        /* 输入输出方向设置 */
        amhw_zlg118_gpio_pin_dir_input(p_hw_gpio, pin);

        /* 驱动能力强弱设置 */
        amhw_zlg118_gpio_pin_driver_high(p_hw_gpio, pin);

        /* 上拉设置 */
        amhw_zlg118_gpio_pin_pu_disable(p_hw_gpio, pin);

        /* 下拉设置 */
        amhw_zlg118_gpio_pin_pd_disable(p_hw_gpio, pin);

        /* 开漏输出设置*/
        amhw_zlg118_gpio_pin_od_disable(p_hw_gpio, pin);
        break;

    /* 浮空输入 */
    case AMHW_ZLG118_GPIO_MODE_IN_FLOATING:

        /* 数字端口/模拟端口设置*/
        amhw_zlg118_gpio_pin_digital_set(p_hw_gpio, pin);

        /* 输入输出方向设置 */
        amhw_zlg118_gpio_pin_dir_input(p_hw_gpio, pin);

        /* 驱动能力强弱设置 */
        amhw_zlg118_gpio_pin_driver_high(p_hw_gpio, pin);

        /* 上拉设置 */
        amhw_zlg118_gpio_pin_pu_disable(p_hw_gpio, pin);

        /* 下拉设置 */
        amhw_zlg118_gpio_pin_pd_disable(p_hw_gpio, pin);

        /* 开漏输出设置*/
        amhw_zlg118_gpio_pin_od_disable(p_hw_gpio, pin);
        break;

    /* 推挽输出 */
    case AMHW_ZLG118_GPIO_MODE_OUT_PP:

        /* 数字端口/模拟端口设置*/
        amhw_zlg118_gpio_pin_digital_set(p_hw_gpio, pin);

        /* 输入输出方向设置 */
        amhw_zlg118_gpio_pin_dir_output(p_hw_gpio, pin);

        /* 驱动能力强弱设置 */
        amhw_zlg118_gpio_pin_driver_high(p_hw_gpio, pin);

        /* 上拉设置 */
        amhw_zlg118_gpio_pin_pu_enable(p_hw_gpio, pin);

        /* 下拉设置 */
        amhw_zlg118_gpio_pin_pd_enable(p_hw_gpio, pin);

        /* 开漏输出设置*/
        amhw_zlg118_gpio_pin_od_disable(p_hw_gpio, pin);
        break;

    /* 开漏输出 */
    case AMHW_ZLG118_GPIO_MODE_OUT_OD:

        /* 数字端口/模拟端口设置*/
        amhw_zlg118_gpio_pin_digital_set(p_hw_gpio, pin);

        /* 输入输出方向设置 */
        amhw_zlg118_gpio_pin_dir_output(p_hw_gpio, pin);

        /* 驱动能力强弱设置 */
        amhw_zlg118_gpio_pin_driver_high(p_hw_gpio, pin);

        /* 上拉设置 */
        amhw_zlg118_gpio_pin_pu_disable(p_hw_gpio, pin);

        /* 下拉设置 */
        amhw_zlg118_gpio_pin_pd_disable(p_hw_gpio, pin);

        /* 开漏输出设置*/
        amhw_zlg118_gpio_pin_od_enable(p_hw_gpio, pin);
        break;

    /* 模拟输出 */
    case AMHW_ZLG118_GPIO_MODE_AOUT:

        /* 数字端口/模拟端口设置*/
        amhw_zlg118_gpio_pin_analog_set(p_hw_gpio, pin);

        /* 输入输出方向设置 */
        amhw_zlg118_gpio_pin_dir_output(p_hw_gpio, pin);

        /* 驱动能力强弱设置 */
        amhw_zlg118_gpio_pin_driver_high(p_hw_gpio, pin);

        /* 上拉设置 */
        amhw_zlg118_gpio_pin_pu_disable(p_hw_gpio, pin);

        /* 下拉设置 */
        amhw_zlg118_gpio_pin_pd_disable(p_hw_gpio, pin);

        /* 开漏输出设置*/
        amhw_zlg118_gpio_pin_od_disable(p_hw_gpio, pin);
        break;
    }
}


/* end of file */
