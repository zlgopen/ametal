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
 * \brief 温湿度传感器 bmp280 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印压强和温度的值
 *
 * \par 源代码
 * \snippet demo_std_bmp280.c src_std_bmp280
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-27  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_bmp280
 * \copydoc demo_std_bmp280.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_bmp280.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_bmp280_entry (am_sensor_handle_t handle)
{
    /* bmp280提供的所有通道ID列举 */
    const int       id[2] = {AM_BMP280_CHAN_1, AM_BMP280_CHAN_2};

    /* 储存两个通道数据的缓存 */
    am_sensor_val_t data[2];

    /*
     * 列出两个通道（压力和温度）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {"pressure", "temperature"};
    const char *data_unit_string[] = {"Pa", "℃"};
    
    am_sensor_enable(handle, id, 2, data);

    while(1) {
        am_sensor_data_get(handle, id, 2, data);

        if (AM_SENSOR_VAL_IS_VALID(data[0])) { /* 该通道数据有效，可以正常使用 */
            am_kprintf("The %s is : %d %s.\r\n", data_name_string[0],
                                                 (data[0].val),
                                                 data_unit_string[0]);
        } else {       //该通道数据无效，数据获取失败
            am_kprintf("The %s get failed!\r\n", data_name_string[0]);
        }
        
        if (AM_SENSOR_VAL_IS_VALID(data[1])) { /* 该通道数据有效，可以正常使用 */
            /* 单位转换为 AM_SENSOR_UNIT_MICRO，以打印显示6位小数 */
            am_sensor_val_unit_convert(&data[1], 1, AM_SENSOR_UNIT_MICRO);
            am_kprintf("The %s is : %d.%06d %s.\r\n", data_name_string[1],
                                                      (data[1].val)/1000000,
                                                      (data[1].val)%1000000,
                                                      data_unit_string[1]);
        } else {       //该通道数据无效，数据获取失败
            am_kprintf("The %s get failed!\r\n", data_name_string[1]);
        }        
        am_mdelay(1000);
    }
}

/* end of file */
