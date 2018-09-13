/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief HTS221驱动文件
 * \sa hts221.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */
 
#include "am_hts221.h" 


/**
 * \brief HTS221从机地址
 */    
#define HTS221_DEVICE_ADDR         0x5F  

/**
 * \brief HTS221寄存器
 */   
#define HTS221_WHO_AM_I            0x0F
#define HTS221_AV_CONF             0x10   
#define HTS221_CTRL_REG1           0x20
#define HTS221_CTRL_REG2           0x21
#define HTS221_CTRL_REG3           0x22
#define HTS221_STATUS_REG          0x27
#define HTS221_HUMIDITY_OUT_L      0x28
#define HTS221_HUMIDITY_OUT_H      0x29
#define HTS221_TEMP_OUT_L          0x2A
#define HTS221_TEMP_OUT_H          0x2B
 
/**
 * \brief HTS221校准寄存器
 */  
#define HTS221_H0_RH_X2            0x30
#define HTS221_H1_RH_X2            0x31
#define HTS221_T0_DEGC_X8          0x32
#define HTS221_T1_DEGC_X8          0x33
#define HTS221_T1_T0_MSB           0x35
#define HTS221_H0_T0_OUT_L         0x36
#define HTS221_H0_T0_OUT_H         0x37
#define HTS221_H1_T0_OUT_L         0x3A
#define HTS221_H1_T0_OUT_H         0x3B
#define HTS221_T0_OUT_L            0x3C
#define HTS221_T0_OUT_H            0x3D
#define HTS221_T1_OUT_L            0x3E
#define HTS221_T1_OUT_H            0x3F
 
/**
 * \brief HTS221温湿度转换完成
 */
#define HTS221_T_DA    (1 << 0)
#define HTS221_H_DA    (1 << 1)

/**
 * \brief 读取温度、湿度校准参数x轴数据
 * \param[in]   handle : HTS221服务操作句柄
 * \param[in]  reg_addr: 寄存器地址
 * \param[in]   calib_x: 指向读取的数据指针    
 * \return       AM_OK : 读取成功
 * \return       OTHER : 读取失败
 */ 
static int __hts221_read_calib_param_x (am_hts221_handle_t handle,
                                        uint8_t reg_addr,
                                        int16_t *calib_x)
{
    uint8_t calib_param_value[2] = {0};
    int     ret   = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取X轴ADC数据 */
    ret = am_i2c_read(p_i2c_dev, reg_addr, &calib_param_value[0], 1);
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    ret = am_i2c_read(p_i2c_dev, (reg_addr + 1), &calib_param_value[1], 1);
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* 高低字节数据合并 */
    *calib_x = (int16_t)(calib_param_value[1] << 8 | calib_param_value[0]);
    
    return ret;  
}

/**
 * \brief 读取温度校准参数y轴数据
 * \param[in]   handle : HTS221服务操作句柄
 * \param[in]  reg_addr: 寄存器地址
 * \param[in]   calib_y: 指向读取的数据指针    
 * \return       AM_OK : 读取成功
 * \return       OTHER : 读取失败
 */
static int __hts221_read_temp_calib_param_y (am_hts221_handle_t handle,
                                             uint8_t reg_addr,
                                             uint16_t *calib_y)
{
    uint8_t calib_param_value[2] = {0};
    int     ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取校准温度低8位数据 */
    ret = am_i2c_read(p_i2c_dev, HTS221_T1_T0_MSB, &calib_param_value[0], 1);
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    ret = am_i2c_read(p_i2c_dev, reg_addr, &calib_param_value[1], 1);
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }    
    
    /* 取出高2位数据 */
    if (reg_addr == HTS221_T0_DEGC_X8) {    
        *calib_y = (uint16_t)(((calib_param_value[0] & 0x03) << 8) 
                 | calib_param_value[1]);
    } 
    else if (reg_addr == HTS221_T1_DEGC_X8) {
        *calib_y = (uint16_t)((((calib_param_value[0] >> 2) & 0x03) << 8) 
                 | calib_param_value[1]); 
    }
    *calib_y = (*calib_y >> 3);
    
   return ret;   
}

/**
 * \brief 读取湿度校准参数y轴数据
 * \param[in]   handle : HTS221服务操作句柄
 * \param[in]  reg_addr: 寄存器地址
 * \param[in]   calib_y: 指向读取的数据指针    
 * \return       AM_OK : 读取成功
 * \return       OTHER : 读取失败
 */
static int __hts221_read_hum_calib_param_y (am_hts221_handle_t handle,
                                            uint8_t reg_addr,
                                            uint8_t *calib_y)
{
    uint8_t calib_param_value = 0;
    int     ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取湿度校准数据 */
    ret = am_i2c_read(p_i2c_dev, reg_addr, &calib_param_value, 1);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  
    
    *calib_y = (calib_param_value>> 1);
    
    return ret;  
}
 
/**
 * \brief 读取校准参数
 */
