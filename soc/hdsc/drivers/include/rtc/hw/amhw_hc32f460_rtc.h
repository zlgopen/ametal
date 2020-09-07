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

/**
 * \brief RTC 控制寄存器0位域结构体
 */
typedef struct
{
    __IO uint32_t RESET                     : 1;  /**< \brief RTC日历计数器复位 */
    uint32_t RESERVED1                      :31;  /**< \brief 保留 */
} stc_rtc_cr0_field_t;

/**
 * \brief RTC 控制寄存器1位域结构体
 */
typedef struct
{
    __IO uint32_t PRDS                      : 3;  /**< \brief 周期中断选择 */
    __IO uint32_t AMPM                      : 1;  /**< \brief 时制选择 */
    __IO uint32_t ALMFCLR                   : 1;
    __IO uint32_t ONEHZOE                   : 1;  /**< \brief 1Hz输出使能 */
    __IO uint32_t ONEHZSEL                  : 1;  /**< \brief 1Hz输出选择 */
    __IO uint32_t START                     : 1;  /**< \brief RTC计数开始 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_rtc_cr1_field_t;

/**
 * \brief RTC 控制寄存器2位域结构体
 */
typedef struct
{
    __IO uint32_t WAIT                      : 1;  /**< \brief 写入/读出控制 */
    __IO uint32_t WAITF                     : 1;  /**< \brief 等待标志 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t ALMF                      : 1;  /**< \brief 闹钟标志 */
    uint32_t RESERVED4                      : 1;  /**< \brief 保留 */
    __IO uint32_t PRDIE                     : 1;  /**< \brief 周期中断使能 */
    __IO uint32_t ALMIE                     : 1;  /**< \brief 闹钟中断使能 */
    __IO uint32_t ALME                      : 1;  /**< \brief 闹钟使能 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_rtc_cr2_field_t;

/**
 * \brief RTC 控制寄存器3位域结构体
 */
typedef struct
{
    uint32_t RESERVED0                      : 4;  /**< \brief 保留 */
    __IO uint32_t LRCEN                     : 1;  /**< \brief 低速振发器使能 */
    uint32_t RESERVED5                      : 2;  /**< \brief 保留 */
    __IO uint32_t RCKSEL                    : 1;  /**< \brief RTC计数时钟选择 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_rtc_cr3_field_t;

/**
 * \brief RTC 秒计数寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SECU                      : 4;  /**< \brief 秒个位计数值  */
    __IO uint32_t SECD                      : 3;  /**< \brief 秒十位计数值 */
    uint32_t RESERVED7                      :25;  /**< \brief 保留 */
} stc_rtc_sec_field_t;

/**
 * \brief RTC 分计数寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t MINU                      : 4;  /**< \brief 分个位计数值  */
    __IO uint32_t MIND                      : 3;  /**< \brief 分十位计数值  */
    uint32_t RESERVED7                      :25;  /**< \brief 保留 */
} stc_rtc_min_field_t;

/**
 * \brief RTC 时计数寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t HOURU                     : 4;  /**< \brief 时个位计数值  */
    __IO uint32_t HOURD                     : 2;  /**< \brief 时十位计数值  */
    uint32_t RESERVED6                      :26;  /**< \brief 保留 */
} stc_rtc_hour_field_t;

/**
 * \brief RTC 周计数寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t WEEK                      : 3;  /**< \brief 周计数值  */
    uint32_t RESERVED3                      :29;  /**< \brief 保留 */
} stc_rtc_week_field_t;

/**
 * \brief RTC 日计数寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DAYU                      : 4;  /**< \brief 日个位计数值  */
    __IO uint32_t DAYD                      : 2;  /**< \brief 日十位计数值  */
    uint32_t RESERVED6                      :26;  /**< \brief 保留 */
} stc_rtc_day_field_t;

/**
 * \brief RTC 月计数寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t MON                       : 5;  /**< \brief 月计数值 */
    uint32_t RESERVED5                      :27;  /**< \brief 保留 */
} stc_rtc_mon_field_t;

