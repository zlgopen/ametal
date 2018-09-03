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
 * \brief 位选器驱动（GPIO）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DIGITRON_COM_GPIO_H
#define __AM_DIGITRON_COM_GPIO_H

#include "ametal.h"
#include "am_digitron_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name am_if_digitron_com_gpio
 * @{
 */

/**
 * \brief GPIO位选器设备
 */
typedef struct am_digitron_com_gpio {
    am_digitron_base_selector_com_t    selector;
    const am_digitron_base_info_t     *p_info;
    int                                last_com;
    const int                         *p_pins;
    int                                pin_num;
} am_digitron_com_gpio_dev_t;

/**
 * \brief GPIO位选器初始化
 *
 * \param[in] p_dev  : GPIO位选器设备实例
 * \param[in] p_info : 数码管基础信息
 * \param[in] p_pins : 引脚表，引脚数目与扫描方式和数码管个数相关
 *                     - 行扫描时，引脚数与行数相等；
 *                     - 列扫描时，引脚数与列数相等。
 *
 * \return 标准的位选器（可供数码管扫描器使用）
 */
am_digitron_base_selector_com_t *am_digitron_com_gpio_init (
        am_digitron_com_gpio_dev_t        *p_dev,
        const am_digitron_base_info_t     *p_info,
        const int                         *p_pins);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DIGITRON_COM_GPIO_H */

/* end of file */
