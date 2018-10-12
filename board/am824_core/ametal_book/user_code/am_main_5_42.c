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
 * \brief 程序清单5.42
 *
 * \note 该历程需要用到miniport拓展板
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "digitron2.h"
#include "am_delay.h"
int am_main (void)
{
    int i = 0, sec = 0;                               // 秒计数器清0

    digitron2_init();
    digitron2_disp_char_set(0, '0');                   // 显示器十位清0
    digitron2_disp_char_set(1, '0');                   // 显示器个位清0
    while(1) {
        digitron2_disp_scan();                         // 每隔5ms调用扫描函数
        am_mdelay(5);
         i++;
         if (i == 200) {                              // 每次循环5ms，循环200次，即为1s
            i = 0;
            sec = (sec + 1) % 60;                     // 秒计数器+1
            digitron2_disp_char_set(0,sec / 10 + '0'); // 更新显示器的十位
            digitron2_disp_char_set(1,sec % 10 + '0'); // 更新显示器的个位
         }
     }
}

/* end of file */

