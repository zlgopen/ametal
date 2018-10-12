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
 * \brief book`s led driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#ifndef __LED_H
#define __LED_H

#define LED0  0
#define LED1  1
#define LED2  2
#define LED3  3
#define LED4  4
#define LED5  5
#define LED6  6
#define LED7  7

// 使用miniport 时,需要将该接口设置为 1，使用板载LED则设置为0 
#define USE_MINIPORT_LED   0  

void led_init(void);                    // 板级初始化
int  led_on(int led_id);                // (输出低电平)点亮LED    
int  led_off(int led_id);               // (输出高电平)熄灭LED
int  led_toggle(int led_id);            // 翻转I/O电平，翻转LED状态



#endif /* __LED_H */

/* end of file */
