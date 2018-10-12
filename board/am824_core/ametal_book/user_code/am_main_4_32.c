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
 * \brief 程序清单4.32
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_softimer.h"
#include "am_delay.h"
#include "am_vdebug.h"
void timer_callback (void *p_arg)
{
    // 定时时间到，调用回调函数执行用户自定义的任务
}

static am_softimer_t timer;                            // 定义一个定时器实例 
int am_main (void)
{
    am_softimer_init(&timer,timer_callback, NULL);     // 初始化定时器
    am_softimer_start(&timer, 5);                      // 启动定时器，定时时间为5ms
      // ...
    am_softimer_stop(&timer);                          // 停止定时器
    while(1) {
    }
}

/* end of file */

