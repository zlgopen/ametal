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

static void __input_key1_proc(void *p_arg, int key_code, int key_state)
{
    if (key_code == KEY_KP0) {
        // 处理按键1
    }
}

static void __input_key2_proc (void *p_arg, int key_code, int key_state)
{
    if (key_code == KEY_KP1){
        // 处理按键2
    }
}

static void __input_key3_proc (void *p_arg, int key_code, int key_state)
{
    if (key_code == KEY_KP2){
        // 处理按键3
    }
}

static am_input_key_handler_t g_key1_handler;
static am_input_key_handler_t g_key2_handler;
static am_input_key_handler_t g_key3_handler;

int am_main (void)
{
    am_input_key_handler_register(&g_key1_handler, __input_key1_proc, NULL);
    am_input_key_handler_register(&g_key2_handler, __input_key2_proc, NULL);
    am_input_key_handler_register(&g_key3_handler, __input_key3_proc, NULL);
    while (1){
    }
}


/* end of file */

