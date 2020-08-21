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
 * \brief 环境光/颜色传感器 TCS3430 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 在门限触发中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_tcs3430.h"
#include "am_delay.h"

/** \brief TCS3430传感器的ID */
const static int __tcs3430_id[4] = {AM_TCS3430_CHAN_1, AM_TCS3430_CHAN_2,
                                    AM_TCS3430_CHAN_3, AM_TCS3430_CHAN_4};

/** \brief TCS3430传感器数据 */
static am_sensor_val_t __tcs3430_data[4];

/**
 * \brief 通道1，CH0的回调函数
 */
static void __pfn_ch0 (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_THRESHOLD) {
        am_sensor_data_get(handle, &__tcs3430_id[0], 1, &__tcs3430_data[0]);
        am_kprintf("The channel %d,type is CH0, value is: %d !\r\n",
                   __tcs3430_id[0],
                   __tcs3430_data[0].val);
    }
}

/**
 * \brief 通道2，CH1的回调函数
 */
static void __pfn_ch1 (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_THRESHOLD) {
        am_sensor_data_get(handle, &__tcs3430_id[1], 1, &__tcs3430_data[1]);
        am_kprintf("The channel %d,type is CH1, value is: %d !\r\n",
                   __tcs3430_id[1],
                   __tcs3430_data[1].val);
    }
}

/**
 * \brief 通道3，CH2的回调函数
 */
static void __pfn_ch2 (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_THRESHOLD) {
        am_sensor_data_get(handle, &__tcs3430_id[2], 1, &__tcs3430_data[2]);
        am_kprintf("The channel %d,type is CH2, value is: %d !\r\n",
                   __tcs3430_id[2],
                   __tcs3430_data[2].val);
    }
}

/**
 * \brief 通道4，CH3的回调函数
 */
static void __pfn_ch3 (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_THRESHOLD) {
        am_sensor_data_get(handle, &__tcs3430_id[3], 1, &__tcs3430_data[3]);
        am_kprintf("The channel %d,type is CH3, value is: %d !\r\n",
                   __tcs3430_id[3],
                   __tcs3430_data[3].val);
    }
}

/**
 * \brief 例程入口
 */
void demo_std_tcs3430_int_entry (am_sensor_handle_t handle)
{
    /* 设置低、高阈值 */
    am_sensor_tcs3430_ioctl(handle, AM_SENSOR_TCS3430_LOW_ALS_THRESHOLD_SET, (void *)300);
    am_sensor_tcs3430_ioctl(handle, AM_SENSOR_TCS3430_HIGH_ALS_THRESHOLD_SET, (void *)2000);

    /* 使能四路通道 */
    am_sensor_enable(handle, __tcs3430_id, 4, __tcs3430_data);

    /* 设置通道0，1，2，3的触发回调函数 */
    am_sensor_trigger_cfg(handle,
                          __tcs3430_id[0],
                          AM_SENSOR_TRIGGER_THRESHOLD,
                          __pfn_ch0,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __tcs3430_id[1],
                          AM_SENSOR_TRIGGER_THRESHOLD,
                          __pfn_ch1,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __tcs3430_id[2],
                          AM_SENSOR_TRIGGER_THRESHOLD,
                          __pfn_ch2,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __tcs3430_id[3],
                          AM_SENSOR_TRIGGER_THRESHOLD,
                          __pfn_ch3,
                          (void*)handle);

    /* 打开通道0，1，2，3的触发模式 */
    am_sensor_trigger_on(handle, __tcs3430_id[0]);
    am_sensor_trigger_on(handle, __tcs3430_id[1]);
    am_sensor_trigger_on(handle, __tcs3430_id[2]);
    am_sensor_trigger_on(handle, __tcs3430_id[3]);

    while (1) {
    
    }
}

/* end of file */