/**
 * \brief RTC 年计数寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t YEARU                     : 4;  /**< \brief 年个位 */
    __IO uint32_t YEARD                     : 4;  /**< \brief 年十位 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_rtc_year_field_t;

/**
 * \brief RTC 分闹钟寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t ALMMINU                   : 4;  /**< \brief 分闹钟个位 */
    __IO uint32_t ALMMIND                   : 3;  /**< \brief 分闹钟十位 */
    uint32_t RESERVED7                      :25;  /**< \brief 保留 */
} stc_rtc_almmin_field_t;

/**
 * \brief RTC 时闹钟寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t ALMHOURU                  : 4;  /**< \brief 时闹钟个位 */
    __IO uint32_t ALMHOURD                  : 2;  /**< \brief 时闹钟十位 */
    uint32_t RESERVED6                      :26;  /**< \brief 保留 */
} stc_rtc_almhour_field_t;

/**
 * \brief RTC 周闹钟寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t ALMWEEK                   : 7;  /**< \brief 周闹钟匹配值 */
    uint32_t RESERVED7                      :25;  /**< \brief 保留 */
} stc_rtc_almweek_field_t;

/**
 * \brief RTC 时钟误差补偿寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t COMP8                     : 1;  /**< \brief 补偿值 */
    uint32_t RESERVED1                      : 6;  /**< \brief 保留 */
    __IO uint32_t COMPEN                    : 1;  /**< \brief 补偿使能 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_rtc_errcrh_field_t;

/**
 * \brief RTC 时钟误差补偿寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t COMP                      : 8;  /**< \brief 补偿值 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_rtc_errcrl_field_t;

/**
 * \brief RTC 寄存器结构体定义
 */
typedef struct amhw_hc32f460_rtc {
    union
    {
        __IO uint32_t CR0;        /**< \brief 控制寄存器0 */
        stc_rtc_cr0_field_t CR0_f;
    };
    union
    {
        __IO uint32_t CR1;        /**< \brief 控制寄存器1 */
        stc_rtc_cr1_field_t CR1_f;
    };
    union
    {
        __IO uint32_t CR2;        /**< \brief 控制寄存器2 */
        stc_rtc_cr2_field_t CR2_f;
    };
    union
    {
        __IO uint32_t CR3;        /**< \brief 控制寄存器3 */
        stc_rtc_cr3_field_t CR3_f;
    };
    union
    {
        __IO uint32_t SEC;        /**< \brief 秒计数寄存器 */
        stc_rtc_sec_field_t SEC_f;
    };
    union
    {
        __IO uint32_t MIN;        /**< \brief 分计数寄存器 */
        stc_rtc_min_field_t MIN_f;
    };
    union
    {
        __IO uint32_t HOUR;       /**< \brief 时计数寄存器 */
        stc_rtc_hour_field_t HOUR_f;
    };
    union
    {
        __IO uint32_t WEEK;       /**< \brief 周计数寄存器 */
        stc_rtc_week_field_t WEEK_f;
    };
    union
    {
        __IO uint32_t DAY;        /**< \brief 日计数寄存器 */
        stc_rtc_day_field_t DAY_f;
    };
    union
    {
        __IO uint32_t MON;        /**< \brief 月计数寄存器 */
        stc_rtc_mon_field_t MON_f;
    };
    union
    {
        __IO uint32_t YEAR;       /**< \brief 年计数寄存器 */
        stc_rtc_year_field_t YEAR_f;
    };
    union
    {
        __IO uint32_t ALMMIN;      /**< \brief 分闹钟寄存器 */
        stc_rtc_almmin_field_t ALMMIN_f;
    };
    union
    {
        __IO uint32_t ALMHOUR;     /**< \brief 时闹钟寄存器 */
        stc_rtc_almhour_field_t ALMHOUR_f;
    };
    union
    {
        __IO uint32_t ALMWEEK;     /**< \brief 周闹钟寄存器 */
        stc_rtc_almweek_field_t ALMWEEK_f;
    };
    union
    {
        __IO uint32_t ERRCRH;      /**< \brief 时钟误差补偿寄存器 */
        stc_rtc_errcrh_field_t ERRCRH_f;
    };
    union
    {
        __IO uint32_t ERRCRL;      /**< \brief 时钟误差补偿寄存器 */
        stc_rtc_errcrl_field_t ERRCRL_f;
    };
} amhw_hc32f460_rtc_t;

