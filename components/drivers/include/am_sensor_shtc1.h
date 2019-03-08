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
 * \brief 温湿度传感器 SHTC1 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 19-02-20  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_SHTC1_H
#define __AM_SENSOR_SHTC1_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/*
 * \brief 传感器SHTC1提供的所有通道ID定义
 */
#define AM_SHTC1_CHAN_1      0             /* SHTC1 的通道1，湿度传感器通道 */
#define AM_SHTC1_CHAN_2      1             /* SHTC1 的通道2，温度传感器通道 */

/**
 * \brief 传感器 SHTC1 设备信息结构体
 */
typedef struct am_sensor_shtc1_devinfo {

    /*
     * \brief I2C 7位 设备地址
     *
     * 该设备7位地址固定为 0x70
     */
    uint8_t i2c_addr;

} am_sensor_shtc1_devinfo_t;

/**
 * \breif 传感器 SHTC1 设备结构体定义
 */
typedef struct am_sensor_shtc1_dev {
    am_sensor_serv_t                  shtc1_dev;  /*< \breif 传感器标准服务   */
    am_i2c_device_t                   i2c_dev;    /*< \brief i2c设备实例      */
    uint8_t                           trigger;    /*< \brief 记录标志位       */
    am_sensor_val_t                   data[2];    /*< \brief 内部数据缓存     */

    /** \brief 设备信息 */
    const am_sensor_shtc1_devinfo_t *dev_info;

} am_sensor_shtc1_dev_t;

/**
 * \brief 传感器 SHTC1 初始化
 *
 * \param[in] p_dev     : 指向传感器SHTC1设备的指针
 * \param[in] p_devinfo : 指向传感器SHTC1设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_shtc1_init (
        am_sensor_shtc1_dev_t            *p_dev,
        const am_sensor_shtc1_devinfo_t  *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief 传感器 SHTC1 去初始化
 *
 * \param[in] handle : am_sensor_shtc1_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_shtc1_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_SHTC1_H */

/* end of file */
