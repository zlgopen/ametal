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
 * \brief MiniPort-View 和 MiniPort-Key联合使用
 *
 * 本驱动适合以下情况：
 * 1. 数码管段码使用GPIO驱动
 * 2. 数码管位码使用GPIO驱动
 * 3. 矩阵键盘采用列扫描方式，列线与数码管位选线复用
 * 4. 矩阵键盘的键值读取使用GPIO读取（该GPIO与数码管独立）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DIGITRON_MINIPORT_VIEW_KEY_H
#define __AM_DIGITRON_MINIPORT_VIEW_KEY_H

#include "ametal.h"
#include "am_digitron_scan_gpio.h"
#include "am_key_matrix_read_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif
 
/**
 * \addtogroup am_if_miniport_view_key
 * @{
 */
 
/**
 * \brief MiniPort View + MiniPort Key 信息
 */
typedef struct am_miniport_view_key_info {
    am_digitron_scan_gpio_info_t scan_info;    /**< \brief 数码管动态扫描相关信息 */
    am_key_matrix_base_info_t    key_info;     /**< \brief 按键基础信息 */
    const int                   *p_pins_row;   /**< \brief 行线引脚（列线复用）  */
} am_miniport_view_key_info_t;

/**
 * \brief MiniPort View + MiniPort Key 设备
 */
typedef struct am_miniport_view_key_dev {
    am_digitron_scan_gpio_dev_t        scan_gpio_dev;
    am_key_matrix_t                    key_dev;
    am_key_matrix_read_gpio_t          read;
    const am_miniport_view_key_info_t *p_info;
} am_miniport_view_key_dev_t;

/**
 * \brief 动态扫描类数码管初始化
 *
 * \param[in] p_dev  : 动态扫描类数码管设备实例
 * \param[in] p_info : 动态扫描类数码管设备实例信息
 *
 * \retval AM_OK      : 初始化成功
 * \retval -AM_EINVAL ：初始化失败，参数存在错误
 */
int am_miniport_view_key_init (am_miniport_view_key_dev_t        *p_dev,
                               const am_miniport_view_key_info_t *p_info);

/**
 * \brief 动态扫描类数码管解初始化
 *
 * \param[in] p_dev   : 动态扫描类数码管设备实例
 *
 * \retval AM_OK      : 解初始化成功
 * \retval -AM_EINVAL ：解初始化失败，参数存在错误
 */
int am_miniport_view_key_deinit (am_miniport_view_key_dev_t *p_dev);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DIGITRON_MINIPORT_VIEW_KEY_H */

/* end of file */
