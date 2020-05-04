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
 *   1. 每10秒发生一次周期中断，串口并打印出当前时间信息。
 *   2. 周四的11点21分0秒产生闹钟中断，打印四行"alarm clock int!"提示信息。
 *   3. 可在am_hwconf_hc32l19x_rtc.c文件中配置1Hz输出控制，若使能，PIOB_14将输出1Hz的方波。
 *   4. 关闭时钟补偿的情况下，测得1Hz方波的实际频率，带入下列计算式计算：
 *           ppm = （1Hz方波的实际频率 - 1Hz）* 10^6
 *      将计算出的结果填入am_hwconf_hc32l19x_rtc.c的设备信息对应位置，可自行决定是否使能高速补偿时钟。
 *      编译、下载，输出端口将输出精度较高的1Hz方波
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
 * - 1.00 19-10-17  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l19x_std_rtc
 * \copydoc demo_hc32l19x_std_rtc.c
 */

/** [src_hc32l19x_std_rtc] */
#include "ametal.h"
#include "am_rtc.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_hc32_rtc.h"
#include "am_hc32.h"
#include "demo_aml19x_core_entries.h"

/** \brief 当前时间 */
static am_tm_t     __g_time;

/** \brief 指示是否获取到时间 */
static am_bool_t   __g_get_flag = AM_FALSE;

/** \brief 初始化时间 */
static am_tm_t     __g_init_time = {
    00,   /** 0  秒             */
    16,   /** 16 分             */
    10,   /** 10 时             */
    23,   /** 23 日             */
    3,    /** 4 月             */
    120,  /** 2020 年     */
    4,    /** 周 4   */
    114,  /** 天 当年的第 114 天 */
    0,    /** 夏令时         */
};

/**
 * \brief 周期回调函数
 */
static void __rtc_period_callback (void *p_arg)
{
    am_rtc_handle_t rtc_handle = (am_rtc_handle_t )p_arg;

    if(am_rtc_time_get(rtc_handle, &__g_time) == AM_OK) {
        __g_get_flag = AM_TRUE;
    }
}

/**
 * \brief 闹钟回调函数
 */
static void __rtc_alarm_callback (void *p_arg)
{
    (void)p_arg;
    AM_DBG_INFO("alarm clock int!\r\n");
}

/**
 * \brief 例程入口
 */
void demo_zsl420_hw_rtc_int_entry (void)
{
    int             ret        = AM_OK;
    am_rtc_handle_t rtc_handle = am_hc32_rtc_inst_init();

    AM_DBG_INFO("demo zsl420 RTC entry!\r\n");

    /* 设置时间 */
    ret= am_rtc_time_set(rtc_handle, &__g_init_time);
    if(ret == AM_OK){
        AM_DBG_INFO("set time success \r\n");
    } else {
        AM_DBG_INFO("set time fail \r\n");
    }

    /* 连接周期中断回调函数 */
    am_hc32_rtc_int_connect(
        rtc_handle,
        AM_HC32_RTC_INT_PERIOD,
        __rtc_period_callback,
        rtc_handle);
    /* 使能周期中断 */
    am_hc32_rtc_int_enable(
        rtc_handle,
        AM_HC32_RTC_INT_PERIOD);

    /* 设置 PRDX 周期 */
    amhw_hc32_rtc_period_int_time_set(HC32_RTC, 10.0);
    /* 使用 PRDX 所设定的周期中断时间间隔 */
    amhw_hc32_rtc_period_int_time_set_enable(HC32_RTC, AM_TRUE);

    /* 设置闹钟时间为每周4 11:21:00 */
    amhw_hc32_rtc_hour_alarm_clock_set(HC32_RTC, 10);
    amhw_hc32_rtc_min_alarm_clock_set(HC32_RTC,  16);
    amhw_hc32_rtc_sec_alarm_clock_set(HC32_RTC,  25);
    amhw_hc32_rtc_week_alarm_clock_set(HC32_RTC, 4);

    /* 连接闹钟中断回调函数 */
    am_hc32_rtc_int_connect(
        rtc_handle,
        AM_HC32_RTC_INT_ALARM,
        __rtc_alarm_callback,
        rtc_handle);
    /* 使能闹钟中断 */
    am_hc32_rtc_int_enable(
        rtc_handle,
        AM_HC32_RTC_INT_ALARM);

    /* 启动闹钟 */
    amhw_hc32_rtc_alarm_clock_enable(HC32_RTC, AM_TRUE);

    while(1) {

        am_mdelay(20);

        if(__g_get_flag != AM_TRUE) {
            continue;
        }

        AM_DBG_INFO(
            "%02d-%02d-%02d %02d:%02d:%02d %02d\n",
            __g_time.tm_year,
            __g_time.tm_mon,
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
