/*******************************************************************************
*                      AMetal
*                ----------------------------
*                innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:   http://www.zlg.cn/
* e-mail:    ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s rtc driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-19  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_rtc.h"
#include "am_vdebug.h"
#include "am_delay.h" 

void app_sys_time_show (void)
{
    // 设定时间初始值为2016年8月26日09:32:30
    am_tm_t tm = {30, 32, 9, 26, 8 - 1, 2016 - 1900, 0, 0, -1};
    
    // 设置系统时间为2016年8月26日09:32:30
    am_tm_set(&tm);
    while(1) {
        am_tm_get(&tm);
        AM_DBG_INFO("%04d-%02d-%02d %02d:%02d:%02d \r\n",
                     tm.tm_year+1900, tm.tm_mon + 1, tm.tm_mday,
                     tm.tm_hour,      tm.tm_min,     tm.tm_sec);
        am_mdelay(1000);
    }
}


/* end of file */
