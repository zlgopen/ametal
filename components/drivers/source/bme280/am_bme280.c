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
 * \brief BME280ѹ��������Ӧ�ýӿ�ʵ��
 * \sa am_bme280.c
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */

#include "am_bme280.h" 

/**
 * \brief BME280�ӻ���ַ
 */    
#define  BME280_DEVICE_ADDR      0x76 

/**
 * \brief BME280�Ĵ�����ַ
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
 * \brief BME280�¶�У׼�����Ĵ�����ַ
 */ 
#define BME280_DIG_T1_LSB_ADDR	 0x88
#define BME280_DIG_T1_MSB_ADDR	 0x89
#define BME280_DIG_T2_LSB_ADDR	 0x8A
#define BME280_DIG_T2_MSB_ADDR   0x8B 
#define BME280_DIG_T3_LSB_ADDR	 0x8C 
#define BME280_DIG_T3_MSB_ADDR	 0x8D 

/**
 * \brief BME280ѹ��У׼�����Ĵ�����ַ
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
 * \brief BME280ʪ��У׼�����Ĵ�����ַ
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
 * \brief BME280��λ����
 */ 
#define BME280_RESET        	 0xB6 
 
/**
 * \brief ��ȡУ׼�����Ĵ�������
 * \param[in]   reg_addr���Ĵ�����ַ
 * \param[out]  calib_value��ָ��У׼�Ĵ���ֵ��ָ�� 
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
static int __bme280_read_calib_param (am_bme280_handle_t handle, 
                                      uint8_t reg_addr,
                                      int16_t *calib_value)
{
    uint8_t temp_value[2] = {0};
    int ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);

    ret = am_i2c_read(p_i2c_dev, reg_addr, temp_value, 2);
    
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    *calib_value = (int16_t)(temp_value[1] << 8 | temp_value[0]);
    
    return ret;  
}

/**
 * \brief ��ȡ�¶�У׼����
 */
static int __read_bme280_temp_calib_param (am_bme280_handle_t handle, 
                                           bme280_calib_param_t *calib_param)
{
    int ret = AM_OK;
                                                         
    /* ��ȡ�¶�У׼����T1 */
    ret = __bme280_read_calib_param (handle,
                                    BME280_DIG_T1_LSB_ADDR,
                                    (int16_t*)&(calib_param->dig_t1));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* ��ȡ�¶�У׼����T2 */
    ret = __bme280_read_calib_param (handle,
                                    BME280_DIG_T2_LSB_ADDR,
                                    &(calib_param->dig_t2)); 
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    /* ��ȡ�¶�У׼����T3 */
    ret = __bme280_read_calib_param (handle,
                                    BME280_DIG_T3_LSB_ADDR,
                                    &(calib_param->dig_t3));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    return ret;
}

/**
 * \brief ��ȡѹ��У׼����
 */
static int __read_bme280_press_calib_param (am_bme280_handle_t handle, 
                                            bme280_calib_param_t *calib_param)
{
    int ret = AM_OK;
    
    /* ��ȡ��ѹУ׼�Ĵ��� */
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P1_LSB_ADDR,
                                    (int16_t*)&(calib_param->dig_p1));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }  
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P2_LSB_ADDR,
                                    &(calib_param->dig_p2));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    } 
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P3_LSB_ADDR,
                                    &(calib_param->dig_p3));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }     
    
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P4_LSB_ADDR,
                                    &(calib_param->dig_p4));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    } 
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P5_LSB_ADDR,
                                    &(calib_param->dig_p5));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }   
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P6_LSB_ADDR,
                                    &(calib_param->dig_p6));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }     
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P7_LSB_ADDR,
                                    &(calib_param->dig_p7));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    } 
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P8_LSB_ADDR,
                                    &(calib_param->dig_p8));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }  
    
    ret = __bme280_read_calib_param (handle, 
                                    BME280_DIG_P9_LSB_ADDR,
                                    &(calib_param->dig_p9));
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    return ret;
}

/**
 * \brief ��ȡʪ��У׼����
 */
