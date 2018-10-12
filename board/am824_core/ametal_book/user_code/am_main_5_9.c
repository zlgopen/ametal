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
 * \brief 程序清单5.9
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
    // 配置GPIO为输出模式，并初始化为高电平
    am_gpio_pin_cfg(PIO0_20,AM_GPIO_OUTPUT_INIT_HIGH);
    while (1) {
        am_gpio_toggle (PIO0_20);     //翻转电平，翻转LED状态
        am_mdelay(200);               //延时200ms
    }
}


/* end of file */

