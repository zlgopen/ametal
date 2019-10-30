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
 *   3. 可在am_hwconf_zlg118_rtc.c文件中配置1Hz输出控制，若使能，PIOB_14将输出1Hz的方波。
 *   4. 关闭时钟补偿的情况下，测得1Hz方波的实际频率，带入下列计算式计算：
 *           ppm = （1Hz方波的实际频率 - 1Hz）* 10^6
 *      将计算出的结果填入am_hwconf_zlg118_rtc.c的设备信息对应位置，可自行决定是否使能高速补偿时钟。
 *      编译、下载，输出端口将输出精度较高的1Hz方波
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zlg118_std_rtc.c src_zlg118_std_rtc
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-17  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_std_rtc
 * \copydoc demo_zlg118_std_rtc.c
 */

/** [src_zlg118_std_rtc] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_rtc.h"
#include "am_zlg118_rtc.h"
#include "am_zlg118_inst_init.h"
#include "demo_am118_core_entries.h"

am_tm_t   __time;
am_bool_t __get_flag =  AM_FALSE;


am_local am_tm_t __g_current_time = {
    0,           /**< \brief 秒 */
    20,          /**< \brief 分 */
    11,          /**< \brief 时 */
    17,          /**< \brief 日 */
    10,          /**< \brief 月 */
    19,          /**< \brief 年 */
    4,           /**< \brief 周 */
};

void period_int_callback(void *drv)
{

    am_rtc_handle_t rtc_handle = (am_rtc_handle_t )drv;

    if(am_rtc_time_get(rtc_handle, &__time) == AM_OK) {
        __get_flag = AM_TRUE;
    }
}

void alarm_clock_int_callback(void *drv)
{
    AM_DBG_INFO("\r\n\r\n");
    AM_DBG_INFO("alarm clock int!\r\n");
    AM_DBG_INFO("alarm clock int!\r\n");
    AM_DBG_INFO("alarm clock int!\r\n");
    AM_DBG_INFO("alarm clock int!\r\n");
    AM_DBG_INFO("\r\n\r\n");
}

/**
 * \brief 例程入口
 */
void demo_zlg118_core_drv_rtc_entry (void)
{
    int             ret          = AM_OK;
    am_rtc_handle_t p_rtc_handle = am_zlg118_rtc_inst_init();

    AM_DBG_INFO("demo am118_core rtc entry!\r\n");

    /* 设置时间 */
    ret= am_rtc_time_set(p_rtc_handle, &__g_current_time);


    if(ret == AM_OK){
        AM_DBG_INFO("set time success \r\n");
    } else {
        AM_DBG_INFO("set time fail \r\n");
    }

    /* 设置周期中断回调函数 */
    am_zlg118_rtc_callback_set(p_rtc_handle,
                               AM_ZLG118_RTC_CALLBACK_PERIOD,
                               period_int_callback,
                               (void *)p_rtc_handle);

    /* 使用自定义周期中断时间选择，20s发生一次中期中断 */
    am_zlg118_rtc_period_int_enable(p_rtc_handle,
                                    AMHW_ZLG118_RTC_PERIOD_INT_TYPE_CUSTOM,
                                    AMHW_ZLG118_RTC_PERIOD_INT_TIME_1_MIN,
                                    10.0);

    /* 设置闹钟中断回调函数 */
    am_zlg118_rtc_callback_set(p_rtc_handle,
                               AM_ZLG118_RTC_CALLBACK_ALARM,
                               alarm_clock_int_callback,
                               (void *)p_rtc_handle);

    /* 闹钟中断设置，在周四的11点21分0秒产生闹钟中断 */
    am_zlg118_rtc_alarm_clock_int_enable(p_rtc_handle,
                                         0,
                                         21,
                                         11,
                                         4);

    while(1) {

        if(__get_flag == AM_TRUE) {
            AM_DBG_INFO("%02d-%02d-%02d %02d:%02d:%02d %02d\n",
                        __time.tm_year,
                        __time.tm_mon,
                        __time.tm_mday,
                        __time.tm_hour,
                        __time.tm_min,
                        __time.tm_sec,
                        __time.tm_wday);

            __get_flag =  AM_FALSE;
        }
        am_mdelay(20);
    }
}
/** [src_zlg118_std_rtc] */

/* end of file */
