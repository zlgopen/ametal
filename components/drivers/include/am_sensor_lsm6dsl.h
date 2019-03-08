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
 * \brief ������ٶȴ����� LSM6DSL ͷ�ļ�
 *
 * \internal
 * \par modification history
 * - 1.00 19-02-20  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_LSM6DSL_H
#define __AM_SENSOR_LSM6DSL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief ������LSM6DSL�ṩ������ͨ��ID����
 */
#define AM_LSM6DSL_CHAN_1      0           /* LSM6DSL ��ͨ��1��X����ٶ�      */
#define AM_LSM6DSL_CHAN_2      1           /* LSM6DSL ��ͨ��2��Y����ٶ�      */
#define AM_LSM6DSL_CHAN_3      2           /* LSM6DSL ��ͨ��3��Z����ٶ�      */
#define AM_LSM6DSL_CHAN_4      3           /* LSM6DSL ��ͨ��4���¶�           */
    
/**
 * \brief ������ LSM6DSL �豸��Ϣ�ṹ��
 */
typedef struct am_sensor_lsm6dsl_devinfo {
    
    /**
     * \brief ����׼��������������
     */
    int     trigger_pin;
    
    /**
     * \brief I2C 7λ �豸��ַ
     *
     * ���豸7λ��ַΪ 0x6B
     */
    uint8_t i2c_addr;

} am_sensor_lsm6dsl_devinfo_t;

/**
 * \breif ������ LSM6DSL �豸�ṹ�嶨��
 */
typedef struct am_sensor_lsm6dsl_dev {
    am_sensor_serv_t               lsm6dsl_dev;  /**< \breif ��������׼����   */
    am_i2c_device_t                i2c_dev;      /**< \brief i2c�豸ʵ��      */
    void                          *p_arg[4];     /**< \brief �����ص��������� */
    uint8_t                        flags[4];     /**< \brief ������־λ       */
    am_isr_defer_job_t             g_myjob;      /**< \brief �ж���ʱ�������� */ 
    uint16_t                       trigger;      /**< \brief ��¼��־λ       */
    am_sensor_val_t                data[4];      /**< \brief �ڲ����ݻ���     */
    am_sensor_val_t                sam_range;    /**< \brief ������Χ         */
    /** \brief ����׼�����������ص����� */
    am_sensor_trigger_cb_t         pfn_trigger_cb[4];
    
    /** \brief �豸��Ϣ */
    const am_sensor_lsm6dsl_devinfo_t *dev_info;

} am_sensor_lsm6dsl_dev_t;

/**
 * \brief ������ LSM6DSL ��ʼ��
 *
 * \param[in] p_dev     : ָ�򴫸���LSM6DSL�豸��ָ��
 * \param[in] p_devinfo : ָ�򴫸���LSM6DSL�豸��Ϣ��ָ��
 * \param[in] handle    : IIC��׼������
 *
 * \return ��������׼�������������Ϊ NULL��������ʼ��ʧ��
 */
am_sensor_handle_t am_sensor_lsm6dsl_init (
        am_sensor_lsm6dsl_dev_t           *p_dev,
        const am_sensor_lsm6dsl_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief ������ LSM6DSL ȥ��ʼ��
 *
 * \param[in] handle : am_sensor_lsm6dsl_init()��ʼ��������õĴ�������׼������
 *
 * \return ��
 */
am_err_t am_sensor_lsm6dsl_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_LSM6DSL_H */

/* end of file */
