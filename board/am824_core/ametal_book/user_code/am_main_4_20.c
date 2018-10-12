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
 * \brief 程序清单4.20
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_delay.h"
#include "digitron1.h"
int am_main (void)
{
    int sec = 0;                                 // 秒计数器清0
    int i   = 0;
    int j   = 0;
    digitron1_init();                            // 初始化定时器，实现自动扫描显示
    digitron1_disp_num_set(0, 0);                // 显示器的十位清0
    digitron1_disp_num_set(1, 0);                // 显示器的个位清0
    while(1) {
        for (i = 0; i < 2; i++) {
            digitron1_disp_num_set(1, sec % 10); // 个位显示
            for (j = 0; j < 50; j++) {           // 扫描50次，耗时250ms
                     digitron1_disp_scan();
                     am_mdelay(5);
            }
            digitron1_disp_code_set(1, 0xFF);    // 后250ms个位熄灭
            for (j = 0; j < 50; j++) {           // 扫描50次，耗时250ms
                     digitron1_disp_scan();
                     am_mdelay(5);
            }
        }
        sec = (sec + 1) % 60;                    // 秒计数器+1
        digitron1_disp_num_set(0, sec / 10);     // 更新显示器的十位
        digitron1_disp_num_set(1, sec % 10);     // 更新显示器的个位
    }
}


/* end of file */

