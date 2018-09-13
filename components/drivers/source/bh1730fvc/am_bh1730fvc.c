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
 * \brief BH1730FVC��ǿ�����봫��������
 *
 * \internal
 * \par modification history:
 * - 1.00 18-09-12  yrz, first implementation.
 * \endinternal
 */

#include "am_bh1730fvc.h"


/**
 * \brief BH1730FVC�ӻ���ַ
 */    
#define     BH1730FVC_DEVICE_ADDR  0x29  

/** 
 * \brief BH1730FVC�豸�Ĵ�����ַ 
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
 * \brief BH1730FVC�豸��ʼ��
 */
am_bh1730fvc_handle_t am_bh1730fvc_init (am_bh1730fvc_dev_t *p_dev, 
                                        const am_bh1730fvc_devinfo_t *p_devinfo, 
                                        am_i2c_handle_t i2c_handle)  
{
    uint8_t bh1730fvc_id = 0 ;
    uint8_t control = 0x00;
    
    /* ��֤������Ч�� */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    

    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* ��ʼBH1730FVC�豸��Ϣ */
    am_i2c_mkdev (&(p_dev->i2c_dev),
                  i2c_handle,
                  BH1730FVC_DEVICE_ADDR,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* ��ȡBH1730FVC ID */
    am_i2c_read (&(p_dev->i2c_dev), BH1730FVC_ID, &bh1730fvc_id, 1);
    AM_DBG_INFO("BH1730FVC_ID   =  0x%x  \r\n", bh1730fvc_id);
    
    /* ���ù���ģʽ */
    control = (p_devinfo->meas_mode 
            | p_devinfo->data_sel 
            | p_devinfo->adc_en 
            | p_devinfo->power_mode);
    am_i2c_write (&(p_dev->i2c_dev), BH1730FVC_CONTROL, &control, 1);
    
    /* ���û���ʱ�� */
    am_i2c_write (&(p_dev->i2c_dev), 
                  BH1730FVC_TIMING, 
                  &(p_devinfo->integrate_time), 
                  1);  

    /* ����ADC�ֱ��� */
    am_i2c_write (&(p_dev->i2c_dev), 
                  BH1730FVC_GAIN, 
                  &(p_devinfo->adc_res_setting), 
                  1); 
                  
    return  p_dev;
}

/**
 * \brief BH1730FVC��������ȡ��ǿADC
 */
int am_bh1730fvc_read_als_adc (am_bh1730fvc_handle_t handle, 
                               uint16_t *p_als_adc)
{
    uint8_t  temp_value[2] = {0};               /* ���ڴ�Ŷ�ȡ��ֵ */
    int      ret  = AM_OK;
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    ret = am_i2c_read (p_i2c_dev, BH1730FVC_DATA0LOW, temp_value, 2);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }  
    
    /* ����ȡ������ԭʼ����ת�� */
    *p_als_adc = ((temp_value[1] << 8) | temp_value[0]) / 10;
    
    return ret;
}

/* end of file */
