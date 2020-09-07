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
 * \brief 高光谱传感器 AS7421 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 在中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_as7421.h"
#include "am_delay.h"

const char *__as7421_data_name_string[] = {"TEMP0", "TEMP1", "TEMP2", "TEMP3"};

/** \brief AS7421传感器的ID */
const static int __as7421_id[2] = {AM_AS7421_CHAN_1, AM_AS7421_CHAN_2};

/** \brief AS7421传感器数据 */
static am_sensor_val_t __as7421_data[68];

/**
 * \brief 通道1，温度的回调函数
 */
static void __pfn_temprature (void *p_arg, uint32_t trigger_src)
{
    uint8_t i = 0;

    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__as7421_id[0], 1, &__as7421_data[0]);
        am_kprintf("The channel %d,type is temprature!\r\n",
                   __as7421_id[0]);

        for (i = 0; i < 4; i++) {
            am_kprintf("The %s channel ADC value is: %d !\r\n",
                       __as7421_data_name_string[i],
                       __as7421_data[i].val);
        }
    }
}

/**
 * \brief 通道2，SPECTRAL的回调函数
 */
static void __pfn_spectral (void *p_arg, uint32_t trigger_src)
{
    uint8_t i = 0;

    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__as7421_id[1], 1, &__as7421_data[4]);
        am_kprintf("The channel %d,type is SPECTRAL!\r\n",
                   __as7421_id[1]);

        for (i = 4; i < 68; i++) {
            am_kprintf("The CH[%d] ADC value is: %d !\r\n",
                       i,
                       __as7421_data[i].val);
        }
    }
}

/**
 * \brief 例程入口
 */
void demo_std_as7421_int_entry (am_sensor_handle_t handle)
{
    /* 使能两路通道 */
    am_sensor_enable(handle, __as7421_id, 2, __as7421_data);

    /* 设置通道0，1的触发回调函数 */
    am_sensor_trigger_cfg(handle,
                          __as7421_id[0],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_temprature,
                          (void*)handle);
    am_sensor_trigger_cfg(handle,
                          __as7421_id[1],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_spectral,
                          (void*)handle);

    /* 打开通道0，1的触发模式 */
    am_sensor_trigger_on(handle, __as7421_id[0]);
    am_sensor_trigger_on(handle, __as7421_id[1]);

    while (1) {

    }
}

/* end of file */
