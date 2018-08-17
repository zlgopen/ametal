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
 * \brief GPIO驱动型LED
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LED_GPIO_H
#define __AM_LED_GPIO_H

#include "ametal.h"
#include "am_led_dev.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_led_gpio
 * \copydoc am_led_gpio.h
 * @{
 */

/**
 * \brief LED信息（GPIO驱动）
 */
typedef struct am_led_gpio_info {

    /** \brief LED基础服务信息 ，包含起始编号和结束编号     */
    am_led_servinfo_t  serv_info;

    /** \brief 使用的GPIO引脚，引脚数目应该为 （结束编号 - 起始编号 + 1）   */
    const int         *p_pins;

    /** \brief LED是否是低电平点亮  */
    am_bool_t          active_low;

} am_led_gpio_info_t;

/**
 * \brief LED设备（GPIO驱动）
 */
typedef struct am_led_gpio_dev {
    am_led_dev_t               isa;
    const am_led_gpio_info_t  *p_info;
} am_led_gpio_dev_t;

/**
 * \brief LED设备初始化（GPIO驱动）
 *
 * \param[in] p_dev  : LED设备
 * \param[in] p_info : LED设备信息
 *
 * \retval AM_OK      : 初始化成功
 * \retval -AM_EINVAL : 初始化失败
 */
int am_led_gpio_init (am_led_gpio_dev_t         *p_dev,
                      const am_led_gpio_info_t  *p_info);

/**
 * \brief LED设备解初始化（GPIO驱动）
 *
 * \param[in] p_dev  : LED设备
 *
 * \retval AM_OK      : 解初始化成功
 * \retval -AM_EINVAL : 解初始化失败
 */
int am_led_gpio_deinit (am_led_gpio_dev_t *p_dev);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LED_GPIO_H */

/* end of file */
