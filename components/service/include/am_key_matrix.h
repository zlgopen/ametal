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
 * \brief 通用矩阵键盘驱动接口（各具体驱动根据自身实现具体的键值读取函数）
 *
 * \internal
 * \par modification history:
 * - 15-09-15 tee, first implementation.
 * \endinternal
 */

#ifndef __AM_KEY_MATRIX_H
#define __AM_KEY_MATRIX_H

#include "ametal.h"
#include "am_types.h"
#include "am_key_matrix_base.h"
 
/**
 * \addtogroup am_if_key_matrix
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 键值获取环境准备
 */
typedef struct am_key_matrix_prepare {
    
    /** \brief 读取键值准备，该函数在键值读取前调用 */
    int (*pfn_prepare) (void *p_cookie, int idx);

    /** \brief 读取键值结束，该函数在键值读取后调用 */
    int (*pfn_finish) (void *p_cookie, int idx); 
    
    /** \brief 函数自定义参数 */
    void *p_cookie;

} am_key_matrix_prepare_t;

/**
 * \brief 键值读取（读取一行或一列的键值）
 */
typedef struct am_key_matrix_read {

    /** \brief 键值读取函数，读取一行或一列的键值 */
    uint32_t (*pfn_val_read) (void *p_cookie, int idx);

    /** \brief 函数自定义参数 */
    void      *p_cookie;

} am_key_matrix_read_t;


/** \brief 矩阵键盘设备  */
typedef struct am_key_matrix {
    const am_key_matrix_base_info_t *p_info;
    uint32_t                         key_samp_last;
    uint32_t                         key_samp_cur;
    uint32_t                         key_final;
    am_key_matrix_prepare_t         *p_prepare;
    am_key_matrix_read_t            *p_read;
} am_key_matrix_t;

/**
 * \brief 按键设备初始化
 *
 * \param[in] p_dev      : 按键设备实例
 * \param[in] p_info     : 按键信息
 * \param[in] p_prepare  : 键值读取的准备工作，若无准备工作，则设置为NULL
 * \param[in] p_read     : 键值读取器
 *
 * \return  AM_OK，初始化成功; 其它值，初始化失败，失败原因请查看错误号
 */
int am_key_matrix_init (am_key_matrix_t                 *p_dev,
                        const am_key_matrix_base_info_t *p_info,
                        am_key_matrix_prepare_t         *p_prepare,
                        am_key_matrix_read_t            *p_read);

/**
 * \brief 执行一次扫描，扫描一行或一列（不自动扫描，一般用于与数码管复用引脚时）
 *
 * \param[in] p_dev    : 按键设备实例
 * \param[in] scan_idx : 本次扫描的行号或列号
 *
 * \return  AM_OK，初始化成功; 其它值，初始化失败，失败原因请查看错误号
 * \note 扫描该行（列）前，需确保数据具有正确读取的有效环境
 */
int am_key_matrix_scan (am_key_matrix_t *p_dev, int scan_idx);

/**
 * \brief 获取最大扫描索引
 * \param[in] p_dev  : 按键设备实例
 * \return  最大扫描索引
 */
int am_key_matrix_scan_idx_max_get (am_key_matrix_t *p_dev);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __AM_KEY_MATRIX_H */

/* end of file */
