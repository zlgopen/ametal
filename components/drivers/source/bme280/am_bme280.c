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
 * \brief BME280压力传感器应用接口实现
 * \sa am_bme280.c
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */

#include "am_bme280.h" 

/**
 * \brief BME280从机地址
 */    
#define  BME280_DEVICE_ADDR      0x76 

/**
 * \brief BME280寄存器地址
 */  
#define  BME280_HUM_LSB          0XFE
#define  BME280_HUM_MSB          0XFD      
#define  BME280_TEMP_XLSB        0XFC        
#define  BME280_TEMP_LSB         0XFB
#define  BME280_TEMP_MSB         0XFA    
#define  BME280_PRESS_XLSB       0XF9      
#define  BME280_PRESS_LSB        0XF8
#define  BME280_PRESS_MSB        0XF7
#define  BME280_CONFIG_REG       0XF5    
#define  BME280_CTRL_MEAS_REG    0XF4 
#define  BME280_STATUS_REG       0XF3
#define  BME280_CTRL_HUM         0XF2
#define  BME280_RESET_REG        0XE0
#define  BME280_ID_REG           0XD0

/**
 * \brief BME280温度校准参数寄存器地址
 */ 
#define BME280_DIG_T1_LSB_ADDR	 0x88
#define BME280_DIG_T1_MSB_ADDR	 0x89
#define BME280_DIG_T2_LSB_ADDR	 0x8A
#define BME280_DIG_T2_MSB_ADDR   0x8B 
#define BME280_DIG_T3_LSB_ADDR	 0x8C 
#define BME280_DIG_T3_MSB_ADDR	 0x8D 

/**
 * \brief BME280压力校准参数寄存器地址
 */
#define BME280_DIG_P1_LSB_ADDR	 0x8E 
#define BME280_DIG_P1_MSB_ADDR	 0x8F 
#define BME280_DIG_P2_LSB_ADDR	 0x90 
#define BME280_DIG_P2_MSB_ADDR	 0x91 
#define BME280_DIG_P3_LSB_ADDR	 0x92 
#define BME280_DIG_P3_MSB_ADDR	 0x93 
#define BME280_DIG_P4_LSB_ADDR	 0x94 
#define BME280_DIG_P4_MSB_ADDR	 0x95 
#define BME280_DIG_P5_LSB_ADDR	 0x96 
#define BME280_DIG_P5_MSB_ADDR	 0x97 
#define BME280_DIG_P6_LSB_ADDR	 0x98 
#define BME280_DIG_P6_MSB_ADDR	 0x99 
#define BME280_DIG_P7_LSB_ADDR	 0x9A 
#define BME280_DIG_P7_MSB_ADDR	 0x9B 
#define BME280_DIG_P8_LSB_ADDR	 0x9C 
#define BME280_DIG_P8_MSB_ADDR	 0x9D
#define BME280_DIG_P9_LSB_ADDR	 0x9E 
#define BME280_DIG_P9_MSB_ADDR	 0x9F 

/**
 * \brief BME280湿度校准参数寄存器地址
 */
#define BME280_DIG_H1_ADDR       0xA1
#define BME280_DIG_H2_LSB_ADDR   0xE1
#define BME280_DIG_H2_MSB_ADDR   0xE2
#define BME280_DIG_H3_ADDR       0xE3
#define BME280_DIG_H4_LSB_ADDR   0xE5
#define BME280_DIG_H4_MSB_ADDR   0xE4
#define BME280_DIG_H5_LSB_ADDR   0xE5
#define BME280_DIG_H5_MSB_ADDR   0xE6
#define BME280_DIG_H6_ADDR       0xE7

/**
 * \brief BME280复位命令
 */ 
#define BME280_RESET        	 0xB6 
 
