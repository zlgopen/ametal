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
 * \brief LPS22HB����ͷ�ļ�
 * \sa am_lsp22hb.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */
 
#ifndef __AM_LPS22HB_H
#define __AM_LPS22HB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
#include "am_zlg116_inst_init.h" 
 
/**
 * \brief LPS22HB����������
 */
typedef enum am_lps22hb_config {

    /**
     * \brief �����õ����ݸ����ٶ�     
     */    
    DATA_UPDATE_1Hz   = AM_SBF(1,4),      /**< \brief ���ݸ����ٶ�Ϊ1Hz       */
    DATA_UPDATE_10Hz  = AM_SBF(2,4),      /**< \brief ���ݸ����ٶ�Ϊ10Hz      */
    DATA_UPDATE_25Hz  = AM_SBF(3,4),      /**< \brief ���ݸ����ٶ�Ϊ25Hz      */    
    DATA_UPDATE_50Hz  = AM_SBF(4,4),      /**< \brief ���ݸ����ٶ�Ϊ50Hz      */    
    DATA_UPDATE_75Hz  = AM_SBF(5,4),      /**< \brief ���ݸ����ٶ�Ϊ75Hz      */    
    
    /**
     * \brief ��ͨ�˲�����    
     */   
    ENABLE_LPFP       = AM_SBF(1,3),      /**< \brief ʹ�ܵ�ͨ�˲�            */
    DISABLE_LPFP      = AM_SBF(0,3),      /**< \brief ��ʹ�ܵ�ͨ�˲�          */
    
    /**
     * \brief ��ͨ�˲�����    
     */   
    LPFP_BW_ODR9       = AM_SBF(0,2),      /**< \brief �˲�����Ϊ �����ٶ�/9  */
    LPFP_BW_ODR20      = AM_SBF(1,2),      /**< \brief �˲�����Ϊ �����ٶ�/20 */    
 
} am_lps22hb_config_t;



/**
 * \brief LPS22HB ʵ����Ϣ�ṹ�嶨��
 */
typedef struct am_lps22hb_devinfo {
    uint8_t output_data_rate;             /**< \brief �������ݸ����ٶ�        */
    uint8_t enable_lpfp;                  /**< \brief �Ƿ�ʹ�ܵ�ͨ�˲�        */
    uint8_t lpfp_bw;                      /**< \brief ���õ�ͨ�˲�����        */
} am_lps22hb_devinfo_t;    
    
/**
 * \brief LPS22HB �豸�ṹ��
 */
typedef struct am_lps22hb_dev {
    
    /**< \brief LPS22HB I2C�豸 */
    am_i2c_device_t i2c_dev; 
    
    /** \brief ���ڱ����豸��Ϣָ�� */
    const am_lps22hb_devinfo_t  *p_devinfo;    
    
} am_lps22hb_dev_t;

/** \brief LPS22HB����������� */
typedef am_lps22hb_dev_t *am_lps22hb_handle_t;


/**
 * \brief LPS22HB��������ʼ��
 *
 * \param[in] p_dev      : ָ��LPS22HB�豸�ṹ���ָ��
 * \param[in] p_devinfo  : ָ��LPS22HB�豸��Ϣ�ṹ���ָ�� 
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return LPS22HB����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_lps22hb_handle_t am_lsp22hb_init (am_lps22hb_dev_t *p_dev, 
                                   const am_lps22hb_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle);

/**
 * \brief LPS22HB��������ȡ�¶�
 *
 * \param[in]  handle : LPS22HB����������
 * \param[out] temp   : ָ���¶ȵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */                                   
int am_lsp22hb_read_temp (am_lps22hb_handle_t handle, int16_t *temp);
                                   
/**
 * \brief LPS22HB��������ȡѹ��
 *
 * \param[in]  handle : LPS22HB����������
 * \param[out] press  : ָ��ѹ����ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */                                   
int am_lsp22hb_read_press (am_lps22hb_handle_t handle, uint32_t *press); 



#ifdef __cplusplus
}
#endif

#endif

/* end of file */ 

 


