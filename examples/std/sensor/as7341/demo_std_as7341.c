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
 * \brief 光谱传感器 AS7341 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印光谱通道AD值
 *
 * \par 源代码
 * \snippet demo_std_as7341.c src_std_as7341
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_as7341
 * \copydoc demo_std_as7341.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_as7341.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_as7341_entry (am_sensor_handle_t handle)
{
    /* AS7341提供的所有通道ID列举 */
    const int id[1] = {AM_AS7341_CHAN_1};

    /* 储存一个通道数据的缓存 */
    am_sensor_val_t data[12];

    int i;

    /*
     * 列出一个通道（光谱）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {"F1", "F2", "F3", "F4", "F5", "F6",
                                      "F7", "F8", "clear", "dark", "prox", "flicker"};
    const char *data_unit_string[] = {" ", " ", " ", " ", " ", " ",
                                      " ", " ", " ", " ", " ", " "};

    am_sensor_enable(handle, id, 1, data);

    while(1) {
        am_sensor_data_get(handle, id, 1, data);
        for(i = 0; i < 12; i++) {
            if (AM_SENSOR_VAL_IS_VALID(data[i])) { /* 该通道数据有效，可以正常使用 */
                am_kprintf("The %s is : %d %s \r\n", data_name_string[i],
                                                          data[i].val,
                                                          data_unit_string[i]);
            } else { //该通道数据无效，数据获取失败
                am_kprintf("The %s get failed!\r\n", data_name_string[i]);
            }
        }
        am_kprintf("\r\n");
        am_mdelay(1000);
    }
}

/* end of file */
