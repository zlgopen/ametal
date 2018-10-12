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
 * \brief 程序清单7.30
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

static void __input_key_proc(void *p_arg, int key_code, int key_state)
{
    if (key_code == KEY_KP0) {
        if (key_state == AM_INPUT_KEY_STATE_PRESSED) {        //  有键按下
            am_led_on(0);
        }else if (key_state == AM_INPUT_KEY_STATE_RELEASED){  // 按键释放
            am_led_off(0);
        }
    }
}


int am_main (void)
{
    am_input_key_handler_register(&g_key_handler, __input_key_proc, (void *)100);
    while (1) {
    }
}



/* end of file */

