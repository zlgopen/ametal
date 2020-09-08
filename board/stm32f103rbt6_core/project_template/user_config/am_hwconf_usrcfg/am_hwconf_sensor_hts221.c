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
 * \brief 传感器 HTS221 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-26  wan, first implementation.
 * \endinternal
 */

#include "stm32f103rbt6_pin.h"
#include "am_sensor_hts221.h"
#include "am_common.h"
#include "am_stm32f103rbt6_inst_init.h"

/** \brief 传感器 HTS221 设备信息实例 */
am_const am_local struct am_sensor_hts221_devinfo __g_hts221_info = {
        PIOB_0,            /*< \brief 触发引脚定义   */
        0x5f               /*< \breif HTS221 I2C地址 */
};

/** \breif 传感器 HTS221 设备结构体定义 */
am_local struct am_sensor_hts221_dev __g_hts221_dev;

/** \brief 传感器 HTS221 设备实例化 */
am_sensor_handle_t am_sensor_hts221_inst_init (void)
{
    return am_sensor_hts221_init(&__g_hts221_dev,
                                 &__g_hts221_info,
                                 am_stm32f103rbt6_i2c1_inst_init());
}

/** \brief 传感器 HTS221 实例解初始化 */
am_err_t am_sensor_hts221_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_hts221_deinit(handle);
}

/* end of file */
