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
 * \brief 段码驱动（GPIO）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DIGITRON_SEG_GPIO_H
#define __AM_DIGITRON_SEG_GPIO_H

#include "ametal.h"
#include "am_digitron_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_digitron_seg_gpio
 * \copydoc am_digitron_seg_gpio.h
 * @{
 */

/**
 * \brief GPIO段码设备
 */
typedef struct am_digitron_seg_gpio {
    am_digitron_base_sender_seg_t      sender;
    const am_digitron_base_info_t     *p_info;
    const int                         *p_pins;
    int                                pin_num;
} am_digitron_seg_gpio_dev_t;


/**
 * \brief GPIO段码发送器初始化
 *
 * \param[in] p_dev  : GPIO段码发送器设备实例
 * \param[in] p_info : 数码管基础信息
 * \param[in] p_pins : 引脚表，0号引脚对应有效段码的最高位，如 ‘a' 段， 以此类推
 *                     引脚数目应该与单次扫描的数码管个数的段数总和相等
 *
 * \return 标准的段码发送器（可供数码管扫描器使用）
 */
am_digitron_base_sender_seg_t *am_digitron_seg_gpio_init (
        am_digitron_seg_gpio_dev_t     *p_dev,
        const am_digitron_base_info_t  *p_info,
        const int                      *p_pins);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DIGITRON_SEG_GPIO_H */

/* end of file */
