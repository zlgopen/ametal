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
 * \brief BMP280压力传感器应用接口实现
 * \sa am_bmp280.c
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */

#include "am_bmp280.h" 

/**
 * \brief BMP280从机地址
 */    
#define  BMP280_DEVICE_ADDR      0x77 

/**
 * \brief BMP280寄存器地址
 */    
#define  BMP280_TEMP_XLSB        0XFC        
#define  BMP280_TEMP_LSB         0XFB
#define  BMP280_TEMP_MSB         0XFA    
#define  BMP280_PRESS_XLSB       0XF9      
#define  BMP280_PRESS_LSB        0XF8
#define  BMP280_PRESS_MSB        0XF7
#define  BMP280_CONFIG_REG       0XF5    
#define  BMP280_CTRL_MEAS_REG    0XF4 
#define  BMP280_STATUS_REG       0XF3
#define  BMP280_RESET_REG        0XE0
#define  BMP280_ID_REG           0XD0

/**
 * \brief BMP280校准参数寄存器地址
 */ 
#define BMP280_DIG_T1_LSB_ADDR	 0x88
#define BMP280_DIG_T1_MSB_ADDR	 0x89
#define BMP280_DIG_T2_LSB_ADDR	 0x8A
#define BMP280_DIG_T2_MSB_ADDR   0x8B 
#define BMP280_DIG_T3_LSB_ADDR	 0x8C 
#define BMP280_DIG_T3_MSB_ADDR	 0x8D 
#define BMP280_DIG_P1_LSB_ADDR	 0x8E 
#define BMP280_DIG_P1_MSB_ADDR	 0x8F 
#define BMP280_DIG_P2_LSB_ADDR	 0x90 
#define BMP280_DIG_P2_MSB_ADDR	 0x91 
#define BMP280_DIG_P3_LSB_ADDR	 0x92 
#define BMP280_DIG_P3_MSB_ADDR	 0x93 
#define BMP280_DIG_P4_LSB_ADDR	 0x94 
#define BMP280_DIG_P4_MSB_ADDR	 0x95 
#define BMP280_DIG_P5_LSB_ADDR	 0x96 
#define BMP280_DIG_P5_MSB_ADDR	 0x97 
#define BMP280_DIG_P6_LSB_ADDR	 0x98 
#define BMP280_DIG_P6_MSB_ADDR	 0x99 
#define BMP280_DIG_P7_LSB_ADDR	 0x9A 
#define BMP280_DIG_P7_MSB_ADDR	 0x9B 
#define BMP280_DIG_P8_LSB_ADDR	 0x9C 
#define BMP280_DIG_P8_MSB_ADDR	 0x9D
#define BMP280_DIG_P9_LSB_ADDR	 0x9E 
#define BMP280_DIG_P9_MSB_ADDR	 0x9F 

/**
 * \brief BMP280复位命令
 */ 
#define BMP280_RESET        	 0xB6 
 
/**
 * \brief 读取校准参数寄存器函数
 * \param[in]  reg_addr：寄存器地址
 * \param[out] calib_value：校准寄存器的值
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败    
 */
static int __bmp280_read_calib_param (am_bmp280_handle_t handle, 
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
static int __read_bmp280_temp_calib_param (am_bmp280_handle_t handle, 
                                           bmp280_calib_param_t *calib_param)
{
    int ret = AM_OK;
                                                         
    /* 读取温度校准参数T1 */
    ret = __bmp280_read_calib_param (handle,
                                    BMP280_DIG_T1_LSB_ADDR,
                                    (int16_t*)&(calib_param->dig_t1));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* 读取温度校准参数T2 */
    ret = __bmp280_read_calib_param (handle,
                                    BMP280_DIG_T2_LSB_ADDR,
                                    &(calib_param->dig_t2)); 
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    /* 读取温度校准参数T3 */
    ret = __bmp280_read_calib_param (handle,
                                    BMP280_DIG_T3_LSB_ADDR,
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
static int __read_bmp280_press_calib_param (am_bmp280_handle_t handle, 
                                            bmp280_calib_param_t *calib_param)
{
    int ret = AM_OK;
    
    /* 读取气压校准寄存器 */
    ret = __bmp280_read_calib_param (handle, 
                                    BMP280_DIG_P1_LSB_ADDR,
                                    (int16_t*)&(calib_param->dig_p1));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  
    
    ret = __bmp280_read_calib_param (handle, 
                                    BMP280_DIG_P2_LSB_ADDR,
                                    &(calib_param->dig_p2));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    ret = __bmp280_read_calib_param (handle, 
                                    BMP280_DIG_P3_LSB_ADDR,
                                    &(calib_param->dig_p3));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }     
    
    
    ret = __bmp280_read_calib_param (handle, 
                                    BMP280_DIG_P4_LSB_ADDR,
                                    &(calib_param->dig_p4));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    ret = __bmp280_read_calib_param (handle, 
                                    BMP280_DIG_P5_LSB_ADDR,
                                    &(calib_param->dig_p5));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }   
    
    ret = __bmp280_read_calib_param (handle, 
                                    BMP280_DIG_P6_LSB_ADDR,
                                    &(calib_param->dig_p6));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }     
    
    ret = __bmp280_read_calib_param (handle, 
                                    BMP280_DIG_P7_LSB_ADDR,
                                    &(calib_param->dig_p7));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    ret = __bmp280_read_calib_param (handle, 
                                    BMP280_DIG_P8_LSB_ADDR,
                                    &(calib_param->dig_p8));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  
    
    ret = __bmp280_read_calib_param (handle, 
                                    BMP280_DIG_P9_LSB_ADDR,
                                    &(calib_param->dig_p9));
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
   return ret;
}


