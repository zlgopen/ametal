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
 * \brief SHTC1��ʪ�ȴ���������������
 *
 * \internal
 * \par modification history:
 * - 1.00 18-09-12  yrz, first implementation.
 * \endinternal
 */

#include "am_shtc1.h"

/**
 * \brief SHTC1�ӻ���ַ
 */    
#define     SHTC1_DEVICE_ADDR    0x70 

/**
 * \brief SHTC1 I2C����
 */
const uint8_t SHTC1_RESET[] = {0x80, 0x5D};   /** \brief �豸��λ����         */
const uint8_t SHTC1_ID[]    = {0xEF, 0xC8};   /** \brief ��ȡID����           */
const uint8_t SHTC1_CSE_T[] = {0x7C, 0xA2};   /** \brief ������ȡ�¶�����     */
const uint8_t SHTC1_CSE_H[] = {0x5C, 0x24};   /** \brief ������ȡʪ������     */
const uint8_t SHTC1_CSD_T[] = {0x78, 0x66};   /** \brief ���¶�ȡ�¶�����     */
const uint8_t SHTC1_CSD_H[] = {0x58, 0xE0};   /** \brief ���¶�ȡ�¶�����     */

/**
 * \brief SHTC1�豸��ʼ��
 */
am_shtc1_handle_t am_shtc1_init (am_shtc1_dev_t *p_dev, 
                                 am_i2c_handle_t i2c_handle)
{
    uint8_t shtc1_id[2] = {0};
    
    /* ��֤������Ч�� */
    if (NULL == p_dev || NULL == i2c_handle) {
        return NULL;
    }    

    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* ��ʼ����SHTC1�豸��Ϣ */
    am_i2c_mkdev (&(p_dev->i2c_dev),
                  i2c_handle,
                  SHTC1_DEVICE_ADDR,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_NONE);
    
    /* ��λSHTC1 */
    am_i2c_write (&(p_dev->i2c_dev), 0x00, SHTC1_RESET, 2);

    /* ��ȡSHTC1 ID */
    am_i2c_write (&(p_dev->i2c_dev), 0x00, SHTC1_ID, 2);
    am_i2c_read  (&(p_dev->i2c_dev), 0x00, shtc1_id, 2);
    AM_DBG_INFO("SHTC1_ID  = 0x%x  \r\n", (shtc1_id[0] << 8 | shtc1_id[1]));
    
    return  p_dev;
}

/**
 * \brief shtc1��������ȡ�¶�
 */
int am_shtc1_read_temp_hum (am_shtc1_handle_t handle, 
                            int8_t  *p_temp, 
                            uint8_t *p_hum)
{
    /* ���ڴ�Ŷ�ȡ��ֵ */
    uint8_t  temp_value[6] = {0};
    int      ret  = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    /* д�������� */
    am_i2c_write (p_i2c_dev, 0x00, SHTC1_CSE_H, 2);

    /* ��ȡ������� */
    ret = am_i2c_read (p_i2c_dev, 0x00, temp_value, 6);
 
    /* �жϷ����Ƿ�ɹ� */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* ʪ�ȼ��� */
    *p_hum  = 100 * ((temp_value[0] << 8) | temp_value[1]) / (1 << 16);
    
    /* �¶ȼ��� */
    *p_temp = (-45) + 175 * ((temp_value[3] << 8) | temp_value[4]) / (1 << 16);

    return ret;
}


