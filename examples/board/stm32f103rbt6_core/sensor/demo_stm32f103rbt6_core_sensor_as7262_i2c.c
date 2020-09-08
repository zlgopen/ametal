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
 * \brief 光谱传感器 AS7262 (I2C模式) 例程，通过标准接口实现(通过查询方式获取数据)
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印光谱通道 (V、B、G、Y、O、R) AD值
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_core_sensor_as7262_i2c.c src_stm32f103rbt6_core_sensor_as7262_i2c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

#include "am_sensor.h"
#include "am_hwconf_sensor_as7262_i2c.h"
#include "demo_std_entries.h"
#include "demo_stm32f103rbt6_core_entries.h"

/**
 * \brief 传感器例程入口
 */
void demo_stm32f103rbt6_core_sensor_as7262_i2c_entry (void)
{
    am_sensor_handle_t handle = am_sensor_as7262_i2c_inst_init();

    demo_std_as7262_entry(handle);
}


/* end of file */
