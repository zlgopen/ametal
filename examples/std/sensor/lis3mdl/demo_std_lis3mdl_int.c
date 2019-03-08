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
 * \brief 三轴磁传感器 LIS3MDL 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 根据数据更新速度，在中断中获取数据，并通过串口打印
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

/** \brief LIS3MDL传感器的ID */
const static int __lis3mdl_id[4] = {AM_LIS3MDL_CHAN_1, AM_LIS3MDL_CHAN_2,
                                    AM_LIS3MDL_CHAN_3, AM_LIS3MDL_CHAN_4};

/* \breif LIS3MDL传感器数据 */
static am_sensor_val_t __lis3mdl_data[4];

/**
 * \brief 通道1，X轴磁的回调函数
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
 * \brief 通道2，Y轴磁的回调函数
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
 * \brief 通道3，Z轴磁的回调函数
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
 * \brief 通道4，温度的回调函数
 */
static void __pfn_temprature (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__lis3mdl_id[3], 1, &__lis3mdl_data[3]);
        am_sensor_val_unit_convert(&__lis3mdl_data[3], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is temprature, value is: %d.%06d  ℃!\r\n",
                   __lis3mdl_id[3],
                   (__lis3mdl_data[3].val)/1000000,
                   (uint32_t)(__lis3mdl_data[3].val)%1000000);
    }
}

/**
 * \brief 例程入口
 */
void demo_std_lis3mdl_int_entry (am_sensor_handle_t handle)
{
    /* 设置该传感器的量程为±4gauss（三通道同步） */
    am_sensor_val_t lis3mdl_range = {4, AM_SENSOR_UNIT_BASE};
    
    am_err_t ret = AM_OK;

    ret = am_sensor_attr_set(handle,
                             __lis3mdl_id[0],
                             AM_SENSOR_ATTR_FULL_SCALE,
                             &lis3mdl_range);
    if (ret != AM_OK) {
        am_kprintf("sensor_attr_set faild!\r\n");
    }

    /* 使能四路通道 */
    am_sensor_enable(handle, __lis3mdl_id, 4, __lis3mdl_data);

    /* 设置通道0,1,2,3的触发回调函数 */
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

    /* 打开通道0,1,2,3的触发模式 */
    am_sensor_trigger_on(handle, __lis3mdl_id[0]);
    am_sensor_trigger_on(handle, __lis3mdl_id[1]);
    am_sensor_trigger_on(handle, __lis3mdl_id[2]);
    am_sensor_trigger_on(handle, __lis3mdl_id[3]);
}

/* end of file */
