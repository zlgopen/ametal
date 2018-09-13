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
 * \brief BME280ѹ����ʪ�ȴ�����Ӧ�ýӿ�ͷ�ļ�
 * \sa am_bme280.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_BME280_H
#define __AM_BME280_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
#include "am_zlg116_inst_init.h"
 
/**
 * \brief BME280����������
 */
typedef enum am_bme280_config {

    /**
     * \brief �����õ�ʪ�ȹ�����������     
     */
    HUMIDITY_OVERESAMPLE_X1  = AM_SBF(1,0),  /**< \brief ʪ�ȹ�����������Ϊ1  */
    HUMIDITY_OVERESAMPLE_X2  = AM_SBF(2,0),  /**< \brief ʪ�ȹ�����������Ϊ2  */ 
    HUMIDITY_OVERESAMPLE_X4  = AM_SBF(3,0),  /**< \brief ʪ�ȹ�����������Ϊ4  */
    HUMIDITY_OVERESAMPLE_X8  = AM_SBF(4,0),  /**< \brief ʪ�ȹ�����������Ϊ8  */
    HUMIDITY_OVERESAMPLE_X16 = AM_SBF(5,0),  /**< \brief ʪ�ȹ�����������Ϊ16 */
    
    /**
     * \brief �����õ�ѹ��������������     
     */
    PRESSURE_OVERESAMPLE_X1  = AM_SBF(1,2),  /**< \brief ѹ��������������Ϊ1  */
    PRESSURE_OVERESAMPLE_X2  = AM_SBF(2,2),  /**< \brief ѹ��������������Ϊ2  */ 
    PRESSURE_OVERESAMPLE_X4  = AM_SBF(3,2),  /**< \brief ѹ��������������Ϊ4  */
    PRESSURE_OVERESAMPLE_X8  = AM_SBF(4,2),  /**< \brief ѹ��������������Ϊ8  */
    PRESSURE_OVERESAMPLE_X16 = AM_SBF(5,2),  /**< \brief ѹ��������������Ϊ16 */
    
    /**
     * \brief �����õ��¶ȹ�����������     
     */
    TEMP_OVERESAMPLE_X1  = AM_SBF(1,5),      /**< \brief �¶ȹ�����������Ϊ1  */
    TEMP_OVERESAMPLE_X2  = AM_SBF(2,5),      /**< \brief �¶ȹ�����������Ϊ2  */ 
    TEMP_OVERESAMPLE_X4  = AM_SBF(3,5),      /**< \brief �¶ȹ�����������Ϊ4  */
    TEMP_OVERESAMPLE_X8  = AM_SBF(4,5),      /**< \brief �¶ȹ�����������Ϊ8  */
    TEMP_OVERESAMPLE_X16 = AM_SBF(5,5),      /**< \brief �¶ȹ�����������Ϊ16 */    
    
    /**
     * \brief �����õĵ�Դģʽ     
     */
    SLEEP_MODE   = AM_SBF(0,0),              /**< \brief ˯��ģʽ             */
    FORCED_MODE  = AM_SBF(1,0),              /**< \brief ǿ��ģʽ             */
    NORMAL_MODE  = AM_SBF(3,0),              /**< \brief ����ģʽ             */

    /**
     * \brief �����õĴ���ʱ��    
     */
    STANDBY_TIME_HALF_MS = AM_SBF(0,5),      /**< \brief ����ʱ��0.5ms        */
    STANDBY_TIME_63MS    = AM_SBF(1,5),      /**< \brief ����ʱ��62.5ms       */
    STANDBY_TIME_125MS   = AM_SBF(2,5),      /**< \brief ����ʱ��125ms        */
    STANDBY_TIME_250MS   = AM_SBF(3,5),      /**< \brief ����ʱ��250ms        */
    STANDBY_TIME_500MS   = AM_SBF(4,5),      /**< \brief ����ʱ��500ms        */
    STANDBY_TIME_1000MS  = AM_SBF(5,5),      /**< \brief ����ʱ��1000ms       */
    STANDBY_TIME_2000MS  = AM_SBF(6,5),      /**< \brief ����ʱ��2000ms       */
    STANDBY_TIME_4000MS  = AM_SBF(7,5),      /**< \brief ����ʱ��4000ms       */

} am_bme280_config_t;
    
   
/**
 * \brief BME280У׼����
 */ 
