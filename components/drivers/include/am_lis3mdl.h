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
 
#ifndef __AM_LIS3MDL_H
#define __AM_LIS3MDL_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_i2c.h"
#include "am_zlg116_inst_init.h"
 
/**
 * \brief LIS3MDL����������
 */
typedef enum am_lis3mdl_config {
    
    /**
     * \brief �¶ȴ���������
     */
    TEMP_ENABLE       =   AM_SBF(1,7),  /**< \brief ʹ���¶Ȳ���              */
    TEMP_DISABLE      =   AM_SBF(0,7),  /**< \brief �����¶Ȳ���              */
    
    /**
     * \brief X��Y�����ģʽѡ��
     */    
    LOW_POWER_MODE_XY   =   AM_SBF(0,5),  /**< \brief �͹���ģʽ              */
    MEDIUM_MODE_XY      =   AM_SBF(1,5),  /**< \brief �е�����ģʽ            */    
    HIGH_MODE_XY        =   AM_SBF(2,5),  /**< \brief ������ģʽ              */    
    ULTRA_HIGH_MODE_XY  =   AM_SBF(3,5),  /**< \brief ��������ģʽ            */    

    /**
     * \brief �����������
     */ 
    ODR_1Hz           =   AM_SBF(0,2),  /**< \brief �������Ƶ��0.625Hz       */
    ODR_2Hz           =   AM_SBF(1,2),  /**< \brief �������Ƶ��1.25Hz        */
    ODR_3Hz           =   AM_SBF(2,2),  /**< \brief �������Ƶ��2.5Hz         */
    ODR_5Hz           =   AM_SBF(3,2),  /**< \brief �������Ƶ��5Hz           */
    ODR_10Hz          =   AM_SBF(4,2),  /**< \brief �������Ƶ��10Hz          */
    ODR_20Hz          =   AM_SBF(5,2),  /**< \brief �������Ƶ��20Hz          */
    ODR_40Hz          =   AM_SBF(6,2),  /**< \brief �������Ƶ��40Hz          */
    ODR_80Hz          =   AM_SBF(7,2),  /**< \brief �������Ƶ��80Hz          */    
   
    /**
     * \brief ���ݿ������ʹ�ܿ���
     */    
    FAST_ODR_ENABLE   =   AM_SBF(1,1),  /**< \brief ʹ�����ݿ������          */
    FAST_ODR_DISABLE  =   AM_SBF(0,1),  /**< \brief ��ʹ�����ݿ������        */
    
    /**
     * \brief �Լ�ʹ�ܿ���
     */    
    SELF_TEST_ENABLE   =   AM_SBF(1,0),  /**< \brief ʹ���Լ�                 */
    SELF_TEST_DISABLE  =   AM_SBF(0,0),  /**< \brief ��ʹ���Լ�               */ 

    /**
     * \brief �����õĴų�������Χ
     */ 
    FULL_4GAUSS        =   AM_SBF(0,5),  /**< \brief �ų�������Χ��4 gauss    */
    FULL_8GAUSS        =   AM_SBF(1,5),  /**< \brief �ų�������Χ��8 gauss    */     
    FULL_12GAUSS       =   AM_SBF(2,5),  /**< \brief �ų�������Χ��12 gauss   */
    FULL_16GAUSS       =   AM_SBF(3,5),  /**< \brief �ų�������Χ��16 gauss   */    
  
    /**
     * \brief �����õ�ϵͳģʽ
     */ 
    CONTINUE_MODE      =   AM_SBF(0,0),  /**< \brief ����ת��ģʽ             */  
    SINGLE_MODE        =   AM_SBF(1,0),  /**< \brief ����ת��ģʽ             */
    POWER_DOWN         =   AM_SBF(2,0),  /**< \brief ����ģʽ                 */
 
    /**
     * \brief Z�����ģʽѡ��
     */    
    LOW_POWER_MODE_Z   =   AM_SBF(0,2),  /**< \brief �͹���ģʽ              */
    MEDIUM_MODE_Z      =   AM_SBF(1,2),  /**< \brief �е�����ģʽ            */    
    HIGH_MODE_Z        =   AM_SBF(2,2),  /**< \brief ������ģʽ              */    
    ULTRA_HIGH_MODE_Z  =   AM_SBF(3,2),  /**< \brief ��������ģʽ            */ 

} am_lis3mdl_config_t;
    
/**
 * \brief LIS3MDL ʵ����Ϣ�ṹ�嶨��
 */
typedef struct am_lis3mdl_devinfo {

    uint8_t temp_en;                    /**< \brief �¶Ȳ���ʹ��λ            */
    uint8_t operate_mode_xy;            /**< \brief X��Y�����ģʽѡ��        */
    uint8_t output_data_rate;           /**< \brief �����������              */
    uint8_t fast_odr_en;                /**< \brief ���ݿ������ʹ��λ        */
    uint8_t self_test_en;               /**< \brief �Լ�ʹ��λ                */
    uint8_t full_scale;                 /**< \brief �ų�������Χ              */
    uint8_t system_mode;                /**< \brief ϵͳģʽ                  */
    uint8_t operate_mode_z;             /**< \brief Z�����ģʽѡ��           */    
    
} am_lis3mdl_devinfo_t;     
    
/**
 * \brief LIS3MDL �豸�ṹ��
 */
typedef struct am_lis3mdl_dev {
    
    /**< \brief LIS3MDL I2C�豸 */
    am_i2c_device_t i2c_dev; 

    /** \brief ���ڱ����豸��Ϣָ�� */
    const am_lis3mdl_devinfo_t  *p_devinfo;    
    
} am_lis3mdl_dev_t;

/** \brief LIS3MDL����������� */
typedef am_lis3mdl_dev_t *am_lis3mdl_handle_t;  

/**
 * \brief LIS3MDL��������ʼ��
 *
 * \param[in] p_dev      : ָ��LIS3MDL�豸�ṹ���ָ��
 * \param[in] p_devinfo  : ָ��LIS3MDL�豸��Ϣ�ṹ���ָ�� 
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return LIS3MDL����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_lis3mdl_handle_t am_lis3mdl_init (am_lis3mdl_dev_t *p_dev, 
                                     const am_lis3mdl_devinfo_t *p_devinfo, 
                                     am_i2c_handle_t i2c_handle);
 
/**
 * \brief LIS3MDL��������ȡ�ų�ǿ����ֵ
 *
 * \param[in]  handle : LIS3MDL����������
 * \param[out] p_data : ָ�����ݵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_lis3mdl_read (am_lis3mdl_handle_t handle, uint16_t *p_data);                                     

/**
 * \brief LIS3MDL��������ȡ�¶�
 *
 * \param[in]  handle : LIS3MDL����������
 * \param[out] p_temp : ָ�����ݵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */                                     
int am_lis3mdl_read_temp (am_lis3mdl_handle_t handle, int8_t *p_temp);                                     
                                     
#ifdef __cplusplus
}
#endif

#endif /* __AM_LIS3MDL_H */

/* end of file */
