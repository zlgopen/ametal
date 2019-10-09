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
 * \brief 六轴传感器FXOS8700CQ 例程，通过标准接口实现(通过查询方式获取数据)
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印加速度和磁感应强度的值
 *
 * \par 源代码
 * \snippet demo_am116_core_sensor_fxos8700cq.c src_am116_core_sensor_fxos8700cq
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-29  fzb, first implementation
 * \endinternal
 */

#include "am_sensor.h"
#include "am_hwconf_sensor_fxos8700cq.h"
#include "demo_std_entries.h"
#include "demo_am116_core_entries.h"

/**
 * \brief 传感器例程入口
 */
void demo_am116_core_sensor_fxos8700cq_entry (void)
{
    am_sensor_handle_t handle = am_sensor_fxos8700cq_inst_init();

    demo_std_fxos8700cq_entry(handle);
}


/* end of file */
