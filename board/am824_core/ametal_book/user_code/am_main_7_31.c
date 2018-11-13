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
 * \brief 程序清单7.31
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-21  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_input.h"
#include "am_led.h"

static am_input_key_handler_t g_key_handler;    // 事件处理器实例定义，全局变量，确保一直有效

// 使用按键基本功能，使用该功能将注释开始，将下面函数注释
//static void __input_key_proc(void *p_arg, int key_code, int key_state, int keep_time)
//{
//    if (key_code == KEY_KP0) {
//        if (key_state == AM_INPUT_KEY_STATE_PRESSED) {        //  有键按下
//            am_led_on(0);
//        }else if (key_state == AM_INPUT_KEY_STATE_RELEASED){  // 按键释放
//            am_led_off(0);
//        }
//    }
//}


// 使用按键长按功能，使用该功能将注释开始，将上面函数注释
static void __input_key_proc(void *p_arg, int key_code, int key_state, int keep_time) 
{
    if (key_code == KEY_KP0) {
        if ((key_state == AM_INPUT_KEY_STATE_PRESSED) && (keep_time == 3000)) {
            am_led_toggle(0); 					// 长按3s,  LED0状态翻转
        }
    }
} 


int am_main (void)
{
    // 这里按键回调函数可以选择按键不同的功能(基本功能或者长按功能,只需要修改)
    am_input_key_handler_register(&g_key_handler, __input_key_proc, (void *)100);
    while (1) {
    }
}



/* end of file */

