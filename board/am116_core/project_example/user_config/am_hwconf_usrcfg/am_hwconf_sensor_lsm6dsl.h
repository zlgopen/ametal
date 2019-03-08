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
 * \brief ������ LSM6DSL �����ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-22  YRZ, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_SENSOR_LSM6DSL_H
#define __AM_HWCONF_SENSOR_LSM6DSL_H

#include "ametal.h"
#include "am_sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief ������ LSM6DSL �豸ʵ����
 */
am_sensor_handle_t am_sensor_lsm6dsl_inst_init (void);

/**
 * \brief ������ LSM6DSL ʵ�����ʼ��
 */
am_err_t am_sensor_lsm6dsl_inst_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
