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
 * \brief 飞行时间传感器 TMF8801 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-02  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_SENSOR_TMF8801_H
#define __AM_HWCONF_SENSOR_TMF8801_H

#include "ametal.h"
#include "am_sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 传感器 TMF8801 设备实例化
 */
am_sensor_handle_t am_sensor_tmf8801_inst_init (void);

/**
 * \brief 传感器 TMF8801 实例解初始化
 */
am_err_t am_sensor_tmf8801_inst_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
