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
 * - 1.00 20-03-02  YRZ, first implementation.
 * \endinternal
 */

#include "stm32f103rbt6_pin.h"
#include "am_sensor_tmf8801.h"
#include "am_common.h"
#include "am_stm32f103rbt6_inst_init.h"

/** \brief TOF传感器 TMF8801 设备信息实例 */
am_const am_local struct am_sensor_tmf8801_devinfo __g_tmf8801_info = {
        PIOC_11,           /*< \brief 使能引脚定义    */
        PIOA_5,            /*< \brief 触发引脚定义    */
        0x41               /*< \breif TMF8801 I2C地址 */
};

/** \breif TOF传感器 TMF8801 设备结构体定义 */
am_local struct am_sensor_tmf8801_dev __g_tmf8801_dev;

/** \brief TOF传感器 TMF8801 设备实例化 */
am_sensor_handle_t am_sensor_tmf8801_inst_init (void)
{
    return am_sensor_tmf8801_init(&__g_tmf8801_dev,
                                  &__g_tmf8801_info,
                                   am_stm32f103rbt6_i2c2_inst_init());
}

/** \brief TOF传感器 TMF8801 实例解初始化 */
am_err_t am_sensor_tmf8801_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_tmf8801_deinit(handle);
}

/* end of file */