/**
 * \brief 读取校准参数寄存器函数
 * \param[in]   reg_addr：寄存器地址
 * \param[out]  calib_value：指向校准寄存器值的指针 
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */
static int __bme280_read_calib_param (am_bme280_handle_t handle, 
                                      uint8_t reg_addr,
                                      int16_t *calib_value)
{
    uint8_t temp_value[2] = {0};
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);

    ret = am_i2c_read(p_i2c_dev, reg_addr, temp_value, 2);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    *calib_value = (int16_t)(temp_value[1] << 8 | temp_value[0]);
    
    return ret;  
}

/**
 * \brief 读取温度校准参数
 */
static int __read_bme280_temp_calib_param (am_bme280_handle_t handle, 
                                           am_bme280_calib_param_t *calib_param)
{
    int ret = AM_OK;
                                                         
    /* 读取温度校准参数T1 */
    ret = __bme280_read_calib_param (handle,
                                    BME280_DIG_T1_LSB_ADDR,
                                    (int16_t*)&(calib_param->dig_t1));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* 读取温度校准参数T2 */
    ret = __bme280_read_calib_param (handle,
                                    BME280_DIG_T2_LSB_ADDR,
                                    &(calib_param->dig_t2)); 
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    /* 读取温度校准参数T3 */
    ret = __bme280_read_calib_param (handle,
                                    BME280_DIG_T3_LSB_ADDR,
                                    &(calib_param->dig_t3));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    return ret;
}

/**
 * \brief 读取压力校准参数
 */
static int __read_bme280_press_calib_param (am_bme280_handle_t handle, 
                                            am_bme280_calib_param_t *calib_param)
{
    int ret = AM_OK;
    
    /* 读取气压校准寄存器 */
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P1_LSB_ADDR,
                                    (int16_t*)&(calib_param->dig_p1));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P2_LSB_ADDR,
                                    &(calib_param->dig_p2));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P3_LSB_ADDR,
                                    &(calib_param->dig_p3));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }     
    
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P4_LSB_ADDR,
                                    &(calib_param->dig_p4));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P5_LSB_ADDR,
                                    &(calib_param->dig_p5));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }   
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P6_LSB_ADDR,
                                    &(calib_param->dig_p6));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }     
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P7_LSB_ADDR,
                                    &(calib_param->dig_p7));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P8_LSB_ADDR,
                                    &(calib_param->dig_p8));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P9_LSB_ADDR,
                                    &(calib_param->dig_p9));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    return ret;
}

/**
 * \brief 读取湿度校准参数
 */
static int __read_bme280_hum_calib_param (am_bme280_handle_t handle, 
                                          am_bme280_calib_param_t *calib_param)
{
    uint8_t temp_value[2] = {0};
    int ret = AM_OK;

    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);   
                               
    /* 读取湿度校准参数1 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H1_ADDR, temp_value, 1);

    /* 检查读取是否失败 */
    if (ret != AM_OK) {
       return ret;
    } 
    calib_param->dig_h1 = temp_value[0];

    /* 读取湿度校准参数2 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H2_LSB_ADDR, temp_value, 2);

    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }    
    calib_param->dig_h2 = (int16_t)(temp_value[1] << 8 | temp_value[0]);

    /* 读取湿度校准参数3 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H3_ADDR, temp_value, 1);

    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    calib_param->dig_h3 = temp_value[0];

    /* 读取湿度校准参数4 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H4_MSB_ADDR, temp_value, 2);

    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }   
    calib_param->dig_h4 = (int16_t)((temp_value[0] << 4) 
                        | (temp_value[1] & 0x0f));

    /* 读取湿度校准参数5 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H5_LSB_ADDR, temp_value, 2);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    calib_param->dig_h5 = (int16_t)((temp_value[1] << 4) 
                        | ((temp_value[0] & 0xf0) >> 4));

    /* 读取湿度校准参数6 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H6_ADDR, temp_value, 1);

    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }   
    calib_param->dig_h6 = temp_value[0];

    return ret;
}

/**
 * \brief 读取BME280温度寄存器
 */
