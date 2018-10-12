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
 * \brief 程序清单4.52
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "digitron1.h"
#include "am_delay.h"
int am_main (void)
{
    int i       = 0;
    int sec     = 0;                                  // 秒计数器清0

    digitron1_hc595_init();                            // 板级初始化
    digitron1_disp_num_set(0, 0);                      // 秒计数器的十位清0
    digitron1_disp_num_set(1, 0);                      // 秒计数器的个位清0
    while(1) {
        digitron1_hc595_disp_scan();                   // 每隔5ms调用动态扫描函数
        am_mdelay(5);
        i ++;
        if (i == 200) {                               // 循环200次，耗时1s
            i = 0;
            sec = (sec + 1) % 60;                     // 秒计数器+1
            digitron1_disp_num_set(0,sec / 10);        // 更新秒计数器的十位
            digitron1_disp_num_set(1,sec % 10);        // 更新秒计数器的个位
        }
    }
}


/* end of file */

