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
 * \brief 在标准矩阵键盘的基础上，添加软件定时器，实现自动扫描
 *
 * \internal
 * \par modification history:
 * - 15-09-15 tee, first implementation.
 * \endinternal
 */

#ifndef __AM_KEY_MATRIX_SOFTIMER_H
#define __AM_KEY_MATRIX_SOFTIMER_H

#include "ametal.h"
#include "am_types.h"
#include "am_key_matrix_base.h"
#include "am_key_matrix.h"
#include "am_softimer.h"

/**
 * \addtogroup am_if_key_matrix_softimer
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 矩阵键盘设备  */
typedef struct am_key_matrix_softimer {
    am_key_matrix_t  *p_real;
    am_softimer_t     timer;
    int               scan_idx;
} am_key_matrix_softimer_t;

/**
 * \brief 初始化，设置软件定时器，实现自动扫描
 *
 * \param[in] p_dev            : 按键设备实例
 * \param[in] p_timer          : 软件定时器实例
 * \param[in] scan_interval_ms : 扫描所有矩阵键盘的时间间隔， 单位 ms
 *
 * \return  AM_OK，初始化成功; 其它值，初始化失败，失败原因请查看错误号
 * \note 扫描该行（列）前，需确保数据具有正确读取的有效环境
 */
int am_key_matrix_softimer_init (am_key_matrix_softimer_t *p_dev,
                                 am_key_matrix_t          *p_key,
                                 int                       scan_interval_ms);

/**
 * \brief 解初始化，停止软件定时器，终止自动扫描
 *
 * \param[in] p_dev  : 按键设备实例
 *
 * \return  AM_OK，解初始化成功; 其它值，解初始化失败，失败原因请查看错误号
 *
 * \note 终止后，如需再次使用软件定时器实现自动扫描，则需要重新调用 \sa
 * am_key_matrix_softimer_init()函数
 */
int am_key_matrix_softimer_deinit (am_key_matrix_softimer_t *p_dev);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __AM_KEY_MATRIX_SOFTIMER_H */

/* end of file */
