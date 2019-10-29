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
 * \brief RTC 例程
 *
 * - 操作步骤：
 *   1. 调用入口函数
 *
 * - 实验现象：
 *   1. 调试串口将每一秒的时间信息打印出来。
 *
 * \par 源代码
 * \snippet demo_std_rtc.c src_std_rtc
 *
 * \internal
 * \par Modification History
 * - 1.00 17-11-16  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_rtc
 * \copydoc demo_std_rtc.c
 */

/** [src_std_rtc] */
#include "ametal.h"
#include "am_rtc.h"
#include "am_delay.h"
#include "am_vdebug.h"

/** \brief 要设置的时间信息 */
am_local am_tm_t __g_current_time = {
    55,          /**< \brief 秒 */
    59,          /**< \brief 分 */
    16,          /**< \brief 时 */
    5,           /**< \brief 日 */
    8-1,         /**< \brief 月 */
    2017-1900,   /**< \brief 年 */
    4,           /**< \brief 周 */
};

/**
 * \brief 例程入口
 */
void demo_std_rtc_entry (am_rtc_handle_t rtc_handle)
{
    int     ret = AM_OK;
    int     tmp = 0;
    am_tm_t time;

    /* 设置时间 */
    ret= am_rtc_time_set(rtc_handle, &__g_current_time);
    if(ret == AM_OK){
        AM_DBG_INFO("set time success \r\n");
    } else {
        AM_DBG_INFO("set time fail \r\n");
    }

    while(1) {
        ret = am_rtc_time_get(rtc_handle, &time);
        if (time.tm_sec != tmp) {
            AM_DBG_INFO("%02d-%02d-%02d %02d:%02d:%02d %02d\n",
                        time.tm_year +1900,
                        time.tm_mon + 1,
                        time.tm_mday,
                        time.tm_hour,
                        time.tm_min,
                        time.tm_sec,
                        time.tm_wday);
        }
        tmp = time.tm_sec;
        am_mdelay(20);
    }
}
/** [src_std_rtc] */

/* end of file */
