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
 * - 1.00 19-10-16  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_RTC_H
#define __AMHW_HC32F460_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_common.h"

/**
 * \addtogroup amhw_hc32f460_if_rtc
 * \copydoc amhw_hc32f460_rtc.h
 * @{
 */

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)

  /* 默认使能匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */

typedef struct
{
    __IO uint32_t RESET                     : 1;
    uint32_t RESERVED1                      :31;
} stc_rtc_cr0_field_t;

typedef struct
{
    __IO uint32_t PRDS                      : 3;
    __IO uint32_t AMPM                      : 1;
    __IO uint32_t ALMFCLR                   : 1;
    __IO uint32_t ONEHZOE                   : 1;
    __IO uint32_t ONEHZSEL                  : 1;
    __IO uint32_t START                     : 1;
    uint32_t RESERVED8                      :24;
} stc_rtc_cr1_field_t;

typedef struct
{
    __IO uint32_t WAIT                      : 1;
    __IO uint32_t WAITF                     : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t ALMF                      : 1;
    uint32_t RESERVED4                      : 1;
    __IO uint32_t PRDIE                     : 1;
    __IO uint32_t ALMIE                     : 1;
    __IO uint32_t ALME                      : 1;
    uint32_t RESERVED8                      :24;
} stc_rtc_cr2_field_t;

typedef struct
{
    uint32_t RESERVED0                      : 4;
    __IO uint32_t LRCEN                     : 1;
    uint32_t RESERVED5                      : 2;
    __IO uint32_t RCKSEL                    : 1;
    uint32_t RESERVED8                      :24;
} stc_rtc_cr3_field_t;

typedef struct
{
    __IO uint32_t SECU                      : 4;
    __IO uint32_t SECD                      : 3;
    uint32_t RESERVED7                      :25;
} stc_rtc_sec_field_t;

typedef struct
{
    __IO uint32_t MINU                      : 4;
    __IO uint32_t MIND                      : 3;
    uint32_t RESERVED7                      :25;
} stc_rtc_min_field_t;

typedef struct
{
    __IO uint32_t HOURU                     : 4;
    __IO uint32_t HOURD                     : 2;
    uint32_t RESERVED6                      :26;
} stc_rtc_hour_field_t;

typedef struct
{
    __IO uint32_t WEEK                      : 3;
    uint32_t RESERVED3                      :29;
} stc_rtc_week_field_t;

typedef struct
{
    __IO uint32_t DAYU                      : 4;
    __IO uint32_t DAYD                      : 2;
    uint32_t RESERVED6                      :26;
} stc_rtc_day_field_t;

typedef struct
{
    __IO uint32_t MON                       : 5;
    uint32_t RESERVED5                      :27;
} stc_rtc_mon_field_t;

typedef struct
{
    __IO uint32_t YEARU                     : 4;
    __IO uint32_t YEARD                     : 4;
    uint32_t RESERVED8                      :24;
} stc_rtc_year_field_t;

typedef struct
{
    __IO uint32_t ALMMINU                   : 4;
    __IO uint32_t ALMMIND                   : 3;
    uint32_t RESERVED7                      :25;
} stc_rtc_almmin_field_t;

typedef struct
{
    __IO uint32_t ALMHOURU                  : 4;
    __IO uint32_t ALMHOURD                  : 2;
    uint32_t RESERVED6                      :26;
} stc_rtc_almhour_field_t;

typedef struct
{
    __IO uint32_t ALMWEEK                   : 7;
    uint32_t RESERVED7                      :25;
} stc_rtc_almweek_field_t;

typedef struct
{
    __IO uint32_t COMP8                     : 1;
    uint32_t RESERVED1                      : 6;
    __IO uint32_t COMPEN                    : 1;
    uint32_t RESERVED8                      :24;
} stc_rtc_errcrh_field_t;

typedef struct
{
    __IO uint32_t COMP                      : 8;
    uint32_t RESERVED8                      :24;
} stc_rtc_errcrl_field_t;

/**
 * \brief RTC 寄存器结构体定义
 */
