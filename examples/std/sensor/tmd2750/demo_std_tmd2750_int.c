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
 * \brief 环境光、接近传感器 TMD2750 例程，通过触发模式实现
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
#include "am_sensor_tmd2750.h"
#include "am_delay.h"

const char *__tmd2750_data_name_string[] = {"proximity", "ALS", "IR"};

/** \brief TMD2750传感器的ID */
const static int __tmd2750_id[2] = {AM_TMD2750_CHAN_1, AM_TMD2750_CHAN_2};

/** \brief TMD2750传感器数据 */
static am_sensor_val_t __tmd2750_data[5];

/**
 * \brief 通道1，Prox的回调函数
 */
static void __pfn_proximity (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_THRESHOLD) {
        am_sensor_data_get(handle, &__tmd2750_id[0], 1, &__tmd2750_data[0]);
        am_kprintf("The channel %d,type is proximity,ADC value is: %d !\r\n",
                   __tmd2750_id[0],
                   __tmd2750_data[0].val);
    }
}

/**
 * \brief 通道2，ALS的回调函数
 */
static void __pfn_als (void *p_arg, uint32_t trigger_src)
{
    uint8_t i = 0;

    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_THRESHOLD) {
        am_sensor_data_get(handle, &__tmd2750_id[1], 1, &__tmd2750_data[1]);
        am_kprintf("The channel %d,type is ALS!\r\n",
                   __tmd2750_id[1]);

        for (i = 1; i < 3; i++) {
            am_kprintf("The %s channel ADC value is: %d !\r\n",
                       __tmd2750_data_name_string[i],
                       __tmd2750_data[i].val);
        }
    }
}

/**
 * \brief 例程入口
 */
void demo_std_tmd2750_int_entry (am_sensor_handle_t handle)
{
    /* 设置低、高阈值 */
    am_sensor_tmd2750_ioctl(handle, AM_SENSOR_TMD2750_LOW_ALS_THRESHOLD_SET, (void *)200);
    am_sensor_tmd2750_ioctl(handle, AM_SENSOR_TMD2750_HIGH_ALS_THRESHOLD_SET, (void *)500);
    am_sensor_tmd2750_ioctl(handle, AM_SENSOR_TMD2750_LOW_PROX_THRESHOLD_SET, (void *)200);
    am_sensor_tmd2750_ioctl(handle, AM_SENSOR_TMD2750_HIGH_PROX_THRESHOLD_SET, (void *)500);

    /* 使能两路通道 */
    am_sensor_enable(handle, __tmd2750_id, 2, __tmd2750_data);

    /* 设置通道0，1的触发回调函数 */
    am_sensor_trigger_cfg(handle,
                          __tmd2750_id[0],
                          AM_SENSOR_TRIGGER_THRESHOLD,
                          __pfn_proximity,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __tmd2750_id[1],
                          AM_SENSOR_TRIGGER_THRESHOLD,
                          __pfn_als,
                          (void*)handle);

    /* 打开通道0，1的触发模式 */
    am_sensor_trigger_on(handle, __tmd2750_id[0]);
    am_sensor_trigger_on(handle, __tmd2750_id[1]);

    while (1) {
    
    }
}

/* end of file */
