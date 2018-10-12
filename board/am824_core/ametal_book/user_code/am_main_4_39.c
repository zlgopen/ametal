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
 * \brief 程序清单4.37
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
#include "am_softimer.h"

static am_softimer_t g_key1_timer;           // 按键扫描的软件定时器变量
static void key1_process(uint8_t key_return) // 按键处理程序
{
    if (key_return == 0) {
        buzzer_beep_async(100);
    } else if (key_return == 1) {
        led_toggle(0);
    }
}

static void key1_softimer_callback(void *p_arg)
{
    uint8_t key_return = key1_scan();
    if (key_return != 0xFF) {
        key1_process(key_return);
    }
}

static void key1_softimer_set (void)
{
    am_softimer_init(&g_key1_timer, key1_softimer_callback, NULL);
    am_softimer_start(&g_key1_timer, 10);
}

int am_main(void)
{
    led_init();
    buzzer_init();
    key1_init();
    key1_softimer_set();
    while(1) {
    }
}



/* end of file */

