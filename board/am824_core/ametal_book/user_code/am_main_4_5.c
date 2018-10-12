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
 * \brief 程序清单4.4
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_delay.h"
#include "led.h"

int am_main (void)
{
    led_init();                 // 板级初始化，熄灭LED0
    while (1) {
        led_on(0);              // 点亮LED0 
        am_mdelay(200);         // 延时200ms，即延时200ms 
        led_off(0);             // 熄灭LED0  
        am_mdelay(200);         // 延时200ms，即延时200ms  
    }
}

/* end of file */
