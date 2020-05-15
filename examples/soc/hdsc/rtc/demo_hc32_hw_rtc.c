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
 * \brief RTC 例程，通过标准接口和中间驱动层函数实现
 *
 * - 实验现象：
 *   1. 每1秒发生一次周期中断，串口并打印出当前时间信息。
 *   2. 周四的10点21分15秒产生闹钟中断，打印一行"alarm clock int!"提示信息。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32l19x_std_rtc.c src_hc32l19x_std_rtc
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-14  licl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l19x_std_rtc
 * \copydoc demo_hc32l19x_std_rtc.c
 */

/** [src_hc32l19x_std_rtc] */
#include "ametal.h"
#include "am_rtc.h"
#include "am_clk.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_hc32.h"
#include "am_hc32_rtc.h"
#include "demo_hc32_entries.h"

/** \brief 当前时间 */
static am_tm_t     __g_time;

/** \brief 指示是否获取到时间 */
static am_bool_t   __g_get_flag = AM_FALSE;

/** \brief 初始化时间 */
static am_tm_t     __g_init_time = {
    00,             /** 秒 */
    21,             /** 分 */
    10,             /** 时 */
    14,             /** 日 */
    5-1,            /** 月 */
    2020-1970,      /** 年 */
    4,              /** 周 */
    135,            /** 当年的第几天 */
    0,              /** 夏令时 */
};

am_local void __hc32_plfm_rtc_init()
{
    /* 外部晶振驱动能力选择 */
    amhw_hc32_rcc_xtl_xtal_driver_set(AMHW_HC32_XTL_XTAL_DRIVER_DEFAULT);

    /* XTL晶振振荡幅度的调整 */
    amhw_hc32_rcc_xtl_xtal_amplitude_set(
    AMHW_HC32_XTL_XTAL_AMPLITUDE_BIG_DEFAULT);

    /* 外部高速时钟XTL稳定时间选择 */
    amhw_hc32_rcc_xtl_waittime_set(AMHW_HC32_XTL_WAITTIME_16384);

    amhw_hc32_rcc_set_start(0x5A5A);
    amhw_hc32_rcc_set_start(0xA5A5);
    amhw_hc32_rcc_xtl_enable();

    /* 等待稳定*/
    while(amhw_hc32_rcc_xtl_state_get() == AM_FALSE);

    am_clk_enable(CLK_RTC);

    /* RTC 时钟源选择 */
    amhw_hc32_rtc_clk_src_sel(HC32_RTC, AMHW_HC32_RTC_CLK_SRC_XTL_32768Hz);
}

/**
 * \brief 周期回调函数
 */
am_local void __rtc_period_callback (void *p_arg)
{
    amhw_hc32_rtc_t *p_hw_rtc = (amhw_hc32_rtc_t *)p_arg;

    /* 读写模式 */
    amhw_hc32_rtc_mode_sel(p_hw_rtc, AMHW_HC32_RTC_MODE_WRITEREAD);
    while(amhw_hc32_rtc_writeread_flag_get(p_hw_rtc) == AM_FALSE);

    __g_time.tm_sec  = amhw_hc32_rtc_sec_get(p_hw_rtc);
    __g_time.tm_min  = amhw_hc32_rtc_min_get(p_hw_rtc);
    __g_time.tm_hour = amhw_hc32_rtc_hour_get(p_hw_rtc);
    __g_time.tm_mday = amhw_hc32_rtc_day_get(p_hw_rtc);
    __g_time.tm_wday = amhw_hc32_rtc_week_get(p_hw_rtc);
    __g_time.tm_mon  = amhw_hc32_rtc_mon_get(p_hw_rtc) - 1;
    __g_time.tm_year = amhw_hc32_rtc_year_get(p_hw_rtc);

    /* 计数值模式 */
    amhw_hc32_rtc_mode_sel(p_hw_rtc, AMHW_HC32_RTC_MODE_COUNTER);
    while(amhw_hc32_rtc_writeread_flag_get(p_hw_rtc) == AM_TRUE);

    __g_get_flag = AM_TRUE;
}

/**
 * \brief 闹钟回调函数
 */
am_local void __rtc_alarm_callback (void *p_arg)
{
    (void)p_arg;
    AM_DBG_INFO("alarm clock int!\r\n");
}

