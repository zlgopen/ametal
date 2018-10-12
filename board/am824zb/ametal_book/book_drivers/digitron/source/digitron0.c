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
 * \brief book`s digitron0 driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "digitron0.h"
#include "am_lpc82x.h"

static const int g_digitron_com[2] = {PIO0_17,PIO0_23};
static const uint8_t g_segcode_list[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
static const int g_digitron_seg[8]={PIO0_8, PIO0_9, PIO0_10, PIO0_11, PIO0_12, PIO0_13, PIO0_14, PIO0_15};

void digitron_segcode_set(uint8_t code)
{
    int i;

    for (i = 0; i < 8; i++) {
        am_gpio_set(g_digitron_seg[i], ((code & (1 << i)) >> i));  // 取出i位的值，传送到相应引脚
    }
}

void digitron_com_sel(uint8_t pos)
{
    int i;

    for (i = 0; i < 2; i++) {
        // 当i与pos相等时，则输出0(有效)，否则输出1(无效)
        am_gpio_set(g_digitron_com[i], !(pos == i)); 
    }
}

static void digitron_disp_code (uint8_t pos, uint8_t code)   // code为待显示数字0~9的段码
{
    digitron_com_sel(pos);                                   // pos取值范围为0、1
    digitron_segcode_set(code);                              // 0~9对应的段码
}                                                            
                                                             
void digitron_disp_num (uint8_t pos, uint8_t num)            // num为待显示的数字0~9
{
    if (num <= 9) {
        digitron_disp_code (pos, g_segcode_list[num]);
    }
}

void digitron_init (void)
{
    int i;

    for (i = 0; i < 2; i++) {
        // 将com端对应引脚设置为输出，并初始化为高电平
        am_gpio_pin_cfg(g_digitron_com[i], AM_GPIO_OUTPUT_INIT_HIGH);
    }
    for (i = 0; i < 8; i++){
        // 将段选端对应引脚设置为输出，并初始化为高电平
        am_gpio_pin_cfg(g_digitron_seg[i], AM_GPIO_OUTPUT_INIT_HIGH);
    }
}

/* end of file */
