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
 * \brief 传感器 AS5600 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_as5600.h"
#include "am_common.h"
#include "zlg237_pin.h"
#include "am_zlg237_inst_init.h"

am_const am_local as5600_param_config_t __g_as5600_param_default = {
        AM_AS5600_WD_OFF,
        AM_AS5600_FTH_6_LSBS,
        AM_AS5600_SF_16,
        AM_AS5600_PWMF_115_HZ,
        AM_AS5600_OUTS_ANALOG_0_TO_100,
        AM_AS5600_HYST_OFF,
        AM_AS5600_PM_NOM,
        AM_AS5600_INCREASE_CLOCKWISE,
        AM_AS5600_OPTION_DISABLE,
        AM_AS5600_OPTION_C,     /* 该配置在 option_enable = AM_AS5600_OPTION_ENABLE 有效 */
        0xC00,                  /* 仅当 option = AM_AS5600_OPTION_C 时有效
                                   最大角度范围： 0~360 度对应 0~4096
                                   如，90 度对应 1024 (0x400) */
};

/** \brief 传感器 AS5600 设备信息实例 */
am_const am_local struct am_sensor_as5600_devinfo __g_as5600_info = {
        &__g_as5600_param_default,          /**< \brief 参数初始化 */
        -1,                                 /**< \brief 报警输出引脚 */
        PIOA_2,                             /**< \brief OUT引脚 */
        -1,                                 /**< \brief PGO引脚 */
        PIOC_7,                             /**< \brief DIR引脚 */
        0x36,                               /**< \brief I2C 7位 设备地址 */
};

/** \breif 传感器 AS5600 设备结构体定义 */
am_local struct am_sensor_as5600_dev __g_as5600_dev;

/** \brief 传感器 AS5600 设备实例化 */
am_sensor_handle_t am_sensor_as5600_inst_init (void)
{
    return am_sensor_as5600_init(&__g_as5600_dev,
                                 &__g_as5600_info,
                                 am_zlg237_i2c2_inst_init());
}

/** \brief 传感器 AS5600 实例解初始化 */
am_err_t am_sensor_as5600_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_as5600_deinit(handle);
}

/* end of file */
