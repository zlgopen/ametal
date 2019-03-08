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
 * \brief ����Ŵ����� LIS3MDL ���̣�ͨ������ģʽʵ��
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
#include "am_sensor_lis3mdl.h"
#include "am_delay.h"

/** \brief LIS3MDL��������ID */
const static int __lis3mdl_id[4] = {AM_LIS3MDL_CHAN_1, AM_LIS3MDL_CHAN_2,
                                    AM_LIS3MDL_CHAN_3, AM_LIS3MDL_CHAN_4};

/* \breif LIS3MDL���������� */
static am_sensor_val_t __lis3mdl_data[4];

/**
 * \brief ͨ��1��X��ŵĻص�����
 */
static void __pfn_mag_x (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg; 
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__lis3mdl_id[0], 1, &__lis3mdl_data[0]);
        am_sensor_val_unit_convert(&__lis3mdl_data[0], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is mag X, value is: %d.%06d  gauss!\r\n",
                   __lis3mdl_id[0],
                   (__lis3mdl_data[0].val)/1000000,
                   (uint32_t)(__lis3mdl_data[0].val)%1000000);
    }
}

/**
 * \brief ͨ��2��Y��ŵĻص�����
 */
static void __pfn_mag_y (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__lis3mdl_id[1], 1, &__lis3mdl_data[1]);
        am_sensor_val_unit_convert(&__lis3mdl_data[1], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is mag Y, value is: %d.%06d  gauss!\r\n",
                   __lis3mdl_id[1],
                   (__lis3mdl_data[1].val/1000000),
                   (uint32_t)(__lis3mdl_data[1].val)%1000000);
    }
}

/**
 * \brief ͨ��3��Z��ŵĻص�����
 */
static void __pfn_mag_z (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__lis3mdl_id[2], 1, &__lis3mdl_data[2]);
        am_sensor_val_unit_convert(&__lis3mdl_data[2], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is mag Z, value is: %d.%06d  gauss!\r\n",
                   __lis3mdl_id[2],
                   (__lis3mdl_data[2].val)/1000000,
                   (uint32_t)(__lis3mdl_data[2].val)%1000000);
    }
}

/**
 * \brief ͨ��4���¶ȵĻص�����
 */
static void __pfn_temprature (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__lis3mdl_id[3], 1, &__lis3mdl_data[3]);
        am_sensor_val_unit_convert(&__lis3mdl_data[3], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is temprature, value is: %d.%06d  ��!\r\n",
                   __lis3mdl_id[3],
                   (__lis3mdl_data[3].val)/1000000,
                   (uint32_t)(__lis3mdl_data[3].val)%1000000);
    }
}

/**
 * \brief �������
 */
void demo_std_lis3mdl_int_entry (am_sensor_handle_t handle)
{
    /* ���øô�����������Ϊ��4gauss����ͨ��ͬ���� */
    am_sensor_val_t lis3mdl_range = {4, AM_SENSOR_UNIT_BASE};
    
    am_err_t ret = AM_OK;

    ret = am_sensor_attr_set(handle,
                             __lis3mdl_id[0],
                             AM_SENSOR_ATTR_FULL_SCALE,
                             &lis3mdl_range);
    if (ret != AM_OK) {
        am_kprintf("sensor_attr_set faild!\r\n");
    }

    /* ʹ����·ͨ�� */
    am_sensor_enable(handle, __lis3mdl_id, 4, __lis3mdl_data);

    /* ����ͨ��0,1,2,3�Ĵ����ص����� */
    am_sensor_trigger_cfg(handle,
                          __lis3mdl_id[0],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_mag_x,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __lis3mdl_id[1],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_mag_y,
                          (void*)handle);    
    am_sensor_trigger_cfg(handle,
                          __lis3mdl_id[2],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_mag_z,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __lis3mdl_id[3],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_temprature,
                          (void*)handle);

    /* ��ͨ��0,1,2,3�Ĵ���ģʽ */
    am_sensor_trigger_on(handle, __lis3mdl_id[0]);
    am_sensor_trigger_on(handle, __lis3mdl_id[1]);
    am_sensor_trigger_on(handle, __lis3mdl_id[2]);
    am_sensor_trigger_on(handle, __lis3mdl_id[3]);
}

/* end of file */
