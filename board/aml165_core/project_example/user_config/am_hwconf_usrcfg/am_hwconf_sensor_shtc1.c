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
 * \brief 传感器 SHTC1 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-22  yrz, first implementation.
 * \endinternal
 */

#include <am_aml165_inst_init.h>
#include "am_sensor_shtc1.h"
#include "am_common.h"
#include "zml165_pin.h"

/** \brief 传感器 SHTC1 设备信息实例 */
am_const am_local struct am_sensor_shtc1_devinfo __g_shtc1_info = {
    0x70               /*< \breif SHTC1 I2C地址 */
};

/** \breif 传感器 SHTC1 设备结构体定义 */
am_local struct am_sensor_shtc1_dev __g_shtc1_dev;

/** \brief 传感器 SHTC1 设备实例化 */
am_sensor_handle_t am_sensor_shtc1_inst_init (void)
{
    return am_sensor_shtc1_init(&__g_shtc1_dev,
                                &__g_shtc1_info,
                                am_zml165_i2c1_inst_init());
}

/** \brief 传感器 SHTC1 实例解初始化 */
am_err_t am_sensor_shtc1_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_shtc1_deinit(handle);
}

/* end of file */
