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
 * \brief 环境光/颜色、接近/手势传感器 TMG49037 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印接近AD值、颜色(红、绿、蓝、透明)AD值以及方向(北、南、西、东)AD值
 *
 * \par 源代码
 * \snippet demo_std_tmg49037.c src_std_tmg49037
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_tmg49037
 * \copydoc demo_std_tmg49037.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_tmg49037.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_tmg49037_entry (am_sensor_handle_t handle)
{
    /* TMG49037提供的所有通道ID列举 */
    const int id[3] = {AM_TMG49037_CHAN_1, AM_TMG49037_CHAN_2, AM_TMG49037_CHAN_3};

    /* 储存三个通道数据的缓存 */
    am_sensor_val_t data[9];

    int i;

    /*
     * 列出三个通道（接近、环境光、手势）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {"proximity", "clear", "red", "green", "blue",
                                      "north", "south", "west", "east"};
    const char *data_unit_string[] = {" ", " ", " ", " ", " ",
                                      " ", " ", " ", " "};

    am_sensor_enable(handle, id, 3, data);

    while(1) {
        am_sensor_data_get(handle, id, 3, data);
        for(i = 0; i < 9; i++) {
            if (AM_SENSOR_VAL_IS_VALID(data[i])) { /* 该通道数据有效，可以正常使用 */
                am_kprintf("The %s is : %d %s \r\n", data_name_string[i],
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
