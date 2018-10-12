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
 * \brief 程序清单5.75
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "digitron1.h"
#include "am_delay.h"
int am_main (void)
{
     int sec  = 0;                                    // 秒计数器清0

    digitron1_hc595_init_with_softimer();              // 板级初始化
    digitron1_disp_num_set(0, 0);                      // 秒计数器的十位清0
    digitron1_disp_num_set(1, 0);                      // 秒计数器的个位清0
    while(1) {
        am_mdelay(1000);
        sec = (sec + 1) % 60;                         // 秒计数器+1
        digitron1_disp_num_set(0, sec / 10);           // 更新秒计数器的十位
        digitron1_disp_num_set(1, sec % 10);           // 更新秒计数器的个位
    }
}



/* end of file */

