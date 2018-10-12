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
#include "am_buzzer.h"
#include "app_alarm_clk_test.h"

static void alarm_callback (void *p_arg)
{
    am_buzzer_beep_async(60 * 1000);	// 蜂鸣器鸣叫1分钟
}

void app_alarm_clk_test (am_rtc_handle_t        rtc_handle,
                         am_alarm_clk_handle_t  alarm_handle)
{
   // 设定时间初始值为2016年8月26日09:32:30
   am_tm_t tm  = {30, 32, 9, 26, 8 - 1, 2016 - 1900, 0, 0, -1};
	 
	 // 设置闹钟时间
   am_alarm_clk_tm_t  alarm_tm = {
      34,         						// 34 分
      9,          						// 09 时
      AM_ALARM_CLK_EVERYDAY,  // 每天闹钟均有效
   };
   // 设置时间值为每天的09:34
   am_alarm_clk_time_set(alarm_handle, &alarm_tm);	

		
		
   // 设置时间为2016年8月26日09:32:30
    am_rtc_time_set(rtc_handle, &tm);
	 
		am_alarm_clk_time_set(alarm_handle, &alarm_tm);
		am_alarm_clk_callback_set(alarm_handle, alarm_callback, NULL);
	 
		am_alarm_clk_on(alarm_handle);
   while(1) {
      am_rtc_time_get(rtc_handle, &tm);
      AM_DBG_INFO("%04d-%02d-%02d %02d:%02d:%02d \r\n",
              tm.tm_year+1900, tm.tm_mon + 1, tm.tm_mday,
              tm.tm_hour,    tm.tm_min,    tm.tm_sec);
      am_mdelay(1000);
   }
}


/* end of file */
