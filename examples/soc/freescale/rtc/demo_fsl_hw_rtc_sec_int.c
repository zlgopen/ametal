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
 * \brief
 *
 * - 操作步骤：
 *      - PTC1引脚外接32.786kHz的时钟输入
 *      - 使用调试串口打印
 *
 * - 实验现象：
 *      - 串口打印出设置的时间
 *      - 以后每秒打印一次时间
 *      - LED0的状态每秒钟翻转一次
 *
 * \par 源代码
 * \snippet demo_fsl_hw_rtc_sec_int.c src_fsl_hw_rtc_sec_int
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_fsl_if_hw_rtc_sec_int
 * \copydoc demo_fsl_hw_rtc_sec_int.c
 */

/** [src_fsl_hw_rtc_sec_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_time.h"
#include "time.h"
#include "hw/amhw_fsl_rtc.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

/**
 * \brief  RTC 秒中断服务函数
 */
static void rtc_sec_isr (void *p_arg)
{
    am_led_toggle(LED0);
}


/**
 * \brief AMetal 应用程序入口
 */
void demo_fsl_hw_rtc_sec_int_entry (amhw_fsl_rtc_t *p_hw_rtc)
{
    uint32_t tmp = 0;

    /* 缓存时间的变量 */
    am_tm_t *p_time;
    time_t simple_time;

    /* 初始时间设置 */
    am_tm_t current_time = {
        58,   /** 秒                     */
        30,   /** 分                     */
        15,   /** 时                     */
        28,   /** 日                     */
        9,    /** 月                     */
        115,  /** 年                     */
        3,    /** 周                     */
        300,  /** 天 当年的第n天 */
        0,    /** 夏令时              */
    };

    /* 将日历时间转换为时间戳 */
    simple_time = mktime((struct tm*)&current_time);

    if (-1 == (int)simple_time) {
        AM_DBG_INFO("mktime failed!\n");
        return;
    }

    /* 关闭秒计数器 */
    amhw_fsl_rtc_time_counter_status_clr(p_hw_rtc);

    /* 分频计数器、秒计数器、校准寄存器清0 */
    amhw_fsl_rtc_prescaler_set(p_hw_rtc, 0x00);
    amhw_fsl_rtc_alarm_set(p_hw_rtc, 0x00);
    amhw_fsl_rtc_compensate_interval_set(p_hw_rtc, 0x00);
    amhw_fsl_rtc_compensate_value_set(p_hw_rtc, 0x00);
    amhw_fsl_rtc_current_compensate_counter_set(p_hw_rtc, 0x00);
    amhw_fsl_rtc_current_compensate_value_set(p_hw_rtc, 0x00);

    /* 设置秒计数器 */
    amhw_fsl_rtc_second_set(p_hw_rtc, simple_time);

    /* 使能秒中断 */
    amhw_fsl_rtc_sec_int_enable(p_hw_rtc);

    /* 连接RTC秒中断并使能 */
    am_int_connect(INUM_RTC_SEC, rtc_sec_isr, NULL);
    am_int_enable(INUM_RTC_SEC);

    /* 使能秒计数器 */
    amhw_fsl_rtc_time_counter_status_set(p_hw_rtc);

    /* 打印设置的时间 */
    AM_DBG_INFO("the time you set is :%02d-%02d-%02d %02d:%02d:%02d\n",
                current_time.tm_year + 1900,
                current_time.tm_mon + 1,
                current_time.tm_mday,
                current_time.tm_hour,
                current_time.tm_min,
                current_time.tm_sec);

    while (1) {
        simple_time = amhw_fsl_rtc_second_get(p_hw_rtc);
        p_time = (am_tm_t *)localtime(&simple_time);

        if ((NULL != p_time) && (tmp != p_time->tm_sec)) {
            AM_DBG_INFO("current time: ");

            AM_DBG_INFO("%02d-%02d-%02d %02d:%02d:%02d\n",
                        p_time->tm_year + 1900,
                        p_time->tm_mon + 1,
                        p_time->tm_mday,
                        p_time->tm_hour,
                        p_time->tm_min,
                        p_time->tm_sec);
        }

        tmp = p_time->tm_sec;
    }
}

/** [src_fsl_hw_rtc_sec_int] */

/* end of file */
