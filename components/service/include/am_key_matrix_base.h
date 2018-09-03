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
 * \brief 通用矩阵键盘基础信息
 *
 * \internal
 * \par modification history:
 * - 15-09-15 tee, first implementation.
 * \endinternal
 */

#ifndef __AM_KEY_MATRIX_BASE_H
#define __AM_KEY_MATRIX_BASE_H

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup am_if_key_matrix_base
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name 使用的扫描方式
 *
 * 具体扫描方式与硬件相关
 *
 * @{
 */

/** \brief 按行扫描，每次扫描一行按键  */
#define AM_KEY_MATRIX_SCAN_MODE_ROW      0

/** \brief 按列扫描，每次扫描一列按键  */
#define AM_KEY_MATRIX_SCAN_MODE_COL      1

/** @} */

/**
 * \brief 矩阵键盘基础信息
 */
typedef struct am_key_matrix_base_info {
    int          row;         /**< \brief 行数目 */
    int          col;         /**< \brief 列数目 */
    const int   *p_codes;     /**< \brief 各个按键对应的编码，按行的顺序依次对应  */
    am_bool_t    active_low;  /**< \brief 按键按下后是否为低电平 */
    uint8_t      scan_mode;   /**< \brief 扫描方式 （按行扫描或按列扫描）*/
} am_key_matrix_base_info_t;

/**
 * \brief 获取按键数目
 * \param[in] p_info : 矩阵键盘基础信息
 * \return 按键数目
 */
am_static_inline
int  am_key_matrix_num_key_get (const am_key_matrix_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->row * p_info->col;
    }
    return 0;
}

/**
 * \brief 获取行数
 * \param[in] p_info : 矩阵键盘基础信息
 * \return 行数
 */
am_static_inline
int  am_key_matrix_num_row_get (const am_key_matrix_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->row;
    }
    return 0;
}

/**
 * \brief 获取列数
 * \param[in] p_info : 矩阵键盘基础信息
 * \return 列数
 */
am_static_inline
int  am_key_matrix_num_col_get (const am_key_matrix_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->col;
    }
    return 0;
}

/**
 * \brief 获取序号为 idx 按键对应的编码
 * \param[in] p_info : 矩阵键盘基础信息
 * \return 按键编码
 */
am_static_inline
int  am_key_matrix_code_get (const am_key_matrix_base_info_t *p_info, int idx)
{
    if ((p_info != NULL) && (idx < am_key_matrix_num_key_get(p_info))) {
        return p_info->p_codes[idx];
    }
    return 0;
}

/**
 * \brief 按键按下后，是否为低电平
 * \param[in] p_info : 矩阵键盘基础信息
 * \return 按键按下后，是否为低电平
 */
am_static_inline
am_bool_t  am_key_matrix_active_low_get (const am_key_matrix_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->active_low;
    }
    return AM_FALSE;
}

/**
 * \brief 获取键盘的扫描方式
 * \param[in] p_info : 矩阵键盘基础信息
 * \return 按键按下后，是否为低电平
 */
am_static_inline
uint8_t  am_key_matrix_scan_mode_get (const am_key_matrix_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->scan_mode;
    }
    return AM_FALSE;
}

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __AM_KEY_MATRIX_BASE_H */

/* end of file */
