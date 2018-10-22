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
 * \brief LED 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 指定 ID 的 LED 以 0.5s 的时间间隔闪烁。
 * 
 * \par 源代码
 * \snippet demo_std_led.c src_std_led
 * 
 * \internal
 * \par Modification history
 * - 1.00 17-04-18  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_std_led
 * \copydoc demo_std_led.c
 */

/** [src_std_led] */
#include "ametal.h"
#include "am_led.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_std_led_entry (int led_id)
{
    while (1) {
        am_led_on(led_id);
        am_mdelay(500);
        am_led_off(led_id);
        am_mdelay(500);
    }
}
/** [src_std_led] */

/* end of file */
