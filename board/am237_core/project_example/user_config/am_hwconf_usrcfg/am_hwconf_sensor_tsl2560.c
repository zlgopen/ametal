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
 * \brief 传感器 TSL2560 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_tsl2560.h"
#include "am_common.h"
#include "zlg237_pin.h"
#include "am_zlg237_inst_init.h"

am_const am_local tsl2560_param_config_t __g_tsl2560_param_default = {
        AM_TSL2560_AGAIN_LOW_GAIN_MODE,         /* AGAIN */
        AM_TSL2560_ATIME_100MS,                 /* ALS ADC的积分时间 */
        0,                                      /* APERS */
        (AM_TSL2560_AIEN | AM_TSL2560_NPIEN),   /* 中断类型，不调用 am_sensor_trigger_cfg 时，该参数无效 */
        200,    /* ALS 低阈值，仅当中断类型使能 AM_TSL2560_AIEN 时该参数有效 */
        500,    /* ALS 高阈值，仅当中断类型使能 AM_TSL2560_AIEN 时该参数有效 */
        100,    /* 单次 ALS 低阈值，仅当中断类型使能 AM_TSL2560_NPIEN 时该参数有效 */
        800,    /* 单次 ALS 高阈值，仅当中断类型使能 AM_TSL2560_NPIEN 时该参数有效 */
};

/** \brief 传感器 TSL2560 设备信息实例 */
am_const am_local struct am_sensor_tsl2560_devinfo __g_tsl2560_info = {
        &__g_tsl2560_param_default,             /**< \brief 参数初始化 */
        PIOA_3,                                 /**< \brief 报警输出引脚 */
        0x39,                                   /**< \brief I2C 7位 设备地址 */
};

/** \breif 传感器 TSL2560 设备结构体定义 */
am_local struct am_sensor_tsl2560_dev __g_tsl2560_dev;

/** \brief 传感器 TSL2560 设备实例化 */
am_sensor_handle_t am_sensor_tsl2560_inst_init (void)
{
    return am_sensor_tsl2560_init(&__g_tsl2560_dev,
                                  &__g_tsl2560_info,
                                  am_zlg237_i2c2_inst_init());
}

/** \brief 传感器 TSL2560 实例解初始化 */
am_err_t am_sensor_tsl2560_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_tsl2560_deinit(handle);
}

/* end of file */
