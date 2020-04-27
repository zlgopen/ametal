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
 * \brief RTC interfaces for operating hardware.
 *
 * \internal
 * \par Modification history
 * - 1.10 20-04-23 zcb, Modify the read and write interface of the
 *                      BCD encoding format register, read and write
 *                      only once
 * - 1.00 19-10-16  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32_RTC_H
#define __AMHW_HC32_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_common.h"

/**
 * \addtogroup amhw_hc32_if_rtc
 * \copydoc amhw_hc32_rtc.h
 * @{
 */

/**
 * \brief RTC 寄存器结构体定义
 */
typedef struct amhw_hc32_rtc {
    __IO uint32_t cr0;       /**< \brief 控制寄存器0 */
    __IO uint32_t cr1;       /**< \brief 控制寄存器1 */
    __IO uint32_t sec;       /**< \brief 秒计数寄存器 */
    __IO uint32_t min;       /**< \brief 分计数寄存器 */
    __IO uint32_t hour;      /**< \brief 时计数寄存器 */
    __IO uint32_t week;      /**< \brief 周计数寄存器 */
    __IO uint32_t day;       /**< \brief 日计数寄存器 */
    __IO uint32_t mon;       /**< \brief 月计数寄存器 */
    __IO uint32_t year;      /**< \brief 年计数寄存器 */
    __IO uint32_t almmin;    /**< \brief 分闹钟寄存器 */
    __IO uint32_t almhour;   /**< \brief 时闹钟寄存器 */
    __IO uint32_t almweek;   /**< \brief 周闹钟寄存器 */
    __IO uint32_t compen;    /**< \brief 时钟误差补偿寄存器 */
    __IO uint32_t almsec;    /**< \brief 秒闹钟寄存器 */
} amhw_hc32_rtc_t;

/**
 * \brief RTC周期中断选择
 */
typedef enum {
    AMHW_HC32_RTC_PERIOD_INT_TYPE_CUSTOM  = 0,    /* 用户自定义时间 */
    AMHW_HC32_RTC_PERIOD_INT_TYPE_SYSTEAM = 1,    /* 外设规定内容选择 */
} amhw_hc32_rtc_period_int_type_t;

/**
 * \brief RTC周期中断时间选择
 */
typedef enum {
    AMHW_HC32_RTC_PERIOD_INT_TIME_NO      = 0ul,    /* 不产生周期中断 */
    AMHW_HC32_RTC_PERIOD_INT_TIME_0_5_SEC = 1ul,    /* 0.5秒 */
    AMHW_HC32_RTC_PERIOD_INT_TIME_1_SEC   = 2ul,    /* 1秒 */
    AMHW_HC32_RTC_PERIOD_INT_TIME_1_MIN   = 3ul,    /* 1分钟 */
    AMHW_HC32_RTC_PERIOD_INT_TIME_1_HOUR  = 4ul,    /* 1小时 */
    AMHW_HC32_RTC_PERIOD_INT_TIME_1_DAY   = 5ul,    /* 1天 */
    AMHW_HC32_RTC_PERIOD_INT_TIME_1_MON   = 6ul,    /* 1月 */
} amhw_hc32_rtc_period_int_time_t;

/**
 * \brief PRDS RTC周期中断选择
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata    amhw_hc32_rtc_period_int_time_t枚举选择
 *
 * \note 如需要在 START=1时写入更改周期中断的时间间隔操作步骤如下：
 *          step1，在 NVIC 中关闭 RTC 中断；
 *          step2，更改周期中断的时间间隔；
 *          step3，清除 RTC 中断标志；
 *          step4，使能 RTC 中断。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_period_int_time_sel (
         amhw_hc32_rtc_t                *p_hw_rtc,
         amhw_hc32_rtc_period_int_time_t choice)
{
    p_hw_rtc->cr0 = (p_hw_rtc->cr0 & (~(0x7ul << 0))) |
                    (choice << 0);
}

/**
 * \brief PRDX 周期中断时间值设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] time     周期中断时间值设置（0.5 ~ 32s）
 *
 * \return AM_FALSE ：时间参数设置错误
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32_rtc_period_int_time_set (amhw_hc32_rtc_t *p_hw_rtc,
                                               float              time)
{
    uint8_t setvalue = 0;

    if(time > 32.0 || time < 0.5) {
        return AM_FALSE;
    }

    setvalue = (uint8_t)(time / 0.5 - 1.0);

    p_hw_rtc->cr0 = (p_hw_rtc->cr0 & (~(0x3f << 8))) |
                    (setvalue << 8);

    return AM_TRUE;
}

/**
 * \brief PRDX 周期中断时间值获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 获取的周期中断时间值
 */
