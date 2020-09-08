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
 * \brief 传感器 AS7262 (I2C模式) 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_as7262_i2c.h"
#include "am_common.h"
#include "stm32f103rbt6_pin.h"
#include "am_stm32f103rbt6_inst_init.h"

am_const am_local as7262_param_config_t __g_as7262_param_default = {
    AM_AS7262_GAIN_1,                   /**< \brief GAIN    */
    AM_AS7262_BANK_MODE_3,              /**< \brief BANK    */
    0x39,                               /**< \brief ITIME (积分时间为 ITIME * 2.8ms)  */
    AM_AS7262_LED_DRV_12P5_MA,          /**< \brief ICL_DRV */
    AM_AS7262_LED_DRV_DISABLE,          /**< \brief LED_DRV */
    AM_AS7262_LED_IND_1_MA,             /**< \brief ICL_IND */
    AM_AS7262_LED_IND_ENABLE,           /**< \brief LED_IND */
};

/** \brief 传感器 AS7262 设备信息实例 */
am_const am_local struct am_sensor_as7262_devinfo __g_as7262_info = {
    &__g_as7262_param_default,          /**< \brief 参数初始化 */
    PIOA_6,                             /**< \brief INT引脚 */
    -1,                                 /**< \brief RST引脚 */
};

/** \brief 传感器 AS7262 (I2C模式) 设备信息实例 */
am_const am_local am_sensor_as7262_i2c_devinfo_t __g_as7262_i2c_info = {
    &__g_as7262_info,                   /**< \brief 设备信息 */
    am_stm32f103rbt6_i2c2_inst_init,           /**< \brief I2C句柄获取函数*/
    am_stm32f103rbt6_i2c2_inst_deinit,         /**< \brief I2C解初始化函数*/
    0x49,                               /**< \brief I2C 7位 设备地址 */
};

/** \breif 传感器 AS7262 设备结构体定义 */
am_local am_sensor_as7262_i2c_dev_t __g_as7262_i2c_dev;

/** \brief 传感器 AS7262 设备实例化 */
am_sensor_handle_t am_sensor_as7262_i2c_inst_init (void)
{
    return am_sensor_as7262_i2c_init(&__g_as7262_i2c_dev,
                                     &__g_as7262_i2c_info);
}

/** \brief 传感器 AS7262 实例解初始化 */
am_err_t am_sensor_as7262_i2c_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_as7262_i2c_deinit(handle);
}

/* end of file */

