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
 * \brief ���ռ����봫���� BH1730FVC ͷ�ļ�
 *
 * \internal
 * \par modification history
 * - 1.00 19-02-20  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_BH1730FVC_H
#define __AM_SENSOR_BH1730FVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief ������BH1730FVC�ṩ������ͨ��ID����
 */
#define AM_BH1730FVC_CHAN_1      0         /* BH1730FVC ��ͨ��1������ǿ��ͨ�� */

/**
 * \brief ������ BH1730FVC �豸��Ϣ�ṹ��
 */
typedef struct am_sensor_bh1730fvc_devinfo {
    
    /**
     * \brief ����׼��������������
     */
    int     trigger_pin;
    
    /**
     * \brief I2C 7λ �豸��ַ
     *
     * ���豸7λ��ַΪ 0x29
     */
    uint8_t i2c_addr;

} am_sensor_bh1730fvc_devinfo_t;

/**
 * \breif ������ BH1730FVC �豸�ṹ�嶨��
 */
typedef struct am_sensor_bh1730fvc_dev {
    am_sensor_serv_t               bh1730fvc_dev;/**< \breif ��������׼����   */
    am_i2c_device_t                i2c_dev;      /**< \brief i2c�豸ʵ��      */
    void                          *p_arg[1];     /**< \brief �����ص��������� */
    uint8_t                        flags[1];     /**< \brief ������־λ       */
    am_isr_defer_job_t             g_myjob;      /**< \brief �ж���ʱ�������� */  
    uint8_t                        trigger;      /**< \brief ��¼��־λ       */
    am_sensor_val_t                data[1];      /**< \brief �ڲ����ݻ���     */
    /** \brief ����׼�����������ص����� */
    am_sensor_trigger_cb_t         pfn_trigger_cb[1];
    
    /** \brief �豸��Ϣ */
    const am_sensor_bh1730fvc_devinfo_t *dev_info;

} am_sensor_bh1730fvc_dev_t;

/**
 * \brief ������ BH1730FVC ��ʼ��
 *
 * \param[in] p_dev     : ָ�򴫸���BH1730FVC�豸��ָ��
 * \param[in] p_devinfo : ָ�򴫸���BH1730FVC�豸��Ϣ��ָ��
 * \param[in] handle    : IIC��׼������
 *
 * \return ��������׼�������������Ϊ NULL��������ʼ��ʧ��
 */
am_sensor_handle_t am_sensor_bh1730fvc_init (
        am_sensor_bh1730fvc_dev_t           *p_dev,
        const am_sensor_bh1730fvc_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief ������ BH1730FVC ȥ��ʼ��
 *
 * \param[in] handle : am_sensor_bh1730fvc_init()��ʼ��������õĴ�������׼������
 *
 * \return ��
 */
am_err_t am_sensor_bh1730fvc_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_BH1730FVC_H */

/* end of file */