am_static_inline
float amhw_hc32_rtc_period_int_time_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    uint8_t setvalue = 0;
    float   getvalue = 0.0;

    setvalue = (p_hw_rtc->cr0 >> 8) & 0x3f;

    getvalue = (float)(setvalue + 1) * 0.5;

    return getvalue;
}

/**
 * \brief PRDSEL 是否使用设置的周期中断时间值
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata  AM_FALSE ：使用amhw_hc32_rtc_period_int_time_sel配置的中断时间
 *                   AM_TRUE  ：使用amhw_hc32_rtc_period_int_time_set配置的中断时间
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_period_int_time_set_enable (amhw_hc32_rtc_t *p_hw_rtc,
                                                 am_bool_t          stata)
{
    if(stata == AM_TRUE) {
        p_hw_rtc->cr0 |= (1ul << 14);
    } else {
        p_hw_rtc->cr0 &= ~(1ul << 14);
    }
}

/**
 * \brief START RTC计数器停止/开启
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata    AM_FALSE ：停止 RTC计数器
 *                     AM_TRUE  ：使能 RTC计数器
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_enable (amhw_hc32_rtc_t *p_hw_rtc,
                             am_bool_t          stata)
{
    if(stata == AM_TRUE) {
        p_hw_rtc->cr0 |= (1ul << 7);
    } else {
        p_hw_rtc->cr0 &= ~(1ul << 7);
    }
}

/**
 * \brief HZ1SEL 高精度1Hz使能/禁能
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata    AM_FALSE ：普通精度1Hz
 *                     AM_TRUE  ：高精度1Hz
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_1hz_high_enable (amhw_hc32_rtc_t *p_hw_rtc,
                                      am_bool_t          stata)
{
    if(stata == AM_TRUE) {
        p_hw_rtc->cr0 |= (1ul << 6);
    } else {
        p_hw_rtc->cr0 &= ~(1ul << 6);
    }
}

/**
 * \brief HZ1OE 1Hz输出使能/禁能
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata    AM_FALSE ：1Hz输出禁能
 *                     AM_TRUE  ：1Hz输出使能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_1hz_out_enable (amhw_hc32_rtc_t *p_hw_rtc,
                                     am_bool_t          stata)
{
    if(stata == AM_TRUE) {
        p_hw_rtc->cr0 |= (1ul << 5);
    } else {
        p_hw_rtc->cr0 &= ~(1ul << 5);
    }
}

/**
 * \brief RTC小时制选择枚举
 */
typedef enum {
    AMHW_HC32_RTC_TIME_MODE_12h = 0ul,    /* 12小时制 */
    AMHW_HC32_RTC_TIME_MODE_24H = 1ul,    /* 24小时制 */
} amhw_hc32_rtc_time_mode_t;

/**
 * \brief AMPM 小时制选择
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata    amhw_hc32_rtc_time_mode_t枚举选择
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_time_mode_sel (amhw_hc32_rtc_t          *p_hw_rtc,
                                    amhw_hc32_rtc_time_mode_t stata)
{
    p_hw_rtc->cr0 = (p_hw_rtc->cr0 & (~(0x1ul << 3))) |
                    (stata << 3);
}

/**
 * \brief AMPM 小时制获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return amhw_hc32_rtc_time_mode_t枚举
 */
am_static_inline
amhw_hc32_rtc_time_mode_t
amhw_hc32_rtc_time_mode_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return (amhw_hc32_rtc_time_mode_t)((p_hw_rtc->cr0 >> 3) & 1ul);
}

/**
 * \brief RTC时钟选择枚举
 */
