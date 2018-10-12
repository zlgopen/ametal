/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s app_key_code_led_show driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_input.h"
#include "am_led.h"
#include "am_buzzer.h"

static void __input_key_proc (void *p_arg, int key_code, int key_state)
{
    if (key_state == AM_INPUT_KEY_STATE_PRESSED) {      // 按键按下
        am_buzzer_beep_async(100);                      // 蜂鸣器“嘀”一声
        switch (key_code) {
        case KEY_0:                                     // KEY0按下，显示00
               am_led_off(0);  am_led_off(1); break;
        case KEY_1:                                     // KEY1按下，显示01
                am_led_on(0);  am_led_off(1);  break;
        case KEY_2:                                     // KEY2按下，显示10
                am_led_on(1);  am_led_off(0);  break;
        case KEY_3:                                     // KEY3按下，显示11
            am_led_on(0);  am_led_on(1);  break;
        default:
                break;
        }
    }
}

int app_key_code_led_show (void)                        // 应用函数入口
{
    static am_input_key_handler_t  key_handler;
    am_input_key_handler_register(&key_handler, __input_key_proc, NULL);
    return AM_OK;
}


/* end of file */