typedef struct amhw_hc32f460_rtc {
    union
    {
        __IO uint32_t CR0;
        stc_rtc_cr0_field_t CR0_f;
    };
    union
    {
        __IO uint32_t CR1;
        stc_rtc_cr1_field_t CR1_f;
    };
    union
    {
        __IO uint32_t CR2;
        stc_rtc_cr2_field_t CR2_f;
    };
    union
    {
        __IO uint32_t CR3;
        stc_rtc_cr3_field_t CR3_f;
    };
    union
    {
        __IO uint32_t SEC;
        stc_rtc_sec_field_t SEC_f;
    };
    union
    {
        __IO uint32_t MIN;
        stc_rtc_min_field_t MIN_f;
    };
    union
    {
        __IO uint32_t HOUR;
        stc_rtc_hour_field_t HOUR_f;
    };
    union
    {
        __IO uint32_t WEEK;
        stc_rtc_week_field_t WEEK_f;
    };
    union
    {
        __IO uint32_t DAY;
        stc_rtc_day_field_t DAY_f;
    };
    union
    {
        __IO uint32_t MON;
        stc_rtc_mon_field_t MON_f;
    };
    union
    {
        __IO uint32_t YEAR;
        stc_rtc_year_field_t YEAR_f;
    };
    union
    {
        __IO uint32_t ALMMIN;
        stc_rtc_almmin_field_t ALMMIN_f;
    };
    union
    {
        __IO uint32_t ALMHOUR;
        stc_rtc_almhour_field_t ALMHOUR_f;
    };
    union
    {
        __IO uint32_t ALMWEEK;
        stc_rtc_almweek_field_t ALMWEEK_f;
    };
    union
    {
        __IO uint32_t ERRCRH;
        stc_rtc_errcrh_field_t ERRCRH_f;
    };
    union
    {
        __IO uint32_t ERRCRL;
        stc_rtc_errcrl_field_t ERRCRL_f;
    };
} amhw_hc32f460_rtc_t;

typedef enum en_rtc_clk_source
{
    RtcClkXtal32 = 0u,                  ///< XTAL32 as clock source
    RtcClkLrc    = 1u,                  ///< LRC as clock source
} en_rtc_clk_source_t;

typedef enum en_rtc_period_int_type
{
    RtcPeriodIntInvalid = 0u,           ///< Period interrupt invalid
    RtcPeriodIntHalfSec = 1u,           ///< 0.5 second period interrupt
    RtcPeriodIntOneSec  = 2u,           ///< 1 second period interrupt
    RtcPeriodIntOneMin  = 3u,           ///< 1 minute period interrupt
    RtcPeriodIntOneHour = 4u,           ///< 1 hour period interrupt
    RtcPeriodIntOneDay  = 5u,           ///< 1 day period interrupt
    RtcPeriodIntOneMon  = 6u            ///< 1 month period interrupt
} en_rtc_period_int_type_t;

typedef enum en_rtc_time_format
{
    RtcTimeFormat12Hour = 0u,           ///< 12 hours mode
    RtcTimeFormat24Hour = 1u,           ///< 24 hours mode
} en_rtc_time_format_t;

typedef enum en_rtc_output_compen
{
    RtcOutputCompenDistributed = 0u,    ///< Distributed compensation 1hz output
    RtcOutputCompenUniform     = 1u,    ///< Uniform Compensation 1hz output
} en_rtc_output_compen_t;

typedef enum en_rtc_functional_state
{
    RTC_DISABLE = 0u,
    RTC_ENABLE  = 1u,
} en_rtc_functional_state_t;


typedef struct stc_rtc_init
{
    en_rtc_clk_source_t      enClkSource;       ///< Clock source
    en_rtc_period_int_type_t enPeriodInt;       ///< Period interrupt condition
    en_rtc_time_format_t     enTimeFormat;      ///< RTC time format
    en_rtc_output_compen_t   enCompenWay;       ///< 1HZ output compensation way
    uint16_t                 u16CompenVal;      ///< Clock error compensation value
    en_rtc_functional_state_t    enCompenEn;        ///< Enable/Disable clock error compensation
} stc_rtc_init_t;

typedef enum en_rtc_irq_type_
{
    RtcIrqPeriod = 0u,                  ///< Period count interrupt request
    RtcIrqAlarm  = 1u,                  ///< Alarm interrupt request
} en_rtc_irq_type_t;

