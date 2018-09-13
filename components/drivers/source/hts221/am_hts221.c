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
 * \brief HTS221�����ļ�
 * \sa hts221.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */
 
#include "am_hts221.h" 


/**
 * \brief HTS221�ӻ���ַ
 */    
#define HTS221_DEVICE_ADDR         0x5F  

/**
 * \brief HTS221�Ĵ���
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
 * \brief HTS221У׼�Ĵ���
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
 * \brief HTS221��ʪ��ת�����
 */
#define HTS221_T_DA    (1 << 0)
#define HTS221_H_DA    (1 << 1)

/**
 * \brief ��ȡ�¶ȡ�ʪ��У׼����x������
 * \param[in]   handle : HTS221����������
 * \param[in]  reg_addr: �Ĵ�����ַ
 * \param[in]   calib_x: ָ���ȡ������ָ��    
 * \return       AM_OK : ��ȡ�ɹ�
 * \return       OTHER : ��ȡʧ��
 */ 
static int __hts221_read_calib_param_x (am_hts221_handle_t handle,
                                        uint8_t reg_addr,
                                        int16_t *calib_x)
{
    uint8_t calib_param_value[2] = {0};
    int     ret   = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡX��ADC���� */
    ret = am_i2c_read(p_i2c_dev, reg_addr, &calib_param_value[0], 1);
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    ret = am_i2c_read(p_i2c_dev, (reg_addr + 1), &calib_param_value[1], 1);
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* �ߵ��ֽ����ݺϲ� */
    *calib_x = (int16_t)(calib_param_value[1] << 8 | calib_param_value[0]);
    
    return ret;  
}

/**
 * \brief ��ȡ�¶�У׼����y������
 * \param[in]   handle : HTS221����������
 * \param[in]  reg_addr: �Ĵ�����ַ
 * \param[in]   calib_y: ָ���ȡ������ָ��    
 * \return       AM_OK : ��ȡ�ɹ�
 * \return       OTHER : ��ȡʧ��
 */
static int __hts221_read_temp_calib_param_y (am_hts221_handle_t handle,
                                             uint8_t reg_addr,
                                             uint16_t *calib_y)
{
    uint8_t calib_param_value[2] = {0};
    int     ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡУ׼�¶ȵ�8λ���� */
    ret = am_i2c_read(p_i2c_dev, HTS221_T1_T0_MSB, &calib_param_value[0], 1);
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    ret = am_i2c_read(p_i2c_dev, reg_addr, &calib_param_value[1], 1);
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }    
    
    /* ȡ����2λ���� */
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
 * \brief ��ȡʪ��У׼����y������
 * \param[in]   handle : HTS221����������
 * \param[in]  reg_addr: �Ĵ�����ַ
 * \param[in]   calib_y: ָ���ȡ������ָ��    
 * \return       AM_OK : ��ȡ�ɹ�
 * \return       OTHER : ��ȡʧ��
 */
static int __hts221_read_hum_calib_param_y (am_hts221_handle_t handle,
                                            uint8_t reg_addr,
                                            uint8_t *calib_y)
{
    uint8_t calib_param_value = 0;
    int     ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡʪ��У׼���� */
    ret = am_i2c_read(p_i2c_dev, reg_addr, &calib_param_value, 1);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }  
    
    *calib_y = (calib_param_value>> 1);
    
    return ret;  
}
 
/**
 * \brief ��ȡУ׼����
 */
static void __hts221_read_calib_param (am_hts221_handle_t handle)
{
    am_lin_t *calib_param_hum  = &(handle->calib_param_hum);
    am_lin_t *calib_param_temp = &(handle->calib_param_temp);
    
    /* ��ȡʪ��У׼���� */
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
    
    
    /* ��ȡ�¶�У׼���� */         
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
 
    /* ���У׼���� */    
    AM_DBG_INFO("T0 = %d ��  T0_out = %d \r\n",
                calib_param_temp->y0,calib_param_temp->x0); 
    AM_DBG_INFO("T1 = %d ��  T1_out = %d \r\n",
                calib_param_temp->y1,calib_param_temp->x1);  
    
    AM_DBG_INFO("H0 = %d rH  H0_out = %d \r\n",
                calib_param_hum->y0,calib_param_hum->x0); 
    AM_DBG_INFO("H1 = %d rH  H1_out = %d \r\n",
                calib_param_hum->y1,calib_param_hum->x1);
}
      
