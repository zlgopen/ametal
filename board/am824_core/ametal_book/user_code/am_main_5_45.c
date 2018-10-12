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
 * \brief 程序清单5.45
 *
 * \note 该历程需要用到miniport拓展板
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "led.h"
#include "buzzer.h"
#include "key1.h"
#include "am_delay.h"
int am_main(void)
{
    uint8_t key_return;                    // key1_scan()返回值变量

    key1_init();
    led_init();
    buzzer_init();
    while(1) {
        key_return = key1_scan();          // 根据key_return的返回值判断按键事件的产生
        if (key_return == 0) {
          // 返回值为0，说明有键按下，蜂鸣器“嘀”一声
            buzzer_beep(100);
        } else if (key_return == 1) {
          // 返回值为1，说明按键释放，LED0翻转
            led_toggle(0);
        }
        am_mdelay(10);                    // 每隔10ms扫描一次按键，延时去抖动
    }
}


/* end of file */

