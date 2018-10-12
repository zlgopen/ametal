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
 * \brief 程序清单6.78
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-19  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "app_alarm_clk_test.h"
#include "am_hwconf_microport.h"

int am_main (void)
{
    am_rtc_handle_t       rtc_handle   	= am_microport_rx8025t_rtc_inst_init();
    am_alarm_clk_handle_t alarm_handle 	= am_microport_rx8025t_alarm_clk_inst_init();
    
    app_alarm_clk_test(rtc_handle, alarm_handle);
    
    while(1) {
    }
}



/* end of file */