/**
 * \brief ��ȡ�¶ȡ�ʪ��ADC
 * \param[in]   handle : HTS221����������
 * \param[in]  reg_addr: �Ĵ�����ַ
 * \param[out]      adc: ָ���ȡ������ָ��    
 * \return       AM_OK : ��ȡ�ɹ�
 * \return       OTHER : ��ȡʧ��
 */
static int16_t __hts221_read_adc (am_hts221_handle_t handle,
                                  uint8_t reg_addr,
                                  int16_t *adc)
{
    uint8_t adc_value[2] = {0};
    int     ret   = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡ��ʪ�ȸߵ��ֽ�ADC */
    ret = am_i2c_read(p_i2c_dev, reg_addr, &adc_value[0], 1);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }     
    
    ret = am_i2c_read(p_i2c_dev, (reg_addr + 1), &adc_value[1], 1);
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    *adc = (int16_t)(adc_value[1] << 8 | adc_value[0]);
    
    return ret;
}

/**
 * \brief ����У׼
 * \param[in]   lin��ָ��У׼������ָ��
 * \param[in]     x����ʪ��ADC   
 * \return     ���Դ�����ֵ
 */
static double __linear_interpolation(am_lin_t *lin, int16_t x) 
{
      return  ((double)((double)(lin->y0) + (((double)(lin->y1 - lin->y0)) 
            * ((double)(x - lin->x0)) / ((double)(lin->x1 - lin->x0)))));
}


/**
 * \brief ��ʼ��HTS221
 */
am_hts221_handle_t am_hts221_init (am_hts221_dev_t *p_dev, 
                                   const am_hts221_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle)  
{   
    uint8_t  hts221_id = 0;
    uint8_t  av_conf   = 0x00;   
    uint8_t  ctrl_reg1 = 0x00; 

    /* ��֤������Ч�� */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    

    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* ��ʼ����HTS221�豸��Ϣ */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                 i2c_handle,
                 HTS221_DEVICE_ADDR,    
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    am_mdelay(5);
    
    /* ��ȡHTS221 ID */ 
    am_i2c_read(&(p_dev->i2c_dev), HTS221_WHO_AM_I, &hts221_id, 1); 
    AM_DBG_INFO("HTS221_ID = 0x%x \r\n",hts221_id);

    /* �����¶�����������ʪ���������� */
    av_conf = (p_devinfo->temp_sample | p_devinfo->hum_sample);
    am_i2c_write(&(p_dev->i2c_dev), HTS221_AV_CONF, &av_conf, 1);
    
    /* ��Դģʽ�����ݸ��·�ʽ��������� */
    ctrl_reg1 = ( p_devinfo->power_mode
              | p_devinfo->data_update_config 
              | p_devinfo->data_update_rate);
    am_i2c_write(&(p_dev->i2c_dev), HTS221_CTRL_REG1, &ctrl_reg1, 1);
    
    am_mdelay(20);
    
    /* ��ȡУ׼���� */
    __hts221_read_calib_param(p_dev); 
    
    return  p_dev;
} 

/**
 * \brief ��ȡ�¶�
 */
int am_hts221_read_temp (am_hts221_handle_t handle, int16_t* temp)
{
    int16_t temp_adc = 0;
    uint8_t status   = 0;
    int     ret      = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡ����ת��״̬ */
    am_i2c_read(p_i2c_dev, HTS221_STATUS_REG, &status, 1);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* �¶�ת����� */                      
    if (status & HTS221_T_DA) {
        __hts221_read_adc(handle, HTS221_TEMP_OUT_L, &temp_adc);
        *temp = __linear_interpolation(&(handle->calib_param_temp), temp_adc);
    }  
    
    return ret; 
}

/**
 * \brief ��ȡʪ��
 */
int am_hts221_read_hum (am_hts221_handle_t handle, uint8_t * hum)
{
    int16_t hum_adc = 0;
    uint8_t status  = 0;
    int     ret      = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡ����ת��״̬ */
    am_i2c_read(p_i2c_dev, HTS221_STATUS_REG, &status, 1);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* ʪ��ת����� */
    if (status & HTS221_H_DA) {    
        __hts221_read_adc(handle, HTS221_HUMIDITY_OUT_L, &hum_adc);
        *hum = __linear_interpolation(&(handle->calib_param_hum), hum_adc);
        
        /* ת��ʪ���޷� */   
        if (*hum > 100) {
            *hum = 100; 
        }
    }
    return ret; 
}

/* end of file */

