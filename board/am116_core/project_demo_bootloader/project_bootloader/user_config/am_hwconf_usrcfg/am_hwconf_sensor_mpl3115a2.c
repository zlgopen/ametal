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
 * \brief 传感器 MPL3115A2 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-11  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_mpl3115a2.h"
#include "am_common.h"
#include "zlg116_pin.h"
#include "am_zlg116_inst_init.h"

/** \brief 传感器 MPL3115A2 设备信息实例 */
am_const am_local struct am_sensor_mpl3115a2_devinfo __g_mpl3115a2_info = {
    PIOB_0,                     /*< \brief 触发引脚定义    */
    0x60,                       /*< \breif MPL3115A2 I2C地址 */
    am_zlg116_i2c1_inst_init,   /*< \brief I2C重新实例初始化函数 */
    am_zlg116_i2c1_inst_deinit, /*< \brief I2C解初始化函数 */
};

/** \breif 传感器 MPL3115A2 设备结构体定义 */
am_local struct am_sensor_mpl3115a2_dev __g_mpl3115a2_dev;

/** \brief 传感器 MPL3115A2 设备实例化 */
am_sensor_handle_t am_sensor_mpl3115a2_inst_init (void)
{
    return am_sensor_mpl3115a2_init(&__g_mpl3115a2_dev,
                                    &__g_mpl3115a2_info,
                                    am_zlg116_i2c1_inst_init());
}

/** \brief 传感器 MPL3115A2 实例解初始化 */
am_err_t am_sensor_mpl3115a2_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_mpl3115a2_deinit(handle);
}

/* end of file */
