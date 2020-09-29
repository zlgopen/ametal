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
 * \brief 传感器 BMG160 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-11  wk, first implementation.
 * \endinternal
 */
 
#ifndef __AM_HWCONF_SENSOR_BMG160_H
#define __AM_HWCONF_SENSOR_BMG160_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 传感器 BMG160 设备实例化
 */
am_sensor_handle_t am_sensor_bmg160_inst_init (void);

/**
 * \brief 传感器 BMG160 实例解初始化
 */
am_err_t am_sensor_bmg160_inst_deinit (am_sensor_handle_t handle); 
    
#ifdef __cplusplus
}
#endif

#endif

/* end of file */
