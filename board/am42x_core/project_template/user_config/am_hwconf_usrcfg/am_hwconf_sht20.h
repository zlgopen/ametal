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
 * \brief 温湿度传感器SHT20 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-03  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_SHT20_H
#define __AM_HWCONF_SHT20_H

#include "ametal.h"
#include "am_sht20.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 温湿度传感器SHT20 设备实例化 */
am_sht20_handle_t am_sht20_inst_init (void);

/** \brief 温湿度传感器SHT20 实例解初始化 */
void am_sht20_inst_deinit (am_sht20_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