typedef enum en_rtc_hour12_ampm
{
    RtcHour12Am = 0u,                   ///< Ante meridiem
    RtcHour12Pm = 1u,                   ///< Post meridiem
} en_rtc_hour12_ampm_t;

typedef struct stc_rtc_date_time
{
    uint8_t              u8Year;        ///< Year (range 0-99)
    uint8_t              u8Month;       ///< Month (range 1-12)
    uint8_t              u8Day;         ///< Day (range 1-31)
    uint8_t              u8Hour;        ///< Hours (range 1-12 when 12 hour format; range 0-23 when 24 hour format)
    uint8_t              u8Minute;      ///< Minutes (range 0-59)
    uint8_t              u8Second;      ///< Seconds (range 0-59)
    uint8_t              u8Weekday;     ///< Weekday (range 0-6)
    en_rtc_hour12_ampm_t enAmPm;        ///< The value is valid when 12-hour format
} stc_rtc_date_time_t;

typedef enum en_rtc_weekday
{
    RtcWeekdaySunday    = 0u,           ///< Sunday
    RtcWeekdayMonday    = 1u,           ///< Monday
    RtcWeekdayTuesday   = 2u,           ///< Tuesday
    RtcWeekdayWednesday = 3u,           ///< Wednesday
    RtcWeekdayThursday  = 4u,           ///< Thursday
    RtcWeekdayFriday    = 5u,           ///< Friday
    RtcWeekdaySaturday  = 6u            ///< Saturday
} en_rtc_weekday_t;

typedef enum en_rtc_data_format
{
    RtcDataFormatDec = 0u,              ///< Decimal format
    RtcDataFormatBcd = 1u,              ///< BCD format
} en_rtc_data_format_t;

/*! Decimal to BCD */
#define DEC2BCD(x)                      ((((x) / 10u) << 4u) + ((x) % 10u))

/*! BCD to decimal */
#define BCD2DEC(x)                      ((((x) >> 4u) * 10u) + ((x) & 0x0Fu))

#define RTC_HOUR12_AMPM_MASK                    (0x20u)

/**
 * \brief RTC周期中断选择
 */
typedef enum {
    AMHW_HC32F460_RTC_PERIOD_INT_TYPE_CUSTOM  = 0,    /* 用户自定义时间 */
    AMHW_HC32F460_RTC_PERIOD_INT_TYPE_SYSTEAM = 1,    /* 外设规定内容选择 */
} amhw_hc32f460_rtc_period_int_type_t;

/**
 * \brief RTC周期中断时间选择
 */
typedef enum {
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_NO      = 0ul,    /* 不产生周期中断 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_0_5_SEC = 1ul,    /* 0.5秒 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_SEC   = 2ul,    /* 1秒 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_MIN   = 3ul,    /* 1分钟 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_HOUR  = 4ul,    /* 1小时 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_DAY   = 5ul,    /* 1天 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_MON   = 6ul,    /* 1月 */
} amhw_hc32f460_rtc_period_int_time_t;

typedef enum {
    AMHW_HC32F460_RTCCLK_ADJUST_4M = 0,    /**< \brief 4  MHz补偿时钟频率 */
    AMHW_HC32F460_RTCCLK_ADJUST_6M,        /**< \brief 6  MHz补偿时钟频率 */
    AMHW_HC32F460_RTCCLK_ADJUST_8M,        /**< \brief 8  MHz补偿时钟频率 */
    AMHW_HC32F460_RTCCLK_ADJUST_10M,       /**< \brief 10 MHz补偿时钟频率 */
    AMHW_HC32F460_RTCCLK_ADJUST_16M,       /**< \brief 16 MHz补偿时钟频率 */
    AMHW_HC32F460_RTCCLK_ADJUST_20M,       /**< \brief 20 MHz补偿时钟频率 */
    AMHW_HC32F460_RTCCLK_ADJUST_24M,       /**< \brief 24 MHz补偿时钟频率 */
    AMHW_HC32F460_RTCCLK_ADJUST_32M,       /**< \brief 32 MHz补偿时钟频率 */
}amhw_hc32f460_rtcclk_adjust;


