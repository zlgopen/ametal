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
 * \brief 程序清单4.10
 *
 * \note 该历程需要用到miniport拓展板
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_board.h"
#include "buzzer.h"

/**
 * \brief AMetal 应用程序入口
 */
int am_main (void)
{
    buzzer_init();         // 蜂鸣器初始化，默认为1KHz频率
    while (1) {               
        buzzer_on();       // 打开蜂鸣器 
        am_mdelay(1000);    // 延时100ms 
        buzzer_off();      // 关闭蜂鸣器 
        am_mdelay(1000);    // 延时100ms 
    }
}

/* end of file */

