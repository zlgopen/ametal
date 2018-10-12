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
 * \brief 程序清单4.44
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "buzzer.h"
#include "led.h"
#include "am_delay.h"
#include "matrixkey.h"

static void key_process (uint8_t code)       // 按键处理程序
{
    if ((code & 0x80) == 0) {                // 按键按下事件，按键释放时蜂鸣器不叫
        buzzer_beep_async(100);
    }
    switch (code) {
    case 0:                                 // KEY0按下，显示00
        led_off(0); led_off(1);  break;     
    case 1:                                // KEY1按下，显示01
        led_on(0);  led_off(1);  break;      
    case 2:                                // KEY2按下，显示10
        led_on(1);  led_off(0);  break;      
    case 3:                                // KEY3按下，显示11
        led_on(0);  led_on(1);   break;
    default:
        break;
    }
}

int am_main (void)
{
    uint8_t key_return;

    buzzer_init();    led_init();      matrixkey_init();
    while(1) {
        key_return = matrixkey_scan();
        if (key_return!= 0xFF) {
            // 产生按键事件
            key_process(key_return);
        }
        am_mdelay(10);
    }
}



/* end of file */