/**
 * \brief PRDS RTC周期中断选择
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32f460_rtc_t结构的指针
 * \param[in] stata    amhw_hc32f460_rtc_period_int_time_t枚举选择
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
void amhw_hc32f460_rtc_period_int_time_sel (
         amhw_hc32f460_rtc_t                *p_hw_rtc,
         amhw_hc32f460_rtc_period_int_time_t choice)
{
//    p_hw_rtc->cr0 = (p_hw_rtc->cr0 & (~(0x7ul << 0))) |
//                    (choice << 0);
}

/**
 * \brief PRDX 周期中断时间值设置
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32f460_rtc_t结构的指针
 * \param[in] time     周期中断时间值设置（0.5 ~ 32s）
 *
 * \return AM_FALSE ：时间参数设置错误
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_rtc_period_int_time_set (amhw_hc32f460_rtc_t *p_hw_rtc,
                                               float              time)
{
//    uint8_t setvalue = 0;

//    if(time > 32.0 || time < 0.5) {
//        return AM_FALSE;
//    }

//    setvalue = (uint8_t)(time / 0.5 - 1.0);

//    p_hw_rtc->cr0 = (p_hw_rtc->cr0 & (~(0x3f << 8))) |
//                    (setvalue << 8);

    return AM_TRUE;
}

/**
 * \brief PRDX 周期中断时间值获取
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32f460_rtc_t结构的指针
 *
 * \return 获取的周期中断时间值
 */
am_static_inline
float amhw_hc32f460_rtc_period_int_time_get (amhw_hc32f460_rtc_t *p_hw_rtc)
{
//    uint8_t setvalue = 0;
    float   getvalue = 0.0;

//    setvalue = (p_hw_rtc->cr0 >> 8) & 0x3f;

//    getvalue = (float)(setvalue + 1) * 0.5;

    return getvalue;
}

/**
 * \brief PRDSEL 是否使用设置的周期中断时间值
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32f460_rtc_t结构的指针
 * \param[in] stata  AM_FALSE ：使用amhw_hc32f460_rtc_period_int_time_sel配置的中断时间
 *                   AM_TRUE  ：使用amhw_hc32f460_rtc_period_int_time_set配置的中断时间
 * \return 无
 */
am_static_inline
void amhw_hc32f460_rtc_period_int_time_set_enable (amhw_hc32f460_rtc_t *p_hw_rtc,
                                                 am_bool_t          stata)
{
//    if(stata == AM_TRUE) {
//        p_hw_rtc->cr0 |= (1ul << 14);
//    } else {
//        p_hw_rtc->cr0 &= ~(1ul << 14);
//    }
}

/**
 * \brief START RTC计数器停止/开启
 *
 * \param[in] p_hw_rtc 指向 amhw_hc32f460_rtc_t结构的指针
 * \param[in] stata    AM_FALSE ：停止 RTC计数器
 *                     AM_TRUE  ：使能 RTC计数器
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_rtc_enable (amhw_hc32f460_rtc_t *p_hw_rtc,
                               am_bool_t          stata)
{
//    if(stata == AM_TRUE) {
//        p_hw_rtc->CR1_f.START = 1;
//    } else {
//        p_hw_rtc->CR1_f.START = 0;
//    }
}

/**
 * \brief RTC小时制选择枚举
 */
typedef enum {
    AMHW_HC32F460_RTC_TIME_MODE_12h = 0ul,    /* 12小时制 */
    AMHW_HC32F460_RTC_TIME_MODE_24H = 1ul,    /* 24小时制 */
} amhw_hc32f460_rtc_time_mode_t;

/**
 * \brief RTC时钟选择枚举
 */
typedef enum {
    AMHW_HC32F460_RTC_CLK_SRC_XTL_32768Hz       = 0,  /* XTL 32.768k */
    AMHW_HC32F460_RTC_CLK_SRC_RCL_32KHz         = 2,  /* RCL 32k */
    AMHW_HC32F460_RTC_CLK_SRC_XTH_4MHz_DIV128   = 4,  /* XTH 4M */
    AMHW_HC32F460_RTC_CLK_SRC_XTH_8MHz_DIV256   = 5,  /* XTH 4M */
    AMHW_HC32F460_RTC_CLK_SRC_XTH_16MHz_DIV512  = 6,  /* XTH 4M */
    AMHW_HC32F460_RTC_CLK_SRC_XTH_32MHz_DIV1024 = 7,  /* XTH 4M */
} amhw_hc32f460_rtc_clk_src_t;

