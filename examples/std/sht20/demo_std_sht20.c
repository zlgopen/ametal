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
 * \brief 温湿度传感器 SHT20 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 每过一秒，通过串口打印湿度和温度的值
 *
 * \par 源代码
 * \snippet demo_std_sht20.c src_std_sht20
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-20  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_sht20
 * \copydoc demo_std_sht20.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sht20.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_sht20_entry (am_sht20_handle_t handle)
{
    float temperature;
    float humidity;

    AM_FOREVER {
        if (am_sht20_temperature_read (handle, &temperature) != AM_OK) {
            AM_DBG_INFO("am_sht20_temperature_read failed!\r\n");
        } else {
            AM_DBG_INFO("Current temperature is %d.%02d°C\r\n",
                        (int)temperature,
                        (int)(temperature * 100) % 100);
        }

        if (am_sht20_humidity_read (handle, &humidity) != AM_OK) {
            AM_DBG_INFO("am_sht20_humidity_read failed!\r\n");
        } else {
            AM_DBG_INFO("Current humidity is %d.%02d\r\n",
                        (int)humidity,
                        (int)(humidity * 100) % 100);
        }
        am_mdelay(1000);
    }
}

/* end of file */
