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
 * \brief  LTR_553ALS_01�⴫����Ӧ�ýӿ�ʵ��
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-07  yrz, first implementation.
 * \endinternal
 */

#include "am_ltr_553als_01.h"


/**
 * \brief LTR_553ALS�ӻ���ַ
 */    
#define     LTR_553ALS_DEVICE_ADDR          0x23 
    
/**
 * \brief LTR_553ALS ID�Ĵ�����ַ
 */    
#define     LTR_553ALS_ALS_CONTR            0x80
#define     LTR_553ALS_PS_CONTR             0x81
#define     LTR_553ALS_PS_LED               0x82
#define     LTR_553ALS_PS_N_PULSES          0x83
#define     LTR_553ALS_PS_MEAS_RATE         0x84
#define     LTR_553ALS_ALS_MEAS_RATE        0x85
#define     LTR_553ALS_PART_ID              0x86
#define     LTR_553ALS_MANUFAC_ID           0x87
#define     LTR_553ALS_ALS_DATA_CH1_0       0x88
#define     LTR_553ALS_ALS_DATA_CH1_1       0x89
#define     LTR_553ALS_ALS_DATA_CH0_0       0x8A
#define     LTR_553ALS_ALS_DATA_CH0_1       0x8B
#define     LTR_553ALS_ALS_PS_STATUS        0x8C
#define     LTR_553ALS_PS_DATA_0            0x8D
#define     LTR_553ALS_PS_DATA_1            0x8E
#define     LTR_553ALS_INTERRUPT            0x8F
#define     LTR_553ALS_PS_THRES_UP_0        0x90
#define     LTR_553ALS_PS_THRES_UP_1        0x91
#define     LTR_553ALS_PS_THRES_LOW_0       0x92
#define     LTR_553ALS_PS_THRES_LOW_1       0x93
#define     LTR_553ALS_PS_OFFSET_1          0x94
#define     LTR_553ALS_PS_OFFSET_0          0x95
#define     LTR_553ALS_ALS_THRES_UP_0       0x97
#define     LTR_553ALS_ALS_THRES_UP_1       0x98
#define     LTR_553ALS_ALS_THRES_LOW_0      0x99
#define     LTR_553ALS_ALS_THRES_LOW_1      0x9A
#define     LTR_553ALS_INTERRUPT_PERSIST    0x9E
    
/**
 * \brief LTR_553ALS����������
 */
typedef enum am_ltr_553als_command {

    LTR_553ALS_SOFT_RESET = 0x02                         /**< \brief �����λ */
    
} am_ltr_553als_command_t;

/**
 * \brief LTR_553ALS�����������λ
 */
int am_ltr_553als_soft_reset (am_ltr_553als_handle_t handle)
{
    int     ret;
    uint8_t soft_reset_cmd = LTR_553ALS_SOFT_RESET;

    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = NULL;

    /* ��֤��������Ч�� */
    if (NULL == handle) {
        return -AM_EINVAL;
    }

    /* ��handle�л�ȡi2c�豸ָ�� */
    p_i2c_dev = &(handle->i2c_dev);

    /* д��λָ�� */
    ret = am_i2c_write(p_i2c_dev,
                      LTR_553ALS_ALS_CONTR,             
                      &soft_reset_cmd,
                      1);

    /* ��Ҫ100ms��ʼ����� */
    am_mdelay(100);

    return ret;
}

/**
 * \brief ��ȡLTR_553ALS����ǿ��ADC
 * \return AM_OK:�ɹ�    OTHER��ʧ��
 */
int am_ltr_553als_read_als (am_ltr_553als_handle_t handle, uint16_t* p_als)
{
    uint8_t  als_temp[4];
    int ret = AM_OK;

    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev); 

    ret = am_i2c_read(p_i2c_dev, LTR_553ALS_ALS_DATA_CH1_0, als_temp, 4);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* �����ǿADC */   
    *p_als = (uint16_t)((uint32_t)(als_temp[1] << 8 | als_temp[0]) 
           + (uint32_t)(als_temp[3] << 8 | als_temp[2])) / 2;
 
    return  ret;     
}

/**
 * \brief ��ȡLTR_553ALS����ADC
 * \return AM_OK:�ɹ�    OTHER��ʧ��
 */