static int __read_bme280_hum_calib_param (am_bme280_handle_t handle, 
                                          bme280_calib_param_t *calib_param)
{
    uint8_t temp_value[2] = {0};
    int ret = AM_OK;

    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);   
                               
    /* ��ȡʪ��У׼����1 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H1_ADDR, temp_value, 1);

    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
       return ret;
    } 
    calib_param->dig_h1 = temp_value[0];

    /* ��ȡʪ��У׼����2 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H2_LSB_ADDR, temp_value, 2);

    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }    
    calib_param->dig_h2 = (int16_t)(temp_value[1] << 8 | temp_value[0]);

    /* ��ȡʪ��У׼����3 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H3_ADDR, temp_value, 1);

    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    calib_param->dig_h3 = temp_value[0];

    /* ��ȡʪ��У׼����4 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H4_MSB_ADDR, temp_value, 2);

    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }   
    calib_param->dig_h4 = (int16_t)((temp_value[0] << 4) 
                        | (temp_value[1] & 0x0f));

    /* ��ȡʪ��У׼����5 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H5_LSB_ADDR, temp_value, 2);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    } 
    calib_param->dig_h5 = (int16_t)((temp_value[1] << 4) 
                        | ((temp_value[0] & 0xf0) >> 4));

    /* ��ȡʪ��У׼����6 */
    ret = am_i2c_read(p_i2c_dev, BME280_DIG_H6_ADDR, temp_value, 1);

    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }   
    calib_param->dig_h6 = temp_value[0];

    return ret;
}

/**
 * \brief ��ȡBME280�¶ȼĴ���
 */
static int __bme280_read_temp_reg (am_bme280_handle_t handle, 
                                   int32_t *temp)
{
    /* ���ڴ�Ŷ�ȡ��ֵ */
    uint8_t temp_value[3] = {0};
    int ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡ�¶ȼĴ��� */
    ret = am_i2c_read(p_i2c_dev, BME280_TEMP_MSB, temp_value, 3);
    
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }    
    
    *temp = (int32_t)(((int32_t)(temp_value[0]) << 12) 
          | ((int32_t)(temp_value[1]) << 4) 
          | ((int32_t)(temp_value[2]) >> 4));
    
    return ret;
}

/**
 * \brief ��ȡBME280��ѹ�Ĵ���
 */
static int __bme280_read_press_reg (am_bme280_handle_t handle, 
                                    int32_t *press)
{
    /* ���ڴ�Ŷ�ȡ��ֵ */
    uint8_t press_value[3] = {0};
    int ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡ��ѹ�Ĵ��� */
    ret = am_i2c_read(p_i2c_dev, BME280_PRESS_MSB, press_value, 3);
    
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }   
    
    *press = (int32_t)(((uint32_t)(press_value[0]) << 12) 
           | ((uint32_t)(press_value[1]) << 4) 
           | ((uint32_t)(press_value[2]) >> 4));
    
    return ret;
}

/**
 * \brief ��ȡBME280ʪ�ȼĴ���
 */
static int __bme280_read_hum_reg (am_bme280_handle_t handle, 
                                  uint16_t *humidity)
{
    /* ���ڴ�Ŷ�ȡ��ֵ */
    uint8_t hum_value[2] = {0};
    int ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡʪ�ȼĴ��� */
    ret = am_i2c_read(p_i2c_dev, BME280_HUM_MSB, hum_value, 2);
    
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }    
    *humidity = (uint16_t)(hum_value[0] << 8 | hum_value[1]);
    
    return ret;
}


/**
 * \brief ��ʼ��bme280
 */
