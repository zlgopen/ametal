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
 * \brief BH1730FVC光强及距离传感器驱动
 *
 * \internal
 * \par modification history:
 * - 1.00 18-09-12  yrz, first implementation.
 * \endinternal
 */

#include "am_bh1730fvc.h"


/**
 * \brief BH1730FVC从机地址
 */    
#define     BH1730FVC_DEVICE_ADDR  0x29  

/** 
 * \brief BH1730FVC设备寄存器地址 
 */
#define     BH1730FVC_CONTROL      (0x00 | 0x80)
#define     BH1730FVC_TIMING       (0x01 | 0x80)
#define     BH1730FVC_INTERRUPT    (0x02 | 0x80)
#define     BH1730FVC_THLLOW       (0x03 | 0x80)
#define     BH1730FVC_THLHIGH      (0x04 | 0x80)
#define     BH1730FVC_THHLOW       (0x05 | 0x80)
#define     BH1730FVC_THHHIGH      (0x06 | 0x80)
#define     BH1730FVC_GAIN         (0x07 | 0x80)
#define     BH1730FVC_ID           (0x12 | 0x80)
#define     BH1730FVC_DATA0LOW     (0x14 | 0x80)
#define     BH1730FVC_DATA0HIGH    (0x15 | 0x80)
#define     BH1730FVC_DATA1LOW     (0x16 | 0x80)
#define     BH1730FVC_DATA1HIGH    (0x17 | 0x80)

/**
 * \brief BH1730FVC设备初始化
 */
am_bh1730fvc_handle_t am_bh1730fvc_init (am_bh1730fvc_dev_t *p_dev, 
                                        const am_bh1730fvc_devinfo_t *p_devinfo, 
                                        am_i2c_handle_t i2c_handle)  
{
    uint8_t bh1730fvc_id = 0 ;
    uint8_t control = 0x00;
    
    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    

    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* 初始BH1730FVC设备信息 */
    am_i2c_mkdev (&(p_dev->i2c_dev),
                  i2c_handle,
                  BH1730FVC_DEVICE_ADDR,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* 读取BH1730FVC ID */
    am_i2c_read (&(p_dev->i2c_dev), BH1730FVC_ID, &bh1730fvc_id, 1);
    AM_DBG_INFO("BH1730FVC_ID   =  0x%x  \r\n", bh1730fvc_id);
    
    /* 配置工作模式 */
    control = (p_devinfo->meas_mode 
            | p_devinfo->data_sel 
            | p_devinfo->adc_en 
            | p_devinfo->power_mode);
    am_i2c_write (&(p_dev->i2c_dev), BH1730FVC_CONTROL, &control, 1);
    
    /* 配置积分时间 */
    am_i2c_write (&(p_dev->i2c_dev), 
                  BH1730FVC_TIMING, 
                  &(p_devinfo->integrate_time), 
                  1);  

    /* 配置ADC分辨率 */
    am_i2c_write (&(p_dev->i2c_dev), 
                  BH1730FVC_GAIN, 
                  &(p_devinfo->adc_res_setting), 
                  1); 
                  
    return  p_dev;
}

/**
 * \brief BH1730FVC传感器读取光强ADC
 */
int am_bh1730fvc_read_als_adc (am_bh1730fvc_handle_t handle, 
                               uint16_t *p_als_adc)
{
    uint8_t  temp_value[2] = {0};               /* 用于存放读取的值 */
    int      ret  = AM_OK;
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    ret = am_i2c_read (p_i2c_dev, BH1730FVC_DATA0LOW, temp_value, 2);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  
    
    /* 将读取出来的原始数据转换 */
    *p_als_adc = ((temp_value[1] << 8) | temp_value[0]) / 10;
    
    return ret;
}

/* end of file */
