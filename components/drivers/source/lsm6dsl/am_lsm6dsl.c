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

/**
 * \brief 读取LSM6DSL三轴加速度寄存器
 * \return AM_OK:成功    OTHER：失败
 */
static int __lsm6dsl_read_reg(am_lsm6dsl_handle_t handle, 
                                        uint8_t reg_addr,
                                        int16_t *p_accel)
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
        *p_accel = (((uint16_t)(accel_temp[1] << 8)| (uint16_t)accel_temp[0])*360/65535);			
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
am_lsm6dsl_handle_t am_lsm6dsl_init(am_lsm6dsl_dev_t *p_dev, 
                          const am_lsm6dsl_devinfo_t *p_devinfo,
                                       am_i2c_handle_t i2c_handle)
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
		
		
    /* 配置FIFO输出速率为12.5Hz */
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