typedef enum en_rtc_clk_source
{
    RtcClkXtal32 = 0u,                  /**< \brief XTAL32 as clock source*/
    RtcClkLrc    = 1u,                  /**< \brief LRC as clock source*/
} en_rtc_clk_source_t;

typedef enum en_rtc_period_int_type
{
    RtcPeriodIntInvalid = 0u,           /**< \brief Period interrupt invalid*/
    RtcPeriodIntHalfSec = 1u,           /**< \brief 0.5 second period interrupt*/
    RtcPeriodIntOneSec  = 2u,           /**< \brief 1 second period interrupt*/
    RtcPeriodIntOneMin  = 3u,           /**< \brief 1 minute period interrupt*/
    RtcPeriodIntOneHour = 4u,           /**< \brief 1 hour period interrupt*/
    RtcPeriodIntOneDay  = 5u,           /**< \brief 1 day period interrupt*/
    RtcPeriodIntOneMon  = 6u            /**< \brief 1 month period interrupt*/
} en_rtc_period_int_type_t;

typedef enum en_rtc_time_format
{
    RtcTimeFormat12Hour = 0u,           /**< \brief 12 hours mode*/
    RtcTimeFormat24Hour = 1u,           /**< \brief 24 hours mode*/
} en_rtc_time_format_t;

typedef enum en_rtc_output_compen
{
    RtcOutputCompenDistributed = 0u,    /**< \brief Distributed compensation 1hz output*/
    RtcOutputCompenUniform     = 1u,    /**< \brief Uniform Compensation 1hz output*/
} en_rtc_output_compen_t;

typedef enum en_rtc_functional_state
{
    RTC_DISABLE = 0u,
    RTC_ENABLE  = 1u,
} en_rtc_functional_state_t;


typedef struct stc_rtc_init
{
    en_rtc_clk_source_t      enClkSource;       /**< \brief Clock source*/
    en_rtc_period_int_type_t enPeriodInt;       /**< \brief Period interrupt condition*/
    en_rtc_time_format_t     enTimeFormat;      /**< \brief RTC time format*/
    en_rtc_output_compen_t   enCompenWay;       /**< \brief 1HZ output compensation way*/
    uint16_t                 u16CompenVal;      /**< \brief Clock error compensation value*/
    en_rtc_functional_state_t    enCompenEn;    /**< \brief Enable/Disable clock error compensation*/
} stc_rtc_init_t;

typedef enum en_rtc_irq_type_
{
    RtcIrqPeriod = 0u,                  /**< \brief Period count interrupt request*/
    RtcIrqAlarm  = 1u,                  /**< \brief Alarm interrupt request*/
} en_rtc_irq_type_t;

typedef enum en_rtc_hour12_ampm
{
    RtcHour12Am = 0u,                   /**< \brief Ante meridiem*/
    RtcHour12Pm = 1u,                   /**< \brief Post meridiem*/
} en_rtc_hour12_ampm_t;

typedef struct stc_rtc_date_time
{
    uint8_t              u8Year;        /**< \brief Year (range 0-99)*/
    uint8_t              u8Month;       /**< \brief Month (range 1-12)*/
    uint8_t              u8Day;         /**< \brief Day (range 1-31)*/
    uint8_t              u8Hour;        /**< \brief Hours (range 1-12 when 12 hour format; range 0-23 when 24 hour format)*/
    uint8_t              u8Minute;      /**< \brief Minutes (range 0-59)*/
    uint8_t              u8Second;      /**< \brief Seconds (range 0-59)*/
    uint8_t              u8Weekday;     /**< \brief Weekday (range 0-6)*/
    en_rtc_hour12_ampm_t enAmPm;        /**< \brief The value is valid when 12-hour format*/
} stc_rtc_date_time_t;

