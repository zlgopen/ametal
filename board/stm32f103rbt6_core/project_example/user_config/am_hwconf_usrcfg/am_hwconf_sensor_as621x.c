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
 * \brief 传感器 AS621X 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-21  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_as621x.h"
#include "am_common.h"
#include "stm32f103rbt6_pin.h"
#include "am_stm32f103rbt6_inst_init.h"

am_const am_local as621x_param_config_t __g_as621x_param_default = {
        AM_AS621X_CONVERSION_RATE_1HZ,          /* 转换速率 */
        AM_AS621X_SLEEP_MODE,                   /* 睡眠模式，ps:中断模式下该位无效，强制设置为连续转换模式 */
        AM_FALSE,                               /* 极性设置:低电平激活 */
        AM_AS621X_CONSECUTIVE_FAULTS_1,         /* 连续故障数 */
        20.5,                                   /* 低温度阈值 */
        40.5,                                   /* 高温度阈值 */
};

/*  ALERT/ADD1 Connection           ADD0            Alert Functionality Enabled             Device Address
 *      SCL                         VSS                     NO                                  0x44
 *      SCL                         VDD                     NO                                  0x45
 *      SCL                         SDA                     NO                                  0x46
 *      SCL                         SCL                     NO                                  0x47
 *
 *      VSS                         VSS                     NO                                  0x48
 *      VSS                         VDD                     NO                                  0x49
 *      VSS                         SDA                     NO                                  0x4A
 *      VSS                         SCL                     NO                                  0x4B
 *
 *      VDD                         VSS                     YES                                 0x48    (当前使用)
 *      VDD                         VDD                     YES                                 0x49
 *      VDD                         SDA                     YES                                 0x4A
 *      VDD                         SCL                     YES                                 0x4B
 */

/** \brief 传感器 AS621X 设备信息实例 */
am_const am_local struct am_sensor_as621x_devinfo __g_as621x_info = {
        &__g_as621x_param_default,              /**< \brief 参数初始化 */
        PIOA_4,                                 /**< \brief 报警输出引脚 */
        0x48,                                   /**< \brief I2C 7位 设备地址 */
};

/** \breif 传感器 AS621X 设备结构体定义 */
am_local struct am_sensor_as621x_dev __g_as621x_dev;

/** \brief 传感器 AS621X 设备实例化 */
am_sensor_handle_t am_sensor_as621x_inst_init (void)
{
    return am_sensor_as621x_init(&__g_as621x_dev,
                                 &__g_as621x_info,
                                 am_stm32f103rbt6_i2c2_inst_init());
}

/** \brief 传感器 AS621X 实例解初始化 */
am_err_t am_sensor_as621x_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_as621x_deinit(handle);
}

/* end of file */
