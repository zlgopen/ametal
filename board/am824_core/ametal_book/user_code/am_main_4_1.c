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
 * \brief 程序清单4.1
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_led.h"
#include "am_delay.h"

/**
 * \brief AMetal 应用程序入口
 */
void am_main (void)
{
    while(1) {
        am_led_toggle(0);           			// 翻转LED0的状态
        am_mdelay(500);             			// 延时500ms
	    }
}

/* end of file */