typedef enum {
    AMHW_HC32_RTC_CLK_SRC_XTL_32768Hz       = 0,  /* XTL 32.768k */
    AMHW_HC32_RTC_CLK_SRC_RCL_32KHz         = 2,  /* RCL 32k */
    AMHW_HC32_RTC_CLK_SRC_XTH_4MHz_DIV128   = 4,  /* XTH 4M */
    AMHW_HC32_RTC_CLK_SRC_XTH_8MHz_DIV256   = 5,  /* XTH 4M */
    AMHW_HC32_RTC_CLK_SRC_XTH_16MHz_DIV512  = 6,  /* XTH 4M */
    AMHW_HC32_RTC_CLK_SRC_XTH_32MHz_DIV1024 = 7,  /* XTH 4M */
} amhw_hc32_rtc_clk_src_t;

/**
 * \brief CKSEL RTC时钟源设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata    amhw_hc32_rtc_clk_src_t枚举选择
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_clk_src_sel (amhw_hc32_rtc_t        *p_hw_rtc,
                                  amhw_hc32_rtc_clk_src_t src)
{
    p_hw_rtc->cr1 = (p_hw_rtc->cr1 & (~(0x7ul << 8))) |
                    (src << 8);
}

/**
 * \brief CKSEL RTC时钟源获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return amhw_hc32_rtc_clk_src_t枚举
 */
am_static_inline
amhw_hc32_rtc_clk_src_t
amhw_hc32_rtc_clk_src_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return (amhw_hc32_rtc_clk_src_t)((p_hw_rtc->cr1 >> 8) & 0x7ul);
}

/**
 * \brief ALMEN 闹钟使能/禁能
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata    AM_FALSE ：闹钟禁能
 *                     AM_TRUE  ：闹钟使能
 *
 * \note 在 START=1日历计数过程中并且 ALMIE=1中断许可的情况下使能ALMEN 时，为防止误动作
 *       请将系统中断关闭。使能后请将 ALMF标志位清除。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_alarm_clock_enable (amhw_hc32_rtc_t *p_hw_rtc,
                                         am_bool_t          stata)
{
    if(stata == AM_TRUE) {
        p_hw_rtc->cr1 |= (1ul << 7);
    } else {
        p_hw_rtc->cr1 &= ~(1ul << 7);
    }
}

/**
 * \brief ALMIE 闹钟中断使能/禁能
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata    AM_FALSE ：闹钟中断禁能
 *                     AM_TRUE  ：闹钟中断使能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_alarm_clock_int_enable (amhw_hc32_rtc_t *p_hw_rtc,
                                             am_bool_t          stata)
{
    if(stata == AM_TRUE) {
        p_hw_rtc->cr1 |= (1ul << 6);
    } else {
        p_hw_rtc->cr1 &= ~(1ul << 6);
    }
}

/**
 * \brief ALMF 闹钟中断标志获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return AM_FALSE ：标志无效
 *         AM_TRUE  ：标志有效
 */
am_static_inline
am_bool_t amhw_hc32_rtc_alarm_clock_int_flag_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return (am_bool_t)((p_hw_rtc->cr1 >> 4) & 1ul);
}

/**
 * \brief ALMF 闹钟中断标志清除
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_alarm_clock_int_flag_clr (amhw_hc32_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr1 &= (~(1ul << 4));
}

/**
 * \brief PRDF 周期中断标志获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return AM_FALSE ：标志无效
 *         AM_TRUE  ：标志有效
 */
am_static_inline
am_bool_t amhw_hc32_rtc_period_int_flag_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return (am_bool_t)((p_hw_rtc->cr1 >> 3) & 1ul);
}

/**
 * \brief PRDF 周期中断标志清除
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_period_int_flag_clr (amhw_hc32_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr1 &= (~(1ul << 3));
}

/**
 * \brief WAITF 写入/读出状态状态获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \note WAIT位设定是否有效标志。在写入/读出前请确认该位是否为“1“。计数过程中，
 *       在 WAIT 位清”0“后等待写入完成后该位才清”0“
 *
 * \return AM_FALSE ：非写入/读出状态
 *         AM_TRUE  ：写入/读出状态
 */