am_bme280_handle_t am_bme280_init (am_bme280_dev_t *p_dev, 
                                   const am_bme280_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle)
{
    uint8_t        bme280_id = 0; 
    /* ���ڼĴ������� */
    const uint8_t  reset     = BME280_RESET;
    uint8_t        ctrl_meas = 0x00;
  
    /* ��֤������Ч�� */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    
    
    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* ��ʼ���ú�BME280�豸��Ϣ */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                  i2c_handle,
                  BME280_DEVICE_ADDR,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    
    /* ��λBME280 */
    am_i2c_write(&(p_dev->i2c_dev), BME280_RESET_REG, &reset, 1);
    
    /* ��ȡBME280 ID */
    am_i2c_read(&(p_dev->i2c_dev),BME280_ID_REG, &bme280_id, 1);
    AM_DBG_INFO("BME280_ID = 0x%x\r\n", bme280_id);

    /* ����ʪ�Ȳ������� */
    am_i2c_write(&(p_dev->i2c_dev), 
                 BME280_CTRL_HUM, 
                 &(p_devinfo->hum_oversampe), 
                 1);    
    
    /* ����BME280��ѹ���¶ȡ�����ģʽ���� */
    ctrl_meas = (p_devinfo->press_oversampe 
              | p_devinfo->temp_oversampe 
              | p_devinfo->power_mode);
    am_i2c_write(&(p_dev->i2c_dev), BME280_CTRL_MEAS_REG, &ctrl_meas, 1);
    
    /* ����BME280����ʱ�� */
    am_i2c_write(&(p_dev->i2c_dev), 
                 BME280_CONFIG_REG, 
                 &(p_devinfo->standby_time), 
                 1); 
                 
    am_mdelay(100);

    /* ��ȡ�¶�У׼���� */
    __read_bme280_temp_calib_param (p_dev, &(p_dev->calib_param));
    
    /* ��ȡ��ѹУ׼���� */
    __read_bme280_press_calib_param (p_dev, &(p_dev->calib_param));    

    /* ��ȡʪ��У׼���� */
    __read_bme280_hum_calib_param (p_dev, &(p_dev->calib_param));

    return p_dev;
}

/**
 * \brief ��ȡBME280�¶�
 * \return �¶�ֵ ��λ ��
 */
int am_bme280_read_temp (am_bme280_handle_t handle, int8_t *temp)
{
    int32_t adc_temp = 0;
    int32_t var1 = 0;
    int32_t var2 = 0;
    int ret = AM_OK;
    
    /* У׼����ָ�� */
    bme280_calib_param_t *calib_param = &(handle->calib_param);
    
    /* ��ȡ�¶�ADC */
    ret = __bme280_read_temp_reg(handle, &adc_temp);
    
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }      
    
    /* ת����ʵ���¶�ֵ */
    var1 = (((double)adc_temp) / 16384.0 
         - ((double) calib_param->dig_t1) / 1024.0) 
         * ((double) calib_param->dig_t2);
    var2 = ((((double)adc_temp) / 131072.0 
         - ((double) calib_param->dig_t1) / 8192.0) 
         * (((double)adc_temp)
         / 131072.0 - ((double) calib_param->dig_t1)
         / 8192.0)) * ((double) calib_param->dig_t3);
    
    /* ����������ѹУ׼ */         
    calib_param->t_fine = var1 + var2;
             
    *temp = (var1 + var2) / 5120.0;
           
    return  ret;     
}

/**
 * \brief ��ȡBME280��ѹ
 * \return ��ѹֵ ��λ Pa
 */
int am_bme280_read_press (am_bme280_handle_t handle, uint32_t *press)
{
    int32_t adc_press = 0;
    int32_t var1 = 0;
    int32_t var2 = 0;
    int32_t t_fine = 0;
    int ret = AM_OK;
    
    /* У׼����ָ�� */
    bme280_calib_param_t *calib_param = &(handle->calib_param);
    
    t_fine = calib_param->t_fine; 
    
    /* ��ȡ��ѹֵADC */             
    ret = __bme280_read_press_reg(handle, &adc_press);
    
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }       

    /* ת����ʵ����ѹֵ */             
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
 * \brief ��ȡBME280ʪ��
 * \return ʪ��ֵ ��λ rH
 */
int am_bme280_read_hum (am_bme280_handle_t handle, uint8_t *humidity)
{
    uint16_t  hum_adc = 0;
    int32_t   var1    = 0;
    int32_t   var2    = 0;
    uint32_t  hum     = 0;
    int       ret     = AM_OK;
    
    /* У׼����ָ�� */
    bme280_calib_param_t *calib_param = &(handle->calib_param);
    
    /* ��ȡʪ��ADC */
    ret = __bme280_read_hum_reg(handle, &hum_adc);
  
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }  
  
    /* ת����ʪ��ֵ */
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

