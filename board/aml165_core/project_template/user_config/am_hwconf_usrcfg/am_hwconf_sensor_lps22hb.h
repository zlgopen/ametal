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
 * \brief 传感器 LPS22HB 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-03  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_SENSOR_LPS22HB_H
#define __AM_HWCONF_SENSOR_LPS22HB_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 传感器 LPS22HB 设备实例化
 */
am_sensor_handle_t am_sensor_lps22hb_inst_init (void);

/**
 * \brief 传感器 LPS22HB 实例解初始化
 */
am_err_t am_sensor_lps22hb_inst_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
