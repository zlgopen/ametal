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
 * \brief 独立按键驱动接口
 *
 * \internal
 * \par modification history:
 * - 15-09-15 tee, first implementation.
 * \endinternal
 */

#ifndef __AM_KEY_GPIO_H
#define __AM_KEY_GPIO_H

#include "ametal.h"
#include "am_types.h"
#include "am_softimer.h"

/**
 * \addtogroup am_if_key_gpio
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 按键信息
 */
typedef struct am_key_gpio_info {
    const int  *p_pins;                /**< \brief 使用的引脚号               */
    const int  *p_codes;               /**< \brief 各个按键对应的编码（上报） */
    int         pin_num;               /**< \brief 按键数目 */
    am_bool_t   active_low;            /**< \brief 是否低电平激活（按下为低电平） */
    int         scan_interval_ms;      /**< \brief 按键扫描时间间隔，一般10ms */
} am_key_gpio_info_t;

/** \brief 按键设备  */
typedef struct am_key_gpio {
    am_softimer_t             timer;
    const am_key_gpio_info_t *p_info;
    uint32_t                  key_prev;
    uint32_t                  key_press;
} am_key_gpio_t;

/**
 * \brief 按键初始化
 *
 * \param[in] p_dev  : 按键设备实例
 * \param[in] p_info : 按键信息
 *
 * \return  AM_OK，初始化成功; 其它值，初始化失败，失败原因请查看错误号
 */
int am_key_gpio_init (am_key_gpio_t *p_dev, const am_key_gpio_info_t *p_info);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __AM_KEY_GPIO_H */

/* end of file */
