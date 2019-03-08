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
 * \brief ������ٶȴ����� BMA253 ͷ�ļ�
 *
 * \internal
 * \par modification history
 * - 1.00 19-02-19  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_BMA253_H
#define __AM_SENSOR_BMA253_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief ������BMA253�ṩ������ͨ��ID����
 */
#define AM_BMA253_CHAN_1      0           /* BMA253 ��ͨ��1��X����ٶ�        */
#define AM_BMA253_CHAN_2      1           /* BMA253 ��ͨ��2��Y����ٶ�        */
#define AM_BMA253_CHAN_3      2           /* BMA253 ��ͨ��3��Z����ٶ�        */
#define AM_BMA253_CHAN_4      3           /* BMA253 ��ͨ��4���¶�             */
    
/**
 * \brief ������ BMA253 �豸��Ϣ�ṹ��
 */
typedef struct am_sensor_bma253_devinfo {
    
    /**
     * \brief ����׼��������������
     */
    int     trigger_pin;
    
    /**
     * \brief I2C 7λ �豸��ַ
     *
     * ���豸7λ��ַΪ 0X18
     */
    uint8_t i2c_addr;

} am_sensor_bma253_devinfo_t;

/**
 * \breif ������ BMA253 �豸�ṹ�嶨��
 */
typedef struct am_sensor_bma253_dev {
    am_sensor_serv_t               bma253_dev;   /**< \breif ��������׼����   */
    am_i2c_device_t                i2c_dev;      /**< \brief i2c�豸ʵ��      */
    void                          *p_arg[4];     /**< \brief �����ص��������� */
    uint8_t                        flags[4];     /**< \brief ������־λ       */
    am_isr_defer_job_t             g_myjob;      /**< \brief �ж���ʱ�������� */ 
    am_sensor_val_t                sam_range;    /**< \brief ������Χ         */ 
    uint16_t                       trigger;      /**< \brief ��¼��־λ       */
    am_sensor_val_t                data[4];      /**< \brief �ڲ����ݻ���     */
    /** \brief ����׼�����������ص����� */
    am_sensor_trigger_cb_t         pfn_trigger_cb[4];
    
    /** \brief �豸��Ϣ */
    const am_sensor_bma253_devinfo_t *dev_info;

} am_sensor_bma253_dev_t;

/**
 * \brief ������ BMA253 ��ʼ��
 *
 * \param[in] p_dev     : ָ�򴫸���BMA253�豸��ָ��
 * \param[in] p_devinfo : ָ�򴫸���BMA253�豸��Ϣ��ָ��
 * \param[in] handle    : IIC��׼������
 *
 * \return ��������׼�������������Ϊ NULL��������ʼ��ʧ��
 */
am_sensor_handle_t am_sensor_bma253_init (
        am_sensor_bma253_dev_t           *p_dev,
        const am_sensor_bma253_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief ������ BMA253 ȥ��ʼ��
 *
 * \param[in] handle : am_sensor_bma253_init()��ʼ��������õĴ�������׼������
 *
 * \return ��
 */
am_err_t am_sensor_bma253_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_BMA253_H */

/* end of file */
