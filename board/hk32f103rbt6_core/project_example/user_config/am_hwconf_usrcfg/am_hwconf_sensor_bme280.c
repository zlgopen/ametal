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
 * \brief 传感器 bme280 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-30  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_bme280.h"
#include "am_common.h"
#include "hk32f103rbt6_pin.h"
#include "am_hk32f103rbt6_inst_init.h"

/** \brief 传感器 bme280 设备信息实例 */
am_const am_local struct am_sensor_bme280_devinfo __g_bme280_info = {
        0x76               /*< \breif bme280 I2C地址 */
};

/** \breif 传感器 bme280 设备结构体定义 */
am_local struct am_sensor_bme280_dev __g_bme280_dev;

/** \brief 传感器 bme280 设备实例化 */
am_sensor_handle_t am_sensor_bme280_inst_init (void)
{
    return am_sensor_bme280_init(&__g_bme280_dev,
                                 &__g_bme280_info,
                                 am_hk32f103rbt6_i2c1_inst_init());
}

/** \brief 传感器 bme280 实例解初始化 */
am_err_t am_sensor_bme280_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_bme280_deinit(handle);
}

/* end of file */
