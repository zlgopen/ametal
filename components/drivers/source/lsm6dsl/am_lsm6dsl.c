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
 * \brief  LSM6DSL������ٶȴ�����Ӧ�ýӿ�ʵ��
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-12  wk, first implementation.
 * \endinternal
 */

#include "am_lsm6dsl.h"

/**
 * \brief ��ȡLSM6DSL������ٶȼĴ���
 * \return AM_OK:�ɹ�    OTHER��ʧ��
 */
static int __lsm6dsl_read_reg(am_lsm6dsl_handle_t handle, 
                                        uint8_t reg_addr,
                                        int16_t *p_accel)
{
    uint8_t accel_temp[2];
    int ret = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    ret =  am_i2c_read(p_i2c_dev, reg_addr, accel_temp, 2);
    
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
		/* ���ݴ��� */
    if (accel_temp[0] & 0x01) {
        *p_accel = (((uint16_t)(accel_temp[1] << 8)| (uint16_t)accel_temp[0])*360/65535);			
    }
    
    return ret;
}
 

int am_lsm6dsl_read_temp(am_lsm6dsl_handle_t handle, int16_t* p_temp)
{
	  int ret = AM_OK;
    /* ��ȡ�¶� */    
    ret = __lsm6dsl_read_reg(handle, LSM6DSL_OUT_TEMP_L, p_temp);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
		
		/* �����¶� */
    *p_temp = (int8_t)(24 + *p_temp * 1.0 / 2);
		
		return  ret;
}


 /**
 * \brief ��ȡlsm6dsl������ٶ�
 * \return AM_OK:�ɹ�    OTHER��ʧ��
 */
int am_lsm6dsl_read_accel(am_lsm6dsl_handle_t handle, int16_t* p_accel)
{
    int ret = AM_OK;
    /* ��ȡX������� */
    ret = __lsm6dsl_read_reg(handle, LSM6DSL_OUTX_L_XL, &p_accel[0]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }  

    /* ��ȡY����ٶ� */    
    ret = __lsm6dsl_read_reg(handle, LSM6DSL_OUTY_L_XL, &p_accel[1]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
		
    /* ��ȡZ����ٶ� */    
    ret = __lsm6dsl_read_reg(handle, LSM6DSL_OUTZ_L_XL, &p_accel[2]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
		
    return  ret;  
}

/**
 * \brief LSM6DSL����Ŵ�������ʼ��
 */
am_lsm6dsl_handle_t am_lsm6dsl_init(am_lsm6dsl_dev_t *p_dev, 
                          const am_lsm6dsl_devinfo_t *p_devinfo,
                                       am_i2c_handle_t i2c_handle)
{
    uint8_t lsm6dsl_id =    0; 
  
    /* ��֤������Ч�� */
    if (NULL == p_dev ||  NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }
		
    p_dev->i2c_dev.handle = i2c_handle; 
		
    /* ��ʼ���ú�LSM6DSL�豸��Ϣ */
    am_i2c_mkdev(&(p_dev->i2c_dev),i2c_handle,
                   LSM6DSL_ADDR,AM_I2C_ADDR_7BIT 
		                   | AM_I2C_SUBADDR_1BYTE);

    /* ��ȡLSM6DSL ID */
    am_i2c_read(&(p_dev->i2c_dev),LSM6DSL_WHO_AM_I, &lsm6dsl_id, 1);
    AM_DBG_INFO("lsm6dsl_ID = 0x%x\r\n", lsm6dsl_id);
		
		
		 /* ����״̬�Ĵ��� */
    am_i2c_write(&(p_dev->i2c_dev), LSM6DSL_CTRL1_XL, &(p_devinfo->ctrl1_xl), 1);
		
		
		 /* ����״̬�Ĵ��� */
    am_i2c_write(&(p_dev->i2c_dev), LSM6DSL_STATUS_REG, &(p_devinfo->status_reg), 1);
		
		
    /* ����FIFO�������Ϊ12.5Hz */
    am_i2c_write(&(p_dev->i2c_dev), LSM6DSL_FIFO_CTRL5, &(p_devinfo->fifo_odr), 1);
		
    return p_dev;
}

/**
 * \brief LSM6DSL �豸���ʼ��
 */
void am_lsm6dsl_deinit (am_lsm6dsl_handle_t handle)
{
	
}

/* end of file */
