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
 * \brief 纯GPIO驱动的动态扫描数码管驱动（位选和段码均为GPIO驱动）
 * 
 * \internal
 * \par modification history:
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DIGITRON_SCAN_GPIO_H
#define __AM_DIGITRON_SCAN_GPIO_H

#include "ametal.h"
#include "am_digitron_disp.h"
#include "am_digitron_scan.h"
#include "am_digitron_dev.h"
#include "am_digitron_seg_gpio.h"
#include "am_digitron_com_gpio.h"
#include "am_digitron_base.h"

#ifdef __cplusplus
extern "C" {
#endif
 
/**
 * \addtogroup am_if_digitron_scan_gpio
 * \copydoc am_digitron_scan_gpio.h
 * @{
 */

/**
 * \brief 动态扫描类数码管信息（GPIO驱动）
 */
typedef struct am_digitron_scan_info {
    am_digitron_scan_devinfo_t  scan_info;  /** \brief 数码管动态扫描相关信息 */
    am_digitron_base_info_t     base_info;  /**< \brief 数码管基础信息  */
    const int                  *p_seg_pins; /**< \brief 段码 GPIO驱动 引脚 */
    const int                  *p_com_pins; /**< \brief 位码 GPIO驱动 引脚 */
} am_digitron_scan_gpio_info_t;

/**
 * \brief 动态扫描类数码管设备（GPIO驱动）
 */
typedef struct am_digitron_scan_gpio_dev {
    am_digitron_scan_dev_t           scan_dev;
    am_digitron_scan_ops_separate_t  scan_ops;
    am_digitron_seg_gpio_dev_t       seg_dev;
    am_digitron_com_gpio_dev_t       com_dev;
} am_digitron_scan_gpio_dev_t;

/**
 * \brief 动态扫描类数码管初始化
 *
 * \param[in] p_dev  : GPIO驱动型动态扫描类数码管设备实例
 * \param[in] p_info : GPIO驱动型动态扫描类数码管设备实例信息
 *
 * \retval AM_OK      : 初始化成功
 * \retval -AM_EINVAL ：初始化失败，参数存在错误
 */
int am_digitron_scan_gpio_init (am_digitron_scan_gpio_dev_t        *p_dev,
                                const am_digitron_scan_gpio_info_t *p_info);

/**
 * \brief 设置数码管扫描回调函数（仅可设置一个）
 *
 *     当一个新的数码管位开始扫描时，会调用该回调函数
 *
 * \param[in] p_dev  : GPIO驱动型动态扫描类数码管设备实例
 * \param[in] pfn_cb : 回调函数
 * \param[in] p_arg  : 回调函数用户参数
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL ：设置失败，参数存在错误
 */
int am_digitron_scan_gpio_cb_set (am_digitron_scan_gpio_dev_t *p_dev,
                                  am_digitron_scan_cb_t        pfn_cb,
                                  void                        *p_arg);

/**
 * \brief 动态扫描类数码管解初始化
 *
 * \param[in] p_dev   : GPIO驱动型动态扫描类数码管设备实例
 *
 * \retval AM_OK      : 解初始化成功
 * \retval -AM_EINVAL ：解初始化失败，参数存在错误
 */
int am_digitron_scan_gpio_deinit (am_digitron_scan_gpio_dev_t *p_dev);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DIGITRON_SCAN_GPIO_H */

/* end of file */
