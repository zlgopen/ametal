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
 * \brief book`s rtc driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-19  adw, first implementation
 * \endinternal
 */

#ifndef __APP_ALARM_CLK_TEST_H
#define __APP_ALARM_CLK_TEST_H

#include "ametal.h"
#include "am_rtc.h"
#include "am_alarm_clk.h"

void app_alarm_clk_test (am_rtc_handle_t         rtc_handle,
                         am_alarm_clk_handle_t   alarm_handle);




#endif /* __APP_ALARM_CLK_TEST_H */

/* end of file */
