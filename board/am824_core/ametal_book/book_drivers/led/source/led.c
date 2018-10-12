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
 * \brief book`s led driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */
//#include "ametal.h"
//#include "am_delay.h"
//#include "am_board.h"
//#include "am_lpc82x.h"
//#include "led.h"

///** \brief 定义led_id 编号 */
//const int led_gpio_tab[] = {
//    PIO0_8, PIO0_9, PIO0_10, PIO0_11, PIO0_12, PIO0_13, PIO0_14, PIO0_15};

//int led_on (int led_id)
//{
//    if (led_id >= sizeof(led_gpio_tab) / sizeof(led_gpio_tab[0])) {
//        return AM_ERROR;
//    }
//    am_gpio_set(led_gpio_tab[led_id], 0);          // 低电平点亮LED 
//    return AM_OK;
//}

//int led_off (int led_id)
//{
//    if (led_id >= sizeof(led_gpio_tab) / sizeof(led_gpio_tab[0])){
//        return AM_ERROR;
//    }
//		am_gpio_set(led_gpio_tab[led_id], 1);          // 高电平熄灭LED 
//    return AM_OK;
//}

//int led_toggle (int led_id)
//{
//    if (led_id >= sizeof(led_gpio_tab) / sizeof(led_gpio_tab[0])) {
//        return AM_ERROR;
//    }
//    am_gpio_toggle(led_gpio_tab[led_id]);          // 翻转LED状态
//    return AM_OK;
//}
//  
//void led_init (void)
//{
//  int i;

//  for (i = 0; i < sizeof(led_gpio_tab) / sizeof(led_gpio_tab[0]); i++) {
//    
//    // 配置GPIO为输出，初始为高电平，熄灭LED  
//    am_gpio_pin_cfg (led_gpio_tab[i], AM_GPIO_OUTPUT_INIT_HIGH);
//  }
//}
#include "ametal.h"
#include "led.h"
#include "am_gpio.h"
#include "am_lpc82x.h"

typedef struct led_info {
    int           pin;          				// LED对应的管脚
    unsigned char  active_level;   				// 0—低电平点亮，1—高电平点亮
} led_info_t;

static const led_info_t g_led_info[] = {		
#if (USE_MINIPORT_LED == 1)
     {PIO0_8, 0}, {PIO0_9, 0}, {PIO0_10, 0}, {PIO0_11, 0},
     {PIO0_12,0}, {PIO0_13, 0}, {PIO0_14, 0}, {PIO0_15, 0}
#else
    {PIO0_20, 0}, {PIO0_21, 0}
#endif
};


void led_init (void)
{
	int i;

	for (i = 0; i < sizeof(g_led_info) / sizeof(g_led_info[0]); i++) {
		//配置GPIO为输出，初始化为高电平，熄灭LED
		am_gpio_pin_cfg(g_led_info[i].pin, AM_GPIO_OUTPUT_INIT_HIGH);
	}
}

int led_on (int led_id)
{
	if (led_id >= sizeof(g_led_info) / sizeof(g_led_info[0])) {
        return AM_ERROR;
	}
	am_gpio_set(g_led_info[led_id].pin, g_led_info[led_id].active_level);
	return AM_OK;
}

int  led_off (int led_id)
{
	if (led_id >= sizeof(g_led_info) / sizeof(g_led_info[0])) {
        return AM_ERROR;
	}
	am_gpio_set(g_led_info[led_id].pin, !g_led_info[led_id]. active_level);
	return AM_OK;
}

int  led_toggle (int led_id)
{
	if (led_id >= sizeof(g_led_info) / sizeof(g_led_info[0])) {
        return AM_ERROR;
	}
	am_gpio_toggle(g_led_info[led_id].pin);
	return AM_OK;
}


/* end of file */
