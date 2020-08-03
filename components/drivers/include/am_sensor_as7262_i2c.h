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
 * \brief 光谱传感器 AS7262 (I2C模式) 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_AS7262_I2C_H
#define __AM_SENSOR_AS7262_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_sensor_as7262.h"
#include "am_i2c.h"

/**
 * \brief 传感器 AS7262 (I2C模式) 设备信息结构体
 */
typedef struct am_sensor_as7262_i2c_devinfo {

    const am_sensor_as7262_devinfo_t        *p_devinfo;

    am_i2c_handle_t (*pfn_i2c_handle_get) (void);           /**< \brief I2C句柄获取函数 */

    void (*pfn_i2c_deinit) (am_i2c_handle_t i2c_handle);    /**< \brief I2C解初始化函数 */

    uint8_t                                 i2c_addr;       /**< \brief I2C 7位 设备地址 */

} am_sensor_as7262_i2c_devinfo_t;

/**
 * \breif 传感器 AS7262 (I2C模式) 设备结构体定义
 */
typedef struct am_sensor_as7262_i2c_dev {
    am_sensor_as7262_dev_t                  isa;                    /**< \breif 传感器标准服务 */

    am_i2c_device_t                         i2c_dev;                /**< \brief I2C设备实例 */

    const am_sensor_as7262_i2c_devinfo_t    *sensor_i2c_dev_info;   /**< \brief 设备信息 */

} am_sensor_as7262_i2c_dev_t;

/**
 * \brief 传感器 AS7262 (I2C模式) 初始化
 *
 * \param[in] p_dev     : 指向传感器AS7262(I2C模式)设备的指针
 * \param[in] p_devinfo : 指向传感器AS7262(I2C模式)设备信息的指针
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_as7262_i2c_init (
        am_sensor_as7262_i2c_dev_t              *p_dev,
        const am_sensor_as7262_i2c_devinfo_t    *p_devinfo);

/**
 * \brief 传感器 AS7262 (I2C模式) 去初始化
 *
 * \param[in] handle : am_sensor_as7262_i2c_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_as7262_i2c_deinit (am_sensor_handle_t handle);


#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_AS7262_I2C_H */

/* end of file */

