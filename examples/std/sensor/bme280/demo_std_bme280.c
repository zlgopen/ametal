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
 * \brief 压力温湿度传感器 BME280 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印压强、湿度和温度的值
 *
 * \par 源代码
 * \snippet demo_std_bme280.c src_std_bme280
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-30  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_bme280
 * \copydoc demo_std_bme280.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_bme280.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_bme280_entry (am_sensor_handle_t handle)
{
    /* bme280提供的所有通道ID列举 */
    const int id[3] = {AM_BME280_CHAN_1, AM_BME280_CHAN_2, AM_BME280_CHAN_3};

    /* 储存三个通道数据的缓存 */
    am_sensor_val_t data[3];

    /*
     * 列出三个通道（压力、湿度和温度）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {"pressure", "temperature", "humidity"};
    const char *data_unit_string[] = {"Pa", "℃", "%rH"};

    am_sensor_enable(handle, id, 3, data);

    while(1) {
        am_sensor_data_get(handle, id, 3, data);

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

        if (AM_SENSOR_VAL_IS_VALID(data[2])) { /* 该通道数据有效，可以正常使用 */
            /* 单位转换为 AM_SENSOR_UNIT_MICRO，以打印显示6位小数 */
            am_sensor_val_unit_convert(&data[2], 1, AM_SENSOR_UNIT_MICRO);
            am_kprintf("The %s is : %d.%06d %s.\r\n", data_name_string[2],
                                                      (data[2].val)/1000000,
                                                      (data[2].val)%1000000,
                                                      data_unit_string[2]);
        } else {       //该通道数据无效，数据获取失败
            am_kprintf("The %s get failed!\r\n", data_name_string[2]);
        }          
        am_mdelay(1000);
    }
}

/* end of file */
