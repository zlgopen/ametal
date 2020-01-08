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
 * \brief 气压温度传感器 MPL3115A2 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 按照设置的频率，在中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-11  fzb, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_mpl3115a2.h"
#include "am_delay.h"

/** \brief MPL3115A2传感器的ID */
const static int __mpl3115a2_id[2] = {AM_MPL3115A2_CHAN_1, AM_MPL3115A2_CHAN_2};

/* \breif MPL3115A2传感器数据 */
static am_sensor_val_t __mpl3115a2_data[2];

/**
 * \brief 通道1，气压/海拔的回调函数
 */
static void __pfn_pressure_or_altitude (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__mpl3115a2_id[0], 1, &__mpl3115a2_data[0]);
        am_sensor_val_unit_convert(&__mpl3115a2_data[0], 1, AM_SENSOR_UNIT_MICRO);

#if(__MPL3115A2_MODE == __MPL3115A2_PRE_MODE) /* 气压 */
        am_kprintf("The channel %d,type is pressure, value is: %d.%06d KPa!\r\n",
                   __mpl3115a2_id[0],
                   (__mpl3115a2_data[0].val/1000000),
                   (__mpl3115a2_data[0].val%1000000));
#elif(__MPL3115A2_MODE == __MPL3115A2_ALT_MODE) /* 海拔 */
        am_kprintf("The channel %d,type is altitude, value is: %d.%04d Meters!\r\n",
                   __mpl3115a2_id[0],
                   (__mpl3115a2_data[0].val/1000000),
                   (__mpl3115a2_data[0].val%1000000));
#endif
    }
}

/**
 * \brief 通道2，温度的回调函数
 */
static void __pfn_temprature (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__mpl3115a2_id[1], 1, &__mpl3115a2_data[1]);
        am_sensor_val_unit_convert(&__mpl3115a2_data[1], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is temprature, value is: %d.%06d  ℃!\r\n",
                   __mpl3115a2_id[1],
                   (__mpl3115a2_data[1].val)/1000000,
                   (__mpl3115a2_data[1].val)%1000000);
        am_kprintf("\r\n");
    }
}

/**
 * \brief 例程入口
 */
void demo_std_mpl3115a2_int_entry (am_sensor_handle_t handle)
{
    /* 设置该传感器的采样时间为：2的0次方 秒 */
    am_sensor_val_t mpl3115a2_rate = {0, AM_SENSOR_UNIT_BASE};
    
    am_err_t ret = AM_OK;

    ret = am_sensor_attr_set(handle,
                             __mpl3115a2_id[0],
                             AM_SENSOR_ATTR_SAMPLING_RATE,
                             &mpl3115a2_rate);
    if (ret != AM_OK) {
        am_kprintf("sensor_attr_set faild!\r\n");
    }

    /* 使能两路通道 */
    am_sensor_enable(handle, __mpl3115a2_id, 2, __mpl3115a2_data);

    /* 设置通道0，1的触发回调函数 */
    am_sensor_trigger_cfg(handle,
                          __mpl3115a2_id[0],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_pressure_or_altitude,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __mpl3115a2_id[1],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_temprature,
                          (void*)handle);

    /* 打开通道0，1的触发模式 */
    am_sensor_trigger_on(handle, __mpl3115a2_id[0]);
    am_sensor_trigger_on(handle, __mpl3115a2_id[1]);
}

/* end of file */
