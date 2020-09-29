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
 * \brief 传感器 FXOS8700CQ 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-29  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_fxos8700cq.h"
#include "am_common.h"
#include "zlg116_pin.h"
#include "am_zlg116_inst_init.h"

/** \brief 传感器 FXOS8700CQ 设备信息实例 */
am_const am_local struct am_sensor_fxos8700cq_devinfo __g_fxos8700cq_info = {
    PIOB_0,                                                 /*< \brief 触发引脚定义    */
    0x1E,                                                   /*< \breif FXOS8700CQ I2C地址 */
    AM_FXOS8700CQ_ODR_HYBRID_MODE_200HZ,                    /* 设置 ODR */
    AM_FXOS8700CQ_HYBRID_MODE,                              /* 混合模式 */
    7,                                                      /* Oversample ratio (OSR) for magnetometer data. range:0~7 */
    AM_FXOS8700CQ_AUTO_CALIBRATION_FEATURE_ENABLED,         /* 自校准使能 */
    AM_FXOS8700CQ_HYB_AUTOINC_ENABLED,                      /* 使能混合模式下地址自增，读取OUT_Z_LSB(0x06)值后自动跳转到M_OUT_X_MSB(0x33) */
    AM_FXOS8700CQ_HIGH_RESOLUTION,                          /* 高分辨率 */
    AM_FXOS8700CQ_LNOISE_REDUCED_NOISE_MODE,                /* 降低噪音模式 */
};

/** \breif 传感器 FXOS8700CQ 设备结构体定义 */
am_local struct am_sensor_fxos8700cq_dev __g_fxos8700cq_dev;

/** \brief 传感器 FXOS8700CQ 设备实例化 */
am_sensor_handle_t am_sensor_fxos8700cq_inst_init (void)
{
    return am_sensor_fxos8700cq_init(&__g_fxos8700cq_dev,
                                  &__g_fxos8700cq_info,
                                  am_zlg116_i2c1_inst_init());
}

/** \brief 传感器 FXOS8700CQ 实例解初始化 */
am_err_t am_sensor_fxos8700cq_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_fxos8700cq_deinit(handle);
}

/* end of file */
