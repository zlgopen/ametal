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
 * \brief 飞行时间传感器 TMF8801 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印距离和温度的值
 *
 * \par 源代码
 * \snippet demo_std_tmf8801.c src_std_tmf8801
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10 yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_tmf8801
 * \copydoc demo_std_tmf8801.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_tmf8801.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_tmf8801_entry (am_sensor_handle_t handle)
{
    /* TMF8801提供的所有通道ID列举 */
    const int id[2] = {AM_TMF8801_CHAN_1, AM_TMF8801_CHAN_2};

    /* 储存两个通道数据的缓存 */
    am_sensor_val_t data[2];

    int i;

    /*
     * 列出两个通道（距离和温度）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {"distance", "temperature"};
    const char *data_unit_string[] = {"mm", "℃"};

    am_sensor_enable(handle, id, 2, data);

    while(1) {
        am_sensor_data_get(handle, id, 2, data);
        for(i = 0; i < 2; i++) {
            if (AM_SENSOR_VAL_IS_VALID(data[i])) { /* 该通道数据有效，可以正常使用 */
                am_kprintf("The %s is : %d %s.\r\n", data_name_string[i],
                                                          data[i].val,
                                                          data_unit_string[i]);
            } else { //该通道数据无效，数据获取失败
                am_kprintf("The %s get failed!\r\n", data_name_string[i]);
            }
        }
        am_mdelay(1000);
    }
}

/* end of file */
