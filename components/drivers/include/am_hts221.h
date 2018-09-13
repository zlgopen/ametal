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
 * \brief HTS221����ͷ�ļ�
 * \sa hts221.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */
 
#ifndef __HTS221_H
#define __HTS221_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
#include "am_zlg116_inst_init.h"  
 

/**
 * \brief HTS221����������
 */
typedef enum am_hts221_config {

    /**
     * \brief �����õ��¶Ȳ���������     
     */    
    TEMP_SAMPLE_4     =   AM_SBF(0,3),    /**< \brief �¶�������Ϊ4           */
    TEMP_SAMPLE_8     =   AM_SBF(1,3),    /**< \brief �¶�������Ϊ8           */
    TEMP_SAMPLE_16    =   AM_SBF(2,3),    /**< \brief �¶�������Ϊ16          */
    TEMP_SAMPLE_32    =   AM_SBF(3,3),    /**< \brief �¶�������Ϊ32          */
    TEMP_SAMPLE_64    =   AM_SBF(4,3),    /**< \brief �¶�������Ϊ64          */
    TEMP_SAMPLE_128   =   AM_SBF(5,3),    /**< \brief �¶�������Ϊ128         */
    TEMP_SAMPLE_256   =   AM_SBF(6,3),    /**< \brief �¶�������Ϊ256         */
    TEMP_SAMPLE_512   =   AM_SBF(7,3),    /**< \brief �¶�������Ϊ512         */     
    
    /**
     * \brief �����õ�ʪ�Ȳ���������     
     */    
    HUM_SAMPLE_4      =   AM_SBF(0,0),    /**< \brief ʪ��������Ϊ4           */
    HUM_SAMPLE_8      =   AM_SBF(1,0),    /**< \brief ʪ��������Ϊ8           */
    HUM_SAMPLE_16     =   AM_SBF(2,0),    /**< \brief ʪ��������Ϊ16          */
    HUM_SAMPLE_32     =   AM_SBF(3,0),    /**< \brief ʪ��������Ϊ32          */
    HUM_SAMPLE_64     =   AM_SBF(4,0),    /**< \brief ʪ��������Ϊ64          */
    HUM_SAMPLE_128    =   AM_SBF(5,0),    /**< \brief ʪ��������Ϊ128         */
    HUM_SAMPLE_256    =   AM_SBF(6,0),    /**< \brief ʪ��������Ϊ256         */
    HUM_SAMPLE_512    =   AM_SBF(7,0),    /**< \brief ʪ��������Ϊ512         */    

    /**
     * \brief �����õĵ�Դģʽ     
     */
    POWER_ON          =   AM_SBF(1,7),    /**< \brief �򿪵�Դ                */
    POWER_DOWN        =   AM_SBF(0,7),    /**< \brief �رյ�Դ                */
    
    /**
     * \brief �����õ����ݸ��·�ʽ     
     */
    UPDATE_CONTINUE   =   AM_SBF(0,2),    /**< \brief ���ݸ���һ��            */
    UPDATE_ONE        =   AM_SBF(1,2),    /**< \brief ������������            */
    
    /**
     * \brief �����õ����ݸ����ٶ�     
     */ 
    DATA_UPDATE_1Hz   = AM_SBF(1,0),      /**< \brief ���ݸ����ٶ�Ϊ1Hz       */
    DATA_UPDATE_7Hz   = AM_SBF(2,0),      /**< \brief ���ݸ����ٶ�Ϊ7Hz       */
    DATA_UPDATE_12Hz  = AM_SBF(3,0),      /**< \brief ���ݸ����ٶ�Ϊ12.5Hz    */    

} am_hts221_config_t;
  
/**
 * \brief HTS221��ʪ��У׼����
 */ 
typedef struct {
    int16_t  x0;
    uint8_t  y0;
    int16_t  x1;
    uint8_t  y1;  
} am_lin_t;

/**
 * \brief HTS221 ʵ����Ϣ�ṹ�嶨��
 */
typedef struct am_hts221_devinfo {
    uint8_t temp_sample;             /**< \brief �����¶�������               */
    uint8_t hum_sample;              /**< \brief ����ʪ��������               */
    uint8_t power_mode;              /**< \brief ���õ�Դģʽ                 */
    uint8_t data_update_config;      /**< \brief �������ݸ��·�ʽ             */
    uint8_t data_update_rate;        /**< \brief �������ݸ����ٶ�             */
} am_hts221_devinfo_t;    
    
/**
 * \brief HTS221 �豸�ṹ��
 */
typedef struct am_hts221_dev {
    
    /**< \brief hts221 I2C�豸 */
    am_i2c_device_t i2c_dev; 
    
    /**< \brief ���ڱ���ʪ��У׼���� */
    am_lin_t calib_param_hum;
    
    /**< \brief ���ڱ����¶�У׼���� */
    am_lin_t calib_param_temp;
    
    /** \brief ���ڱ����豸��Ϣָ�� */
    const am_hts221_devinfo_t  *p_devinfo;    
    
} am_hts221_dev_t;

/** \brief HTS221����������� */
typedef am_hts221_dev_t *am_hts221_handle_t;

/**
 * \brief HTS221��������ʼ��
 *
 * \param[in] p_dev      : ָ��HTS221�豸�ṹ���ָ��
 * \param[in] p_devinfo  : ָ��HTS221�豸��Ϣ�ṹ���ָ�� 
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return HTS221����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_hts221_handle_t am_hts221_init (am_hts221_dev_t *p_dev, 
                                   const am_hts221_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle);

/**
 * \brief HTS221��������ȡ�¶�
 *
 * \param[in]  handle : HTS221����������
 * \param[out] temp   : ָ���¶ȵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */                                      
int am_hts221_read_temp (am_hts221_handle_t handle, int16_t* temp);
                                   
/**
 * \brief HTS221��������ȡʪ��
 *
 * \param[in]  handle : HTS221����������
 * \param[out] hum   : ָ��ʪ�ȵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */                                      
int am_hts221_read_hum (am_hts221_handle_t handle, uint8_t * hum);

                                   
    
#ifdef __cplusplus
}
#endif

#endif

/* end of file */ 



