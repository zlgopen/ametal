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
 * \brief 程序清单6.92
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-19  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "app_sys_time_show.h"
#include "am_hwconf_microport.h"


int am_main (void)
{
    am_microport_ds1302_rtc_inst_init();
    
    app_sys_time_show ();
    
    while(1) {
    }
}



/* end of file */

