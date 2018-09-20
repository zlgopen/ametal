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
 * \brief  BMG160三轴陀螺仪传感器应用接口实现
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-11  wk, first implementation.
 * \endinternal
 */

#include "am_bmg160.h"

/**
 * \brief 读取BMG160三轴陀螺仪寄存器
 * \return AM_OK:成功    OTHER：失败
 */
static int __bmg160_read_rate_reg(am_bmg160_handle_t handle, 
                                  uint8_t            reg_addr,
                                  int16_t            *p_rate)
{
    uint8_t rate_temp[2];
    int     ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    ret =  am_i2c_read(p_i2c_dev, reg_addr, rate_temp, 2);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* 数据处理 */
    if (rate_temp[0] & 0x01) {
        *p_rate = (((uint16_t)(rate_temp[1] << 8)
		          | (uint16_t)rate_temp[0])*360/65535);			
    }
		
    return ret;
}
 
 /**
 * \brief 读取bmg160三轴陀螺仪
 * \return AM_OK:成功    OTHER：失败
 */
int am_bmg160_read_rate(am_bmg160_handle_t handle, int16_t* p_rate)
{
    int ret = AM_OK;

    /* 读取X轴角速率 */
    ret = __bmg160_read_rate_reg(handle, BMG160_RATE_X_LSB, &p_rate[0]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  

    /* 读取Y轴加速度 */    
    ret = __bmg160_read_rate_reg(handle, BMG160_RATE_Y_LSB, &p_rate[1]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
		
    /* 读取Z轴加速度 */    
    ret = __bmg160_read_rate_reg(handle, BMG160_RATE_Z_LSB, &p_rate[2]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    return  ret;  
}

/**
 * \brief 读取bmg160温度
 * \return AM_OK:成功    OTHER：失败
 */
int am_bmg160_read_temp (am_bmg160_handle_t handle, int8_t* p_temp)
{
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);  
    
    ret = am_i2c_read(p_i2c_dev,BMG160_TEMP,(uint8_t*)(p_temp), 1);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* 计算温度 */
    *p_temp = (int8_t)(24 + *p_temp * 1.0 / 2);
    
    return  ret;     
}

/**
 * \brief BMG160传感器初始化
 */
am_bmg160_handle_t am_bmg160_init(am_bmg160_dev_t           *p_dev, 
                                  const am_bmg160_devinfo_t *p_devinfo,
                                  am_i2c_handle_t           i2c_handle)
{
    uint8_t bmg160_id =    0; 
  
    /* 验证参数有效性 */
    if (NULL == p_dev ||  NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }
		
    p_dev->i2c_dev.handle = i2c_handle; 
		
    /* 初始配置好BMG160设备信息 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
	              i2c_handle,
                  BMG160_ADDR,
				  AM_I2C_ADDR_7BIT 
		        | AM_I2C_SUBADDR_1BYTE);

    /* 读取BMG160 ID */
    am_i2c_read(&(p_dev->i2c_dev),BMG160_CHIP_ID, &bmg160_id, 1);
    AM_DBG_INFO("bmg160_ID = 0x%x\r\n", bmg160_id);
    
    /* 配置陀螺仪范围为±125°/s */
    am_i2c_write(&(p_dev->i2c_dev), BMG160_RANGE, &(p_devinfo->range), 1);

    /* 配置滤波带宽为230Hz */
    am_i2c_write(&(p_dev->i2c_dev), BMG160_BW, &(p_devinfo->bw), 1);
       
    /* 配置电源为正常模式、睡眠周期2ms */
    am_i2c_write(&(p_dev->i2c_dev), BMG160_LPM1, &(p_devinfo->sleep_period), 1);    
        
    return p_dev;
}

/**
 * \brief BMG160 设备解初始化
 */
void am_bmg160_deinit (am_bmg160_handle_t handle)
{

}

/* end of file */
