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
 * \brief ������������� LIS3MDL ͷ�ļ�
 *
 * \internal
 * \par modification history
 * - 1.00 19-02-22  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_LIS3MDL_H
#define __AM_SENSOR_LIS3MDL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief ������LIS3MDL�ṩ������ͨ��ID����
 */
#define AM_LIS3MDL_CHAN_1      0           /* LIS3MDL ��ͨ��1��X�����        */
#define AM_LIS3MDL_CHAN_2      1           /* LIS3MDL ��ͨ��2��Y�����        */
#define AM_LIS3MDL_CHAN_3      2           /* LIS3MDL ��ͨ��3��Z�����        */
#define AM_LIS3MDL_CHAN_4      3           /* LIS3MDL ��ͨ��4���¶�           */
    
/**
 * \brief ������ LIS3MDL �豸��Ϣ�ṹ��
 */
typedef struct am_sensor_lis3mdl_devinfo {
    
    /**
     * \brief ����׼��������������
     */
    int     trigger_pin;
    
    /**
     * \brief I2C 7λ �豸��ַ
     *
     * ���豸7λ��ַΪ 0x1C
     */
    uint8_t i2c_addr;

} am_sensor_lis3mdl_devinfo_t;

/**
 * \breif ������ LIS3MDL �豸�ṹ�嶨��
 */
typedef struct am_sensor_lis3mdl_dev {
    am_sensor_serv_t               lis3mdl_dev;  /**< \breif ��������׼����   */
    am_i2c_device_t                i2c_dev;      /**< \brief i2c�豸ʵ��      */
    void                          *p_arg[4];     /**< \brief �����ص��������� */
    uint8_t                        flags[4];     /**< \brief ������־λ       */
    am_isr_defer_job_t             g_myjob;      /**< \brief �ж���ʱ�������� */ 
    uint16_t                       trigger;      /**< \brief ��¼��־λ       */
    am_sensor_val_t                data[4];      /**< \brief �ڲ����ݻ���     */
    am_sensor_val_t                sam_range;    /**< \brief ��������         */
    /** \brief ����׼�����������ص����� */
    am_sensor_trigger_cb_t         pfn_trigger_cb[4];
    
    /** \brief �豸��Ϣ */
    const am_sensor_lis3mdl_devinfo_t *dev_info;

} am_sensor_lis3mdl_dev_t;

/**
 * \brief ������ LIS3MDL ��ʼ��
 *
 * \param[in] p_dev     : ָ�򴫸���LIS3MDL�豸��ָ��
 * \param[in] p_devinfo : ָ�򴫸���LIS3MDL�豸��Ϣ��ָ��
 * \param[in] handle    : IIC��׼������
 *
 * \return ��������׼�������������Ϊ NULL��������ʼ��ʧ��
 */
am_sensor_handle_t am_sensor_lis3mdl_init (
        am_sensor_lis3mdl_dev_t           *p_dev,
        const am_sensor_lis3mdl_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief ������ LIS3MDL ȥ��ʼ��
 *
 * \param[in] handle : am_sensor_lis3mdl_init()��ʼ��������õĴ�������׼������
 *
 * \return ��
 */
am_err_t am_sensor_lis3mdl_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_LIS3MDL_H */

/* end of file */
