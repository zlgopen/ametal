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
 * \brief 气压温度传感器 MPL3115A2 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印气压/海拔 和 温度的值
 *
 * \par 源代码
 * \snippet demo_std_mpl3115a2.c src_std_mpl3115a2
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-11  fzb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_mpl3115a2
 * \copydoc demo_std_mpl3115a2.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_mpl3115a2.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_mpl3115a2_entry (am_sensor_handle_t handle)
{
    /* MPL3115A2供的所有通道ID列举 */
    const int       id[2] = {AM_MPL3115A2_CHAN_1, AM_MPL3115A2_CHAN_2};

    /* 储存两个通道数据的缓存 */
    am_sensor_val_t data[2];

    int             i;

    /*
     * 列出两个通道（气压/海拔 和 温度）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {
                                    #if(__MPL3115A2_MODE == __MPL3115A2_PRE_MODE) /* 气压 */
                                        "pressure", 
                                    #elif(__MPL3115A2_MODE == __MPL3115A2_ALT_MODE) /* 海拔 */
                                        "altitude",
                                    #endif
                                        "temperature"};
    const char *data_unit_string[] = {
                                    #if(__MPL3115A2_MODE == __MPL3115A2_PRE_MODE) /* 气压 */
                                        "KPa",
                                    #elif(__MPL3115A2_MODE == __MPL3115A2_ALT_MODE) /* 海拔 */
                                        "Meters",
                                    #endif
                                        "℃"};

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
        am_kprintf("\r\n");
        am_mdelay(1000);
    }
}

/* end of file */