am_static_inline
am_err_t amhw_hc32f460_rtc_deinit(amhw_hc32f460_rtc_t *p_hw_rtc)
{
    uint8_t u8RegSta;
    am_err_t enRet = AM_OK;
    
    p_hw_rtc->CR0_f.RESET = 0u;
    /* Waiting for normal count status or end of RTC software reset */    
    do
    {
        u8RegSta = (uint8_t)p_hw_rtc->CR0_f.RESET;
    } while ((1u == u8RegSta));

    if (1u == u8RegSta)
    {
        enRet = AM_ERROR;
    }
    else
    {
        /* Initialize all RTC registers */
        p_hw_rtc->CR0_f.RESET = 1u;
    }

    return enRet;
}

am_static_inline
am_err_t amhw_hc32f460_rtc_init(amhw_hc32f460_rtc_t *p_hw_rtc, const stc_rtc_init_t *pstcRtcInit)
{
    am_err_t enRet = AM_OK;

    if (NULL == pstcRtcInit)
    {
        enRet = AM_ERROR;
    }
    else
    {
        /* Configure clock */
        if (RtcClkLrc == pstcRtcInit->enClkSource)
        {
            p_hw_rtc->CR3_f.LRCEN = 1u;
        }
        p_hw_rtc->CR3_f.RCKSEL = pstcRtcInit->enClkSource;

        /* Configure control register */
        p_hw_rtc->CR1_f.PRDS = pstcRtcInit->enPeriodInt;
        p_hw_rtc->CR1_f.AMPM = pstcRtcInit->enTimeFormat;
        p_hw_rtc->CR1_f.ONEHZSEL = pstcRtcInit->enCompenWay;

        /* Configure clock error compensation register */
        p_hw_rtc->ERRCRH_f.COMP8 = ((uint32_t)pstcRtcInit->u16CompenVal >> 8u) & 0x01u;
        p_hw_rtc->ERRCRL = (uint32_t)pstcRtcInit->u16CompenVal & 0x00FFu;
        p_hw_rtc->ERRCRH_f.COMPEN = pstcRtcInit->enCompenEn;
    }

    return enRet;
}

am_static_inline
am_bool_t amhw_hc32f460_rtc_irq_cmd(amhw_hc32f460_rtc_t *p_hw_rtc, en_rtc_irq_type_t enIrq, en_rtc_functional_state_t enNewSta)
{
    am_bool_t enRet = AM_OK;

    /* enable/disable interrupt */
    switch (enIrq)
    {
        case RtcIrqPeriod:
            p_hw_rtc->CR2_f.PRDIE = enNewSta;
            break;
        case RtcIrqAlarm:
            p_hw_rtc->CR2_f.ALMIE = enNewSta;
            break;
        default:
            break;
    }

    return enRet;
}

am_static_inline
am_bool_t amhw_hc32f460_rtc_cmd(amhw_hc32f460_rtc_t *p_hw_rtc, en_rtc_functional_state_t enNewSta)
{
    am_bool_t enRet = AM_OK;

    p_hw_rtc->CR1_f.START = enNewSta;

    return enRet;
}

am_static_inline
am_err_t amhw_hc32f460_rtc_enter_rw_mode(amhw_hc32f460_rtc_t *p_hw_rtc)
{
    uint8_t u8RegSta;
    am_err_t enRet = AM_OK;


    /* Mode switch when RTC is running */
    if (0u != p_hw_rtc->CR1_f.START)
    {
        p_hw_rtc->CR2_f.WAIT = 1u;
        /* Waiting for WAITF bit set */
        do
        {
            u8RegSta = (uint8_t)p_hw_rtc->CR2_f.WAITF;
        } while ((0u == u8RegSta));

        if (0u == u8RegSta)
        {
            enRet = AM_ERROR;
        }
    }

    return enRet;
}