typedef enum en_rtc_weekday
{
    RtcWeekdaySunday    = 0u,           /**< \brief Sunday*/
    RtcWeekdayMonday    = 1u,           /**< \brief Monday*/
    RtcWeekdayTuesday   = 2u,           /**< \brief Tuesday*/
    RtcWeekdayWednesday = 3u,           /**< \brief Wednesday*/
    RtcWeekdayThursday  = 4u,           /**< \brief Thursday*/
    RtcWeekdayFriday    = 5u,           /**< \brief Friday*/
    RtcWeekdaySaturday  = 6u            /**< \brief Saturday*/
} en_rtc_weekday_t;

typedef enum en_rtc_data_format
{
    RtcDataFormatDec = 0u,              /**< \brief Decimal format*/
    RtcDataFormatBcd = 1u,              /**< \brief BCD format*/
} en_rtc_data_format_t;

/** \brief  Decimal to BCD */
#define DEC2BCD(x)                      ((((x) / 10u) << 4u) + ((x) % 10u))

/** \brief  BCD to decimal */
#define BCD2DEC(x)                      ((((x) >> 4u) * 10u) + ((x) & 0x0Fu))

#define RTC_HOUR12_AMPM_MASK                    (0x20u)

/**
 * \brief RTC周期中断选择
 */
typedef enum {
    AMHW_HC32F460_RTC_PERIOD_INT_TYPE_CUSTOM  = 0,    /**< \brief 用户自定义时间 */
    AMHW_HC32F460_RTC_PERIOD_INT_TYPE_SYSTEAM = 1,    /**< \brief 外设规定内容选择 */
} amhw_hc32f460_rtc_period_int_type_t;

/**
 * \brief RTC周期中断时间选择
 */
typedef enum {
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_NO      = 0ul,    /**< \brief 不产生周期中断 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_0_5_SEC = 1ul,    /**< \brief 0.5秒 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_SEC   = 2ul,    /**< \brief 1秒 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_MIN   = 3ul,    /**< \brief 1分钟 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_HOUR  = 4ul,    /**< \brief 1小时 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_DAY   = 5ul,    /**< \brief 1天 */
    AMHW_HC32F460_RTC_PERIOD_INT_TIME_1_MON   = 6ul,    /**< \brief 1月 */
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
} amhw_hc32f460_rtcclk_adjust;

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

}

/**
 * \brief RTC小时制选择枚举
 */
typedef enum {
    AMHW_HC32F460_RTC_TIME_MODE_12h = 0ul,    /**< \brief 12小时制 */
    AMHW_HC32F460_RTC_TIME_MODE_24H = 1ul,    /**< \brief 24小时制 */
} amhw_hc32f460_rtc_time_mode_t;

/**
 * \brief RTC时钟选择枚举
 */
typedef enum {
    AMHW_HC32F460_RTC_CLK_SRC_XTL_32768Hz       = 0,  /**< \brief XTL 32.768k */
    AMHW_HC32F460_RTC_CLK_SRC_RCL_32KHz         = 2,  /**< \brief RCL 32k */
    AMHW_HC32F460_RTC_CLK_SRC_XTH_4MHz_DIV128   = 4,  /**< \brief XTH 4M */
    AMHW_HC32F460_RTC_CLK_SRC_XTH_8MHz_DIV256   = 5,  /**< \brief XTH 4M */
    AMHW_HC32F460_RTC_CLK_SRC_XTH_16MHz_DIV512  = 6,  /**< \brief XTH 4M */
    AMHW_HC32F460_RTC_CLK_SRC_XTH_32MHz_DIV1024 = 7,  /**< \brief XTH 4M */
} amhw_hc32f460_rtc_clk_src_t;