typedef struct bme280_calib_param {
	uint16_t  dig_t1;
	int16_t   dig_t2;
	int16_t   dig_t3;
	uint16_t  dig_p1;
	int16_t   dig_p2;
	int16_t   dig_p3;
	int16_t   dig_p4;
	int16_t   dig_p5;
	int16_t   dig_p6;
	int16_t   dig_p7;
	int16_t   dig_p8;
	int16_t   dig_p9;
	int32_t   t_fine;
    uint8_t   dig_h1;
    int16_t   dig_h2;
    uint8_t   dig_h3;
    int16_t   dig_h4;
    int16_t   dig_h5;
    uint8_t   dig_h6;    
} bme280_calib_param_t;
    
/**
 * \brief BME280 ʵ����Ϣ�ṹ�嶨��
 */
typedef struct am_bme280_devinfo {
    uint8_t hum_oversampe;                /**< \brief ʪ�ȹ�����������        */    
    uint8_t press_oversampe;              /**< \brief ѹ��������������        */
    uint8_t temp_oversampe;               /**< \brief �¶ȹ�����������        */
    uint8_t power_mode;                   /**< \brief ��Դģʽ                */
    uint8_t standby_time;                 /**< \brief ���ô���ʱ��            */
} am_bme280_devinfo_t;
  
/**
 * \brief BME280 �豸�ṹ��
 */
typedef struct am_bme280_dev {
    
    /**< \brief BMP280 I2C�豸 */
    am_i2c_device_t i2c_dev; 
    
    /** \brief ���ڱ���У׼���� */
    bme280_calib_param_t calib_param; 
    
    /** \brief ���ڱ����豸��Ϣָ�� */
    const am_bme280_devinfo_t  *p_devinfo;    
    
} am_bme280_dev_t;

/** \brief BME280����������� */
typedef am_bme280_dev_t *am_bme280_handle_t;
  
/**
 * \brief BME280��������ȡ�¶�
 *
 * \param[in]  handle : BME280����������
 * \param[out] temp   : ָ���¶ȵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_bme280_read_temp (am_bme280_handle_t handle, int8_t *temp);

/**
 * \brief BME280��������ȡ��ѹ
 *
 * \param[in]  handle : BME280����������
 * \param[out] press  : ָ����ѹ��ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_bme280_read_press (am_bme280_handle_t handle, uint32_t *press);

/**
 * \brief BME280��������ȡʪ��
 *
 * \param[in]  handle   : BME280����������
 * \param[out] humidity : ָ��ʪ�ȵ�ָ��
 *
 * \retval AM_OK : ��ȡ�ɹ�
 * \retval OTHER : ��ȡʧ��
 */
int am_bme280_read_hum (am_bme280_handle_t handle, uint8_t *humidity);

/**
 * \brief BME280��������ʼ��
 *
 * \param[in] p_dev      : ָ��BME280�豸�ṹ���ָ��
 * \param[in] p_devinfo  : ָ��BME280�豸��Ϣ�ṹ���ָ�� 
 * \param[in] i2c_handle : I2C��׼����������
 *
 * \return BME280����������,���Ϊ NULL��������ʼ��ʧ��
 */
am_bme280_handle_t am_bme280_init (am_bme280_dev_t *p_dev, 
                                   const am_bme280_devinfo_t *p_devinfo, 
                                   am_i2c_handle_t i2c_handle);


#ifdef __cplusplus
}
#endif

#endif

/* end of file */