static void __hts221_read_calib_param (am_hts221_handle_t handle)
{
    am_lin_t *calib_param_hum  = &(handle->calib_param_hum);
    am_lin_t *calib_param_temp = &(handle->calib_param_temp);
    
    /* 读取湿度校准参数 */
    __hts221_read_calib_param_x (handle,
                                HTS221_H0_T0_OUT_L,
                                &(calib_param_hum->x0)); 

    __hts221_read_hum_calib_param_y (handle,
                                    HTS221_H0_RH_X2,
                                    &(calib_param_hum->y0));

    __hts221_read_calib_param_x (handle,
                                HTS221_H1_T0_OUT_L,
                                &(calib_param_hum->x1));  

    __hts221_read_hum_calib_param_y (handle,
                                    HTS221_H1_RH_X2,
                                    &(calib_param_hum->y1));
    
    
    /* 读取温度校准参数 */         
    __hts221_read_calib_param_x (handle,
                                HTS221_T0_OUT_L,
                                &(calib_param_temp->x0));

    __hts221_read_temp_calib_param_y (handle,
                                     HTS221_T0_DEGC_X8,
                                     (uint16_t*)&(calib_param_temp->y0));

    __hts221_read_calib_param_x (handle,
                                HTS221_T1_OUT_L,
                                &(calib_param_temp->x1));        

    __hts221_read_temp_calib_param_y (handle,
                                     HTS221_T1_DEGC_X8,
                                     (uint16_t*)&(calib_param_temp->y1));
 
    /* 输出校准数据 */    
    AM_DBG_INFO("T0 = %d ℃  T0_out = %d \r\n",
                calib_param_temp->y0,calib_param_temp->x0); 
    AM_DBG_INFO("T1 = %d ℃  T1_out = %d \r\n",
                calib_param_temp->y1,calib_param_temp->x1);  
    
    AM_DBG_INFO("H0 = %d rH  H0_out = %d \r\n",
                calib_param_hum->y0,calib_param_hum->x0); 
    AM_DBG_INFO("H1 = %d rH  H1_out = %d \r\n",
                calib_param_hum->y1,calib_param_hum->x1);
}
      
/**
 * \brief 读取温度、湿度ADC
 * \param[in]   handle : HTS221服务操作句柄
 * \param[in]  reg_addr: 寄存器地址
 * \param[out]      adc: 指向读取的数据指针    
 * \return       AM_OK : 读取成功
 * \return       OTHER : 读取失败
 */
static int16_t __hts221_read_adc (am_hts221_handle_t handle,
                                  uint8_t reg_addr,
                                  int16_t *adc)
{
    uint8_t adc_value[2] = {0};
    int     ret   = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取温湿度高低字节ADC */
    ret = am_i2c_read(p_i2c_dev, reg_addr, &adc_value[0], 1);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }     
    
    ret = am_i2c_read(p_i2c_dev, (reg_addr + 1), &adc_value[1], 1);
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    *adc = (int16_t)(adc_value[1] << 8 | adc_value[0]);
    
    return ret;
}

/**
 * \brief 线性校准
 * \param[in]   lin：指向校准参数的指针
 * \param[in]     x：温湿度ADC   
 * \return     线性处理后的值
 */
static double __linear_interpolation(am_lin_t *lin, int16_t x) 
{
      return  ((double)((double)(lin->y0) + (((double)(lin->y1 - lin->y0)) 
            * ((double)(x - lin->x0)) / ((double)(lin->x1 - lin->x0)))));
}


/**
 * \brief 初始化HTS221
 */
am_hts221_handle_t am_hts221_init (am_hts221_dev_t *p_dev, 
                                   const am_hts221_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle)  
{   
    uint8_t  hts221_id = 0;
    uint8_t  av_conf   = 0x00;   
    uint8_t  ctrl_reg1 = 0x00; 

    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    

    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* 初始配置HTS221设备信息 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                 i2c_handle,
                 HTS221_DEVICE_ADDR,    
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    am_mdelay(5);
    
    /* 读取HTS221 ID */ 
    am_i2c_read(&(p_dev->i2c_dev), HTS221_WHO_AM_I, &hts221_id, 1); 
    AM_DBG_INFO("HTS221_ID = 0x%x \r\n",hts221_id);

    /* 配置温度样本容量、湿度样本容量 */
    av_conf = (p_devinfo->temp_sample | p_devinfo->hum_sample);
    am_i2c_write(&(p_dev->i2c_dev), HTS221_AV_CONF, &av_conf, 1);
    
    /* 电源模式、数据更新方式、输出速率 */
    ctrl_reg1 = ( p_devinfo->power_mode
              | p_devinfo->data_update_config 
              | p_devinfo->data_update_rate);
    am_i2c_write(&(p_dev->i2c_dev), HTS221_CTRL_REG1, &ctrl_reg1, 1);
    
    am_mdelay(20);
    
    /* 读取校准参数 */
    __hts221_read_calib_param(p_dev); 
    
    return  p_dev;
} 

/**
 * \brief 读取温度
 */
int am_hts221_read_temp (am_hts221_handle_t handle, int16_t* temp)
{
    int16_t temp_adc = 0;
    uint8_t status   = 0;
    int     ret      = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取数据转换状态 */
    am_i2c_read(p_i2c_dev, HTS221_STATUS_REG, &status, 1);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* 温度转换完成 */                      
    if (status & HTS221_T_DA) {
        __hts221_read_adc(handle, HTS221_TEMP_OUT_L, &temp_adc);
        *temp = __linear_interpolation(&(handle->calib_param_temp), temp_adc);
    }  
    
    return ret; 
}

/**
 * \brief 读取湿度
 */
int am_hts221_read_hum (am_hts221_handle_t handle, uint8_t * hum)
{
    int16_t hum_adc = 0;
    uint8_t status  = 0;
    int     ret      = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取数据转换状态 */
    am_i2c_read(p_i2c_dev, HTS221_STATUS_REG, &status, 1);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* 湿度转换完成 */
    if (status & HTS221_H_DA) {    
        __hts221_read_adc(handle, HTS221_HUMIDITY_OUT_L, &hum_adc);
        *hum = __linear_interpolation(&(handle->calib_param_hum), hum_adc);
        
        /* 转换湿度限幅 */   
        if (*hum > 100) {
            *hum = 100; 
        }
    }
    return ret; 
}

/* end of file */

