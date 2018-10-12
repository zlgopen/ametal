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
 * \brief book`s buzzer driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_delay.h"
#include "am_lpc82x_inst_init.h"
#include "buzzer.h"

static am_pwm_handle_t g_pwm_handle;                  //全局PWM handle变量 
static void __beep_timer_callback (void *p_arg);      //蜂鸣器软件定时器回调函数 
static am_softimer_t beep_timer;                      //蜂鸣器软件定时器实例 

void buzzer_init (void)
{
		g_pwm_handle  = am_lpc82x_sct0_pwm_inst_init ();
		am_pwm_config(g_pwm_handle, 1, 500000, 1000000);    		          // 初始化后默认频率1KHz 
		am_softimer_init(&beep_timer,__beep_timer_callback, &beep_timer); // 初始化软件定时器
}


void buzzer_freq_set (uint32_t freq)
{  
    uint32_t period_ns = 1000000000 / freq;
    am_pwm_config(g_pwm_handle, 1, period_ns / 2, period_ns); 		// 通道1对应PIO0_24 
}

void buzzer_on (void)
{
    am_pwm_enable(g_pwm_handle, 1);              // 使能配置好的通道PWM输出 
}

void buzzer_off (void)
{
    am_pwm_disable(g_pwm_handle, 1);             // 禁能PWM输出 
}                                                
                                                 
void buzzer_beep (unsigned int ms)               // 蜂鸣器鸣叫ms毫秒 
{
    buzzer_on();
    am_mdelay(ms);
    buzzer_off();
}

static void __beep_timer_callback (void *p_arg)
{
    am_softimer_t *p_timer = (am_softimer_t *)p_arg;
    am_softimer_stop(p_timer);
    buzzer_off();
}

void buzzer_beep_async (unsigned int ms)
{
    am_softimer_start(&beep_timer, ms);
    buzzer_on();
}

/* end of file */
