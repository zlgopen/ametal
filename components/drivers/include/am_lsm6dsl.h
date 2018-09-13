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
 * \brief    LSM6DSL三轴加速度传感器应用接口文件
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
#include "am_zlg116_inst_init.h"
    
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
	STATUS_REG         =   0x03,           /* 温度、陀螺仪、加速度计新数据可用 */
	
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
  uint8_t status_reg;                       /**< \brief 状态寄存器         */
	uint8_t fifo_odr;                       /**< \brief FIFO输出速率         */
	

} am_lsm6dsl_devinfo_t;

	/** \brief i2c地址 */
#define    LSM6DSL_ADDR                         0x6B

/** \brief 设备寄存器地址 */
#define    LSM6DSL_FUNC_CFG_ACCESS              0x01   /* 嵌入式功能配置寄存器 */

#define    LSM6DSL_SENSOR_SYNC_TIME_FRAME       0x04   /* 传感器同步配置寄存器 */
#define    LSM6DSL_SENSOR_SYNC_RES_RATIO        0x05

#define    LSM6DSL_FIFO_CTRL1                   0x06   /* FIFO配置寄存器 */
#define    LSM6DSL_FIFO_CTRL2                   0x07
#define    LSM6DSL_FIFO_CTRL3                   0x08
#define    LSM6DSL_FIFO_CTRL4                   0x09
#define    LSM6DSL_FIFO_CTRL5                   0x0A

#define    LSM6DSL_DRDY_PULSE_CFG_G             0x0B
#define    LSM6DSL_RESERVED                     0x0C
#define    LSM6DSL_INT1_CTRL                    0x0D   /* INT1引脚控制 */
#define    LSM6DSL_INT2_CTRL                    0x0E   /* INT2引脚控制 */

#define    LSM6DSL_WHO_AM_I                     0x0F   /* ID */

#define    LSM6DSL_CTRL1_XL                     0x10   /* 加速度计和陀螺仪控制寄存器 */
#define    LSM6DSL_CTRL2_G                      0x11
#define    LSM6DSL_CTRL3_C                      0x12
#define    LSM6DSL_CTRL4_C                      0x13
#define    LSM6DSL_CTRL5_C                      0x14
#define    LSM6DSL_CTRL6_C                      0x15
#define    LSM6DSL_CTRL7_G                      0x16
#define    LSM6DSL_CTRL8_XL                     0x17
#define    LSM6DSL_CTRL9_XL                     0x18
#define    LSM6DSL_CTRL10_C                     0x19

#define    LSM6DSL_MASTER_CONFIG                0x1A   /* I2C主配置寄存器 */

#define    LSM6DSL_WAKE_UP_SRC                  0x1B   /* 中断寄存器 */
#define    LSM6DSL_TAP_SRC                      0x1C
#define    LSM6DSL_D6D_SRC                      0x1D

#define    LSM6DSL_STATUS_REG                   0x1E    /* 状态数据寄存器用户接口 */

#define    LSM6DSL_OUT_TEMP_L                   0x20    /* 温度输出数据寄存器 */
#define    LSM6DSL_OUT_TEMP_H                   0x21

#define    LSM6DSL_OUTX_L_G                     0x22    /* 陀螺仪输出寄存器用户接口 */
#define    LSM6DSL_OUTX_H_G                     0x23
#define    LSM6DSL_OUTY_L_G                     0x24
#define    LSM6DSL_OUTY_H_G                     0x25
#define    LSM6DSL_OUTZ_L_G                     0x26	
#define    LSM6DSL_OUTZ_H_G                     0x27

#define    LSM6DSL_OUTX_L_XL                    0x28    /* 加速度计输出寄存器 */
#define    LSM6DSL_OUTX_H_XL                    0x29
#define    LSM6DSL_OUTY_L_XL                    0x2A
#define    LSM6DSL_OUTY_H_XL                    0x2B
#define    LSM6DSL_OUTZ_L_XL                    0x2C
#define    LSM6DSL_OUTZ_H_XL                    0x2D