/**
 * \brief 读取BMP280温度寄存器
 */
static int __bmp280_read_temp_reg (am_bmp280_handle_t handle, 
                                   int32_t *temp)
{
    /* 用于存放读取的值 */
    uint8_t temp_value[3] = {0};
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取温度寄存器 */
    ret = am_i2c_read(p_i2c_dev, BMP280_TEMP_MSB, temp_value, 3);
    
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
 * \brief 读取BMP280气压寄存器
 */
static int32_t __bmp280_read_press_reg (am_bmp280_handle_t handle, 
                                        int32_t *press)
{
    /* 用于存放读取的值 */
    uint8_t press_value[3] = {0};
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* 读取气压寄存器 */
    ret = am_i2c_read(p_i2c_dev, BMP280_PRESS_MSB, press_value, 3);
    
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
 * \brief 初始化bmp280
 */
am_bmp280_handle_t am_bmp280_init (am_bmp280_dev_t *p_dev, 
                                   const am_bmp280_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle)
{
    uint8_t        bmp280_id = 0; 
    /* 用于寄存器配置 */
    const uint8_t  reset     = BMP280_RESET;
    uint8_t        ctrl_meas = 0x00;
  
    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    
    
    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* 初始配置好BMP280设备信息 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                i2c_handle,
                BMP280_DEVICE_ADDR,
                AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    
    /* 复位BMP280 */
    am_i2c_write(&(p_dev->i2c_dev), BMP280_RESET_REG, &reset, 1);
    
    /* 读取BMP280 ID */
    am_i2c_read(&(p_dev->i2c_dev),BMP280_ID_REG, &bmp280_id, 1);
    AM_DBG_INFO("BMP280_ID = 0x%x\r\n", bmp280_id);

    /* 配置BMP280过采样为x16 Normal Mode */
    ctrl_meas = (p_devinfo->press_oversampe 
              | p_devinfo->temp_oversampe 
              | p_devinfo->power_mode);
    am_i2c_write(&(p_dev->i2c_dev), BMP280_CTRL_MEAS_REG, &ctrl_meas, 1);
    
    /* 配置BMP280待机时间 */
    am_i2c_write(&(p_dev->i2c_dev), 
                BMP280_CONFIG_REG, 
                &(p_devinfo->standby_time), 
                1); 
                 
    am_mdelay(100);

    /* 读取温度校准参数 */
    __read_bmp280_temp_calib_param (p_dev, &(p_dev->calib_param));
    
    /* 读取气压校准参数 */
    __read_bmp280_press_calib_param (p_dev, &(p_dev->calib_param));    

    return p_dev;
}

/**
 * \brief 读取BMP280温度
 */
int am_bmp280_read_temp (am_bmp280_handle_t handle, int8_t *temp)
{
    int32_t adc_temp = 0;
    int32_t var1 = 0;
    int32_t var2 = 0;
    int ret = AM_OK;
    
    /* 校准参数指针 */
    bmp280_calib_param_t *calib_param = &(handle->calib_param);
    
    /* 读取温度ADC */
    ret = __bmp280_read_temp_reg(handle, &adc_temp);
    
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
 * \brief 读取BMP280气压
 */
int am_bmp280_read_press (am_bmp280_handle_t handle, uint32_t *press)
{
    int32_t adc_press = 0;
    int32_t var1 = 0;
    int32_t var2 = 0;
    int32_t t_fine = 0;
    int ret = AM_OK;
    
    /* 校准参数指针 */
    bmp280_calib_param_t *calib_param = &(handle->calib_param);
    
    t_fine = calib_param->t_fine; 
    
    /* 读取气压值ADC */             
    ret = __bmp280_read_press_reg(handle,&adc_press);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }       

    /* 转换成实际气压值 */             
    var1 = ((double)t_fine / 2.0)-64000.0;
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

/* end of file */

