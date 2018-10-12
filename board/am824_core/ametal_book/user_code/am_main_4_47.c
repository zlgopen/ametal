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
 * \brief 程序清单4.47
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "matrixkey.h"
#include "digitron1.h"
#include "am_delay.h"

static uint8_t g_disp_num = 0;

static void key_process (uint8_t code)
{
    static uint8_t adj_state = 0;                           // 正常状态为0，调节状态为1
    static uint8_t adj_pos;                                 // 闪烁位，初始化时个位闪烁
    uint8_t        num_single;                              // 个位计数器
    uint8_t        num_ten;                                 // 十位计数器

    switch (code) {
    case 0:  
            adj_state = !adj_state;                         // 状态翻转
            if (adj_state == 1) {                           // 切换到调节状态
                digitron1_disp_blink_set(1, AM_TRUE);       // 个位闪烁
            } else {                                        // 切换到正常状态
                digitron1_disp_blink_set(adj_pos, AM_FALSE);// 停止闪烁
                adj_pos = 1;
            }
           break;
    case 1:                                                 // 闪烁位加1
            if (adj_state == 1) {    
                num_single = g_disp_num % 10;
                num_ten   = g_disp_num / 10;
                if (adj_pos == 1) {
                    num_single = (num_single + 1) % 10;     // 个位加1，0 ~ 9
                } else {
                    num_ten   = (num_ten + 1) % 10;         // 十位加1，0 ~ 9
                }
                g_disp_num = num_ten * 10 + num_single;
                digitron1_disp_num_set(0,num_ten);          // 更新显示器的十位
                digitron1_disp_num_set(1,num_single);       // 更新显示器的个位
        }
        break;
    case 2:                                                // 切换调节位
        if (adj_state == 1) {
                digitron1_disp_blink_set(adj_pos, AM_FALSE);
                adj_pos = !adj_pos;
                digitron1_disp_blink_set(adj_pos, AM_TRUE);
            }
            break;
    case 3:                                                  // 闪烁位减1
            num_single = g_disp_num % 10;
            num_ten   = g_disp_num / 10;
            if (adj_state == 1) {
                if (adj_pos == 1) {
                    num_single = (num_single - 1 + 10) % 10; // 个位减1，0 ~ 9
                } else {                                     
                    num_ten   = (num_ten - 1 + 10) % 10;     // 十位减1，0 ~ 9
                }
            }
            g_disp_num = num_ten * 10 + num_single;
            digitron1_disp_num_set(0,num_ten);                // 更新显示器的十位
            digitron1_disp_num_set(1,num_single);             // 更新显示器的个位
            break;
    default:
            break;
    }
}

int am_main(void)
{
    uint8_t key_return;

    matrixkey_init();
    digitron1_init();
    digitron1_disp_num_set(0,g_disp_num / 10);                 // 更新显示器的十位
    digitron1_disp_num_set(1,g_disp_num % 10);                 // 更新显示器的个位
    while(1) {
        key_return = matrixkey_scan_with_digitron(digitron1_disp_scan);

        if (key_return!= 0xFF) {
            // 有按键事件产生
            key_process(key_return);
        }
				am_mdelay(5);
    } 
}


/* end of file */

