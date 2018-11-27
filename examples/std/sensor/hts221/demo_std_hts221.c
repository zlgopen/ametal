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
 * \brief 温湿度传感器 HTS221 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印湿度和温度的值
 *
 * \par 源代码
 * \snippet demo_std_hts221.c src_std_hts221
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-27  wan, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_hts221
 * \copydoc demo_std_hts221.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_hts221.h"
#include "am_delay.h"
#include "am_hwconf_sensor_hts221.h"

/**
 * \brief 例程入口
 */
void demo_std_hts221_entry (void)
{
    am_sensor_handle_t handle = NULL;

    /* HTS221提供的所有通道ID列举 */
    const int       id[2] = {AM_HTS221_CHAN_1, AM_HTS221_CHAN_2};

    /* 储存两个通道数据的缓存 */
    am_sensor_val_t data[2];

    int             i;

    /*
     * 列出两个通道（湿度和温度）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {"humidity", "temperature"};
    const char *data_unit_string[] = {"%rH", "℃"};

    handle = am_sensor_hts221_inst_init();

    am_sensor_enable(handle, id, 2, data);

    while(1) {
        am_sensor_data_get(handle, id, 2, data);
        for(i=0; i<2; i++) {
            if (AM_SENSOR_VAL_IS_VALID(data[i])) { /* 该通道数据有效，可以正常使用 */
                /* 单位转换为 AM_SENSOR_UNIT_MICRO，以打印显示6位小数 */
                am_sensor_val_unit_convert(&data[i], 1, AM_SENSOR_UNIT_MICRO);
                am_kprintf("The %s is : %d.%06d %s.\r\n", data_name_string[i],
                                                          (data[i].val)/1000000,
                                                          (data[i].val)%1000000,
                                                          data_unit_string[i]);
            } else {       //该通道数据无效，数据获取失败
                am_kprintf("The %s get failed!\r\n", data_name_string[i]);
            }
        }
        am_mdelay(1000);
    }
}

/* end of file */
