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
 * \brief    MMC5883MA三轴磁传感器应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-12  wk, first implementation.
 * \endinternal
 */

#ifndef __AM_MMC5883MA_H
#define __AM_MMC5883MA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"

/**
 * \addtogroup am_if_mmc5883ma
 * \copydoc am_mmc5883ma.h
 * @{
 */
/**
 * \brief MMC5883MA传感器配置
 */
	
typedef enum am_mmc5883ma_config {
/**
 * \brief 可配置的测量频率     
 */
	Start_Measurement                  = 0x57,  /**< \brief 启动测量 */
	FREQUENCY_14                       = 0x01,  /**< \brief 测量频率14Hz */
	FREQUENCY_5                        = 0x02,  /**< \brief 测量频率5Hz */
	FREQUENCY_2point2                  = 0x03,  /**< \brief 测量频率2.2Hz */
	FREQUENCY_1                        = 0x04,  /**< \brief 测量频率1Hz */
	FREQUENCY_half                     = 0x05,  /**< \brief 测量频率1/2Hz */
	FREQUENCY_one_fourth               = 0x06,  /**< \brief 测量频率1/4Hz */
	FREQUENCY_one_eighths              = 0x07,  /**< \brief 测量频率1/8Hz */
	FREQUENCY_one_sixteenth            = 0x08,  /**< \brief 测量频率1/16Hz */
	FREQUENCY_one_thirty_two           = 0x09,  /**< \brief 测量频率1/32Hz */
	FREQUENCY_one_sixty_four           = 0x0A,  /**< \brief 测量频率1/64Hz */
} am_mmc5883ma_config_t;
	

#define MMC5883MA_ADDR                  0x30  /**< \brief i2c地址 */

/** \brief 设备寄存器地址 */
#define MMC5883MA_XOUT_LOW              0x00  /**< \brief Xout LSB */
#define MMC5883MA_XOUT_HIGH             0x01  /**< \brief Xout MSB */
#define MMC5883MA_YOUT_LOW              0x02  /**< \brief Yout LSB */
#define MMC5883MA_YOUT_HIGH             0x03  /**< \brief Yout MSB */
#define MMC5883MA_ZOUT_LOW              0x04  /**< \brief Zout LSB */
#define MMC5883MA_ZOUT_HIGH             0x05  /**< \brief Zout MSB */
#define MMC5883MA_TEMPERATURE           0x06  /**< \brief Temperature output*/
#define MMC5883MA_STATUS                0x07  /**< \brief Device status */
#define MMC5883MA_INTERNAL_CONTROL_0    0x08  /**< \brief Control register 0 */
#define MMC5883MA_INTERNAL_CONTROL_1    0x09  /**< \brief Control register 1 */
#define MMC5883MA_INTERNAL_CONTROL_2    0x0A  /**< \brief Control register 2 */
#define MMC5883MA_X_THRESHOLD           0x0B  /**< \brief Motion detection threshold of X */
#define MMC5883MA_Y_THRESHOLD           0x0C  /**< \brief Motion detection threshold of Y */
#define MMC5883MA_Z_THRESHOLD           0x0D  /**< \brief Motion detection threshold of Z */
#define MMC5883MA_PRODUCT_ID_1          0x2F  /**< \brief Product ID */

/**
 * \brief MMC5883MA实例信息结构体定义
 */
typedef struct am_mmc5883ma_devinfo {    
    uint8_t frequency;                        /**< \brief 测量频率 */
	uint8_t start_measurement;                /**< \brief 启动测量 */
} am_mmc5883ma_devinfo_t;

/**
 * \brief MMC5883MA 设备结构体
 */
typedef struct am_mmc5883ma_dev {
    am_i2c_device_t i2c_dev;                  /**< \brief MMC5883MA I2C设备 */
} am_mmc5883ma_dev_t;

/** \brief BMG160操作句柄定义 */
typedef am_mmc5883ma_dev_t *am_mmc5883ma_handle_t;

/**
 * \brief MMC5883MA传感器读取
 *
 * \param[in]  handle  : MMC5883MA服务操作句柄
 * \param[out] p_accel : 指向陀螺仪的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_mmc5883ma_read_magnetic(am_mmc5883ma_handle_t handle, 
	                           int16_t*              p_magnetic);

/**
 * \brief MMC5883MA传感器读取温度
 *
 * \param[in]  handle : MMC5883MA服务操作句柄
 * \param[out] p_temp : 指向温度的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_mmc5883ma_read_temp (am_mmc5883ma_handle_t handle,int8_t* p_temp);

/**
 * \brief MMC5883MA传感器初始化
 *
 * \param[in] p_dev      : 指向MMC5883MA设备结构体的指针
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return MMC5883MA服务操作句柄,如果为 NULL，表明初始化失败
 */
am_mmc5883ma_handle_t am_mmc5883ma_init (am_mmc5883ma_dev_t           *p_dev,
                                         const am_mmc5883ma_devinfo_t *p_devinfo,  
					                     am_i2c_handle_t              i2c_handle);

/**
 * \brief MMC5883MA 设备解初始化
 * \param[in]  handle : MMC5883MA服务操作句柄
 * \return 无
 */
void am_mmc5883ma_deinit(am_mmc5883ma_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MMC5883MA_H */

/* end of file */
