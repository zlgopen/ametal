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
 * \brief 压力温度传感器 LPS22HB 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 按照设置的频率，在中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-04  yrz, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_lps22hb.h"
#include "am_delay.h"

/** \brief LPS22HB传感器的ID */
const static int __lps22hb_id[2] = {AM_LPS22HB_CHAN_1, AM_LPS22HB_CHAN_2};

/* \breif LPS22HB传感器数据 */
static am_sensor_val_t __lps22hb_data[2];

/**
 * \brief 通道1，湿度的回调函数
 */
static void __pfn_pressure (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__lps22hb_id[0], 1, &__lps22hb_data[0]);
        am_sensor_val_unit_convert(&__lps22hb_data[0], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is pressure, value is: %d Pa!\r\n",
                   __lps22hb_id[0],
                   __lps22hb_data[0].val);
    }
}

/**
 * \brief 通道2，温度的回调函数
 */
static void __pfn_temprature (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__lps22hb_id[1], 1, &__lps22hb_data[1]);
        am_sensor_val_unit_convert(&__lps22hb_data[1], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is temprature, value is: %d.%06d  ℃!\r\n",
                   __lps22hb_id[1],
                   (__lps22hb_data[1].val)/1000000,
                   (__lps22hb_data[1].val)%1000000);
    }
}

/**
 * \brief 例程入口
 */
void demo_std_lps22hb_int_entry (am_sensor_handle_t handle)
{
    /* 设置该传感器的数据输出频率为10Hz（两路通道同步） */
    am_sensor_val_t lps22hb_rate = {10, AM_SENSOR_UNIT_BASE};
    
    am_err_t ret = AM_OK;

    ret = am_sensor_attr_set(handle,
                             __lps22hb_id[0],
                             AM_SENSOR_ATTR_SAMPLING_RATE,
                             &lps22hb_rate);
    if (ret != AM_OK) {
        am_kprintf("sensor_attr_set faild!\r\n");
    }

    /* 使能两路通道 */
    am_sensor_enable(handle, __lps22hb_id, 2, __lps22hb_data);

    /* 设置通道0，1的触发回调函数 */
    am_sensor_trigger_cfg(handle,
                          __lps22hb_id[0],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_pressure,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __lps22hb_id[1],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_temprature,
                          (void*)handle);

    /* 打开通道0，1的触发模式 */
    am_sensor_trigger_on(handle, __lps22hb_id[0]);
    am_sensor_trigger_on(handle, __lps22hb_id[1]);
}

/* end of file */
