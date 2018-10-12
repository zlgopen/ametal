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
 * \brief book`s digitron1 driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "digitron2.h"
#include "am_lpc82x.h"
#include "am_softimer.h"
static const int g_digitron_com[2] = {PIO0_17,PIO0_23};
// 数组下标0 ~ 7 分别对应a, b, c, d, e, f, g, dp
static const int g_digitron_seg[8] = {
    PIO0_8, PIO0_9, PIO0_10, PIO0_11, PIO0_12, PIO0_13, PIO0_14, PIO0_15};

static const uint8_t segcodeTab[] = {                            // 从'-'开始的编码表
    0x40, 0x80, 0x00, 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,  // 45 ~ 54
    0x07, 0x7F, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 55 ~ 64
    0x77, 0x7C, 0x39, 0x5e, 0x79, 0x71, 0x00, 0x00, 0x00, 0x00,  // 65 ~ 74
    0x00, 0x00, 0x00, 0x37, 0x3F, 0x73, 0x00, 0x50, 0x00, 0x00,  // 75 ~ 84
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 85 ~ 94
    0x00, 0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x00, 0x00,  // 95 ~ 104
    0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x3F, 0x73, 0x00, 0x50   // 105 ~ 114
};
static am_softimer_t g_timer;                                    // 定义定时器实例，自动扫描显示
static uint8_t g_blink_flag = 0;                                 // 闪烁标志位
static uint8_t g_digitron_disp_buf[2];                           // 显示缓冲区

uint8_t digitron2_char_decode (char ch)
{
    if (ch >= '-' && (ch < '-' + sizeof(segcodeTab)))  // 编码表从'-'开始
        return segcodeTab[ch - '-'];
    return 0x00;
}

static void digitron2_segcode_set (uint8_t code)               // 段码传送函数
{
    int i;

    for (i = 0; i < 8; i++)
        am_gpio_set(g_digitron_seg[i], ((code & (1 << i)) >> i));
}

static void digitron2_com_sel (uint8_t pos)                   // 位码传送函数
{
    int i;

    for (i = 0; i < 2; i++)
        am_gpio_set(g_digitron_com[i], !(pos == i));
}

void digitron2_init (void)                            // 板级初始化函数
{
    int i;

    for (i = 0; i < 2; i++)
        // 将com端对应引脚设置为输出，并初始化为高电平
        am_gpio_pin_cfg(g_digitron_com[i], AM_GPIO_OUTPUT_INIT_HIGH);
    for (i = 0; i < 8; i++)
        // 将段选端对应引脚设置为输出，并初始化为高电平
        am_gpio_pin_cfg(g_digitron_seg[i], AM_GPIO_OUTPUT_INIT_HIGH);
    digitron2_disp_code_set(0,0x00);                 // 初始设置缓冲区中的段码值为无效值
    digitron2_disp_code_set(1,0x00);                 // 初始设置缓冲区中的段码值为无效值
}

void digitron2_disp_scan (void)
{
    static uint8_t  pos = 0;
    static uint8_t  cnt = 0;                        // 扫描次数记录

    digitron2_segcode_set(~0x00);                    // 消影，段码0x00取反
    digitron2_com_sel(pos);                          // 当前显示位
    if (((g_blink_flag & (1 << pos))  && (cnt <= 49)) || ((g_blink_flag & (1 << pos)) == 0)) {
        digitron2_segcode_set(~g_digitron_disp_buf[pos]);      // 获取缓冲区的段码，段码取反
    }
    cnt = (cnt + 1) % 100;                          // 记录500ms，0 ~ 99循环
    pos = (pos + 1) % 2;                            // 切换到下一个显示位
}

static void timer_callback(void *p_arg)
{
    digitron2_disp_scan();                           // 每隔5ms调用扫描显示函数
}

static void digitron2_softimer_set (void)
{
    am_softimer_init(&g_timer, timer_callback, NULL);       // 初始化软件定时器
    am_softimer_start(&g_timer, 5);                         // 启动5ms软件定时器
}

void digitron2_init_with_softimer (void)
{
    digitron2_init ();                                    // 板级初始化函数
    digitron2_softimer_set();                             // 自动扫描软件定时器
}

void digitron2_disp_code_set (uint8_t pos, uint8_t code)  // 传送段码到显示缓冲器函数
{
    g_digitron_disp_buf[pos] = code;
}

//uint8_t digitron_char_decode (char ch)                   // 传送字符到显示缓冲器函数
//{
//    int i;

//    for (i = 0; i < sizeof(segcodeTab) / sizeof(segcodeTab[0]); i++) {
//        if (segcodeTab[i][0] == ch) {
//            return segcodeTab[i][1];
//        }
//    }
//      return 0x00;                                       // 没有搜索，返回熄灭段码
//}

void digitron2_disp_char_set (uint8_t pos, char ch)       // 获取待显示字符的段码函数
{
    digitron2_disp_code_set(pos, digitron2_char_decode(ch));
}

void digitron2_disp_blink_set (uint8_t pos, am_bool_t isblink)
{
    if (isblink) {
        g_blink_flag |= (1 << pos);
    } else {
        g_blink_flag &= ~(1 << pos);
    } 
}


/* end of file */
