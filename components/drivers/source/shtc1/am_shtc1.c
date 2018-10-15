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
 * \brief SHTC1温湿度传感器传感器驱动
 *
 * \internal
 * \par modification history:
 * - 1.00 18-09-12  yrz, first implementation.
 * \endinternal
 */

#include "am_shtc1.h"

/**
 * \brief SHTC1从机地址
 */    
#define     SHTC1_DEVICE_ADDR    0x70 

/**
 * \brief SHTC1 I2C命令
 */
const uint8_t SHTC1_RESET[] = {0x80, 0x5D};   /** \brief 设备复位命令         */
const uint8_t SHTC1_ID[]    = {0xEF, 0xC8};   /** \brief 读取ID命令           */
const uint8_t SHTC1_CSE_T[] = {0x7C, 0xA2};   /** \brief 连续读取温度命令     */
const uint8_t SHTC1_CSE_H[] = {0x5C, 0x24};   /** \brief 连续读取湿度命令     */
const uint8_t SHTC1_CSD_T[] = {0x78, 0x66};   /** \brief 重新读取温度命令     */
const uint8_t SHTC1_CSD_H[] = {0x58, 0xE0};   /** \brief 重新读取温度命令     */

/**
 * \brief SHTC1设备初始化
 */
am_shtc1_handle_t am_shtc1_init (am_shtc1_dev_t *p_dev, 
                                 am_i2c_handle_t i2c_handle)
{
    uint8_t shtc1_id[2] = {0};
    
    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == i2c_handle) {
        return NULL;
    }    

    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* 初始配置SHTC1设备信息 */
    am_i2c_mkdev (&(p_dev->i2c_dev),
                  i2c_handle,
                  SHTC1_DEVICE_ADDR,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_NONE);
    
    /* 复位SHTC1 */
    am_i2c_write (&(p_dev->i2c_dev), 0x00, SHTC1_RESET, 2);

    /* 读取SHTC1 ID */
    am_i2c_write (&(p_dev->i2c_dev), 0x00, SHTC1_ID, 2);
    am_i2c_read  (&(p_dev->i2c_dev), 0x00, shtc1_id, 2);
    AM_DBG_INFO("SHTC1_ID  = 0x%x  \r\n", (shtc1_id[0] << 8 | shtc1_id[1]));
    
    return  p_dev;
}

/**
 * \brief shtc1传感器读取温度
 */
int am_shtc1_read_temp_hum (am_shtc1_handle_t handle, 
                            int8_t  *p_temp, 
                            uint8_t *p_hum)
{
    /* 用于存放读取的值 */
    uint8_t  temp_value[6] = {0};
    int      ret  = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 写测量命令 */
    am_i2c_write (p_i2c_dev, 0x00, SHTC1_CSE_H, 2);

    /* 等待数据转换完成 */
    am_mdelay(10);
    
    /* 读取测量结果 */
    ret = am_i2c_read (p_i2c_dev, 0x00, temp_value, 6);
 
    /* 判断发送是否成功 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* 湿度计算 */
    *p_hum  = 100 * ((temp_value[0] << 8) | temp_value[1]) / (1 << 16);
    
    /* 温度计算 */
    *p_temp = (-45) + 175 * ((temp_value[3] << 8) | temp_value[4]) / (1 << 16);

    return ret;
}


