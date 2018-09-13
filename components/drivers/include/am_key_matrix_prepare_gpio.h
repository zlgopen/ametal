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
 * \brief 矩阵键盘环境准备（GPIO）
 *
 * 以按列扫描为例，每次可以读取一列按键的键值，在读取之前，需要将该列的公共端
 * 设置为低电平（或高电平），以便检测各个按键的按下。一列按键检测结束后，必须
 * 恢复电平，以便检测下一列按键。
 *
 * \internal
 * \par modification history:
 * - 15-09-15 tee, first implementation.
 * \endinternal
 */

#ifndef __AM_KEY_MATRIX_PREPARE_GPIO_H
#define __AM_KEY_MATRIX_PREPARE_GPIO_H

#include "ametal.h"
#include "am_types.h"
#include "am_key_matrix.h"
#include "am_key_matrix_base.h"

/**
 * \addtogroup am_if_key_matrix_prepare_gpio
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 矩阵键盘设备  */
typedef struct am_key_matrix_prepare_gpio {
    am_key_matrix_prepare_t          isa;
    const am_key_matrix_base_info_t *p_info;
    int                              last_idx;
    const int                       *p_pins;
    int                              pin_num;
} am_key_matrix_prepare_gpio_t;

/**
 * \brief 键值读取设备初始化（GPIO）
 *
 * \param[in] p_dev       : 按键设备实例
 * \param[in] p_base_info : 矩阵键盘基础信息
 * \param[in] p_pins      : GPIO引脚列表
 *                          - 若按照行扫描，则引脚数应该与行数相等
 *                          - 若按照列扫描，则引脚数应该与列数相等
 * \return 通用的矩阵键盘设备
 */
am_key_matrix_prepare_t * am_key_matrix_prepare_gpio_init (
    am_key_matrix_prepare_gpio_t    *p_dev,
    const am_key_matrix_base_info_t *p_info,
    const int                       *p_pins);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __AM_KEY_MATRIX_VAL_READ_GPIO_H */

/* end of file */
