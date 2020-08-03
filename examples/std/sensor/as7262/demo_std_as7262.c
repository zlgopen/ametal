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
 * \brief 光谱传感器 AS7262 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印通道值
 *
 * \par 源代码
 * \snippet demo_std_as7262_i2c.c src_std_as7262
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_as7262
 * \copydoc demo_std_as7262.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_as7262.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_as7262_entry (am_sensor_handle_t handle)
{
    /* AS7262提供的所有通道ID列举 */
    const int id[6] = {AM_AS7262_CHAN_1, AM_AS7262_CHAN_2,
                       AM_AS7262_CHAN_3, AM_AS7262_CHAN_4,
                       AM_AS7262_CHAN_5, AM_AS7262_CHAN_6};

    /* 储存六个通道数据的缓存 */
    am_sensor_val_t data[6];

    int i;

    /*
     * 列出六个通道（光谱）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {"V", "B", "G", "Y", "O", "R"};
    const char *data_unit_string[] = {" ", " ", " ", " ", " ", " "};

    am_sensor_enable(handle, id, 6, data);

    while(1) {
        am_sensor_data_get(handle, id, 6, data);
        am_kprintf("\r\n");
        for(i = 0; i < 6; i++) {
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
