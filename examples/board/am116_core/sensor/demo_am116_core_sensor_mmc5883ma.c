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
 * \brief 三轴磁传感器 MMC5883MA 例程，通过标准接口实现(通过查询方式获取数据)
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印X、Y、Z三轴的磁感应强度值
 *
 * \par 源代码
 * \snippet demo_am116_core_sensor_mmc5883ma.c src_am116_core_sensor_mmc5883ma
 *
 * \internal
 * \par Modification history
 * - 1.00 19-1-17  wk, first implementation
 * \endinternal
 */

#include "am_sensor.h"
#include "am_hwconf_sensor_mmc5883ma.h"
#include "demo_std_entries.h"

/**
 * \brief 传感器例程入口
 */
void demo_am116_core_sensor_mmc5883ma_entry (void)
{
    am_sensor_handle_t handle = am_sensor_mmc5883ma_inst_init();

    demo_std_mmc5883ma_entry(handle);
}


/* end of file */