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
  
#ifndef __AM_BH_1730FVC_H
#define __AM_BH_1730FVC_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_i2c.h"
#include "am_zlg116_inst_init.h"
    
/**
 * \brief BH1730FVC����������
 */
typedef enum am_bh1730fvc_config {
 
    /**
     * \brief �����õĲ���ģʽ    
     */     
    MEAS_CONTINUE   =  AM_SBF(0,3),       /**< \brief ��������                */
    MEAS_ONE_TIME   =  AM_SBF(1,3),       /**< \brief ����һ��                */
    
    /**
     * \brief �����õ���������   
     */     
    TYPE0_AND_TYPE1 =  AM_SBF(0,2),       /**< \brief ��������0��1            */
    TYPE0_ONLY      =  AM_SBF(1,2),       /**< \brief ����������0             */
 
    /**
     * \brief �����õĲ���״̬   
     */    
    ADC_DIS         =  AM_SBF(0,1),       /**< \brief ����ADCת��             */    
    ADC_EN          =  AM_SBF(1,1),       /**< \brief ʹ��ADCת��             */
    
    /**
     * \brief �����õĵ�Դģʽ   
     */    
    POWER_ON        =  AM_SBF(1,0),       /**< \brief �򿪵�Դ                */    
    POWER_DOWN      =  AM_SBF(0,0),       /**< \brief �رյ�Դ                */ 

    /**
     * \brief �����õ�ADC�ֱ���   
     */   
    ADC_MODE_X1     =  AM_SBF(0,0),       /**< \brief X1����ģʽ              */ 
    ADC_MODE_X2     =  AM_SBF(1,0),       /**< \brief X2����ģʽ              */
    ADC_MODE_X64    =  AM_SBF(2,0),       /**< \brief X64����ģʽ             */
    ADC_MODE_X128   =  AM_SBF(3,0),       /**< \brief X128����ģʽ            */

} am_bh1730fvc_config_t;


/**
 * \brief BH1730FVC ʵ����Ϣ�ṹ�嶨��
 */
typedef struct am_bh1730fvc_devinfo {

    uint8_t meas_mode;                    /**< \brief ���ò���ģʽ            */
    uint8_t data_sel;                     /**< \brief ������������            */
    uint8_t adc_en;                       /**< \brief ���ò���״̬            */
    uint8_t power_mode;                   /**< \brief ���õ�Դģʽ            */
    /**
     * \brief Integration Time : ITIME_ms = Tint * 964 * (256 - integrate_time)
     * \brief Measurement time : Tmt = ITIME_ms + Tint * 714
     */
    uint8_t integrate_time;               /**< \brief ���û���ʱ��            */ 
    uint8_t adc_res_setting;              /**< \brief ����ADC�ֱ���           */ 
    
} am_bh1730fvc_devinfo_t;     
    
/**
 * \brief BH1730FVC �豸�ṹ��
 */
typedef struct am_bh1730fvc_dev {
    
    /**< \brief BH1730FVC I2C�豸 */
    am_i2c_device_t i2c_dev; 

    /** \brief ���ڱ����豸��Ϣָ�� */
    const am_bh1730fvc_devinfo_t  *p_devinfo;    
    
} am_bh1730fvc_dev_t;

/** \brief BH1730FVC����������� */
typedef am_bh1730fvc_dev_t *am_bh1730fvc_handle_t;    


/**
 * \brief BH1730FVC��������ʼ��
 *
 * \param[in] p_dev      : ָ��BH1730FVC�豸�ṹ���ָ��
 * \param[in] p_devinfo  : ָ��BH1730FVC�豸��Ϣ�ṹ���ָ�� 
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return BH1730FVC����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_bh1730fvc_handle_t am_bh1730fvc_init (am_bh1730fvc_dev_t *p_dev, 
                                        const am_bh1730fvc_devinfo_t *p_devinfo, 
                                        am_i2c_handle_t i2c_handle);

/**
 * \brief BH1730FVC��������ȡ�¶�
 *
 * \param[in]  handle     : BH1730FVC����������
 * \param[out] p_als_adc  : ָ�����ǿ�ȵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */                                         
int am_bh1730fvc_read_als_adc (am_bh1730fvc_handle_t handle, 
                               uint16_t *p_als_adc);
                                    

#ifdef __cplusplus
}
#endif

#endif /* __BH_1730FVC_H */

/* end of file */
