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
 * \brief 矩阵键盘GPIO键值读取
 *
 * \internal
 * \par modification history:
 * - 15-09-15 tee, first implementation.
 * \endinternal
 */

#ifndef __AM_KEY_MATRIX_READ_GPIO_H
#define __AM_KEY_MATRIX_READ_GPIO_H

#include "ametal.h"
#include "am_types.h"
#include "am_key_matrix.h"
#include "am_key_matrix_base.h"

/**
 * \addtogroup am_if_key_matrix_read_gpio
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 矩阵键盘键值读取设备  */
typedef struct am_key_matrix_read_gpio {
    am_key_matrix_read_t             isa;
    const am_key_matrix_base_info_t *p_info;
    const int                       *p_pins;
    int                              pin_num;
} am_key_matrix_read_gpio_t;

/**
 * \brief 矩阵键盘键值读取设备初始化（GPIO）
 *
 * \param[in] p_dev      : 按键设备实例
 * \param[in] p_info     : 按键信息
 * \param[in] p_pins     :  GPIO引脚列表，用于读取一次扫描下的所有按键的键值
 *                          - 若按照行扫描，则引脚数应该与列数相等
 *                          - 若按照列扫描，则引脚数应该与行数相等
 *
 * \return 通用的矩阵键盘键值读取
 */
am_key_matrix_read_t * am_key_matrix_read_gpio_init (
    am_key_matrix_read_gpio_t       *p_dev,
    const am_key_matrix_base_info_t *p_info,
    const int                       *p_pins);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __AM_KEY_MATRIX_READ_GPIO_H */

/* end of file */
