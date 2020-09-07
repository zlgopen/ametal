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
 * \brief 传感器 BMA253 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-22  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_bma253.h"
#include "am_common.h"
#include "hk32f103rbt6_pin.h"
#include "am_hk32f103rbt6_inst_init.h"

/** \brief 传感器 BMA253 设备信息实例 */
am_const am_local struct am_sensor_bma253_devinfo __g_bma253_info = {
    PIOB_0,            /*< \brief 触发引脚定义    */
    0x18               /*< \breif BMA253 I2C地址 */
};

/** \breif 传感器 BMA253 设备结构体定义 */
am_local struct am_sensor_bma253_dev __g_bma253_dev;

/** \brief 传感器 BMA253 设备实例化 */
am_sensor_handle_t am_sensor_bma253_inst_init (void)
{
    return am_sensor_bma253_init(&__g_bma253_dev,
                                 &__g_bma253_info,
                                 am_hk32f103rbt6_i2c1_inst_init());
}

/** \brief 传感器 BMA253 实例解初始化 */
am_err_t am_sensor_bma253_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_bma253_deinit(handle);
}

/* end of file */
