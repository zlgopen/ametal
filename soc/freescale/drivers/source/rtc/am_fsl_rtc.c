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
 * \brief RTC 模块的驱动层接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-13  sdy, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_fsl_rtc.h"

#ifdef USE_TIME_LIB
#include <time.h>
#include <string.h>
#endif /* USE_TIME_LIB */

/** 平年每月的起始天数 */
static const uint32_t __non_leap_year[] =
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

/** 闰年每月的起始天数 */
static const uint32_t __leap_year[] =
{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};


/** \brief 时间获取函数 */
static int __fsl_rtc_time_get (void *p_drv, am_tm_t *p_tm);

/** \brief 时间设置函数 */
static int __fsl_rtc_time_set (void *p_drv, am_tm_t *p_tm);

/** \brief RTC 驱动函数 */
static const struct am_rtc_drv_funcs __g_rtc_drv_funcs = {
    __fsl_rtc_time_set,
    __fsl_rtc_time_get,
};

/**
 *  \brief 秒转化为tm时间数据
 *
 *  \param p_time 指向tm时间数据的指针
 *  \param  seconds 秒数据
 *  \return 无
 */
#ifdef USE_TIME_LIB
static void __sec2tm (am_tm_t *p_time, uint32_t sec)
{
    time_t     second = sec;
    struct tm *p_tm   = NULL;

    p_tm = localtime(&second);

    if ((NULL != p_time) && (NULL != p_tm)) {
        memcpy(p_time, p_tm, sizeof(am_tm_t));
    }
}
#else

/* 一年中每个月的天数，非闰年 */
const char __days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int __week_get (int year, int month, int day)
{
    day++;
    month++;
    year += 1900;

    /* 如果是一月或二月进行换算 */
    if ((month == 1) || (month == 2)) {
        month += 12;
        year--;
    }

    /* 得到的星期几的整数 */
    return (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 -
            year / 100 + year / 400) % 7;
}

static void __sec2tm (am_tm_t *p_time, uint32_t sec)
{
    uint32_t four_year_pass;
    int32_t  hour_num;

    if (NULL == p_time) {
        return;
    }

    p_time->tm_isdst = 0;

    /* 取秒时间 */
    p_time->tm_sec = (int)(sec % 60);
    sec /= 60;

    /* 取分钟时间 */
    p_time->tm_min = (int)(sec % 60);
    sec /= 60;

    /* 取过去多少个四年，每四年有 1461*24 小时 */
    four_year_pass = ((uint32_t)sec / (1461L * 24L));

    /* 计算年份 */
    p_time->tm_year = (four_year_pass << 2) + 70;

    /* 四年中剩下的小时数 */
    sec %= 1461L * 24L;

    /* 校正闰年影响的年份，计算一年中剩下的小时数 */
    for (;;) {

        /* 一年的小时数 */
        hour_num = 365 * 24;

        /* 判断闰年 */
        if ((p_time->tm_year & 3) == 0) {

            /* 是闰年，一年则多24小时，即一天 */
            hour_num += 24;
        }
        if (sec < hour_num) {
            break;
        }
        p_time->tm_year++;
        sec -= hour_num;
    }

    /* 小时数 */
    p_time->tm_hour = (int)(sec % 24);

    /* 一年中剩下的天数 */
    sec /= 24;

    /* 假定为闰年 */
    sec++;

    p_time->tm_yday = sec - 1;

    /* 校正润年的误差，计算月份，日期 */
    if ((p_time->tm_year & 3) == 0) {
        if (sec > 60) {
            sec--;
            p_time->tm_yday = sec;
        } else {
            if (sec == 60) {
                p_time->tm_mon = 1;
                p_time->tm_mday = 29;

                /* 计算星期 */
                p_time->tm_wday = __week_get(p_time->tm_year, p_time->tm_mon, p_time->tm_mday);
                return;
            }
        }
    }

    /* 计算月日 */
    for (p_time->tm_mon = 0; __days[p_time->tm_mon] < sec; p_time->tm_mon++) {
        sec -= __days[p_time->tm_mon];
    }

    p_time->tm_mday = (int)(sec);

    /* 计算星期 */
    p_time->tm_wday = __week_get(p_time->tm_year, p_time->tm_mon, p_time->tm_mday);
}
#endif /* USE_TIME_LIB */

/**
 *  \brief tm时间数据转化为秒
 *
 *  \param p_time 指向tm时间数据的指针
 *
 *  \return 秒数据
 *
 *  \note 时间是从1900年开始的
 */