am_static_inline
am_bool_t amhw_hc32_rtc_writeread_flag_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return ((p_hw_rtc->cr1 >> 1) & 1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief RTC操作模式选择
 */
typedef enum {
    AMHW_HC32_RTC_MODE_COUNTER   = 0ul,    /* 计数值模式 */
    AMHW_HC32_RTC_MODE_WRITEREAD = 1ul,    /* 读写模式 */
} amhw_hc32_rtc_mode_t;

/**
 * \brief WAIT RTC操作模式选择
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata    amhw_hc32_rtc_mode_t枚举选择
 *
 * \note 在写入/读出时请将该位置“1”，由于计数器在连续计数，请在1秒的时间内完成写入/读出操作并将该位清“0“。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_mode_sel (amhw_hc32_rtc_t     *p_hw_rtc,
                               amhw_hc32_rtc_mode_t mode)
{
    p_hw_rtc->cr1 = ((p_hw_rtc->cr1) & (~(0x1ul << 0))) |
                    (mode << 0);
}

/**
 * \brief 秒计数器设置
 *
 * \param[in] p_hw_rtc : 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    : 秒计数值
 *
 * \retval 无
 */
am_static_inline
void amhw_hc32_rtc_sec_set (amhw_hc32_rtc_t *p_hw_rtc, uint8_t value)
{
    p_hw_rtc->sec = AM_HEX_TO_BCD(value);
}

/**
 * \brief 秒计数器获取
 *
 * \param[in] p_hw_rtc : 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 秒
 */
am_static_inline
uint8_t amhw_hc32_rtc_sec_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return AM_BCD_TO_HEX(p_hw_rtc->sec);
}

/**
 * \brief MINH、MINL 分计数器设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    分计数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_min_set (amhw_hc32_rtc_t *p_hw_rtc, uint8_t value)
{
    p_hw_rtc->min = AM_HEX_TO_BCD(value);
}

/**
 * \brief MINH、MINL 分计数器获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 分数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_min_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return AM_BCD_TO_HEX(p_hw_rtc->min);
}

/**
 * \brief HOURH、HOURL 时计数器设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    时计数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_hour_set (amhw_hc32_rtc_t *p_hw_rtc, uint8_t value)
{
    p_hw_rtc->hour = AM_HEX_TO_BCD(value);
}

/**
 * \brief HOURH、HOURL 时计数器获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 时数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_hour_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return AM_BCD_TO_HEX(p_hw_rtc->hour);
}

/**
 * \brief DAYH、DAYL 日计数器设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    日计数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_day_set (amhw_hc32_rtc_t *p_hw_rtc, uint8_t value)
{
    p_hw_rtc->day = AM_HEX_TO_BCD(value);
}

/**
 * \brief DAYH、DAYL 日计数器获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 日数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_day_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return AM_BCD_TO_HEX(p_hw_rtc->day);
}

/**
 * \brief WEEK 周计数器设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    周计数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_week_set (amhw_hc32_rtc_t *p_hw_rtc, uint8_t value)
{
    p_hw_rtc->week = (uint32_t)(value & 0x7);
}

/**
 * \brief WEEK 周计数器获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 周数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_week_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return (p_hw_rtc->week & 0x7ul);
}

/**
 * \brief MON 月计数器设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    月计数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_mon_set (amhw_hc32_rtc_t *p_hw_rtc, uint8_t  value)
{
    p_hw_rtc->mon = AM_HEX_TO_BCD(value);
}

/**
 * \brief MON 月计数器获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 月数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_mon_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return AM_BCD_TO_HEX(p_hw_rtc->mon);
}

/**
 * \brief YEARH、YEARL 年计数器设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    年计数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_year_set (amhw_hc32_rtc_t *p_hw_rtc, uint8_t value)
{
    p_hw_rtc->year = AM_HEX_TO_BCD(value);
}

/**
 * \brief YEARH、YEARL 年计数器获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 年数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_year_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return AM_BCD_TO_HEX(p_hw_rtc->year);
}

/**
 * \brief ALMSECH、ALMSECL 秒闹钟数值设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    秒闹钟数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_sec_alarm_clock_set (amhw_hc32_rtc_t *p_hw_rtc,
                                        uint8_t          value)
{
    p_hw_rtc->almsec = AM_HEX_TO_BCD(value);
}

/**
 * \brief ALMSECH、ALMSECL 秒闹钟数值获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 秒闹钟数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_sec_alarm_clock_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return AM_BCD_TO_HEX(p_hw_rtc->almsec);
}

/**
 * \brief ALMMINH、ALMMINL 分闹钟数值设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    分闹钟数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_min_alarm_clock_set (amhw_hc32_rtc_t *p_hw_rtc,
                                        uint8_t          value)
{
    p_hw_rtc->almmin = AM_HEX_TO_BCD(value);
}

/**
 * \brief ALMMINH、ALMMINL 分闹钟数值获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 分闹钟数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_min_alarm_clock_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return AM_BCD_TO_HEX(p_hw_rtc->almmin);
}

/**
 * \brief ALMHOURH、ALMHOURL 时闹钟数值设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    时闹钟数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_hour_alarm_clock_set (amhw_hc32_rtc_t *p_hw_rtc,
                                         uint8_t          value)
{
    p_hw_rtc->almhour = AM_HEX_TO_BCD(value);
}

/**
 * \brief ALMHOURH、ALMHOURL 时闹钟数值获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 时闹钟数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_hour_alarm_clock_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    return AM_BCD_TO_HEX(p_hw_rtc->almhour);
}

/**
 * \brief ALMWEEK 周闹钟数值设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    周闹钟数值
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
void amhw_hc32_rtc_week_alarm_clock_set (amhw_hc32_rtc_t  *p_hw_rtc,
                                         uint8_t           value)
{
    p_hw_rtc->almweek = (uint32_t)(0x1 << (value & 0x7));
}

/**
 * \brief ALMWEEK 周闹钟数值获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 *
 * \return 周闹钟数值
 */
