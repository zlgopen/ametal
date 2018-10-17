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
 * \brief  LSM6DSL三轴加速度传感器应用接口实现
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-12  wk, first implementation.
 * \endinternal
 */

#include "am_lsm6dsl.h"


/** \brief i2c地址 */
#define    LSM6DSL_ADDR                         0x6B

/** \brief 设备寄存器地址 */
#define    LSM6DSL_FUNC_CFG_ACCESS              0x01   /**< \brief 嵌入式功能配置寄存器 */

#define    LSM6DSL_SENSOR_SYNC_TIME_FRAME       0x04   /**< \brief 传感器同步配置寄存器 */
#define    LSM6DSL_SENSOR_SYNC_RES_RATIO        0x05

#define    LSM6DSL_FIFO_CTRL1                   0x06   /**< \brief FIFO配置寄存器 */
#define    LSM6DSL_FIFO_CTRL2                   0x07
#define    LSM6DSL_FIFO_CTRL3                   0x08
#define    LSM6DSL_FIFO_CTRL4                   0x09
#define    LSM6DSL_FIFO_CTRL5                   0x0A

#define    LSM6DSL_DRDY_PULSE_CFG_G             0x0B
#define    LSM6DSL_RESERVED                     0x0C
#define    LSM6DSL_INT1_CTRL                    0x0D   /**< \brief INT1引脚控制 */
#define    LSM6DSL_INT2_CTRL                    0x0E   /**< \brief INT2引脚控制 */

#define    LSM6DSL_WHO_AM_I                     0x0F   /**< \brief ID */

#define    LSM6DSL_CTRL1_XL                     0x10   /**< \brief 加速度计和陀螺仪控制寄存器 */
#define    LSM6DSL_CTRL2_G                      0x11
#define    LSM6DSL_CTRL3_C                      0x12
#define    LSM6DSL_CTRL4_C                      0x13
#define    LSM6DSL_CTRL5_C                      0x14
#define    LSM6DSL_CTRL6_C                      0x15
#define    LSM6DSL_CTRL7_G                      0x16
#define    LSM6DSL_CTRL8_XL                     0x17
#define    LSM6DSL_CTRL9_XL                     0x18
#define    LSM6DSL_CTRL10_C                     0x19

#define    LSM6DSL_MASTER_CONFIG                0x1A   /**< \brief I2C主配置寄存器 */

#define    LSM6DSL_WAKE_UP_SRC                  0x1B   /**< \brief 中断寄存器 */
#define    LSM6DSL_TAP_SRC                      0x1C
#define    LSM6DSL_D6D_SRC                      0x1D

#define    LSM6DSL_STATUS_REG                   0x1E   /**< \brief 状态数据寄存器用户接口 */

#define    LSM6DSL_OUT_TEMP_L                   0x20   /**< \brief 温度输出数据寄存器 */
#define    LSM6DSL_OUT_TEMP_H                   0x21

#define    LSM6DSL_OUTX_L_G                     0x22   /**< \brief 陀螺仪输出寄存器用户接口 */
#define    LSM6DSL_OUTX_H_G                     0x23
#define    LSM6DSL_OUTY_L_G                     0x24
#define    LSM6DSL_OUTY_H_G                     0x25
#define    LSM6DSL_OUTZ_L_G                     0x26	
#define    LSM6DSL_OUTZ_H_G                     0x27

#define    LSM6DSL_OUTX_L_XL                    0x28   /**< \brief 加速度计输出寄存器 */
#define    LSM6DSL_OUTX_H_XL                    0x29
#define    LSM6DSL_OUTY_L_XL                    0x2A
#define    LSM6DSL_OUTY_H_XL                    0x2B
#define    LSM6DSL_OUTZ_L_XL                    0x2C
#define    LSM6DSL_OUTZ_H_XL                    0x2D

#define    LSM6DSL_ENSORHUB1_REG                0x2E   /**< \brief 传感器集线器输出寄存器 */
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

#define    LSM6DSL_FIFO_STATUS1                 0x3A   /**< \brief FIFO状态寄存器 */
#define    LSM6DSL_FIFO_STATUS2                 0x3B
#define    LSM6DSL_FIFO_STATUS3                 0x3C
#define    LSM6DSL_FIFO_STATUS4                 0x3D

#define    LSM6DSL_FIFO_DATA_OUT_L              0x3E   /**< \brief FIFO数据输出寄存器 */
#define    LSM6DSL_FIFO_DATA_OUT_H              0x3F

#define    LSM6DSL_TIMESTAMP0_REG               0x40   /**< \brief 时间戳输出寄存器 */
#define    LSM6DSL_TIMESTAMP1_REG               0x41
#define    LSM6DSL_TIMESTAMP2_REG               0x42

#define    LSM6DSL_STEP_TIMESTAMP_L             0x49   /**< \brief 步进计数器时间戳寄存器 */
#define    LSM6DSL_STEP_TIMESTAMP_H             0x4A

#define    LSM6DSL_STEP_COUNTER_L               0x4B   /**< \brief 步数计数器输出寄存器 */
#define    LSM6DSL_STEP_COUNTER_H               0x4C

#define    LSM6DSL_SENSORHUB13_REG              0x4D   /**< \brief 传感器集线器输出寄存器 */
#define    LSM6DSL_SENSORHUB14_REG              0x4E
#define    LSM6DSL_SENSORHUB15_REG              0x4F
#define    LSM6DSL_SENSORHUB16_REG              0x50
#define    LSM6DSL_SENSORHUB17_REG              0x51
#define    LSM6DSL_SENSORHUB18_REG              0x52

