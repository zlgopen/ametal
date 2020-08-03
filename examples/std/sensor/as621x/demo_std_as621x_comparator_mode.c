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
 * \brief 温度传感器 AS621X 比较模式例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印温度的值
 *
 * \par 源代码
 * \snippet demo_std_as621x_comparator_mode.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-21  fzb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_as621x_comparator_mode
 * \copydoc demo_std_as621x_comparator_mode.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_as621x.h"
#include "am_delay.h"

am_local void __alert_bit_info_get(am_sensor_handle_t handle)
{
    am_sensor_as621x_dev_t *p_dev = handle->p_drv;

    uint8_t alert_bit;

    am_sensor_as621x_ioctl(handle, AM_SENSOR_AS621X_ALERT_BIT_GET, &alert_bit);
    if (alert_bit == p_dev->dev_info->p_param_default->polarity_high) {
        am_kprintf("The temperature has exceeded THIGH!\r\n");
    } else {
        am_kprintf("The temperature has fallen below TLOW or Reset!\r\n");
    }
}

/**
 * \brief 例程入口
 */
void demo_std_as621x_comparator_mode_entry (am_sensor_handle_t handle)
{
    /* AS621X提供的所有通道ID列举 */
    const int       id[1] = {AM_AS621X_CHAN_1};

    /* 储存一个通道数据的缓存 */
    am_sensor_val_t data[1];

    int             i;

    /* 获取SM位的值 */
    uint8_t         sm_bit;

    /*
     * 列出一个通道（温度）数据的名字和单位字符串，便于打印
     */
    const char *data_name_string[] = {"temperature"};
    const char *data_unit_string[] = {"℃"};

    am_sensor_enable(handle, id, 1, data);

    while(1) {
        am_sensor_data_get(handle, id, 1, data);
        for(i = 0; i < 1; i++) {
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
        __alert_bit_info_get(handle);
        am_sensor_as621x_ioctl(handle, AM_SENSOR_AS621X_SM_GET, &sm_bit);
        if (sm_bit == AM_AS621X_SLEEP_MODE) {
            /* 睡眠模式下重新开启单次转换 */
            am_sensor_as621x_ioctl(handle, AM_SENSOR_AS621X_SS_SET, (void *)AM_TRUE);
        }
        am_mdelay(1000);
    }
}

/* end of file */
