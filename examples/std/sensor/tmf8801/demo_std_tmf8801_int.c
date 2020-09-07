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
 * \brief 飞行时间传感器 TMF8801 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 按照设置的频率，在中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yrz, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_tmf8801.h"
#include "am_delay.h"

/** \brief TMF8801传感器的ID */
const static int __tmf8801_id[2] = {0, 1};

/* \breif TMF8801传感器数据 */
static am_sensor_val_t __tmf8801_data[2];

/**
 * \brief 通道1，距离的回调函数
 */
static void __pfn_distance (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__tmf8801_id[0], 1, &__tmf8801_data[0]);
        am_kprintf("The channel %d,type is distance, value is: %d mm!\r\n",
                   __tmf8801_id[0],
                   __tmf8801_data[0]);
    }
}

/**
 * \brief 通道2，温度的回调函数
 */
static void __pfn_temprature (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__tmf8801_id[1], 1, &__tmf8801_data[1]);
        am_kprintf("The channel %d,type is temprature, value is: %d ℃!\r\n",
                   __tmf8801_id[1],
                   __tmf8801_data[1].val);
    }
}

/**
 * \brief 例程入口
 */
void demo_std_tmf8801_int_entry (am_sensor_handle_t handle)
{
    /* 使能两路通道 */
    am_sensor_enable(handle, __tmf8801_id, 2, __tmf8801_data);

    /* 设置通道0，1的触发回调函数 */
    am_sensor_trigger_cfg(handle,
                          __tmf8801_id[0],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_distance,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __tmf8801_id[1],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_temprature,
                          (void*)handle);

    /* 打开通道0，1的触发模式 */
    am_sensor_trigger_on(handle, __tmf8801_id[0]);
    am_sensor_trigger_on(handle, __tmf8801_id[1]);

    while (1) {
    
    }
}

/* end of file */