#define    LSM6DSL_FUNC_SRC1                    0x53   /**< \brief 中断寄存器 */
#define    LSM6DSL_FUNC_SRC2                    0x54

#define    LSM6DSL_WRIST_TILT_IA                0x55   /**< \brief 中断寄存器 */

#define    LSM6DSL_TAP_CFG                      0x58   /**< \brief 中断寄存器 */
#define    LSM6DSL_TAP_THS_6D                   0x59
#define    LSM6DSL_INT_DUR2                     0x5A
#define    LSM6DSL_WAKE_UP_THS                  0x5B
#define    LSM6DSL_WAKE_UP_DUR                  0x5C
#define    LSM6DSL_FREE_FALL                    0x5D
#define    LSM6DSL_MD1_CFG                      0x5E
#define    LSM6DSL_MD2_CFG                      0x5F

#define    LSM6DSL_MASTER_CMD_CODE              0x60
#define    LSM6DSL_SENS_SYNC_SPI_ERROR_CODE     0x61

#define    LSM6DSL_OUT_MAG_RAW_X_L              0x66   /**< \brief 数据输出寄存器 */
#define    LSM6DSL_OUT_MAG_RAW_X_H              0x67
#define    LSM6DSL_OUT_MAG_RAW_Y_L              0x68
#define    LSM6DSL_OUT_MAG_RAW_Y_H              0x69
#define    LSM6DSL_OUT_MAG_RAW_Z_L              0x6A
#define    LSM6DSL_OUT_MAG_RAW_Z_H              0x6B

#define    LSM6DSL_X_OFS_USR                    0x73   /**< \brief 加速度计用户偏移校正 */
#define    LSM6DSL_Y_OFS_USR                    0x74
#define    LSM6DSL_Z_OFS_USR                    0x75

/**
 * \brief 读取LSM6DSL三轴加速度寄存器
 * \return AM_OK:成功    OTHER：失败
 */
static int __lsm6dsl_read_reg(am_lsm6dsl_handle_t handle, 
                              uint8_t             reg_addr,
                              int16_t             *p_accel)
{
    uint8_t accel_temp[2];
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    ret =  am_i2c_read(p_i2c_dev, reg_addr, accel_temp, 2);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    /* 数据处理 */
    if (accel_temp[0] & 0x01) {
        *p_accel = (((uint16_t)(accel_temp[1] << 8)
		           | (uint16_t)accel_temp[0])*360/65535);			
    }
    
    return ret;
}
 

int am_lsm6dsl_read_temp(am_lsm6dsl_handle_t handle, int16_t* p_temp)
{
	int ret = AM_OK;
    /* 读取温度 */    
        ret = __lsm6dsl_read_reg(handle, LSM6DSL_OUT_TEMP_L, p_temp);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
	/* 计算温度 */
    *p_temp = (int8_t)(24 + *p_temp * 1.0 / 2);
		
    return  ret;
}


/**
 * \brief 读取lsm6dsl三轴加速度
 * \return AM_OK:成功    OTHER：失败
 */
int am_lsm6dsl_read_accel(am_lsm6dsl_handle_t handle, int16_t* p_accel)
{
    int ret = AM_OK;
    /* 读取X轴角速率 */
    ret = __lsm6dsl_read_reg(handle, LSM6DSL_OUTX_L_XL, &p_accel[0]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  

    /* 读取Y轴加速度 */    
    ret = __lsm6dsl_read_reg(handle, LSM6DSL_OUTY_L_XL, &p_accel[1]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
		
    /* 读取Z轴加速度 */    
    ret = __lsm6dsl_read_reg(handle, LSM6DSL_OUTZ_L_XL, &p_accel[2]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
		
    return  ret;  
}

/**
 * \brief LSM6DSL三轴磁传感器初始化
 */
am_lsm6dsl_handle_t am_lsm6dsl_init(am_lsm6dsl_dev_t     *p_dev, 
                              const am_lsm6dsl_devinfo_t *p_devinfo,
                                    am_i2c_handle_t      i2c_handle)
{
    uint8_t lsm6dsl_id =    0; 
  
    /* 验证参数有效性 */
    if (NULL == p_dev ||  NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }
		
    p_dev->i2c_dev.handle = i2c_handle; 
		
    /* 初始配置好LSM6DSL设备信息 */
    am_i2c_mkdev(&(p_dev->i2c_dev),i2c_handle,
                 LSM6DSL_ADDR,AM_I2C_ADDR_7BIT 
		         | AM_I2C_SUBADDR_1BYTE);

    /* 读取LSM6DSL ID */
    am_i2c_read(&(p_dev->i2c_dev),LSM6DSL_WHO_AM_I, &lsm6dsl_id, 1);
    AM_DBG_INFO("lsm6dsl_ID = 0x%x\r\n", lsm6dsl_id);
		
		
    /* 配置状态寄存器 */
    am_i2c_write(&(p_dev->i2c_dev), LSM6DSL_CTRL1_XL, &(p_devinfo->ctrl1_xl), 1);
		
		
    /* 配置状态寄存器 */
    am_i2c_write(&(p_dev->i2c_dev), LSM6DSL_STATUS_REG, &(p_devinfo->status_reg), 1);
		
		
    /* 配置FIFO输出速率 */
    am_i2c_write(&(p_dev->i2c_dev), LSM6DSL_FIFO_CTRL5, &(p_devinfo->fifo_odr), 1);
		
    return p_dev;
}

/**
 * \brief LSM6DSL 设备解初始化
 */
void am_lsm6dsl_deinit (am_lsm6dsl_handle_t handle)
{
	
}

/* end of file */
