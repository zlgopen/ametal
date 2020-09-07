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
 * \brief 温度传感器 AS621X 中断模式例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 中断模式下温度超过阈值后报警输出，在中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-21  fzb, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_as621x.h"
#include "am_delay.h"

/** \brief AS621X 传感器的ID */
const static int __as621x_id[1] = {AM_AS621X_CHAN_1};

/** \breif AS621X传感器数据 */
static am_sensor_val_t __as621x_data[1];

/**
 * \brief 通道1，温度报警的回调函数
 */
static void __pfn_temprature (void *p_arg, uint32_t trigger_src)
{
    float low_temp = 0,high_temp = 0;
    int   m_low_temp = 0,m_high_temp = 0;/* m为放大10^6的意思 */

    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;
    if (trigger_src & AM_SENSOR_TRIGGER_THRESHOLD) {
        am_sensor_data_get(handle, &__as621x_id[0], 1, &__as621x_data[0]);
        am_sensor_val_unit_convert(&__as621x_data[0], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is temprature alarm, value is: %d.%06d  ℃!\r\n",
                   __as621x_id[0],
                   (__as621x_data[0].val)/1000000,
                   (__as621x_data[0].val)%1000000);

        am_sensor_as621x_ioctl(handle, AM_SENSOR_AS621X_TLOW_THRESHOLD_GET, &low_temp);
        am_sensor_as621x_ioctl(handle, AM_SENSOR_AS621X_THIGH_THRESHOLD_GET, &high_temp);
        m_low_temp = (int)(low_temp * 1000000);
        m_high_temp = (int)(high_temp * 1000000);
        if (__as621x_data[0].val >= m_high_temp) {
            am_kprintf("The temperature exceeds THIGH!\r\n");
        } else if (__as621x_data[0].val <= m_low_temp) {
            am_kprintf("The temperature falls below TLOW!\r\n");
        }
    }
}

/**
 * \brief 例程入口
 */
void demo_std_as621x_interrupt_mode_entry (am_sensor_handle_t handle)
{
    am_err_t ret = AM_OK;

    /* 设置该传感器的数据输出频率为1Hz */
    ret = am_sensor_as621x_ioctl(handle,
                                 AM_SENSOR_AS621X_CR_SET,
                                (void *)AM_AS621X_CONVERSION_RATE_1HZ);

    if (ret != AM_OK) {
        am_kprintf("sensor_attr_set faild!\r\n");
    }

    /* 使能一路通道 */
    am_sensor_enable(handle, __as621x_id, 1, __as621x_data);

    /* 设置通道1的触发回调函数 */
    am_sensor_trigger_cfg(handle,
                          __as621x_id[0],
                          AM_SENSOR_TRIGGER_THRESHOLD,
                          __pfn_temprature,
                          (void*)handle);

    /* 打开通道1的触发模式 */
    am_sensor_trigger_on(handle, __as621x_id[0]);

}

/* end of file */
