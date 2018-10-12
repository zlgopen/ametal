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
 * \brief 程序清单4.31
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
#include "am_softimer.h"
static am_softimer_t timer_sec;
static void timer_sec_callback (void *p_arg)
{
    static int sec = 0;                           // 秒计数器清0

    sec = (sec + 1) % 60;                         // 秒计数器+1
    digitron1_disp_num_set(0,sec / 10);            // 更新显示器的十位
    digitron1_disp_num_set(1,sec % 10);            // 更新显示器的个位
}

int am_main (void)
{
    digitron1_init_with_softimer ();
    am_softimer_init(&timer_sec, timer_sec_callback, NULL);    // 初始化定时器
    am_softimer_start(&timer_sec,1000);                        // 启动定时器，定时时间为1s
    digitron1_disp_num_set(0,0);                   // 显示器的十位清0
    digitron1_disp_num_set(1,0);                   // 显示器的个位清0
    while(1) {
    }
}



/* end of file */

