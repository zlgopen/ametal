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
 * \brief 程序清单5.22
 *
 * \note 该历程需要用到miniport拓展板
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_gpio.h"
#include "am_lpc82x.h" 

// 对应两个数码管的com端
static const int g_digitron_com[2] = {PIO0_17,PIO0_23};

// 0 ~ 7分别对应 a, b, c, d, e, f, g, dp
static  const int g_digitron_seg[8]={
     PIO0_8, PIO0_9, PIO0_10, PIO0_11, PIO0_12, PIO0_13, PIO0_14, PIO0_15}; 


int am_main (void)
{
    int i;

    for (i = 0; i < 2; i++) {
        // 将com端对应引脚设置为输出，并初始化为高电平
        am_gpio_pin_cfg(g_digitron_com[i], AM_GPIO_OUTPUT_INIT_HIGH);
    }
    for (i = 0; i < 8; i++) {
        // 将段选端对应引脚设置为输出，并初始化为高电平
        am_gpio_pin_cfg(g_digitron_seg[i], AM_GPIO_OUTPUT_INIT_HIGH);
    }
    am_gpio_set(g_digitron_com[0], 0);            // 使com0端0有效
    am_gpio_set(g_digitron_seg[1], 0);            // 点亮b段LED
    am_gpio_set(g_digitron_seg[2], 0);            // 点亮c段LED
    while(1) {
    }
}


/* end of file */