am_local void __rtc_irq_proc(void *p_arg)
{
    amhw_hc32_rtc_t *p_hw_rtc = (amhw_hc32_rtc_t *)p_arg;

    /* 周期中断 */
    if (amhw_hc32_rtc_period_int_flag_get(p_hw_rtc) == AM_TRUE) {
        amhw_hc32_rtc_period_int_flag_clr(p_hw_rtc);
        __rtc_period_callback(p_arg);
    }

    /* 闹钟中断 */
    if (amhw_hc32_rtc_alarm_clock_int_flag_get(p_hw_rtc) == AM_TRUE) {
        amhw_hc32_rtc_alarm_clock_int_flag_clr(p_hw_rtc);
        __rtc_alarm_callback(p_arg);
    }
}

/**
 * \brief 例程入口
 */
void demo_hc32_hw_rtc_entry (void *p_hw_rtc)
{
    __hc32_plfm_rtc_init();

    /* 设置为24小时制 */
    amhw_hc32_rtc_time_mode_sel(p_hw_rtc, AMHW_HC32_RTC_TIME_MODE_24H);

    /* RTC使能 */
    amhw_hc32_rtc_enable(p_hw_rtc, AM_TRUE);

    /* 设置时间 */
    amhw_hc32_rtc_mode_sel(p_hw_rtc, AMHW_HC32_RTC_MODE_WRITEREAD);
    while(amhw_hc32_rtc_writeread_flag_get(p_hw_rtc) == AM_FALSE);
    amhw_hc32_rtc_sec_set(p_hw_rtc, __g_init_time.tm_sec);
    amhw_hc32_rtc_min_set(p_hw_rtc, __g_init_time.tm_min);
    amhw_hc32_rtc_hour_set(p_hw_rtc, __g_init_time.tm_hour);
    amhw_hc32_rtc_day_set(p_hw_rtc, __g_init_time.tm_mday);
    amhw_hc32_rtc_week_set(p_hw_rtc, __g_init_time.tm_wday);
    amhw_hc32_rtc_mon_set(p_hw_rtc, __g_init_time.tm_mon + 1);
    amhw_hc32_rtc_year_set(p_hw_rtc, __g_init_time.tm_year);
    amhw_hc32_rtc_mode_sel(p_hw_rtc, AMHW_HC32_RTC_MODE_COUNTER);
    while(amhw_hc32_rtc_writeread_flag_get(p_hw_rtc) == AM_TRUE);

    AM_DBG_INFO("set time success \r\n");

    /* 设置 PRDX 周期 */
    amhw_hc32_rtc_period_int_time_set(p_hw_rtc, 1.0);
    /* 使用 PRDX 所设定的周期中断时间间隔 */
    amhw_hc32_rtc_period_int_time_set_enable(p_hw_rtc, AM_TRUE);

    /* 设置闹钟时间为每周4 10:21:15 */
    amhw_hc32_rtc_hour_alarm_clock_set(p_hw_rtc, 10);
    amhw_hc32_rtc_min_alarm_clock_set(p_hw_rtc,  21);
    amhw_hc32_rtc_sec_alarm_clock_set(p_hw_rtc,  15);
    amhw_hc32_rtc_week_alarm_clock_set(p_hw_rtc, 4);

    /* 连接中断 */
    am_int_connect(INUM_RTC, __rtc_irq_proc, p_hw_rtc);
    am_int_enable(INUM_RTC);

    /* 使能闹钟中断 */
    amhw_hc32_rtc_alarm_clock_int_enable(p_hw_rtc, AM_TRUE);

    /* 启动闹钟 */
    amhw_hc32_rtc_alarm_clock_enable(p_hw_rtc, AM_TRUE);

    while(1) {

        am_mdelay(20);

        if(__g_get_flag != AM_TRUE) {
            continue;
        }

        AM_DBG_INFO(
            "%02d-%02d-%02d %02d:%02d:%02d %02d\n",
            __g_time.tm_year + 1970,
            __g_time.tm_mon + 1,
            __g_time.tm_mday,
            __g_time.tm_hour,
            __g_time.tm_min,
            __g_time.tm_sec,
            __g_time.tm_wday);

        __g_get_flag =  AM_FALSE;
    }
}
/** [src_hc32l19x_std_rtc] */

/* end of file */
