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
 * \brief  BMA253三轴加速度传感器应用接口实现
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-07  yrz, first implementation.
 * \endinternal
 */

#include "am_bma253.h"

/**
 * \brief BMA253从机地址
 */    
#define BMA253_DEVICE_ADDR			             0x18 
    
/**
 * \brief BMA253 ID寄存器地址
 */    
#define BMA2x2_CHIP_ID_ADDR			             0x00 
    
/**
 * \brief 数据寄存器地址
 */
#define BMA2x2_X_AXIS_LSB_ADDR                   0x02 
#define BMA2x2_X_AXIS_MSB_ADDR                   0x03 
#define BMA2x2_Y_AXIS_LSB_ADDR                   0x04 
#define BMA2x2_Y_AXIS_MSB_ADDR                   0x05 
#define BMA2x2_Z_AXIS_LSB_ADDR                   0x06 
#define BMA2x2_Z_AXIS_MSB_ADDR                   0x07 
#define BMA2x2_TEMP_ADDR			             0x08 

/**
 * \brief 状态寄存器地址
 */
#define BMA2x2_STAT1_ADDR			             0x09 
#define BMA2x2_STAT2_ADDR			             0x0A 
#define BMA2x2_STAT_TAP_SLOPE_ADDR		         0x0B 
#define BMA2x2_STAT_ORIENT_HIGH_ADDR		     0x0C 
#define BMA2x2_STAT_FIFO_ADDR			         0x0E 
#define BMA2x2_RANGE_SELECT_ADDR		         0x0F 
#define BMA2x2_BW_SELECT_ADDR                    0x10 
#define BMA2x2_MODE_CTRL_ADDR                    0x11 
#define BMA2x2_LOW_NOISE_CTRL_ADDR               0x12 
#define BMA2x2_DATA_CTRL_ADDR                    0x13 
#define BMA2x2_RST_ADDR                          0x14 

/**
 * \brief 中断寄存器地址
 */
#define BMA2x2_INTR_ENABLE1_ADDR                 0x16 
#define BMA2x2_INTR_ENABLE2_ADDR                 0x17 
#define BMA2x2_INTR_SLOW_NO_MOTION_ADDR          0x18 
#define BMA2x2_INTR1_PAD_SELECT_ADDR             0x19 
#define BMA2x2_INTR_DATA_SELECT_ADDR             0x1A 
#define BMA2x2_INTR2_PAD_SELECT_ADDR             0x1B 
#define BMA2x2_INTR_SOURCE_ADDR                  0x1E 
#define BMA2x2_INTR_SET_ADDR                     0x20 
#define BMA2x2_INTR_CTRL_ADDR                    0x21 

/**
 * \brief 特性寄存器地址
 */
#define BMA2x2_LOW_DURN_ADDR                     0x22 
#define BMA2x2_LOW_THRES_ADDR                    0x23 
#define BMA2x2_LOW_HIGH_HYST_ADDR                0x24 
#define BMA2x2_HIGH_DURN_ADDR                    0x25 
#define BMA2x2_HIGH_THRES_ADDR                   0x26 
#define BMA2x2_SLOPE_DURN_ADDR                   0x27 
#define BMA2x2_SLOPE_THRES_ADDR                  0x28 
#define BMA2x2_SLOW_NO_MOTION_THRES_ADDR         0x29 
#define BMA2x2_TAP_PARAM_ADDR                    0x2A 
#define BMA2x2_TAP_THRES_ADDR                    0x2B 
#define BMA2x2_ORIENT_PARAM_ADDR                 0x2C 
#define BMA2x2_THETA_BLOCK_ADDR                  0x2D 
#define BMA2x2_THETA_FLAT_ADDR                   0x2E 
#define BMA2x2_FLAT_HOLD_TIME_ADDR               0x2F 

#define BMA2x2_SELFTEST_ADDR                     0x32 
#define BMA2x2_EEPROM_CTRL_ADDR                  0x33 
#define BMA2x2_SERIAL_CTRL_ADDR                  0x34 

/**
 * \brief 偏差寄存器地址
 */
#define BMA2x2_OFFSET_CTRL_ADDR                  0x36 
#define BMA2x2_OFFSET_PARAMS_ADDR                0x37 
#define BMA2x2_OFFSET_X_AXIS_ADDR                0x38 
#define BMA2x2_OFFSET_Y_AXIS_ADDR                0x39 
#define BMA2x2_OFFSET_Z_AXIS_ADDR                0x3A 

