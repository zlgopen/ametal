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
 * \brief  MMC5883MA����Ŵ�����Ӧ�ýӿ�ʵ��
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-12  wk, first implementation.
 * \endinternal
 */

#include "am_mmc5883ma.h"

/**
 * \brief ��ȡMMC5883MA���������ǼĴ���
 * \return AM_OK:�ɹ�    OTHER��ʧ��
 */
int __mmc5883ma_read_reg(am_mmc5883ma_handle_t handle, 
	                                   uint8_t reg_addr,
                                  int16_t *p_magnetic)
{
    uint8_t magnetic[2];
    int ret = AM_OK;
	  const uint8_t init_value[1] = {0x57};
		
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    ret =  am_i2c_read(p_i2c_dev, reg_addr, magnetic, 2);
    
		am_i2c_write(p_i2c_dev,MMC5883MA_INTERNAL_CONTROL_0,&init_value[0], 1);
		 
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* ���ݴ��� */
    if (magnetic[0] & 0x01) {
        *p_magnetic = (((uint16_t)(magnetic[1] << 8)| (uint16_t)magnetic[0])*360/65535);			
    }
		
    return ret;
}
 
 /**
 * \brief ��ȡmmc5883ma����������
 * \return AM_OK:�ɹ�    OTHER��ʧ��
 */
int am_mmc5883ma_read_magnetic(am_mmc5883ma_handle_t handle, int16_t* p_magnetic)
{
    int ret = AM_OK;

    /* ��ȡX������� */
    ret = __mmc5883ma_read_reg(handle, MMC5883MA_XOUT_LOW, &p_magnetic[0]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }  

    /* ��ȡY����ٶ� */    
    ret = __mmc5883ma_read_reg(handle, MMC5883MA_YOUT_LOW, &p_magnetic[1]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
		
    /* ��ȡZ����ٶ� */    
    ret = __mmc5883ma_read_reg(handle, MMC5883MA_ZOUT_LOW, &p_magnetic[2]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
		
    /* ��ȡ�¶� */    
    ret = __mmc5883ma_read_reg(handle, MMC5883MA_TEMPERATURE, &p_magnetic[3]);
    /* ����ȡ�Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }
		
    return  ret;  
}

/**
 * \brief MMC5883MA����Ŵ�������ʼ��
 */
am_mmc5883ma_handle_t am_mmc5883ma_init(am_mmc5883ma_dev_t *p_dev, 
                          const am_mmc5883ma_devinfo_t *p_devinfo,
                                       am_i2c_handle_t i2c_handle)
{
    uint8_t mmc5883ma_id = 0; 
  
    /* ��֤������Ч�� */
    if (NULL == p_dev ||  NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }
		
    p_dev->i2c_dev.handle = i2c_handle; 
		
    /* ��ʼ���ú�MMC5883MA�豸��Ϣ */
    am_i2c_mkdev(&(p_dev->i2c_dev),i2c_handle,
                   MMC5883MA_ADDR,AM_I2C_ADDR_7BIT 
		                   | AM_I2C_SUBADDR_1BYTE);

    /* ��ȡMMC5883MA ID */
    am_i2c_read(&(p_dev->i2c_dev),MMC5883MA_PRODUCT_ID_1, &mmc5883ma_id, 1);
    AM_DBG_INFO("mmc5883ma_ID = 0x%x\r\n", mmc5883ma_id);
		
		 /* ���������ų��¶Ȳ��� */
    am_i2c_write(&(p_dev->i2c_dev), MMC5883MA_INTERNAL_CONTROL_0, &(p_devinfo->start_measurement), 1);
		
    /* ���ò���Ƶ��Ϊ14Hz */
    am_i2c_write(&(p_dev->i2c_dev), MMC5883MA_INTERNAL_CONTROL_2, &(p_devinfo->frequency), 1);
		
    return p_dev;
}

/**
 * \brief MMC5883MA �豸���ʼ��
 */
void am_mmc5883ma_deinit (am_mmc5883ma_handle_t handle)
{
	
}

/* end of file */
