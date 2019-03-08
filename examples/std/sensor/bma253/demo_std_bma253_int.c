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
 * \brief ������ٶȴ����� BMA253 ���̣�ͨ������ģʽʵ��
 *
 * - ʵ������
 *   1. �������ݸ����ٶȣ����ж��л�ȡ���ݣ���ͨ�����ڴ�ӡ
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-20  yrz, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_bma253.h"
#include "am_delay.h"

/** \brief BMA253��������ID */
const static int __bma253_id[4] = {AM_BMA253_CHAN_1, AM_BMA253_CHAN_2,
                                   AM_BMA253_CHAN_3, AM_BMA253_CHAN_4};

/* \breif BMA253���������� */
static am_sensor_val_t __bma253_data[4];

/**
 * \brief ͨ��1��X����ٶȵĻص�����
 */
static void __pfn_accel_x (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__bma253_id[0], 1, &__bma253_data[0]);
        am_sensor_val_unit_convert(&__bma253_data[0], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is accel X, value is: %d.%06d  m/s^2!\r\n",
                   __bma253_id[0],
                   (__bma253_data[0].val)/1000000,
                   (uint32_t)(__bma253_data[0].val)%1000000);
    }
}

/**
 * \brief ͨ��2��Y����ٶȵĻص�����
 */
static void __pfn_accel_y (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__bma253_id[1], 1, &__bma253_data[1]);
        am_sensor_val_unit_convert(&__bma253_data[1], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is accel Y, value is: %d.%06d  m/s^2!\r\n",
                   __bma253_id[1],
                   (__bma253_data[1].val)/1000000,
                   (uint32_t)(__bma253_data[1].val)%1000000);
    }
}

/**
 * \brief ͨ��3��Z����ٶȵĻص�����
 */
static void __pfn_accel_z (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__bma253_id[2], 1, &__bma253_data[2]);
        am_sensor_val_unit_convert(&__bma253_data[2], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is accel Z, value is: %d.%06d  m/s^2!\r\n",
                   __bma253_id[2],
                   (__bma253_data[2].val)/1000000,
                   (uint32_t)(__bma253_data[2].val)%1000000);
    }
}

/**
 * \brief ͨ��4���¶ȵĻص�����
 */
static void __pfn_temprature (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__bma253_id[3], 1, &__bma253_data[3]);
        am_sensor_val_unit_convert(&__bma253_data[3], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is temprature, value is: %d.%06d  ��!\r\n",
                   __bma253_id[3],
                   (__bma253_data[3].val)/1000000,
                   (uint32_t)(__bma253_data[3].val)%1000000);
    }
}

/**
 * \brief �������
 */
void demo_std_bma253_int_entry (am_sensor_handle_t handle)
{
    /* ���øô�����������Ϊ��2G����ͨ��ͬ���� */
    am_sensor_val_t bma253_range = {2, AM_SENSOR_UNIT_BASE};
    
    am_err_t ret = AM_OK;

    ret = am_sensor_attr_set(handle,
                             __bma253_id[0],
                             AM_SENSOR_ATTR_FULL_SCALE,
                             &bma253_range);
    if (ret != AM_OK) {
        am_kprintf("sensor_attr_set faild!\r\n");
    }

    /* ʹ����·ͨ�� */
    am_sensor_enable(handle, __bma253_id, 4, __bma253_data);

    /* ����ͨ��0,1,2,3�Ĵ����ص����� */
    am_sensor_trigger_cfg(handle,
                          __bma253_id[0],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_accel_x,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __bma253_id[1],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_accel_y,
                          (void*)handle);    
    am_sensor_trigger_cfg(handle,
                          __bma253_id[2],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_accel_z,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __bma253_id[3],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_temprature,
                          (void*)handle);

    /* ��ͨ��0,1,2,3�Ĵ���ģʽ */
    am_sensor_trigger_on(handle, __bma253_id[0]);
    am_sensor_trigger_on(handle, __bma253_id[1]);
    am_sensor_trigger_on(handle, __bma253_id[2]);
    am_sensor_trigger_on(handle, __bma253_id[3]);
}

/* end of file */
