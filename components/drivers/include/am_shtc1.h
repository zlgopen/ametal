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
 * \brief SHTC1��ʪ�ȴ���������
 *
 * \internal
 * \par modification history:
 * - 1.00 18-09-12  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SHTC1_H
#define __AM_SHTC1_H

#ifdef __cplusplus
extern "C" {
#endif

 
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_i2c.h"
#include "am_zlg116_inst_init.h"
   
/**
 * \brief SHTC1 �豸�ṹ��
 */
typedef struct am_shtc1_dev {
    
    /**< \brief SHTC1 I2C�豸 */
    am_i2c_device_t i2c_dev; 
  
} am_shtc1_dev_t;

/** \brief SHTC1����������� */
typedef am_shtc1_dev_t *am_shtc1_handle_t;

/**
 * \brief SHTC1��������ʼ��
 *
 * \param[in] p_dev      : ָ��SHTC1�豸�ṹ���ָ��
 * \param[in] p_devinfo  : ָ��SHTC1�豸��Ϣ�ṹ���ָ�� 
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return SHTC1����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_shtc1_handle_t am_shtc1_init (am_shtc1_dev_t *p_dev, 
                                 am_i2c_handle_t i2c_handle);

/**
 * \brief SHTC1��������ȡ��ʪ��
 *
 * \param[in]  handle : SHTC1����������
 * \param[out] p_temp : ָ���¶����ݵ�ָ��
 * \param[out] p_hum  : ָ��ʪ�����ݵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */                                 
int am_shtc1_read_temp_hum (am_shtc1_handle_t handle, 
                            int8_t *p_temp, 
                            uint8_t *p_hum);                                 
                          
#ifdef __cplusplus
}
#endif

#endif /* __SHTC_1_H */

/* end of file */
