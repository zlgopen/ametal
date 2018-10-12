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
 * \brief 程序清单4.26
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
    int sec     = 0;                                 // 秒计数器清0
    int i       = 0;

    digitron1_init();
    digitron1_disp_num_set(0, 0);                    // 显示器的十位清0
    digitron1_disp_num_set(1, 0);                    // 显示器的个位清0
    digitron1_disp_blink_set(1, AM_TRUE);            // com1（秒计数器个位）闪烁
    while(1) {
        digitron1_disp_scan();                       // 每隔5ms调用动态扫描函数
        am_mdelay(5);
        i++;
        if (i == 200) {                              // 循环200次即运行时间达到1s
            i = 0;
            sec = (sec + 1) % 60;                    // 秒计数器+1
            digitron1_disp_num_set(0, sec / 10);     // 更新显示器的十位
            digitron1_disp_num_set(1, sec % 10);     // 更新显示器的个位
        }
    }
}



/* end of file */

