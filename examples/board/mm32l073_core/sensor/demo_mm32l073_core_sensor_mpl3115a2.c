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
 * \brief 气压温度传感器 MPL3115A2 例程，通过标准接口实现(通过查询方式获取数据)
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印气压/海拔和温度的值
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_sensor_mpl3115a2.c src_mm32l073_core_sensor_mpl3115a2
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-11  fzb, first implementation
 * \endinternal
 */

#include "am_sensor.h"
#include "am_hwconf_sensor_mpl3115a2.h"
#include "demo_std_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief 传感器例程入口
 */
void demo_mm32l073_core_sensor_mpl3115a2_entry (void)
{
    am_sensor_handle_t handle = am_sensor_mpl3115a2_inst_init();

    demo_std_mpl3115a2_entry(handle);
}


/* end of file */
