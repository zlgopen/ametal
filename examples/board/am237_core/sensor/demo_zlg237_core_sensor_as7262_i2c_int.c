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
 * \brief 光谱传感器 AS7262 (I2C模式) 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 在中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

#include "am_sensor.h"
#include "am_hwconf_sensor_as7262_i2c.h"
#include "demo_std_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 传感器例程入口
 */
void demo_zlg237_core_sensor_as7262_i2c_int_entry (void)
{
    am_sensor_handle_t handle = am_sensor_as7262_i2c_inst_init();

    demo_std_as7262_int_entry(handle);
}


/* end of file */
