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
 * \brief 传感器 MMC5883MA 配置文件
 * 
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-30  wk, first implementation
 * \endinternal
 */
 
#include "am_sensor_mmc5883ma.h"
#include "am_common.h"
#include "hk32f103rbt6_pin.h"
#include "am_hk32f103rbt6_inst_init.h"

/** \brief 传感器 MMC5883MA 设备信息实例 */
am_const am_local struct am_sensor_mmc5883ma_devinfo __g_mmc5883ma_info = {
        PIOB_0,            /*< \brief 触发引脚定义   */
        0x30               /*< \breif MMC5883MA I2C地址 */
};

/** \breif 传感器 MMC5883MA 设备结构体定义 */
am_local struct am_sensor_mmc5883ma_dev __g_mmc5883ma_dev;

/** \brief 传感器 MMC5883MA 设备实例化 */
am_sensor_handle_t am_sensor_mmc5883ma_inst_init (void)
{
    return am_sensor_mmc5883ma_init(&__g_mmc5883ma_dev,
                                    &__g_mmc5883ma_info,
                                    am_hk32f103rbt6_i2c1_inst_init());
}

/** \brief 传感器 MMC5883MA 实例解初始化 */
am_err_t am_sensor_mmc5883ma_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_mmc5883ma_deinit(handle);
}

/* end of file */
