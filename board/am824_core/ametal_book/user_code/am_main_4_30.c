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
 * \brief 程序清单4.30
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
    int sec      = 0;                        // 秒计数器清0

    digitron1_init_with_softimer ();          // 板级初始化，5ms软件定时器
    digitron1_disp_num_set(0, 0);             // 显示器的十位清0
    digitron1_disp_num_set(1, 0);             // 显示器的个位清0
    digitron1_disp_blink_set(1, AM_TRUE);     // com1闪烁
    while(1) {
        am_mdelay(1000);
        sec = (sec + 1) % 60;                // 秒计数器+1
        digitron1_disp_num_set(0,sec / 10);   // 更新显示器的十位
        digitron1_disp_num_set(1,sec % 10);   // 更新显示器的个位
    }
}


/* end of file */

