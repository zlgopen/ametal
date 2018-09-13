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
 * \brief    BMA253������ٶȴ�����Ӧ�ýӿ��ļ�
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-07  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_BMA253_H
#define __AM_BMA253_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
#include "am_i2c.h"
#include "am_zlg116_inst_init.h"
    
/**
 * \addtogroup am_if_bma253
 * \copydoc am_bma253.h
 * @{
 */

/**
 * \brief BMA253����������
 */
typedef enum am_bma253_config {
    /**
     * \brief �����õļ��ٶȷ�Χ     
     */
    PMU_RANGE_2G   =    0X03,             /**< \brief ���ٶȷ�Χ��2g          */
    PMU_RANGE_4G   =    0X05,             /**< \brief ���ٶȷ�Χ��4g          */
    PMU_RANGE_8G   =    0X08,             /**< \brief ���ٶȷ�Χ��8g          */
    PMU_RANGE_16G  =    0X0C,             /**< \brief ���ٶȷ�Χ��16g         */

    /**
     * \brief �����õ��˲�����    
     */    
    PMU_BW_8Hz     =    0X08,             /**< \brief �����˲�����7.81Hz      */
    PMU_BW_16Hz    =    0X09,             /**< \brief �����˲�����15.36Hz     */
    PMU_BW_32Hz    =    0X0A,             /**< \brief �����˲�����31.25Hz     */
    PMU_BW_63Hz    =    0X0B,             /**< \brief �����˲�����62.5Hz      */
    PMU_BW_125Hz   =    0X0C,             /**< \brief �����˲�����125Hz       */
    PMU_BW_250Hz   =    0X0D,             /**< \brief �����˲�����250Hz       */
    PMU_BW_500Hz   =    0X0E,             /**< \brief �����˲�����500Hz       */
    PMU_BW_1000Hz  =    0X0F,             /**< \brief �����˲�����1000Hz      */
    
    /**
     * \brief �����õĵ�Դģʽ   
     */ 
    NORMAL_MODE       = 0X00,             /**< \brief ����ģʽ                */
    DEEP_SUSPEND_MODE = 0X20,             /**< \brief ��ȹ���ģʽ            */
    LOW_POWER_MODE    = 0X40,             /**< \brief �͹���ģʽ              */
    SUSPEND_MODE      = 0X80,             /**< \brief ����ģʽ                */
    
    /**
     * \brief �����õ�˯������   
     */
    SLEEP_DUR_HALF_MS = AM_SBF(5,1),      /**< \brief ���õ�˯������Ϊ0.5ms   */
    SLEEP_DUR_1MS     = AM_SBF(6,1),      /**< \brief ���õ�˯������Ϊ1ms     */
    SLEEP_DUR_2MS     = AM_SBF(7,1),      /**< \brief ���õ�˯������Ϊ2ms     */
    SLEEP_DUR_4MS     = AM_SBF(8,1),      /**< \brief ���õ�˯������Ϊ4ms     */
    SLEEP_DUR_6MS     = AM_SBF(9,1),      /**< \brief ���õ�˯������Ϊ6ms     */
    SLEEP_DUR_10MS    = AM_SBF(10,1),     /**< \brief ���õ�˯������Ϊ10ms    */
    SLEEP_DUR_25MS    = AM_SBF(11,1),     /**< \brief ���õ�˯������Ϊ25ms    */
    SLEEP_DUR_50MS    = AM_SBF(12,1),     /**< \brief ���õ�˯������Ϊ50ms    */
    SLEEP_DUR_100MS   = AM_SBF(13,1),     /**< \brief ���õ�˯������Ϊ100ms   */
    SLEEP_DUR_500MS   = AM_SBF(14,1),     /**< \brief ���õ�˯������Ϊ500ms   */
    SLEEP_DUR_1000MS  = AM_SBF(15,1)      /**< \brief ���õ�˯������Ϊ1000ms  */
    
} am_bma253_config_t;
       
/**
 * \brief BMA253 ʵ����Ϣ�ṹ�嶨��
 */
typedef struct am_bma253_devinfo {
    uint8_t pmu_range;                    /**< \brief ���ٶȷ�Χ              */
    uint8_t pmu_bw;                       /**< \brief �����˲�����            */
    uint8_t power_mode;                   /**< \brief ��Դģʽ                */
    uint8_t sleep_period;                 /**< \brief �͹���ģʽ˯������      */
} am_bma253_devinfo_t;
       
/**
 * \brief BMA253 �豸�ṹ��
 */
typedef struct am_bma253_dev {
    
    /**< \brief BMA253 I2C�豸 */
    am_i2c_device_t i2c_dev; 
    
    /** \brief ���ڱ����豸��Ϣָ�� */
    const am_bma253_devinfo_t  *p_devinfo;    
    
} am_bma253_dev_t;

/** \brief BMA253����������� */
typedef am_bma253_dev_t *am_bma253_handle_t;


/**
 * \brief BMA253��������ȡ������ٶ�
 *
 * \param[in]  handle  : BMA253����������
 * \param[out] p_accel : ָ����ٶȵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_bma253_read_accel (am_bma253_handle_t handle, int16_t* p_accel);

/**
 * \brief BMA253��������ȡ�¶�
 *
 * \param[in]  handle : BMA253����������
 * \param[out] p_temp : ָ���¶ȵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_bma253_read_temp (am_bma253_handle_t handle,int8_t* p_temp);

/**
 * \brief BMA253�����������λ
 * \param[in]  handle : BMA253����������
 * \retval AM_OK : �����λ�ɹ�
 * \retval OTHER : �����λʧ��
 */
int am_bma253_soft_reset (am_bma253_handle_t handle);

/**
 * \brief BMA253��������ʼ��
 *
 * \param[in] p_dev      : ָ��BMA253�豸�ṹ���ָ��
 * \param[in] p_devinfo  : ָ��BMA253�豸��Ϣ�ṹ���ָ�� 
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return BMA253����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_bma253_handle_t am_bma253_init (am_bma253_dev_t *p_dev, 
                                   const am_bma253_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle);

/**
 * \brief BMA253 �豸���ʼ��
 * \param[in]  handle : BMA253����������
 * \return ��
 */
void am_bma253_deinit (am_bma253_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_BMA253_H */

/* end of file */
