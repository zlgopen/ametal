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
 * \brief 程序清单4.55
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "buzzer.h"
#include "led.h"
#include "matrixkey.h"
#include "digitron1.h"
#include "lm75.h"
#include "am_delay.h"
static uint8_t g_temp_high  = 30;          // 温度上限值，初始为30度
static uint8_t g_temp_low   = 28;          // 温度下限值，初始为28度
static uint8_t adj_state    = 0;           // 0-正常状态，1-调节上限状态, 2-调节下限状态
static uint8_t adj_pos;                    // 当前调节的位，切换为调节模式时，初始为调节个位

static void key_state_process (void)       // 状态处理函数，KEY0
{
    adj_state = (adj_state + 1) % 3;       // 状态切换，0 ~ 2
    if (adj_state == 1) {
        // 状态切换到调节上限状态
        led_on(0);        led_off(1);
        adj_pos = 1;
        digitron1_disp_blink_set(adj_pos,AM_TRUE);     // 调节位个位闪烁
        digitron1_disp_num_set(0, g_temp_high / 10);   // 显示温度上限值十位
        digitron1_disp_num_set(1, g_temp_high % 10);   // 显示温度上限值个位
    } else if (adj_state == 2) {
        // 状态切换到调节下限状态
        led_on(1);        led_off(0);
        digitron1_disp_blink_set(adj_pos, AM_FALSE);   // 当前调节位停止闪烁
        adj_pos = 1;                                  // 调节位恢复为个位
        digitron1_disp_blink_set(adj_pos, AM_TRUE);
        digitron1_disp_num_set(0,g_temp_low / 10);
        digitron1_disp_num_set(1,g_temp_low % 10);
    } else {
        // 切换为正常状态
        led_off(0);        led_off(1);    
        digitron1_disp_blink_set(adj_pos, AM_FALSE);  // 当前调节位停止闪烁
        adj_pos = 1;                                 // 调节位恢复为个位
    }
}

#define VAL_ADJ_TYPE_ADD        1
#define VAL_ADJ_TYPE_SUB        0

static void key_val_process(uint8_t type)            // 调节值设置函数（1-加，0-减）
{                                                   
    uint8_t num_single;                              // 调节数值时，临时记录个位调节
    uint8_t num_ten;                                 // 调节数值时，临时记录十位调节

    if (adj_state == 0)                              // 正常状态，不允许调节
        return;
    if (adj_state == 1) {
        num_single = g_temp_high % 10;               // 调节上限值
        num_ten   = g_temp_high / 10;
    } else if (adj_state == 2){
        num_single = g_temp_low % 10;                // 调节下限值
        num_ten   = g_temp_low / 10;
    }
    if (type == 1) {                                 // 加1操作
        if (adj_pos == 1) {
            num_single = (num_single + 1) % 10;      // 个位加1，0 ~ 9
        } else {
            num_ten   = (num_ten + 1) % 10;          // 十位加1，0 ~ 9
        }
        } else {                                     // 减1操作
        if (adj_pos == 1) {
            num_single = (num_single - 1 + 10) % 10; // 个位减1，0 ~ 9
        } else {
            num_ten   = (num_ten - 1 + 10) % 10;     // 十位减1，0 ~ 9
        }
    }
    if (adj_state == 1) {
        if (num_ten * 10 + num_single >= g_temp_low) {
            g_temp_high = num_ten * 10 + num_single;    // 确保是有效的设置
        } else {
            num_ten   = g_temp_high / 10;               // 无效的设置，值不变
            num_single = g_temp_high % 10; 
        }
    } else if (adj_state == 2){
        if (num_ten * 10 + num_single <= g_temp_high) {
            g_temp_low = num_ten * 10 + num_single;     // 确保是有效的设置
        } else {
            num_ten   = g_temp_low / 10;                // 无效的设置，值不变
            num_single = g_temp_low % 10;
        }
    }
    digitron1_disp_num_set(0, num_ten);                  // 更新显示器的十位
    digitron1_disp_num_set(1, num_single);               // 更新显示器的个位
}

static void key_pos_process(void)                       // 调节位切换
{
    if (adj_state != 0) {
        // 当前是在调节模式中才允许切换调节位
        digitron1_disp_blink_set(adj_pos, AM_FALSE);
        adj_pos = !adj_pos;
        digitron1_disp_blink_set(adj_pos, AM_TRUE);
    }
}

static void key_process (uint8_t code)
{
    switch (code) {
    case 0: 
        key_state_process();                          // 调节状态切换
        break;
        case 1:                                       // 当前调节位加1
        key_val_process(VAL_ADJ_TYPE_ADD);
        break;
        case 2:                                       // 切换当前调节位
        key_pos_process();
        break;
    case 3:                                           // 当前调节位减1
        key_val_process(VAL_ADJ_TYPE_SUB);
        break;
        default:
        break;
    }
}

int am_main(void)
{
    uint8_t key_code;
    int16_t temp;                                     // 保存温度值
    int i = 0;
    // 如果用GPIO驱动数码管，则只要用digitron_init()与digitron_disp_scan()替换相应的函数即可
    buzzer_init();
    led_init();
    matrixkey_init();
    digitron1_hc595_init();                            // digitron_init();
    lm75_init();
    while(1) {
        // 温度读取模块,正常模式下，显示温度值，500ms执行一次
            if (adj_state == 0) {
                temp = lm75_read();
                if (temp < 0) {
                temp = -1 * temp;                     // 温度为负时，也只显示温度数值
                }
                temp = temp >> 8;                     //  temp只保留温度整数部分
                digitron1_disp_num_set(0, temp / 10);
                digitron1_disp_num_set(1, temp % 10);
                if (temp > g_temp_high || temp < g_temp_low) {
                buzzer_on();
                } else {
                buzzer_off();
                }
            }
            for (i = 0; i < 100; i++) {
            // 矩阵键盘每隔5ms扫描一次，100次即为500ms
            key_code = matrixkey_scan_with_digitron(digitron1_hc595_disp_scan);
            if (key_code != 0xFF) {
                if ((key_code & 0x80) == 0) {      // 按键按下时，蜂鸣器“嘀”一声
                    buzzer_beep_async(100);        // 异步方式
                    key_process(key_code);         // 有按键事件产生
                }
            }
						// 矩阵键盘和数码管每隔5ms扫描一次 100 次 即为 500ms
            if ((adj_state == 0) && (((i + 1) % 50) == 0)) {
                led_toggle(0);    led_toggle(1);
            }
            am_mdelay(5);
        }
    }
}




/* end of file */

