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
 * \brief 传感器 HTS221 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-27  wan, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_SENSOR_HTS221_H
#define __AM_HWCONF_SENSOR_HTS221_H

#include "ametal.h"
#include "am_sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 传感器 HTS221 设备实例化
 */
am_sensor_handle_t am_sensor_hts221_inst_init (void);

/**
 * \brief 传感器 HTS221 实例解初始化
 */
am_err_t am_sensor_hts221_inst_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
