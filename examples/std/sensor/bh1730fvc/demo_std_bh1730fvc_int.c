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
 * \brief 光传感器 BH1730FVC 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 按照数据更新速率，在中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-20  yrz, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_bh1730fvc.h"
#include "am_delay.h"

/** \brief BH1730FVC传感器的ID */
const static int __bh1730fvc_id[1] = {AM_BH1730FVC_CHAN_1};

/** \breif BH1730FVC传感器数据 */
static am_sensor_val_t __bh1730fvc_data[1];

/**
 * \brief 通道1，光强的回调函数
 */
static void __pfn_light (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;

    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__bh1730fvc_id[0], 1, &__bh1730fvc_data[0]);
        am_sensor_val_unit_convert(&__bh1730fvc_data[0], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is light, value is: %d.%06d Lux!\r\n",
                   __bh1730fvc_id[0],
                   (__bh1730fvc_data[0].val)/1000000,
                   (__bh1730fvc_data[0].val)%1000000);
    }
}

/**
 * \brief 例程入口
 */
void demo_std_bh1730fvc_int_entry (am_sensor_handle_t handle)
{
    /* 使能两路通道 */
    am_sensor_enable(handle, __bh1730fvc_id, 1, __bh1730fvc_data);

    /* 设置通道0的触发回调函数 */
    am_sensor_trigger_cfg(handle,
                          __bh1730fvc_id[0],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_light,
                          (void*)handle);

    /* 打开通道0的触发模式 */
    am_sensor_trigger_on(handle, __bh1730fvc_id[0]);
}

/* end of file */