static int __bme280_read_temp_reg (am_bme280_handle_t handle, 
                                   int32_t *temp)
{
    /* 用于存放读取的值 */
    uint8_t temp_value[3] = {0};
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取温度寄存器 */
    ret = am_i2c_read(p_i2c_dev, BME280_TEMP_MSB, temp_value, 3);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }    
    
    *temp = (int32_t)(((int32_t)(temp_value[0]) << 12) 
          | ((int32_t)(temp_value[1]) << 4) 
          | ((int32_t)(temp_value[2]) >> 4));
    
    return ret;
}

/**
 * \brief 读取BME280气压寄存器
 */
static int __bme280_read_press_reg (am_bme280_handle_t handle, 
                                    int32_t *press)
{
    /* 用于存放读取的值 */
    uint8_t press_value[3] = {0};
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取气压寄存器 */
    ret = am_i2c_read(p_i2c_dev, BME280_PRESS_MSB, press_value, 3);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }   
    
    *press = (int32_t)(((uint32_t)(press_value[0]) << 12) 
           | ((uint32_t)(press_value[1]) << 4) 
           | ((uint32_t)(press_value[2]) >> 4));
    
    return ret;
}

/**
 * \brief 读取BME280湿度寄存器
 */
static int __bme280_read_hum_reg (am_bme280_handle_t handle, 
                                  uint16_t *humidity)
{
    /* 用于存放读取的值 */
    uint8_t hum_value[2] = {0};
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取湿度寄存器 */
    ret = am_i2c_read(p_i2c_dev, BME280_HUM_MSB, hum_value, 2);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }    
    *humidity = (uint16_t)(hum_value[0] << 8 | hum_value[1]);
    
    return ret;
}


/**
 * \brief 初始化bme280
 */
am_bme280_handle_t am_bme280_init (am_bme280_dev_t *p_dev, 
                                   const am_bme280_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle)
{
    uint8_t        bme280_id = 0; 
    /* 用于寄存器配置 */
    const uint8_t  reset     = BME280_RESET;
    uint8_t        ctrl_meas = 0x00;
  
    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    
    
    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* 初始配置好BME280设备信息 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                  i2c_handle,
                  BME280_DEVICE_ADDR,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    
    /* 复位BME280 */
    am_i2c_write(&(p_dev->i2c_dev), BME280_RESET_REG, &reset, 1);
    
    /* 读取BME280 ID */
    am_i2c_read(&(p_dev->i2c_dev),BME280_ID_REG, &bme280_id, 1);
    AM_DBG_INFO("BME280_ID = 0x%x\r\n", bme280_id);

    /* 配置湿度采样数量 */
    am_i2c_write(&(p_dev->i2c_dev), 
                 BME280_CTRL_HUM, 
                 &(p_devinfo->hum_oversampe), 
                 1);    
    
    /* 配置BME280气压、温度、工作模式配置 */
    ctrl_meas = (p_devinfo->press_oversampe 
              | p_devinfo->temp_oversampe 
              | p_devinfo->power_mode);
    am_i2c_write(&(p_dev->i2c_dev), BME280_CTRL_MEAS_REG, &ctrl_meas, 1);
    
    /* 配置BME280待机时间 */
    am_i2c_write(&(p_dev->i2c_dev), 
                 BME280_CONFIG_REG, 
                 &(p_devinfo->standby_time), 
                 1); 
                 
    am_mdelay(100);

    /* 读取温度校准参数 */
    __read_bme280_temp_calib_param (p_dev, &(p_dev->calib_param));
    
    /* 读取气压校准参数 */
    __read_bme280_press_calib_param (p_dev, &(p_dev->calib_param));    

    /* 读取湿度校准参数 */
    __read_bme280_hum_calib_param (p_dev, &(p_dev->calib_param));

    return p_dev;
}

/**
 * \brief 读取BME280温度
 * \return 温度值 单位 ℃
 */
