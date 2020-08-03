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
 * \brief 角度传感器 AS5600 例程，通过标准接口实现(通过查询方式获取数据)
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印角度AD值
 *
 * \par 源代码
 * \snippet demo_zlg237_core_sensor_as5600.c src_zlg237_core_sensor_as5600
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

#include "am_sensor.h"
#include "am_hwconf_sensor_as5600.h"
#include "demo_std_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 传感器例程入口
 */
void demo_zlg237_core_sensor_as5600_entry (void)
{
    am_sensor_handle_t handle = am_sensor_as5600_inst_init();

    demo_std_as5600_entry(handle);
}


/* end of file */
