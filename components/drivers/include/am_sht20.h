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
 * \brief  SHT20 温湿度传感器应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 2017-7-13  sky, first implementation.
 * \endinternal
 */

#ifndef __AM_SHT20_H
#define __AM_SHT20_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"

/**
 * \addtogroup am_if_sht20
 * \copydoc am_sht20.h
 * @{
 */

/**
 * \brief SHT20 设备结构体
 */
typedef struct am_sht20_dev {
    am_i2c_device_t i2c_dev;       /**< \brief SHT20 I2C设备 */
} am_sht20_dev_t;

/** \brief PCF85063操作句柄定义 */
typedef am_sht20_dev_t *am_sht20_handle_t;

/**
 * \brief SHT20传感器读取温度
 *
 * \param[in]  handle        : SHT20服务操作句柄
 * \param[out] p_temperature : 指向温度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_sht20_temperature_read (am_sht20_handle_t handle, float *p_temperature);

/**
 * \brief SHT20传感器读取湿度
 *
 * \param[in]  handle     : SHT20服务操作句柄
 * \param[out] p_humidity : 指向相对湿度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_sht20_humidity_read (am_sht20_handle_t handle, float *p_humidity);

/**
 * \brief SHT20传感器软件复位
 * \param[in]  handle : SHT20服务操作句柄
 * \retval AM_OK : 软件复位成功
 * \retval OTHER : 软件复位失败
 */
int am_sht20_soft_reset (am_sht20_handle_t handle);

/**
 * \brief SHT20传感器初始化
 *
 * \param[in] p_dev      : 指向SHT20设备结构体的指针
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return SHT20服务操作句柄,如果为 NULL，表明初始化失败
 */
am_sht20_handle_t am_sht20_init (am_sht20_dev_t *p_dev, am_i2c_handle_t i2c_handle);

/**
 * \brief SHT20 设备解初始化
 * \param[in]  handle : SHT20服务操作句柄
 * \return 无
 */
void am_sht20_deinit (am_sht20_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_PCF85063_H */

/* end of file */
