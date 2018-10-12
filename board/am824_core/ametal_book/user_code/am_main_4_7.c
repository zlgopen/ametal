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
 * \brief 程序清单4.7
 *
 * \note 该历程需要用到miniport拓展板,需要在led.h文件将宏USE_MINIPORT_LED设为1
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
    int i = 0;

    led_init();               // 板级初始化 
    while(1) {
        led_on(i);            // 点亮LED（i）
        am_mdelay(100);       // 延时100ms 
        led_off(i);           // 熄灭LED（i） 
        i++;                  // 指向下一个LED         
        if(i == 8) {          // LED循环一次后，i = 8  
            i = 0;            // 即将i初始化为0        
        }
    }
}

/* end of file */
