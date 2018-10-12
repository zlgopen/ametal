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
 * \brief 程序清单7.5
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-21  adw, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_delay.h"
#include "am_led.h"

int am_main (void)
{
    int i = 0;
    while(1) {
        am_led_on(i);        // 点亮LED（i）
        am_mdelay(200);      // 延时200ms
        am_led_off(i);       // 熄灭LED（i）
        i = (i + 1) % 2;     // 仅有2个LED
    }
}

/* end of file */

