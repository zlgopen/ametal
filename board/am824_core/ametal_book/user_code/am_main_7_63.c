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
 * \brief 程序清单7.63
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-21  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_buzzer.h"
#include "am_led.h"
#include "am_input.h"
#include "am_digitron_disp.h"
#include "am_temp.h"
#include "am_hwconf_lm75.h"
#include "am_hwconf_miniport.h"
#include "string.h"

static uint8_t g_temp_high = 30;            // 温度上限值，初始为30度
static uint8_t g_temp_low  = 25;            // 温度下限值，初始为28度
static uint8_t adj_state   = 0;             // 0-正常状态，1-调节上限状态, 2-调节下限状态
static uint8_t adj_pos;                     // 当前调节的位，切换为调节模式时，初始为调节个位

static void __digitron_disp_num (int num)
{
    char buf[3];
    am_snprintf(buf, 3, "%2d", num);
    am_digitron_disp_str(0, 0, strlen(buf), buf);
}

static void key_state_process (void)       // 状态处理函数，KEY0
{
    adj_state = (adj_state + 1) % 3;       // 状态切换，0 ~ 2
    if (adj_state == 1) {
        // 状态切换到调节上限状态
        am_led_on(0);
        am_led_off(1);
        adj_pos = 1;
        am_digitron_disp_blink_set(0, adj_pos, AM_TRUE);    // 调节位个位闪烁
        __digitron_disp_num(g_temp_high);                   // 显示温度上限值
    } else if (adj_state == 2) {
        // 状态切换到调节下限状态
        am_led_on(1);
        am_led_off(0);
        am_digitron_disp_blink_set(0, adj_pos, AM_FALSE);   // 当前调节位停止闪烁
        adj_pos = 1;                                        // 调节位恢复为个位
        am_digitron_disp_blink_set(0, adj_pos, AM_TRUE);
        __digitron_disp_num(g_temp_low);                    // 显示温度下限值
    } else {
        // 切换为正常状态
        am_led_off(0);
        am_led_off(1);
        am_digitron_disp_blink_set(0, adj_pos, AM_FALSE);   // 当前调节位停止闪烁
        adj_pos = 1;                                        // 调节位恢复为个位
    }
}

#define VAL_ADJ_TYPE_ADD    1
#define VAL_ADJ_TYPE_SUB    0

static void key_val_process(uint8_t type)                  // 调节值设置函数（1-加，0-减）
{
    uint8_t num_single    =     0;                         // 调节数值时，临时记录个位调节
    uint8_t num_ten     =     0;                           // 调节数值时，临时记录十位调节

    if (adj_state == 0)                                    // 正常状态，不允许调节
        return;
    if (adj_state == 1) {
        num_single = g_temp_high % 10;                     // 调节上限值
        num_ten    = g_temp_high / 10;
    } else if (adj_state == 2){
        num_single = g_temp_low % 10;                      // 调节下限值
        num_ten    = g_temp_low / 10;
    }
    if (type == VAL_ADJ_TYPE_ADD) {                        // 加1操作
        if (adj_pos == 1) {
            num_single = (num_single + 1) % 10;            // 个位加1，0 ~ 9
        } else {
            num_ten   = (num_ten + 1) % 10;                // 十位加1，0 ~ 9
        }
    } else {                                               // 减1操作
        if (adj_pos == 1) {
            num_single = (num_single - 1 + 10) % 10;       // 个位减1，0 ~ 9
        } else {
            num_ten   = (num_ten - 1 + 10) % 10;           // 十位减1，0 ~ 9
        }
    }

    if (adj_state == 1) {
        if (num_ten * 10 + num_single >= g_temp_low) {
            g_temp_high = num_ten * 10 + num_single;       // 确保是有效的设置
        } else {
            num_ten    = g_temp_high / 10;                 // 无效的设置，值不变
            num_single = g_temp_high % 10;
        }
        __digitron_disp_num(g_temp_high);                  // 显示温度上限值
    } else if (adj_state == 2) {
        if (num_ten * 10 + num_single <= g_temp_high) {
            g_temp_low = num_ten * 10 + num_single;        // 确保是有效的设置
        }
        __digitron_disp_num(g_temp_low);                   // 显示温度下限值
    }
}

static void key_pos_process(void)                           // 调节位切换
{
    if (adj_state != 0) {
        // 当前是在调节模式中才允许切换调节位
        am_digitron_disp_blink_set(0, adj_pos, AM_FALSE);
        adj_pos = !adj_pos;
        am_digitron_disp_blink_set(0, adj_pos, AM_TRUE);
    }
}

static void key_callback (void *p_arg,int key_code, int key_state, int keep_time)
{
    if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
        switch (key_code) {
        case KEY_0:                       // 调节状态切换
            key_state_process();
            break;
        case KEY_1:                       // 当前调节位加1
            key_val_process(VAL_ADJ_TYPE_ADD);
            break;
        case KEY_2:                      // 切换当前调节位
            key_pos_process();
            break;
        case KEY_3:                      // 当前调节位减1
            key_val_process(VAL_ADJ_TYPE_SUB);
            break;
        default:
            break;
        }
    }
}

int am_main (void)
{
    am_temp_handle_t         temp_handle = am_temp_lm75_inst_init();; // 温度传感器句柄
    am_input_key_handler_t   key_handler;
    int32_t                  temp;

    // 初始化，并设置8段ASCII解码
    am_miniport_view_key_inst_init();
    am_digitron_disp_decode_set(0, am_digitron_seg8_ascii_decode);
    am_input_key_handler_register(&key_handler, key_callback, NULL);
    while(1) {
        // 温度读取模块,正常模式下，显示温度值，500ms执行一次，LED闪烁
        if (adj_state == 0) {
            am_temp_read(temp_handle, &temp);
            if (temp < 0) {
                temp = -1 * temp;            // 温度为负时，也只显示温度数值
            }
            temp = temp / 1000;              //  temp_cur 只保留温度整数部分
            __digitron_disp_num(temp);
            if (temp > g_temp_high || temp < g_temp_low ) {
                am_buzzer_on();
            } else {
                am_buzzer_off();
            }
            am_led_toggle(0);
            am_led_toggle(1);
            am_mdelay(500);
        }
    }
}



/* end of file */

