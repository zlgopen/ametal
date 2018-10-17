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
 * \brief LSM6DSL三轴加速度传感器应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-12  wk, first implementation.
 * \endinternal
 */

#ifndef __AM_LSM6DSL_H
#define __AM_LSM6DSL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
    
/**
 * \addtogroup am_if_lsm6dsl
 * \copydoc am_lsm6dsl.h
 * @{
 */
/**
 * \brief LSM6DSL传感器配置
 */

typedef enum am_lsm6dsl_config {
	
	/**
	 * \brief 可配置的状态寄存器
	 */
	STATUS_REG         =   0x03,           /**< \brief 温度、陀螺仪、加速度计新数据可用 */
	CTRL1_XL           =   0x10,
	
	/**
	 * \brief 可配置的FIFO输出速率
	 */
	FIFO_ODR_12point5  =   0x01,           /**< \brief FIFO输出速率12.5Hz  */
	FIFO_ODR_26        =   0x02,           /**< \brief FIFO输出速率26Hz    */
	FIFO_ODR_52        =   0x03,           /**< \brief FIFO输出速率52Hz    */
	FIFO_ODR_104       =   0x04,           /**< \brief FIFO输出速率104Hz   */
	FIFO_ODR_208       =   0x05,           /**< \brief FIFO输出速率208Hz   */
	FIFO_ODR_416       =   0x06,           /**< \brief FIFO输出速率416Hz   */
	FIFO_ODR_833       =   0x07,           /**< \brief FIFO输出速率833Hz   */
	FIFO_ODR_1point66k =   0x08,           /**< \brief FIFO输出速率1.66kHz */
	FIFO_ODR_3point33k =   0x09,           /**< \brief FIFO输出速率3.33kHz */
	FIFO_ODR_6point66k =   0x0A,           /**< \brief FIFO输出速率6.66kHz */
	
}am_lsm6dsl_config_t;
	
/**
 * \brief BMG160实例信息结构体定义
 */
typedef struct am_lsm6dsl_devinfo {    
	
    uint8_t ctrl1_xl;
    uint8_t status_reg;                     /**< \brief 状态寄存器         */
	uint8_t fifo_odr;                       /**< \brief FIFO输出速率       */
	

} am_lsm6dsl_devinfo_t;

/**
 * \brief LSM6DSL 设备结构体
 */
typedef struct am_lsm6dsl_dev {
    am_i2c_device_t i2c_dev;                /**< \brief LSM6DSL I2C设备    */
} am_lsm6dsl_dev_t;

/** \brief LSM6DSL 操作句柄定义 */
typedef am_lsm6dsl_dev_t *am_lsm6dsl_handle_t;	
	
/**
 * \brief LSM6DSL传感器读取
 *
 * \param[in]  handle  : LSM6DSL服务操作句柄
 * \param[out] p_accel : 指向陀螺仪的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_lsm6dsl_read_temp(am_lsm6dsl_handle_t handle, int16_t* p_temp);


/**
 * \brief LSM6DSL传感器读取
 *
 * \param[in]  handle  : LSM6DSL服务操作句柄
 * \param[out] p_accel : 指向陀螺仪的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
int am_lsm6dsl_read_accel(am_lsm6dsl_handle_t handle, int16_t* p_accel);



/**
 * \brief LSM6DSL传感器初始化
 *
 * \param[in] p_dev      : 指向LSM6DSL设备结构体的指针
 * \param[in] p_devinfo  : 指向LSM6DSL设备信息结构体的指针  
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return LSM6DSL服务操作句柄,如果为 NULL，表明初始化失败
 */
am_lsm6dsl_handle_t am_lsm6dsl_init (am_lsm6dsl_dev_t           *p_dev,
                                     const am_lsm6dsl_devinfo_t *p_devinfo,  
					                 am_i2c_handle_t            i2c_handle);

/**
 * \brief LSM6DSL 设备解初始化
 * \param[in]  handle : LSM6DSL服务操作句柄
 * \return 无
 */
void am_lsm6dsl_deinit(am_lsm6dsl_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LSM6DSL_H */

/* end of file */