am_static_inline
am_err_t amhw_hc32f460_rtc_exit_rw_mode(amhw_hc32f460_rtc_t *p_hw_rtc)
{
    uint8_t u8RegSta;
    am_err_t enRet = AM_OK;
    
    /* Mode switch when RTC is running */
    if (0u != p_hw_rtc->CR1_f.START)
    {
        p_hw_rtc->CR2_f.WAIT = 0u;
        /* Waiting for WAITF bit reset */
        do
        {
            u8RegSta = (uint8_t)p_hw_rtc->CR2_f.WAITF;
        } while ((1u == u8RegSta));

        if (1u == u8RegSta)
        {
            enRet = AM_ERROR;
        }
    }

    return enRet;
}

am_static_inline
am_err_t amhw_hc32f460_rtc_set_data_time(amhw_hc32f460_rtc_t *p_hw_rtc, en_rtc_data_format_t enFormat, const stc_rtc_date_time_t *pstcRtcDateTime,
                            en_rtc_functional_state_t enUpdateDateEn, en_rtc_functional_state_t enUpdateTimeEn)
{
    am_err_t enRet = AM_OK;
    /* Check update status */
    if (((RTC_DISABLE == enUpdateDateEn) && (RTC_DISABLE == enUpdateTimeEn)) || (NULL == pstcRtcDateTime))
    {
        enRet = AM_ERROR;
    }
    else
    {
        /* Check the date parameters */
        if (RTC_ENABLE == enUpdateDateEn)
        {
            if (RtcDataFormatDec == enFormat)
            {
                ;
            }
            else
            {
//                BCD2DEC(pstcRtcDateTime->u8Year);
//                BCD2DEC(pstcRtcDateTime->u8Month);
//                BCD2DEC(pstcRtcDateTime->u8Day);
            }
        }
        /* Check the time parameters */
        if (RTC_ENABLE == enUpdateTimeEn)
        {
            if (RtcDataFormatDec == enFormat)
            {
                if (RtcTimeFormat12Hour == p_hw_rtc->CR1_f.AMPM)
                {
                    ;
                }
                else  /* RtcTimeFormat24Hour */
                {
                    ;
                }
            }
            else
            {
                if (RtcTimeFormat12Hour == p_hw_rtc->CR1_f.AMPM)
                {
//                    BCD2DEC(pstcRtcDateTime->u8Hour);
                }
                else
                {
//                    BCD2DEC(pstcRtcDateTime->u8Hour);
                }
//                BCD2DEC(pstcRtcDateTime->u8Minute);
//                BCD2DEC(pstcRtcDateTime->u8Second);
            }
        }

        /* Enter read/write mode */
        if (amhw_hc32f460_rtc_enter_rw_mode(p_hw_rtc) == AM_ERROR)
        {
            enRet = AM_ERROR;
        }
        else
        {
            /* Update date */
            if (RTC_ENABLE == enUpdateDateEn)
            {
                if (RtcDataFormatDec == enFormat)
                {
                    p_hw_rtc->YEAR = DEC2BCD((uint32_t)pstcRtcDateTime->u8Year);
                    p_hw_rtc->MON = DEC2BCD((uint32_t)pstcRtcDateTime->u8Month);
                    p_hw_rtc->DAY = DEC2BCD((uint32_t)pstcRtcDateTime->u8Day);
                }
                else
                {
                    p_hw_rtc->YEAR = pstcRtcDateTime->u8Year;
                    p_hw_rtc->MON = pstcRtcDateTime->u8Month;
                    p_hw_rtc->DAY = pstcRtcDateTime->u8Day;
                }
                p_hw_rtc->WEEK = pstcRtcDateTime->u8Weekday;
            }
            /* Update time */
            if (RTC_ENABLE == enUpdateTimeEn)
            {
                if (RtcDataFormatDec == enFormat)
                {
                    if ((RtcTimeFormat12Hour == p_hw_rtc->CR1_f.AMPM) &&
                        (RtcHour12Pm == pstcRtcDateTime->enAmPm))
                    {
                        p_hw_rtc->HOUR = DEC2BCD((uint32_t)pstcRtcDateTime->u8Hour) | RTC_HOUR12_AMPM_MASK;
                    }
                    else
                    {
                        p_hw_rtc->HOUR = DEC2BCD((uint32_t)pstcRtcDateTime->u8Hour);
                    }
                    p_hw_rtc->MIN = DEC2BCD((uint32_t)pstcRtcDateTime->u8Minute);
                    p_hw_rtc->SEC = DEC2BCD((uint32_t)pstcRtcDateTime->u8Second);
                }
                else
                {
                    if ((RtcTimeFormat12Hour == p_hw_rtc->CR1_f.AMPM) &&
                        (RtcHour12Pm == pstcRtcDateTime->enAmPm))
                    {
                        p_hw_rtc->HOUR = (uint32_t)pstcRtcDateTime->u8Hour | RTC_HOUR12_AMPM_MASK;
                    }
                    else
                    {
                        p_hw_rtc->HOUR = (uint32_t)pstcRtcDateTime->u8Hour;
                    }
                    p_hw_rtc->MIN = pstcRtcDateTime->u8Minute;
                    p_hw_rtc->SEC = pstcRtcDateTime->u8Second;
                }
            }
            /* Exit read/write mode */
            if (amhw_hc32f460_rtc_exit_rw_mode(p_hw_rtc) == AM_ERROR)
            {
                enRet = AM_ERROR;
            }
        }
    }

    return enRet;
}

