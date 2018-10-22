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
 * \brief LED 流水灯
 *
 * - 实验现象：
 *   1. LED依次点亮，形成流水灯效果
 * 
 * \par 源代码
 * \snippet demo_std_led_water_light.c src_std_led_water_light
 * 
 * \internal
 * \par Modification history
 * - 1.00 17-04-18  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_std_led_water_light
 * \copydoc demo_std_led_water_light.c
 */

/** [src_std_led_water_light] */
#include "ametal.h"
#include "am_led.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_led_water_light_entry (int led_id_start, int num)
{
    int32_t i = 0;

    while(1) {
        for (i = led_id_start; i < (led_id_start + num); i++) {
             am_led_on(i);
             am_mdelay(150);        /* 延时150ms */
             am_led_off(i);
        }
    }
}
/** [src_std_led_water_light] */

/* end of file */
