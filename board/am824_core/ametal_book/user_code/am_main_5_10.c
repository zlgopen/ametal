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
 * \brief 程序清单5.10
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_gpio.h"
#include "am_lpc82x.h"
#include "am_delay.h"
int am_main (void)
{
    // 初始化PIO0_24为输出模式，并初始化为高电平，关闭蜂鸣器
    am_gpio_pin_cfg(PIO0_24, AM_GPIO_OUTPUT_INIT_HIGH);
    while(1) {
        am_gpio_toggle(PIO0_24);                    // 翻转PIO0_24的状态
        am_udelay(500);
    }
}



/* end of file */

