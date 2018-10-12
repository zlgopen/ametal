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
 * \brief book`s key1 driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "key1.h"
#include "am_lpc82x.h"
#include "am_softimer.h"

static uint8_t key_last_value;                       // 上次键值变量
static uint8_t key_final_value;                      // 最终键值变量
static const int key_pin = PIO0_1;                   // 单个独立按键对应的管脚

static pfn_key1_callback_t  g_key1_callback = NULL;  // 保存应用程序注册的回调函数
static void              	 *g_key1_arg   = NULL;		 // 保存应用程序自定义的参数
static am_softimer_t        g_key1_timer;            // 用于按键扫描的软件定时器

static void key1_softimer_callback(void *p_arg)
{
  uint8_t key_return = key1_scan();
  if (key_return!= 0xFF) {
    // 产生按键事件
		g_key1_callback(g_key1_arg, key_return);
    }
}

void key1_softimer_set (void)
{
    am_softimer_init(&g_key1_timer, key1_softimer_callback, NULL);
    am_softimer_start(&g_key1_timer,10); 	// 启动定时器，10ms调用一次回调函数
}

void key1_init_with_softimer(pfn_key1_callback_t p_func, void *p_arg)
{
    key1_init();
	  g_key1_callback  = p_func;
	  g_key1_arg       = p_arg;
    key1_softimer_set();
}


void key1_init (void)                                // 板级初始化函数
{
    am_gpio_pin_cfg(key_pin, AM_GPIO_INPUT);         // PIO0_1内置上拉电阻，所以初始化为1
    key_final_value = key_last_value = am_gpio_get(key_pin); // 上次键值和最终键值均初始化为1
}

uint8_t key1_scan (void)                                // 独立按键扫描函数
{
    uint8_t ret_flag = 0xFF;                           // 按键状态标志初始化为0xFF

    uint8_t key_current_value = am_gpio_get(PIO0_1);   // 获取当前键值
    if (key_last_value == key_current_value) {
        // 由于两次扫描得到的值相等，因此按键行为有效
        if (key_current_value!= key_final_value) {
            // 按键状态发生改变
            key_final_value = key_current_value;  // 保存两次扫描后相等的键值
            if (key_current_value == 0) {
                ret_flag = 0;                     // 有键按下，ret_flag标志置0
            } else {
                ret_flag = 1;                     // 按键释放，ret_flag标志置1
            }
        }
    }
    key_last_value = key_current_value;         	// 保存上次键值
    return ret_flag;                              // 返回按键状态标志ret_flag
}


/* end of file */