#ifdef USE_TIME_LIB
static uint32_t __tm2sec (am_tm_t *p_time)
{
    time_t time = 0;

    if (NULL != p_time) {
        time = mktime((struct tm*)p_time);
    }

    return (uint32_t)((time != -1) ? time : 0);
}
#else
static uint32_t __tm2sec (am_tm_t *p_time)
{
    unsigned int mon  = p_time->tm_mon + 1;
    unsigned int year = p_time->tm_year + 1900;

    if (NULL == p_time) {
        return 0;
    }

    /* 1..12 -> 11,12,1..10 */
    if (0 >= (int) (mon -= 2)) {
        mon += 12;                  /* Puts Feb last since it has leap day */
        year -= 1;
    }

    return ((((uint32_t)
            (year / 4 - year / 100 + year / 400 + 367 * mon / 12 + p_time->tm_mday) +
            year * 365 - 719499) *
            24 + p_time->tm_hour) * /* now have hours */
            60 + p_time->tm_min) *  /* now have minutes */
            60 + p_time->tm_sec;    /* finally seconds */
}
#endif /* USE_TIME_LIB */

/**
 *  \brief Brief
 *
 *  \param[in] p_drv 指向RTC设备的指针
 *  \param[in,out] p_tm 指向时间信息结构体的指针
 *
 *  \return AM_OK
 */
static int __fsl_rtc_time_get (void *p_drv, am_tm_t *p_tm)
{
    uint32_t sec;
    am_fsl_rtc_dev_t *p_dev   = (am_fsl_rtc_dev_t *)p_drv;
    sec                       = amhw_fsl_rtc_second_get(p_dev->p_devinfo->p_hw_rtc);
    __sec2tm(p_tm, sec);
    return AM_OK;
}

static int __fsl_rtc_time_set (void *p_drv, am_tm_t *p_tm)
{
    uint32_t sec;

    am_fsl_rtc_dev_t *p_dev   = (am_fsl_rtc_dev_t *)p_drv;
    amhw_fsl_rtc_t *p_hw_rtc  = p_dev->p_devinfo->p_hw_rtc;
    sec                       = __tm2sec(p_tm);

    /** 关闭计数器 */
    amhw_fsl_rtc_time_counter_status_clr(p_hw_rtc);

    /** 设置预分频计数器 */
    amhw_fsl_rtc_prescaler_set(p_hw_rtc, 0x00);

    /** 设置秒计数器 */
    amhw_fsl_rtc_second_set(p_hw_rtc, sec);

    /** 开启计数器 */
    amhw_fsl_rtc_time_counter_status_set(p_hw_rtc);

    return AM_OK;
}


/**
 * \brief 初始化RTC
 *
 * \param[in] p_dev     : 指向RTC设备
 * \param[in] p_devinfo : 指向RTC设备信息
 *
 * \return RTC标准服务操作句柄。如果为 NULL，表明初始化失败。
 */
