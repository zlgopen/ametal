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
 * \brief LIS3MDL����Ŵ���������
 *
 * \internal
 * \par modification history:
 * - 1.00 18-09-12  yrz, first implementation.
 * \endinternal
 */

#include "am_lis3mdl.h"

/**
 * \brief BH1730FVC�ӻ���ַ
 */    
#define     LIS3MDL_DEVICE_ADDR    0x1C 

/**
 * \brief LIS3MDL�豸�Ĵ�����ַ
 */ 
#define     LIS3MDL_ID             0x0F
#define     LIS3MDL_CTRL_REG1      0x20
#define     LIS3MDL_CTRL_REG2      0x21
#define     LIS3MDL_CTRL_REG3      0x22
#define     LIS3MDL_CTRL_REG4      0x23
#define     LIS3MDL_CTRL_REG5      0x24
#define     LIS3MDL_STATUS_REG     0x27
#define     LIS3MDL_OUT_X_L        0x28
#define     LIS3MDL_OUT_X_H        0x29
#define     LIS3MDL_OUT_Y_L        0x2A
#define     LIS3MDL_OUT_Y_H        0x2B
#define     LIS3MDL_OUT_Z_L        0x2C
#define     LIS3MDL_OUT_Z_H        0x2D
#define     LIS3MDL_TEMP_OUT_L     0x2E
#define     LIS3MDL_TEMP_OUT_H     0x2F
#define     LIS3MDL_INT_CFG        0x30
#define     LIS3MDL_INT_SRC        0x31
#define     LIS3MDL_INT_THS_L      0x32
#define     LIS3MDL_INT_THS_H      0x33

/**
 * \brief LIS3MDL��λ����
 */ 
#define     LIS3MDL_RST            0x04 

/**
 * \brief LIS3MDL�豸��ʼ��
 */
am_lis3mdl_handle_t am_lis3mdl_init (am_lis3mdl_dev_t *p_dev, 
                                     const am_lis3mdl_devinfo_t *p_devinfo, 
                                     am_i2c_handle_t i2c_handle)
{
    uint8_t lis3mdl_id   = 0x00;
    uint8_t lis3mdl_rst  = LIS3MDL_RST;
    uint8_t control_reg1 = 0x00;
    
    /* ��֤������Ч�� */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    

    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle;     
    
    /* ��ʼ���ú�LIS3MDL�豸��Ϣ */
    am_i2c_mkdev (&(p_dev->i2c_dev),
                  i2c_handle,
                  LIS3MDL_DEVICE_ADDR,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* ��λ LIS3MDL */
    am_i2c_write (&(p_dev->i2c_dev), LIS3MDL_CTRL_REG2, &lis3mdl_rst, 1);    
    
    /* ��ȡ LIS3MDL ID */
    am_i2c_read (&(p_dev->i2c_dev), LIS3MDL_ID, &lis3mdl_id, 1);
    AM_DBG_INFO("LIS3MDL_ID =  0x%x \r\n", lis3mdl_id);
    
    /* ���ÿ��ƼĴ���1 */
    control_reg1 = (p_devinfo->temp_en 
                 | p_devinfo->operate_mode_xy 
                 | p_devinfo->output_data_rate 
                 | p_devinfo->fast_odr_en
                 | p_devinfo->self_test_en);
    am_i2c_write (&(p_dev->i2c_dev), LIS3MDL_CTRL_REG1, &control_reg1, 1);
    
    /* ���ÿ��ƼĴ���2 */
    am_i2c_write (&(p_dev->i2c_dev), 
                  LIS3MDL_CTRL_REG2, 
                  &(p_devinfo->full_scale), 
                  1);
    /* ���ÿ��ƼĴ���3 */
    am_i2c_write (&(p_dev->i2c_dev), 
                  LIS3MDL_CTRL_REG3, 
                  &(p_devinfo->system_mode), 
                  1); 
    /* ���ÿ��ƼĴ���4 */
    am_i2c_write (&(p_dev->i2c_dev), 
                  LIS3MDL_CTRL_REG4, 
                  &(p_devinfo->operate_mode_z), 
                  1);
 
    return  p_dev;
}

/**
 * \brief LIS3MDL��������ȡ�������ֵ
 */
int am_lis3mdl_read (am_lis3mdl_handle_t handle, uint16_t *p_data)
{
    /* ���ڴ�Ŷ�ȡ��ֵ */
    uint8_t  temp_value[6] = {0};
    int      ret  = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ��ȡԭʼ���� */
    ret = am_i2c_read (p_i2c_dev, LIS3MDL_OUT_X_L, temp_value, 6);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }   
    
    /* �����ݴ������ */
    p_data[0] = (temp_value[1] << 8) | temp_value[0];
	p_data[1] = (temp_value[3] << 8) | temp_value[2];
	p_data[2] = (temp_value[5] << 8) | temp_value[4];
    
    return ret;
}

/**
 * \brief LIS3MDL��������ȡ�¶�
 */
int am_lis3mdl_read_temp (am_lis3mdl_handle_t handle, int8_t *p_temp)
{
    /* ���ڴ�Ŷ�ȡ��ֵ */
    uint8_t  temp_value[2] = {0};
    int16_t  temp = 0;
    int      ret  = AM_OK;

    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* ����Ƿ�ʹ���¶Ȳ��� */
    if (handle->p_devinfo->temp_en != TEMP_ENABLE) {
        return ret;
    }
     
    /* ��ȡԭʼ���� */
    ret = am_i2c_read (p_i2c_dev, LIS3MDL_TEMP_OUT_L, temp_value, 2);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }   
    
    /* �����ݴ������ */
    temp = (int16_t)(temp_value[1] << 8 | temp_value[0]);
    
    *p_temp = temp / 8 + 25;
    
    return ret;
}

/* end of file */
