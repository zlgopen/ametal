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
 * \brief 按键检测
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "ir_thremometer_key.h"
#include "am_input.h"
#include "ir_thremometer_beep.h"
#include "ir_thremometer_timer.h"

#define KEY_TIMEOUT 10000
#define KEY_SHORT_PRESS_TIMEOUT 10
#define KEY_LONG_PRESS_5S 5000
#define KEY_LONG_PRESS_10S 10000

static am_input_key_handler_t key_handler;
static key_press_t key_press = KEY_4_SHORT_PRESSED;

const static key_press_t long_press[4] = {
	KEY_1_LONG_PRESSED, KEY_2_LONG_PRESSED, KEY_3_LONG_PRESSED, KEY_4_LONG_PRESSED
};

const static key_press_t short_press[4] = {
	KEY_1_SHORT_PRESSED, KEY_2_SHORT_PRESSED, KEY_3_SHORT_PRESSED, KEY_4_SHORT_PRESSED
};

/**
 * \brief 按键事件回调函数
 */
am_local void __input_key_proc (void *p_arg, int key_code, int key_state, int keep_time)
{
	static int long_press_flag = 0;
	static am_bool_t if_pressed = AM_FALSE; 
	
    if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
		if (if_pressed == AM_FALSE) {
			if_pressed = AM_TRUE;
			ir_thremometer_beep_on(1, BEEP_SHORT_NOISE);
			ir_thremometer_timer_stop();
			ir_thremometer_timer_clear_cnt();
		} 
		if (keep_time >= KEY_LONG_PRESS_5S && keep_time < KEY_LONG_PRESS_10S) {
			if (key_code >= KEY_1 && key_code <= KEY_4) {
				if (long_press_flag == 0) {
					key_press = long_press[key_code - 2];
				}
				long_press_flag++;
			}
		} else if (keep_time >= KEY_LONG_PRESS_10S) {
			if (key_code == KEY_1) {
				key_press = KEY_1_LONG_PRESSED_10S;
			}
		}
//        AM_DBG_INFO("key_code: %d Press! Keep_time is %d\r\n", key_code, keep_time);
    } else if (key_state == AM_INPUT_KEY_STATE_RELEASED){
		if (key_code >= KEY_1 && key_code <= KEY_4 && long_press_flag == 0) {
				key_press = short_press[key_code - 2];
		}
		long_press_flag = 0;
		if_pressed = AM_FALSE;
		ir_thremometer_timer_start();
    }
}
/**
 * \brief 按键初始化
 */
void ir_thremometer_key_init(void)
{
	am_input_key_handler_register(&key_handler, __input_key_proc, NULL);
	ir_thremometer_timer_init();
	ir_thremometer_timer_start();
}
/**
 * \brief 获取按键值
 */
key_press_t ir_thremometer_get_key(void)
{
	return key_press;
}
/**
 * \brief 重置按键值
 */
void ir_thremometer_release_key(void)
{
	key_press = KEY_RELEASE;
}
/**
 * \brief 测试按键短按
 */
void ir_thremometer_set_default_key(void)
{
	key_press = KEY_4_SHORT_PRESSED;
}
/* end of file */
