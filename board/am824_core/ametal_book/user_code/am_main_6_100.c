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
 * \brief 程序清单6.100
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-19  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_led.h"
#include "am_zlg72128.h"
#include "am_hwconf_zlg72128.h"

// 自定义按键处理回调函数
static void __key_callback (void *p_arg,uint8_t key_val,uint8_t repeat_cnt,uint8_t funkey_val)
{
    if (key_val == AM_ZLG72128_KEY_1_1) {        // 第1行第1个按键按下
            // 功能键F0按下
        if (AM_ZLG72128_FUNKEY_CHECK(funkey_val, AM_ZLG72128_FUNKEY_0)) {
            am_led_toggle(1);
        } else {                                // 功能键F0未按下
            am_led_toggle(0);
        }
    }
}

int am_main (void)
{
    am_zlg72128_handle_t zlg72128_handle = am_zlg72128_inst_init();
    am_zlg72128_key_cb_set(zlg72128_handle, __key_callback, NULL);                    
    // _key_callback注册的回调函数，回调函数的第一个参数不使用，设置为NULL
    while (1){
    }
}



/* end of file */