am_static_inline
am_err_t amhw_hc32f460_rtc_deinit(amhw_hc32f460_rtc_t *p_hw_rtc)
{
    uint8_t u8RegSta;
    am_err_t enRet = AM_OK;
    
    p_hw_rtc->CR0_f.RESET = 0u;
    /** \brief Waiting for normal count status or end of RTC software reset */
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
        /** \brief Initialize all RTC registers */
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
        /** \brief Configure clock */
        if (RtcClkLrc == pstcRtcInit->enClkSource)
        {
            p_hw_rtc->CR3_f.LRCEN = 1u;
        }
        p_hw_rtc->CR3_f.RCKSEL = pstcRtcInit->enClkSource;

        /** \brief Configure control register */
        p_hw_rtc->CR1_f.PRDS = pstcRtcInit->enPeriodInt;
        p_hw_rtc->CR1_f.AMPM = pstcRtcInit->enTimeFormat;
        p_hw_rtc->CR1_f.ONEHZSEL = pstcRtcInit->enCompenWay;

        /** \brief Configure clock error compensation register */
        p_hw_rtc->ERRCRH_f.COMP8 = ((uint32_t)pstcRtcInit->u16CompenVal >> 8u) & 0x01u;
        p_hw_rtc->ERRCRL = (uint32_t)pstcRtcInit->u16CompenVal & 0x00FFu;
        p_hw_rtc->ERRCRH_f.COMPEN = pstcRtcInit->enCompenEn;
    }

    return enRet;
}

am_static_inline
am_bool_t amhw_hc32f460_rtc_irq_cmd(amhw_hc32f460_rtc_t      *p_hw_rtc,
                                    en_rtc_irq_type_t         enIrq,
                                    en_rtc_functional_state_t enNewSta)
{
    am_bool_t enRet = AM_OK;

    /** \brief enable/disable interrupt */
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
am_bool_t amhw_hc32f460_rtc_cmd(amhw_hc32f460_rtc_t      *p_hw_rtc,
                                en_rtc_functional_state_t enNewSta)
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


    /** \brief Mode switch when RTC is running */
    if (0u != p_hw_rtc->CR1_f.START)
    {
        p_hw_rtc->CR2_f.WAIT = 1u;

        /** \brief Waiting for WAITF bit set */
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
    
    /** \brief Mode switch when RTC is running */
    if (0u != p_hw_rtc->CR1_f.START)
    {
        p_hw_rtc->CR2_f.WAIT = 0u;

        /** \brief Waiting for WAITF bit reset */
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
am_err_t amhw_hc32f460_rtc_set_data_time(amhw_hc32f460_rtc_t       *p_hw_rtc,
                                         en_rtc_data_format_t       enFormat,
                                         const stc_rtc_date_time_t *pstcRtcDateTime,
                                         en_rtc_functional_state_t  enUpdateDateEn,
                                         en_rtc_functional_state_t  enUpdateTimeEn)
{
    am_err_t enRet = AM_OK;
    /** \brief Check update status */
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
            /** \brief Exit read/write mode */
            if (amhw_hc32f460_rtc_exit_rw_mode(p_hw_rtc) == AM_ERROR)
            {
                enRet = AM_ERROR;
            }
        }
    }

    return enRet;
}

am_static_inline
am_err_t amhw_hc32f460_rtc_get_data_time(amhw_hc32f460_rtc_t *p_hw_rtc,
                                         en_rtc_data_format_t enFormat,
                                         stc_rtc_date_time_t *pstcRtcDateTime)
{
    am_err_t enRet = AM_OK;

    if(NULL == pstcRtcDateTime)
    {
        enRet = AM_ERROR;
    }
    else
    {
        /** \brief Enter read/write mode */
        if (amhw_hc32f460_rtc_enter_rw_mode(p_hw_rtc) == AM_ERROR)
        {
            enRet = AM_ERROR;
        }
        else
        {
            /** \brief Get RTC date and time registers */
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

            /** \brief Check decimal format*/
            if (RtcDataFormatDec == enFormat)
            {
                pstcRtcDateTime->u8Year = BCD2DEC(pstcRtcDateTime->u8Year);
                pstcRtcDateTime->u8Month = BCD2DEC(pstcRtcDateTime->u8Month);
                pstcRtcDateTime->u8Day = BCD2DEC(pstcRtcDateTime->u8Day);
                pstcRtcDateTime->u8Hour = BCD2DEC(pstcRtcDateTime->u8Hour);
                pstcRtcDateTime->u8Minute = BCD2DEC(pstcRtcDateTime->u8Minute);
                pstcRtcDateTime->u8Second = BCD2DEC(pstcRtcDateTime->u8Second);
            }

            /** \brief exit read/write mode */
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

#endif/* __HC32F460_RTC_H */

/* end of file */