/**
 * \brief GP寄存器地址
 */
#define BMA2x2_GP0_ADDR                          0x3B 
#define BMA2x2_GP1_ADDR                          0x3C 

/**
 * \brief FIFO寄存器地址
 */
#define BMA2x2_FIFO_MODE_ADDR                    0x3E 
#define BMA2x2_FIFO_DATA_OUTPUT_ADDR             0x3F 
#define BMA2x2_FIFO_WML_TRIG                     0x30 

/**
 * \brief 软件复位
 */
#define SOFT_RESET                               0xB6  

/**
 * \brief 读取bma253三轴加速度寄存器
 * \return AM_OK:成功    OTHER：失败
 */
static int __bma253_read_accel_reg (am_bma253_handle_t handle, 
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
        *p_accel = (int16_t)((accel_temp[1]) << 4 | accel_temp[0] >> 4);    
    }
    return ret;
}

/**
 * \brief 读取bma253三轴加速度
 * \return AM_OK:成功    OTHER：失败
 */
int am_bma253_read_accel (am_bma253_handle_t handle, int16_t* p_accel)
{
    int ret = AM_OK;

    /* 读取X轴加速度 */
    ret = __bma253_read_accel_reg(handle, BMA2x2_X_AXIS_LSB_ADDR, &p_accel[0]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  

    /* 读取Y轴加速度 */    
    ret = __bma253_read_accel_reg(handle, BMA2x2_Y_AXIS_LSB_ADDR, &p_accel[1]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }

    /* 读取Z轴加速度 */    
    ret = __bma253_read_accel_reg(handle, BMA2x2_Z_AXIS_LSB_ADDR, &p_accel[2]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    return  ret;  
}

/**
 * \brief 读取bma253温度
 * \return AM_OK:成功    OTHER：失败
 */
int am_bma253_read_temp (am_bma253_handle_t handle, int8_t* p_temp)
{
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);  
    
    ret = am_i2c_read(p_i2c_dev,BMA2x2_TEMP_ADDR, (uint8_t*)(p_temp), 1);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* 计算温度 */
    *p_temp = (int8_t)(24 + *p_temp * 1.0 / 2);
    
    return  ret;     
}

/**
 * \brief BMA253传感器软件复位
 */
int am_bma253_soft_reset (am_bma253_handle_t handle)
{
    int     ret;
    uint8_t soft_reset_cmd = SOFT_RESET;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* 验证参数的有效性 */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* 从handle中获取i2c设备指针 */
    p_i2c_dev = &(handle->i2c_dev);

    /* 写复位指令 */
    ret = am_i2c_write(p_i2c_dev,
                       BMA2x2_RST_ADDR,             
                       &soft_reset_cmd,
                       1);

    /* 需要15ms达到空闲状态 */
    am_mdelay(15);

    return ret;
}

/**
 * \brief BMA253传感器初始化
 */
am_bma253_handle_t am_bma253_init (am_bma253_dev_t *p_dev, 
                                   const am_bma253_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle)
{
    uint8_t  bma253_id = 0; 
    /* 用于寄存器配置 */
    uint8_t  pmu_lpw   = 0x00;    
    
    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* 初始配置好BMA253设备信息 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                 i2c_handle,
                 BMA253_DEVICE_ADDR,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* 读取BMA253 ID */
    am_i2c_read(&(p_dev->i2c_dev), BMA2x2_CHIP_ID_ADDR, &bma253_id, 1);
    AM_DBG_INFO("bma253_ID = 0x%x\r\n", bma253_id);
    
    /* 配置加速度范围 */
    am_i2c_write(&(p_dev->i2c_dev), 
                 BMA2x2_RANGE_SELECT_ADDR, 
                 &(p_devinfo->pmu_range), 
                 1);

    /* 配置滤波带宽 */
    am_i2c_write(&(p_dev->i2c_dev), 
                 BMA2x2_BW_SELECT_ADDR, 
                 &(p_devinfo->pmu_bw), 
                 1);
       
    /* 配置电源模式、睡眠周期 */
    pmu_lpw = (p_devinfo->power_mode | p_devinfo->sleep_period);
    am_i2c_write(&(p_dev->i2c_dev), BMA2x2_MODE_CTRL_ADDR, &pmu_lpw, 1);    
        
    return p_dev;
}

/**
 * \brief BMA253 设备解初始化
 */
void am_bma253_deinit (am_bma253_handle_t handle)
{

}

/* end of file */