am_rtc_handle_t am_fsl_rtc_init (am_fsl_rtc_dev_t           *p_dev,
                                 const am_fsl_rtc_devinfo_t *p_devinfo)
{
    amhw_fsl_rtc_t *p_hw_rtc;

    if (NULL == p_dev || NULL == p_devinfo ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_hw_rtc                = p_devinfo->p_hw_rtc;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->rtc_serv.p_funcs = (struct am_rtc_drv_funcs *)&__g_rtc_drv_funcs;
    p_dev->rtc_serv.p_drv   = p_dev;

    /**
     *  设备初始化
     */

    /* 仅当计数器未使能、溢出、无效时才初始化 */
    if ((0 == amhw_fsl_rtc_time_counter_status_get(p_hw_rtc)) ||
        (0 != amhw_fsl_rtc_count_over_status_get(p_hw_rtc))   ||
        (0 != amhw_fsl_rtc_is_time_invalid(p_hw_rtc))) {

        /* 关闭秒计数器 */
        amhw_fsl_rtc_time_counter_status_clr(p_hw_rtc);

        /* 如果计数器无效或溢出，清零分频计数器、秒计数器 */
        if (amhw_fsl_rtc_is_time_invalid(p_hw_rtc) ||
            amhw_fsl_rtc_count_over_status_get(p_hw_rtc)) {

            amhw_fsl_rtc_prescaler_set(p_hw_rtc, 0x00);
            amhw_fsl_rtc_second_set(p_hw_rtc, 0x00);
        }

        /* 校准寄存器清0 */
        amhw_fsl_rtc_alarm_set(p_hw_rtc, 0x00);
        amhw_fsl_rtc_compensate_interval_set(p_hw_rtc, 0x00);
        amhw_fsl_rtc_compensate_value_set(p_hw_rtc, 0x00);

        /** 使能秒计数器 */
        amhw_fsl_rtc_time_counter_status_set(p_hw_rtc);
    }

    return &(p_dev->rtc_serv);
}

/**
 *  \brief 解除RTC初始化
 *
 *  \param p_dev 指向RTC设备的指针
 *
 *  \return 无
 */
void am_fsl_rtc_deinit (am_rtc_handle_t handle)
{

    am_fsl_rtc_dev_t *p_dev = (am_fsl_rtc_dev_t *)handle;

    if (p_dev == NULL || p_dev->p_devinfo == NULL ) {
        return ;
    }

    amhw_fsl_rtc_time_counter_status_clr(p_dev->p_devinfo->p_hw_rtc);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    p_dev->p_devinfo        = NULL;
    p_dev->rtc_serv.p_funcs = NULL;
    p_dev->rtc_serv.p_drv   = NULL;
    p_dev                   = NULL;
}

/**
 *  \brief 设置RTC的校准配置
 *
 *  \param p_dev 指向RTC设备的指针
 *
 *  \param frq RTC时钟的实际频率，以Hz计
 *
 *  \return 实际校准后的频率
 */
int am_fsl_rtc_compensation_set (am_fsl_rtc_dev_t *p_dev, float frq)
{
    int i, j;             /** 循环计数         */
    int m, n;             /** 最优配置暂存     */
    float err_aim;        /** 目标偏差         */
    float err, err_pre;   /** 调整后的决定偏差 */
    amhw_fsl_rtc_t *p_hw_rtc;
    p_hw_rtc = p_dev->p_devinfo->p_hw_rtc;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    err_aim = 32768.0f - frq;
    err_pre = err = err_aim > 0 ? err_aim : -err_aim;

    /** 超过调整下限 */
    if (frq < (32768 - 127)) {
        m = -127;
        n = 1;
        amhw_fsl_rtc_current_interval_and_value_set(p_hw_rtc, n - 1, m);
        return (32768.0f * frq) / (32768.0f - m * 1.0);
    }

    /** 超过调整上限 */
    else if (frq > (32768 + 128)) {
        m = 128;
        n = 1;
        amhw_fsl_rtc_current_interval_and_value_set(p_hw_rtc, n - 1, m);
        return (32768.0f * frq) / (32768.0f - m * 1.0);
    }

    /** 恰好是整数 */
    else if (0 == (frq - (int)frq)) {
        m = 32768 - frq;
        n = 1;
    }

    /** 小数调整 */
    else {
        m = (int)err_aim;
        n = 1;

        for (j = 1; j < 256; j ++) {

            /** 加快 */
            if (err_aim > 0) {
                for (i = (int)err_aim * j;
                     (i < 128) && (i < (int)(err_aim + 1)*j);
                     i++) {
                    err = err_aim > i * 1.0 / j ?
                          err_aim - i * 1.0 / j :
                          i * 1.0 / j - err_aim;

                    if (err < err_pre) {
                        err_pre = err;
                        m = i;
                        n = j;

                        /** 得到无偏差参数跳出循环 */
                        if (0 == err_pre) {
                            goto out;
                        }
                    }
                }

                /** i超过上限跳出循环 */
                if (i > 127) {
                    break;
                }
            }

            /** 减慢 */
            else {
                for (i = (int)err_aim * j;
                     (i > -127) && (i > (int)(err_aim - 1)*j);
                     i--) {
                    err = err_aim > i * 1.0 / j ?
                          err_aim - i * 1.0 / j :
                          i * 1.0 / j - err_aim;

                    if (err < err_pre) {
                        err_pre = err;
                        m = i;
                        n = j;

                        /** 得到无偏差参数跳出循环 */
                        if (0 == err_pre) {
                            goto out;
                        }
                    }
                }

                /** i超过下限跳出循环 */
                if (i < -128) {
                    break;
                }
            }
        }
    }

out:
    amhw_fsl_rtc_current_interval_and_value_set(p_hw_rtc, n - 1, m);
    return (int)(frq + err_pre);
}

/**
 *  \brief 设置RTC闹钟时间
 *
 *  \param p_dev 指向RTC设备的指针
 *  \param p_tm 需要设置的时间
 *
 *  \return 无
 */
void am_fsl_rtc_alarm_set (am_fsl_rtc_dev_t *p_dev, am_tm_t *p_tm)
{
    uint32_t sec;

    sec = __tm2sec(p_tm);
    amhw_fsl_rtc_second_set(p_dev->p_devinfo->p_hw_rtc, sec);
}

/**
 *  \breif 根据日期计算一年中的第几天
 *
 *  \param day
 *  \param month
 *  \param year
 *
 *  \return 一年中的第几天 (1月1日记为第0天)
 */
int am_fsl_rtc_date2yday (uint8_t day, uint8_t month, uint32_t year)
{
    if (((0 == year % 4) && (0 != year % 100)) || (year % 400 == 0)) {
        return __leap_year[month-1] + day - 1;
    }
    else {
        return __non_leap_year[month - 1] + day - 1;
    }
}

/** end of the file */
