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
 * \brief ��ʪ�ȴ����� SHTC1 ͷ�ļ�
 *
 * \internal
 * \par modification history
 * - 1.00 19-02-20  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_SHTC1_H
#define __AM_SENSOR_SHTC1_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/*
 * \brief ������SHTC1�ṩ������ͨ��ID����
 */
#define AM_SHTC1_CHAN_1      0             /* SHTC1 ��ͨ��1��ʪ�ȴ�����ͨ�� */
#define AM_SHTC1_CHAN_2      1             /* SHTC1 ��ͨ��2���¶ȴ�����ͨ�� */

/**
 * \brief ������ SHTC1 �豸��Ϣ�ṹ��
 */
typedef struct am_sensor_shtc1_devinfo {

    /*
     * \brief I2C 7λ �豸��ַ
     *
     * ���豸7λ��ַ�̶�Ϊ 0x70
     */
    uint8_t i2c_addr;

} am_sensor_shtc1_devinfo_t;

/**
 * \breif ������ SHTC1 �豸�ṹ�嶨��
 */
typedef struct am_sensor_shtc1_dev {
    am_sensor_serv_t                  shtc1_dev;  /*< \breif ��������׼����   */
    am_i2c_device_t                   i2c_dev;    /*< \brief i2c�豸ʵ��      */
    uint8_t                           trigger;    /*< \brief ��¼��־λ       */
    am_sensor_val_t                   data[2];    /*< \brief �ڲ����ݻ���     */

    /** \brief �豸��Ϣ */
    const am_sensor_shtc1_devinfo_t *dev_info;

} am_sensor_shtc1_dev_t;

/**
 * \brief ������ SHTC1 ��ʼ��
 *
 * \param[in] p_dev     : ָ�򴫸���SHTC1�豸��ָ��
 * \param[in] p_devinfo : ָ�򴫸���SHTC1�豸��Ϣ��ָ��
 * \param[in] handle    : IIC��׼������
 *
 * \return ��������׼�������������Ϊ NULL��������ʼ��ʧ��
 */
am_sensor_handle_t am_sensor_shtc1_init (
        am_sensor_shtc1_dev_t            *p_dev,
        const am_sensor_shtc1_devinfo_t  *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief ������ SHTC1 ȥ��ʼ��
 *
 * \param[in] handle : am_sensor_shtc1_init()��ʼ��������õĴ�������׼������
 *
 * \return ��
 */
am_err_t am_sensor_shtc1_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_SHTC1_H */

/* end of file */
