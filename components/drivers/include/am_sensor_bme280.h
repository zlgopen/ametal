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
 * \brief 压力温度传感器 BME280 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 18-11-30  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_BME280_H
#define __AM_SENSOR_BME280_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/*
 * \brief 传感器BME280提供的所有通道ID定义
 */
#define AM_BME280_CHAN_1      0             /* BME280 的通道1，压力传感器通道 */
#define AM_BME280_CHAN_2      1             /* BME280 的通道2，温度传感器通道 */
#define AM_BME280_CHAN_3      2             /* BME280 的通道3，湿度传感器通道 */

/**
 * \brief 传感器 BME280 设备信息结构体
 */
typedef struct am_sensor_bme280_devinfo {

    /*
     * \brief I2C 7位 设备地址
     *
     * 该设备7位地址为 0x76 或 0x77
     */
    uint8_t i2c_addr;

} am_sensor_bme280_devinfo_t;

/**
 * \brief BME280 湿度和温度校准值保存
 */
typedef struct am_bme280_calibration_data {
	uint16_t  dig_t1;
	int16_t   dig_t2;
	int16_t   dig_t3;
	uint16_t  dig_p1;
	int16_t   dig_p2;
	int16_t   dig_p3;
	int16_t   dig_p4;
	int16_t   dig_p5;
	int16_t   dig_p6;
	int16_t   dig_p7;
	int16_t   dig_p8;
	int16_t   dig_p9;
	int32_t   t_fine;
    uint8_t   dig_h1;
    int16_t   dig_h2;
    uint8_t   dig_h3;
    int16_t   dig_h4;
    int16_t   dig_h5;
    uint8_t   dig_h6;      
} am_bme280_calibration_data_t;

/**
 * \breif 传感器 BME280 设备结构体定义
 */
typedef struct am_sensor_bme280_dev {
    am_sensor_serv_t                  bme280_dev; /*< \breif 传感器标准服务 */
    am_i2c_device_t                   i2c_dev;    /*< \brief i2c设备实例 */
    am_bme280_calibration_data_t      cal_val;    /*< \brief 保存校准值 */
    uint8_t                           trigger;    /*< \brief 记录标志位 */
    am_sensor_val_t                   data[3];    /*< \brief 内部数据缓存 */

    /** \brief 设备信息 */
    const am_sensor_bme280_devinfo_t *dev_info;

} am_sensor_bme280_dev_t;

/**
 * \brief 传感器 BME280 初始化
 *
 * \param[in] p_dev     : 指向传感器BME280设备的指针
 * \param[in] p_devinfo : 指向传感器BME280设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_bme280_init (
        am_sensor_bme280_dev_t           *p_dev,
        const am_sensor_bme280_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief 传感器 BME280 去初始化
 *
 * \param[in] handle : am_sensor_bme280_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_bme280_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_BME280_H */

/* end of file */
