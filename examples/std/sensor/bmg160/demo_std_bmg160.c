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
 * \brief 三轴陀螺仪传感器 BMG160 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印X、Y、Z三轴的角速度值
 *
 * \par 源代码
 * \snippet demo_std_bmg160.c src_std_bmg160
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-11  wk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_bmg160
 * \copydoc demo_std_bmg160.c
 */
 
#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_bmg160.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_bmg160_entry (am_sensor_handle_t handle)
{

    /* BMG160提供的所有通道ID列举 */
    const int       id[3] = {AM_BMG160_CHAN_1, AM_BMG160_CHAN_2, AM_BMG160_CHAN_3};

    /* 储存三个通道数据的缓存 */
    am_sensor_val_t data[3];

    int             i; 
    
    /*
     * 列出三个通道（X、Y、Z轴角速度）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {"x_axis_angular_velocity", 
                                      "y_axis_angular_velocity",
                                      "z_axis_angular_velocity"};
    const char *data_unit_string[] = {"rad/s", "rad/s", "rad/s"};

    am_sensor_enable(handle, id, 3, data);
    
    while(1) {
        am_sensor_data_get(handle, id, 3, data);
        for(i=0; i<3; i++) {
            if (AM_SENSOR_VAL_IS_VALID(data[i])) { /* 该通道数据有效，可以正常使用 */
                /* 单位转换为 AM_SENSOR_UNIT_MICRO，以打印显示6位小数 */
                am_sensor_val_unit_convert(&data[i], 1, AM_SENSOR_UNIT_MICRO);
                am_kprintf("The %s is : %d.%06d %s.\r\n", data_name_string[i],
                                                         (data[i].val)/1000000,
                                                         (data[i].val)%1000000,
                                                          data_unit_string[i]);
            } else {       /* 该通道数据无效，数据获取失败 */
                am_kprintf("The %s get failed!\r\n", data_name_string[i]);
            }
        }
        am_mdelay(1000);
    }
}

/* end of file */