am_static_inline
am_err_t amhw_hc32f460_rtc_get_data_time(amhw_hc32f460_rtc_t *p_hw_rtc, en_rtc_data_format_t enFormat, stc_rtc_date_time_t *pstcRtcDateTime)
{
    am_err_t enRet = AM_OK;

    if(NULL == pstcRtcDateTime)
    {
        enRet = AM_ERROR;
    }
    else
    {
        /* Enter read/write mode */
        if (amhw_hc32f460_rtc_enter_rw_mode(p_hw_rtc) == AM_ERROR)
        {
            enRet = AM_ERROR;
        }
        else
        {
            /* Get RTC date and time registers */
            pstcRtcDateTime->u8Year = (uint8_t)(p_hw_rtc->YEAR);
            pstcRtcDateTime->u8Month = (uint8_t)(p_hw_rtc->MON);
            pstcRtcDateTime->u8Day = (uint8_t)(p_hw_rtc->DAY);
            pstcRtcDateTime->u8Weekday = (uint8_t)(p_hw_rtc->WEEK);
            pstcRtcDateTime->u8Hour = (uint8_t)(p_hw_rtc->HOUR);
            pstcRtcDateTime->u8Minute = (uint8_t)(p_hw_rtc->MIN);
            pstcRtcDateTime->u8Second = (uint8_t)(p_hw_rtc->SEC);
            if (RtcTimeFormat12Hour == p_hw_rtc->CR1_f.AMPM)
            {
                if (RTC_HOUR12_AMPM_MASK == (pstcRtcDateTime->u8Hour & RTC_HOUR12_AMPM_MASK))
                {
                    pstcRtcDateTime->u8Hour &= (uint8_t)(~RTC_HOUR12_AMPM_MASK);
                    pstcRtcDateTime->enAmPm = RtcHour12Pm;
                }
                else
                {
                    pstcRtcDateTime->enAmPm = RtcHour12Am;
                }
            }

            /* Check decimal format*/
            if (RtcDataFormatDec == enFormat)
            {
                pstcRtcDateTime->u8Year = BCD2DEC(pstcRtcDateTime->u8Year);
                pstcRtcDateTime->u8Month = BCD2DEC(pstcRtcDateTime->u8Month);
                pstcRtcDateTime->u8Day = BCD2DEC(pstcRtcDateTime->u8Day);
                pstcRtcDateTime->u8Hour = BCD2DEC(pstcRtcDateTime->u8Hour);
                pstcRtcDateTime->u8Minute = BCD2DEC(pstcRtcDateTime->u8Minute);
                pstcRtcDateTime->u8Second = BCD2DEC(pstcRtcDateTime->u8Second);
            }

            /* exit read/write mode */
            if (amhw_hc32f460_rtc_exit_rw_mode(p_hw_rtc) == AM_ERROR)
            {
                enRet = AM_ERROR;
            }
        }
    }

    return enRet;
}

/**
 * \brief 使用匿名联合体段结束
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)

  /* 允许匿名联合体使能 */
#elif defined(__GNUC__)

  /* 默认使用匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif
/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif/* __HC32_RTC_H */

/* end of file */
