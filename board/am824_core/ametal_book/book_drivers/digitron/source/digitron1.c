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
#include "digitron1.h"
#include "am_lpc82x.h"
#include "am_softimer.h"
#include "hc595.h"

static const int g_digitron_com[2] = {PIO0_17,PIO0_23};    // com0、com1

// 数组下标0 ~ 7分别对应数码管的a, b, c, d, e, f, g, dp
static const int g_digitron_seg[8]={PIO0_8,PIO0_9,PIO0_10,PIO0_11,PIO0_12,PIO0_13,PIO0_14,PIO0_15};

static const uint8_t g_segcode_list[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
static uint8_t g_digitron_disp_buf[2];                    // 显示缓冲区

static am_softimer_t g_timer;                             // 定义定时器实例，用于自动扫描显示

static uint8_t g_blink_flag = 0;                          // 闪烁标志位

static void digitron1_segcode_set (uint8_t code)           // 段码传送函数
{
    int i;

    for (i = 0; i < 8; i++)
        am_gpio_set(g_digitron_seg[i], ((code & (1 << i)) >> i));  // 取出i位的值，设置到相应管脚
}

static void digitron1_com_sel (uint8_t pos)                       // 位码传送函数
{
    int i;

    for (i = 0; i < 2; i++)                               // 最多只有两个数码管
        am_gpio_set(g_digitron_com[i], !(pos == i)); // 当i与pos相等时，则输出0，否则输出1
}

void digitron1_init (void)                                 // 板级初始化函数
{
    int i;

    for (i = 0; i < 2; i++)
        // 将com端对应的引脚设置为输出，并初始化为高电平
        am_gpio_pin_cfg(g_digitron_com[i], AM_GPIO_OUTPUT_INIT_HIGH);
    for (i = 0; i < 8; i++)
        // 将8个段码对应的引脚设置为输出，并初始化为高电平
        am_gpio_pin_cfg(g_digitron_seg[i], AM_GPIO_OUTPUT_INIT_HIGH);
    digitron1_disp_code_set(0,0xFF);                       // 初始设置缓冲区中的段码值为无效值
    digitron1_disp_code_set(1,0xFF);                       // 初始设置缓冲区中的段码值为无效值
}

void digitron1_disp_scan (void)
{
    static uint8_t  pos = 0;
    static uint8_t  cnt = 0;                          // 每执行一次（5ms）加1

    digitron1_segcode_set(0xFF);                       // 消影，熄灭全部数码管
    digitron1_com_sel(pos);                            // 当前显示位
    if (((g_blink_flag & (1 << pos)) && (cnt <= 49)) || ((g_blink_flag & (1 << pos)) == 0))
          digitron1_segcode_set(g_digitron_disp_buf[pos]);     // 传送正常显示段码
    cnt = (cnt + 1) % 100;                            // 记录500ms，0 ~ 99循环
    pos = (pos + 1) % 2;                              // 切换到下一个显示位
}

static void timer_callback(void *p_arg)
{
    digitron1_disp_scan();                             // 每隔5ms调用扫描函数
}

static void digitron1_softimer_set (void)
{
    am_softimer_init(&g_timer, timer_callback, NULL); // 初始化软件定时器
    am_softimer_start(&g_timer, 5);                   // 启动5ms软件定时器
}

void digitron1_init_with_softimer (void)
{
    digitron1_init ();                                // 板级初始化函数
    digitron1_softimer_set();                          // 自动扫描软件定时器
}

void digitron1_disp_code_set (uint8_t pos, uint8_t code)   // 传送段码到显示缓冲区函数
{
    g_digitron_disp_buf[pos] = code;
}

void digitron1_disp_num_set (uint8_t pos, uint8_t num)     // 传送数字0 ~ 9到显示缓冲区函数
{
    if (num <= 9) 
        g_digitron_disp_buf[pos] = g_segcode_list[num];
}

uint8_t digitron1_num_decode (uint8_t num)                 // 获取待显示数字的段码函数
{
    return g_segcode_list[num];
}

void digitron1_disp_blink_set (uint8_t pos, am_bool_t isblink)    // 闪烁处理函数
{
    if (isblink) {
        g_blink_flag |= (1 << pos);
    } else {
        g_blink_flag &= ~(1 << pos);
    } 
}



static void digitron1_hc595_segcode_set (uint8_t code)				// 传送段码到显示缓冲区函数
{
    hc595_send_data(code);							    // 串行输入并行输出数据
}

void digitron1_hc595_init (void)							// 板级初始化函数
{
    int i;

    for (i = 0; i < 2; i++) {
		// 将com端引脚设置为输出，并初始化为高电平
		am_gpio_pin_cfg(g_digitron_com[i], AM_GPIO_OUTPUT_INIT_HIGH);
    }
    digitron1_disp_code_set(0, 0xFF);                 // 初始设置缓冲区中的段码值为无效值
    digitron1_disp_code_set(1, 0xFF);                 // 初始设置缓冲区中的段码值为无效值
    hc595_init();
}

void digitron1_hc595_disp_scan (void)			// 动态扫描显示函数
{
    static uint8_t pos = 0;								// 初始化com变量
    static uint8_t cnt = 0;  							// 扫描计数器变量

    digitron1_hc595_segcode_set(0xFF);         			// 消影，熄灭全部数码管
    digitron1_com_sel(pos);                     			// 当前显示位
    if (((g_blink_flag & (1 << pos)) && (cnt <= 49)) ||((g_blink_flag & (1 << pos)) == 0)) {
          //digitron_hc595_segcode_set(g_digitron_disp_buf[pos]);	// 传送正常显示段码
          hc595_send_data_async(g_digitron_disp_buf[pos], NULL, NULL); // 程序清单4.79 修改内容
	}
    cnt = (cnt + 1) % 100;             					// cnt扫描次数加1，循环0 ~ 99
    pos = (pos + 1) % 2;                    		// 切换到下一个显示位
}

static void timer_hc595_callback(void *p_arg)
{
    digitron1_hc595_disp_scan();                 // 每隔5ms调用扫描函数
}

static void digitron1_hc595_softimer_set (void)
{
    am_softimer_init(&g_timer, timer_hc595_callback, NULL);
    am_softimer_start(&g_timer, 5);  			// 启动定时器，每5ms调用一次回调函数
}

void digitron1_hc595_init_with_softimer (void)
{
    digitron1_hc595_init();
    digitron1_hc595_softimer_set();
}


/* end of file */