int am_ltr_553als_read_ps (am_ltr_553als_handle_t handle, uint16_t* p_ps)
{
    uint8_t  ps_temp[2];
    int ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);  

    ret = am_i2c_read(p_i2c_dev, LTR_553ALS_PS_DATA_0, ps_temp, 2);

    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* ���ݺϳ� */   
    *p_ps = (uint16_t)((ps_temp[1] & 0x07) << 8 | ps_temp[0]); 

    return  ret;     
}

/**
 * \brief LTR_553ALS��������ʼ��
 */
am_ltr_553als_handle_t am_ltr_553als_init (am_ltr_553als_dev_t *p_dev,
                                      const am_ltr_553als_devinfo_t *p_devinfo,
                                      am_i2c_handle_t i2c_handle)
{
    /* ���ڱ�����ʱ���� */
    uint8_t ltr_553als_id = 0;
    uint8_t als_contr     = 0;
    uint8_t ps_contr      = 0;
    uint8_t ps_led        = 0;
    uint8_t als_meas_rate = 0;
    
    /* ��֤������Ч�� */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }
    
    p_dev->i2c_dev.handle = i2c_handle;
    p_dev->p_devinfo = p_devinfo;
    

    /* ��ʼ���ú�LTR_553ALS�豸��Ϣ */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                 i2c_handle,
                 LTR_553ALS_DEVICE_ADDR,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    /* �����λ */
    am_ltr_553als_soft_reset(p_dev);
    
    /* ��ȡLTR_553ALS ID */
    am_i2c_read(&(p_dev->i2c_dev),LTR_553ALS_MANUFAC_ID, &ltr_553als_id, 1);
    AM_DBG_INFO("LTR_553ALS_ID = 0x%x\r\n", ltr_553als_id);
   

    /* ���ù��ղ�����Χ������ģʽ */
    als_contr = (p_devinfo->als_gain | 0x03);
    am_i2c_write(&(p_dev->i2c_dev), 
                 LTR_553ALS_ALS_CONTR, 
                 &als_contr,
                 1);

    /* ���ýӽ��������ֱ��ʡ�����ģʽ */
    ps_contr = (p_devinfo->ps_gain | 0x03);
    am_i2c_write(&(p_dev->i2c_dev), 
                 LTR_553ALS_PS_CONTR,
                 &ps_contr, 
                 1);

    /* ���ýӽ�������LED����Ƶ�ʡ�ռ�ձȡ����� */
    ps_led = (p_devinfo->led_pulse_freq 
           | p_devinfo->led_current_duty 
           | p_devinfo->led_current);
    
    am_i2c_write(&(p_dev->i2c_dev), LTR_553ALS_PS_LED, &ps_led, 1);

    /* ���ýӽ�������LED���������� */
    am_i2c_write(&(p_dev->i2c_dev), 
                 LTR_553ALS_PS_N_PULSES, 
                 &(p_devinfo->led_pulse_num), 
                 1);
    
    /* ���ýӽ��������������� */
    am_i2c_write(&(p_dev->i2c_dev), 
                 LTR_553ALS_PS_MEAS_RATE, 
                 &(p_devinfo->ps_meas_rate), 
                 1); 

    /* ���ù⴫������������ */
    als_meas_rate = (p_devinfo->als_integre_time | p_devinfo->als_meas_rate);
    am_i2c_write(&(p_dev->i2c_dev), 
                 LTR_553ALS_ALS_MEAS_RATE, 
                 &als_meas_rate, 
                 1);
  
    am_mdelay(100);

    return p_dev;
}

/**
 * \brief LTR_553ALS �豸���ʼ��
 */
void am_ltr_553als_deinit (am_ltr_553als_handle_t handle)
{
    const uint8_t als_contr = 0x00;
    const uint8_t ps_contr  = 0x00;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ���ù��մ���������ģʽ */
    am_i2c_write(p_i2c_dev, LTR_553ALS_ALS_CONTR, &als_contr, 1);

    /* ���ýӽ�����������ģʽ */
    am_i2c_write(p_i2c_dev, LTR_553ALS_PS_CONTR, &ps_contr, 1);
}

/* end of file */
