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
 * \brief 程序清单 9.73
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_delay.h"
#include "am_led.h"
#include "am_input.h"
#include "model_bool.h"
#include "view_led.h"

static model_bool_t __g_model_bool;      			// 定义一个布尔型模型实例
static void __input_key_proc (void *p_arg, int key_code, int key_state, int keep_time)
{
	    am_bool_t value;
	
	    if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
	        if (model_bool_get(&__g_model_bool, &value) == 0) {
	            model_bool_set(&__g_model_bool, !value);
	        }
	    }
}
	
int am_main (void)
{
    view_led_t              view_led0;     		//定义一个LED视图实例
    am_input_key_handler_t  key_handler;   	  //定义一个按键处理器

    //注册按键事件
    am_input_key_handler_register(&key_handler, __input_key_proc, (void *)NULL);
    //初始化模型,value的初始值为AM_FALSE
    model_bool_init(&__g_model_bool, AM_TRUE);
    //初始化视图实例
    view_led_init(&view_led0, 0);
    //添加视图
    model_attach(&(__g_model_bool.isa), &(view_led0.isa));
    while (1) {
    }
}
/* end of file */