int am_bme280_read_temp (am_bme280_handle_t handle, int8_t *temp)
{
    int32_t adc_temp = 0;
    int32_t var1 = 0;
    int32_t var2 = 0;
    int ret = AM_OK;
    
    /* 校准参数指针 */
    am_bme280_calib_param_t *calib_param = &(handle->calib_param);
    
    /* 读取温度ADC */
    ret = __bme280_read_temp_reg(handle, &adc_temp);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }      
    
    /* 转换成实际温度值 */
    var1 = (((double)adc_temp) / 16384.0 
         - ((double) calib_param->dig_t1) / 1024.0) 
         * ((double) calib_param->dig_t2);
    var2 = ((((double)adc_temp) / 131072.0 
         - ((double) calib_param->dig_t1) / 8192.0) 
         * (((double)adc_temp)
         / 131072.0 - ((double) calib_param->dig_t1)
         / 8192.0)) * ((double) calib_param->dig_t3);
    
    /* 保存用于气压校准 */         
    calib_param->t_fine = var1 + var2;
             
    *temp = (var1 + var2) / 5120.0;
           
    return  ret;     
}

/**
 * \brief 读取BME280气压
 * \return 气压值 单位 Pa
 */
int am_bme280_read_press (am_bme280_handle_t handle, uint32_t *press)
{
    int32_t adc_press = 0;
    int32_t var1 = 0;
    int32_t var2 = 0;
    int32_t t_fine = 0;
    int ret = AM_OK;
    
    /* 校准参数指针 */
    am_bme280_calib_param_t *calib_param = &(handle->calib_param);
    
    t_fine = calib_param->t_fine; 
    
    /* 读取气压值ADC */             
    ret = __bme280_read_press_reg(handle, &adc_press);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }       

    /* 转换成实际气压值 */             
    var1 = ((double)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)calib_param->dig_p6) / 32768.0;
    var2 = var2 + var1 * ((double)calib_param->dig_p5) * 2.0;
    var2 = (var2 / 4.0) + (((double)calib_param->dig_p4) * 65536.0);
    var1 = (((double)calib_param->dig_p3) * var1 * var1 / 524288.0
         + ((double)calib_param->dig_p2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)calib_param->dig_p1);
    *press = 1048576.0 - (double)adc_press;
    *press = (*press - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)calib_param->dig_p9) * (*press) * (*press) / 2147483648.0;
    var2 = (*press) * ((double)calib_param->dig_p8) / 32768.0;
    *press = *press + (var1 + var2 + ((double)calib_param->dig_p7)) / 16.0;
    
    return  ret;             
}

/**
 * \brief 读取BME280湿度
 * \return 湿度值 单位 rH
 */
int am_bme280_read_hum (am_bme280_handle_t handle, uint8_t *humidity)
{
    uint16_t  hum_adc = 0;
    int32_t   var1    = 0;
    int32_t   var2    = 0;
    uint32_t  hum     = 0;
    int       ret     = AM_OK;
    
    /* 校准参数指针 */
    am_bme280_calib_param_t *calib_param = &(handle->calib_param);
    
    /* 读取湿度ADC */
    ret = __bme280_read_hum_reg(handle, &hum_adc);
  
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  
  
    /* 转换成湿度值 */
    var1 = (calib_param->t_fine - (int32_t)(76800));
    var1 = (((((hum_adc << 14) - (((int32_t)calib_param->dig_h4) << 20)
         - (((int32_t)calib_param->dig_h5) * var1)) + ((int32_t)16384)) >> 15)
         * (((((((var1 * ((int32_t)calib_param->dig_h6)) >> 10) 
         * (((var1 * ((int32_t)calib_param->dig_h3)) >> 11) 
         + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) 
         * ((int32_t)calib_param->dig_h2) + 8192) >> 14));
    
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) 
         * ((int32_t)calib_param->dig_h1)) >> 4)); 
    var2 = (uint32_t)(var1 >> 12);
    
    hum  = var2 / 1024;
    
    if (hum > 100)  hum = 100;
    *humidity = hum;
    
    return  ret;
} 

/* end of file */

