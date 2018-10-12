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
 * \brief 程序清单4.6
 *
 * \note 该例程需要用到miniport拓展板
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
    led_init();                 // 板集初始化，熄灭LED灯 
    while (1) {
        led_toggle(0);          // 翻转I/O电平,改变LED0状态
        am_mdelay(200);         // 延时200ms
    }
}


/* end of file */
