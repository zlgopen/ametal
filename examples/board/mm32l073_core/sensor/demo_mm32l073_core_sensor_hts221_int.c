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
 * \brief 温湿度传感器 HTS221 例程，通过触发模式实现
 *
 * - 实验现象：
 *   1. 按照设置的频率，在中断中获取数据，并通过串口打印
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-27  wan, first implementation
 * \endinternal
 */

#include "am_sensor.h"
#include "am_hwconf_sensor_hts221.h"
#include "demo_std_entries.h"
#include "demo_am116_core_entries.h"

/**
 * \brief 传感器例程入口
 */
void demo_am116_core_sensor_hts221_int_entry (void)
{
    am_sensor_handle_t handle = am_sensor_hts221_inst_init();

    demo_std_hts221_int_entry(handle);
}


/* end of file */