am_static_inline
uint8_t amhw_hc32_rtc_week_alarm_clock_get (amhw_hc32_rtc_t *p_hw_rtc)
{
    uint8_t i = 0;

    for(i = 0; i < 7; i++) {
        if (AM_BIT_ISSET(p_hw_rtc->almweek, i)) {
            return i;
        }
    }
    return 255;
}

/**
 * \brief CR 时钟误差补偿设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] value    误差补偿值（-274.6 ~ 212.6）ppm
 *
 * \return AM_FALSE : 设置失败，参数错误
 *         AM_TRUE  ： 设置成功
 */
am_static_inline
am_bool_t amhw_hc32_rtc_compensate_set (amhw_hc32_rtc_t *p_hw_rtc,
                                          float              value)
{
    uint8_t  temp_int;
    float    temp = 0.0;
    char     i    = 0;
    uint16_t data = 0;

    if(value < (-274.6) || value > (212.6)) {
        return AM_FALSE;
    }

    /* 计算补偿目标值[ppm]*(2^15)/(10^6) */
    temp = ((value * (1ul << 15)) / 1000000.0);

    if(temp < 0.0) {
        temp = -temp;
    }

    /* 得到整数部分 */
    temp_int = (uint8_t)temp;

    /* 得到小数部分 */
    temp = temp - temp_int;

    /* 得到设定值的整数部分，且预留低5位的小数部分*/
    data = (temp_int << 5);

    /* 将小数部分转换成二进制，同时嵌入到设定值的低5位 */
    for(i = 4; i > 0; i--) {
        temp = temp * 2;
        data = data | ((uint8_t)temp << i);
    }

    /* 如果是负数，取2的补码 */
    if(value < 0.0) {
        data = ~data;
        data = data + 1;
    }

    /* 得到最终的设定数据 */
    data = data + (1ul << 5);

    p_hw_rtc->compen = (p_hw_rtc->compen & (~(0x1fful << 0))) |
                       ((data & 0x1fful) << 0);

    return AM_TRUE;
}

/**
 * \brief EN 时钟误差补偿使能
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32_rtc_t结构的指针
 * \param[in] stata  AM_FALSE ：禁止时钟误差补偿
 *                   AM_TRUE  ：使能时钟误差补偿
 * \return 无
 */
am_static_inline
void amhw_hc32_rtc_compensate_enable (amhw_hc32_rtc_t *p_hw_rtc,
                                        am_bool_t          stata)
{
    if(stata == AM_TRUE) {
        p_hw_rtc->compen |= (1ul << 15);
    } else {
        p_hw_rtc->compen &= ~(1ul << 15);
    }
}
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif/* __HC32_RTC_H */

/* end of file */
