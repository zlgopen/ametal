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
 * \brief 程序清单4.42
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "led.h"
#include "buzzer.h"
#include "key1.h"

static void key1_process(void *p_arg, uint8_t key_return)    // 按键处理程序
{
    if (key_return == 0) {                            // 按键按下
        buzzer_beep_async(100);
    } else if (key_return == 1) {                     // 按键释放
        led_toggle(0);
    }
}

int am_main (void)
{
    led_init();
    buzzer_init();
    key1_init_with_softimer(key1_process, NULL);    // 无需使用自定义参数NULL
    while(1) {
    }
}



/* end of file */