#define    LSM6DSL_ENSORHUB1_REG                0x2E   /* 传感器集线器输出寄存器 */
#define    LSM6DSL_ENSORHUB2_REG                0x2F
#define    LSM6DSL_ENSORHUB3_REG                0x30
#define    LSM6DSL_ENSORHUB4_REG                0x31
#define    LSM6DSL_ENSORHUB5_REG                0x32
#define    LSM6DSL_ENSORHUB6_REG                0x33
#define    LSM6DSL_ENSORHUB7_REG                0x34
#define    LSM6DSL_ENSORHUB8_REG                0x35
#define    LSM6DSL_ENSORHUB9_REG                0x36
#define    LSM6DSL_ENSORHUB10_REG               0x37
#define    LSM6DSL_ENSORHUB11_REG               0x38
#define    LSM6DSL_ENSORHUB12_REG               0x39

#define    LSM6DSL_FIFO_STATUS1                 0x3A   /* FIFO状态寄存器 */
#define    LSM6DSL_FIFO_STATUS2                 0x3B
#define    LSM6DSL_FIFO_STATUS3                 0x3C
#define    LSM6DSL_FIFO_STATUS4                 0x3D

#define    LSM6DSL_FIFO_DATA_OUT_L              0x3E   /* FIFO数据输出寄存器 */
#define    LSM6DSL_FIFO_DATA_OUT_H              0x3F

#define    LSM6DSL_TIMESTAMP0_REG               0x40   /* 时间戳输出寄存器 */
#define    LSM6DSL_TIMESTAMP1_REG               0x41
#define    LSM6DSL_TIMESTAMP2_REG               0x42

#define    LSM6DSL_STEP_TIMESTAMP_L             0x49   /* 步进计数器时间戳寄存器 */
#define    LSM6DSL_STEP_TIMESTAMP_H             0x4A

#define    LSM6DSL_STEP_COUNTER_L               0x4B   /* 步数计数器输出寄存器 */
#define    LSM6DSL_STEP_COUNTER_H               0x4C

#define    LSM6DSL_SENSORHUB13_REG              0x4D   /* 传感器集线器输出寄存器 */
#define    LSM6DSL_SENSORHUB14_REG              0x4E
#define    LSM6DSL_SENSORHUB15_REG              0x4F
#define    LSM6DSL_SENSORHUB16_REG              0x50
#define    LSM6DSL_SENSORHUB17_REG              0x51
#define    LSM6DSL_SENSORHUB18_REG              0x52

#define    LSM6DSL_FUNC_SRC1                    0x53   /* 中断寄存器 */
#define    LSM6DSL_FUNC_SRC2                    0x54

#define    LSM6DSL_WRIST_TILT_IA                0x55   /* 中断寄存器 */

#define    LSM6DSL_TAP_CFG                      0x58   /* 中断寄存器 */
#define    LSM6DSL_TAP_THS_6D                   0x59
#define    LSM6DSL_INT_DUR2                     0x5A
#define    LSM6DSL_WAKE_UP_THS                  0x5B
#define    LSM6DSL_WAKE_UP_DUR                  0x5C
#define    LSM6DSL_FREE_FALL                    0x5D
#define    LSM6DSL_MD1_CFG                      0x5E
#define    LSM6DSL_MD2_CFG                      0x5F

#define    LSM6DSL_MASTER_CMD_CODE              0x60
#define    LSM6DSL_SENS_SYNC_SPI_ERROR_CODE     0x61

#define    LSM6DSL_OUT_MAG_RAW_X_L              0x66   /* 外部磁力计原始数据输出寄存器 */
#define    LSM6DSL_OUT_MAG_RAW_X_H              0x67
#define    LSM6DSL_OUT_MAG_RAW_Y_L              0x68
#define    LSM6DSL_OUT_MAG_RAW_Y_H              0x69
#define    LSM6DSL_OUT_MAG_RAW_Z_L              0x6A
#define    LSM6DSL_OUT_MAG_RAW_Z_H              0x6B

#define    LSM6DSL_X_OFS_USR                    0x73   /* 加速度计用户偏移校正 */
#define    LSM6DSL_Y_OFS_USR                    0x74
#define    LSM6DSL_Z_OFS_USR                    0x75


/**
 * \brief LSM6DSL 设备结构体
 */
typedef struct am_lsm6dsl_dev {
    am_i2c_device_t i2c_dev;       /**< \brief LSM6DSL I2C设备 */
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
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return LSM6DSL服务操作句柄,如果为 NULL，表明初始化失败
 */
am_lsm6dsl_handle_t am_lsm6dsl_init (am_lsm6dsl_dev_t *p_dev,
                         const am_lsm6dsl_devinfo_t *p_devinfo,  
																   am_i2c_handle_t i2c_handle);

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
