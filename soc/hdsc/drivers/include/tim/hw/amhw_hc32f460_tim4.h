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
 * \brief 定时器TIM接口
 *
 * \internal
 * \par History
 * - 1.00 
 * \endinternal
 */

#ifndef __AMHW_HC32F460_TIM4_H
#define __AMHW_HC32F460_TIM4_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

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
 *******************************************************************************
 ** \brief Timer4 counter mode enumeration
 **
 ******************************************************************************/
typedef enum tim4_functional_state
{
    TIM4_DISABLE = 0u,
    TIM4_ENABLE  = 1u,
} tim4_functional_state_t;

typedef enum tim4_counter_mode
{
    TIM4_COUNT_MODE_SAWTOOTH_WAVE   = 0u, 
    TIM4_COUNT_MODE_TRIANGULAR_WAVE = 1u, 
}tim4_counter_mode_t;

typedef enum tim4_count_dir
{
    TIM4_COUNT_DIR_DOWN = 0u,            
    TIM4_COUNT_DIR_UP   = 1u,            
} tim4_count_dir_t;

typedef enum tim4_irq_type
{
    TIMER4_IRQ_CAPTURE_OR_COMPARE_CH1 = 0u,               
    TIMER4_IRQ_CAPTURE_OR_COMPARE_CH2 = 1u,              
    TIMER4_IRQ_CAPTURE_OR_COMPARE_CH3 = 2u,              
    TIMER4_IRQ_CAPTURE_OR_COMPARE_CH4 = 3u,              
    TIMER4_IRQ_CAPTURE_OR_COMPARE_CH5 = 4u,              
    TIMER4_IRQ_CAPTURE_OR_COMPARE_CH6 = 5u,              
    TIMER4_IRQ_CAPTURE_OR_COMPARE_CH7 = 6u,              
    TIMER4_IRQ_CAPTURE_OR_COMPARE_CH8 = 7u,              
    TIMER4_IRQ_OVERFLOW               = 8u,              
    TIMER4_IRQ_UNDERFLOW              = 9u,              
} tim4_irq_type_t;

/******************************************************************************/
typedef enum timer4_cnt_int
{
    TIMER4_CNT_ZERO_MATCH_INT = (1ul << 8),    
    TIMER4_CNT_PEAK_MATCH_INT = (1ul << 13),    
} timer4_cnt_int_t;

/**
 * \brief TMR4 通用控制状态寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t OCEH                      : 1;  /**< \brief 端口输出使能H*/
    __IO uint16_t OCEL                      : 1;  /**< \brief 端口输出使能L*/
    __IO uint16_t OCPH                      : 1;  /**< \brief 输出无效时端口状态H*/
    __IO uint16_t OCPL                      : 1;  /**< \brief 输出无效时端口状态L*/
    __IO uint16_t OCIEH                     : 1;  /**< \brief 计数匹配H中断使能*/
    __IO uint16_t OCIEL                     : 1;  /**< \brief 计数匹配L中断使能*/
    __IO uint16_t OCFH                      : 1;  /**< \brief 计数匹配H*/
    __IO uint16_t OCFL                      : 1;  /**< \brief 计数匹配L*/
    uint16_t RESERVED8                      : 8;  /**< \brief 保留*/
} stc_tmr4_ocsr_field_t;

/**
 * \brief TMR4 通用扩展控制寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t CHBUFEN                   : 2;  /**< \brief OCCRxh缓存传送*/
    __IO uint16_t CLBUFEN                   : 2;  /**< \brief OCCRxl缓存传送*/
    __IO uint16_t MHBUFEN                   : 2;  /**< \brief OCMRxh缓存转送*/
    __IO uint16_t MLBUFEN                   : 2;  /**< \brief OCMRxl缓存转送*/
    __IO uint16_t LMCH                      : 1;  /**< \brief 周期间隔响应功能链接H*/
    __IO uint16_t LMCL                      : 1;  /**< \brief 周期间隔响应功能链接L*/
    __IO uint16_t LMMH                      : 1;  /**< \brief 周期间隔响应功能链接H*/
    __IO uint16_t LMML                      : 1;  /**< \brief 周期间隔响应功能链接L*/
    __IO uint16_t MCECH                     : 1;  /**< \brief 扩展控制使能H*/
    __IO uint16_t MCECL                     : 1;  /**< \brief 扩展控制使能L*/
    uint16_t RESERVED14                     : 2;  /**< \brief 保留*/
} stc_tmr4_ocer_field_t;

/**
 * \brief TMR4 通用模式控制寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t OCFDCH                    : 1;  /**< \brief 向下计数OCFH状态H*/
    __IO uint16_t OCFPKH                    : 1;  /**< \brief 上溢点OCFH状态H*/
    __IO uint16_t OCFUCH                    : 1;  /**< \brief 向上计数OCFH状态H*/
    __IO uint16_t OCFZRH                    : 1;  /**< \brief 下溢点OCFH状态H*/
    __IO uint16_t OPDCH                     : 2;  /**< \brief 向下计数端口状态H*/
    __IO uint16_t OPPKH                     : 2;  /**< \brief 上溢点端口状态H*/
    __IO uint16_t OPUCH                     : 2;  /**< \brief 向上计数端口状态H*/
    __IO uint16_t OPZRH                     : 2;  /**< \brief 下溢点端口状态H*/
    __IO uint16_t OPNPKH                    : 2;  /**< \brief 上溢点端口状态H*/
    __IO uint16_t OPNZRH                    : 2;  /**< \brief 下溢点端口状态H*/
} stc_tmr4_ocmrh_field_t;

/**
 * \brief TMR4 通用模式控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t OCFDCL                    : 1;  /**< \brief 向下计数OCFL状态L*/
    __IO uint32_t OCFPKL                    : 1;  /**< \brief 上溢点OCFL状态L*/
    __IO uint32_t OCFUCL                    : 1;  /**< \brief 向上计数OCFL状态L*/
    __IO uint32_t OCFZRL                    : 1;  /**< \brief 下溢点OCFL状态L*/
    __IO uint32_t OPDCL                     : 2;  /**< \brief 向下计数端口状态L*/
    __IO uint32_t OPPKL                     : 2;  /**< \brief 上溢点端口状态L*/
    __IO uint32_t OPUCL                     : 2;  /**< \brief 向上计数端口状态L*/
    __IO uint32_t OPZRL                     : 2;  /**< \brief 下溢点端口状态L*/
    __IO uint32_t OPNPKL                    : 2;  /**< \brief 上溢点端口状态L*/
    __IO uint32_t OPNZRL                    : 2;  /**< \brief 下溢点端口状态L*/
    __IO uint32_t EOPNDCL                   : 2;  /**< \brief 扩张向下计数端口状态L*/
    __IO uint32_t EOPNUCL                   : 2;  /**< \brief 扩张向上计数端口状态L*/
    __IO uint32_t EOPDCL                    : 2;  /**< \brief 扩张向下计数端口状态L*/
    __IO uint32_t EOPPKL                    : 2;  /**< \brief 扩张上溢点端口状态L*/
    __IO uint32_t EOPUCL                    : 2;  /**< \brief 扩张向上计数端口状态L*/
    __IO uint32_t EOPZRL                    : 2;  /**< \brief 扩张下溢点端口*/
    __IO uint32_t EOPNPKL                   : 2;  /**< \brief 扩张上溢点端口状态L*/
    __IO uint32_t EOPNZRL                   : 2;  /**< \brief 扩张下溢点端口状态L*/
} stc_tmr4_ocmrl_field_t;

/**
 * \brief TMR4 控制状态寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t CKDIV                     : 4;  /**< \brief 计数时钟分频*/
    __IO uint16_t CLEAR                     : 1;  /**< \brief 计数器清零*/
    __IO uint16_t MODE                      : 1;  /**< \brief 波形模式*/
    __IO uint16_t STOP                      : 1;  /**< \brief 计数器使能*/
    __IO uint16_t BUFEN                     : 1;  /**< \brief 缓存使能*/
    __IO uint16_t IRQPEN                    : 1;  /**< \brief 上溢中断使能*/
    __IO uint16_t IRQPF                     : 1;  /**< \brief 上溢状态*/
    uint16_t RESERVED10                     : 3;  /**< \brief 保留*/
    __IO uint16_t IRQZEN                    : 1;  /**< \brief 下溢中断使能*/
    __IO uint16_t IRQZF                     : 1;  /**< \brief 下溢状态*/
    __IO uint16_t ECKEN                     : 1;  /**< \brief 时钟源选择*/
} stc_tmr4_ccsr_field_t;

/**
 * \brief TMR4 有效周期寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t ZIM                       : 4;  /**< \brief 下溢中断屏蔽设定*/
    __IO uint16_t PIM                       : 4;  /**< \brief 上溢中断屏蔽设定*/
    __IO uint16_t ZIC                       : 4;  /**< \brief 下溢中断屏蔽状态*/
    __IO uint16_t PIC                       : 4;  /**< \brief 上溢中断屏蔽状态*/
} stc_tmr4_cvpr_field_t;

/**
 * \brief TMR4 PWM基本控制寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t DIVCK                     : 4;  /**< \brief 计数时钟分频*/
    __IO uint16_t PWMMD                     : 2;  /**< \brief PWM输出模式*/
    __IO uint16_t LVLS                      : 2;  /**< \brief PWM输出极性控制*/
    uint16_t RESERVED8                      : 8;  /**< \brief 保留*/
} stc_tmr4_pocr_field_t;

/**
 * \brief TMR4 重载控制状态寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t RTIDU                     : 1;  /**< \brief 重载中断屏蔽U*/
    __IO uint16_t RTIDV                     : 1;  /**< \brief 重载中断屏蔽V*/
    __IO uint16_t RTIDW                     : 1;  /**< \brief 重载中断屏蔽W*/
    uint16_t RESERVED3                      : 1;  /**< \brief 保留*/
    __IO uint16_t RTIFU                     : 1;  /**< \brief 计数匹配状态U*/
    __IO uint16_t RTICU                     : 1;  /**< \brief 清零计数匹配状态U*/
    __IO uint16_t RTEU                      : 1;  /**< \brief 重载计数器启动U*/
    __IO uint16_t RTSU                      : 1;  /**< \brief 重载计数器停止U*/
    __IO uint16_t RTIFV                     : 1;  /**< \brief 计数匹配状态V*/
    __IO uint16_t RTICV                     : 1;  /**< \brief 清零计数匹配状态V*/
    __IO uint16_t RTEV                      : 1;  /**< \brief 重载计数器启动V*/
    __IO uint16_t RTSV                      : 1;  /**< \brief 重载计数器停止V*/
    __IO uint16_t RTIFW                     : 1;  /**< \brief 计数匹配状态W*/
    __IO uint16_t RTICW                     : 1;  /**< \brief 清零计数匹配状态W*/
    __IO uint16_t RTEW                      : 1;  /**< \brief 重载计数器启动W*/
    __IO uint16_t RTSW                      : 1;  /**< \brief 重载计数器停止W*/
} stc_tmr4_rcsr_field_t;

/**
 * \brief TMR4 专用控制状态寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t BUFEN                     : 2;  /**< \brief SCCR&SCMR缓存传送*/
    __IO uint16_t EVTOS                     : 3;  /**< \brief EVT输出选择*/
    __IO uint16_t LMC                       : 1;  /**< \brief 周期间隔响应功能链接*/
    uint16_t RESERVED6                      : 2;  /**< \brief 保留*/
    __IO uint16_t EVTMS                     : 1;  /**< \brief EVT模式选择*/
    __IO uint16_t EVTDS                     : 1;  /**< \brief EVT延时对象选择*/
    uint16_t RESERVED10                     : 2;  /**< \brief 保留*/
    __IO uint16_t DEN                       : 1;  /**< \brief 向下计数EVT使能*/
    __IO uint16_t PEN                       : 1;  /**< \brief 上溢点EVT使能*/
    __IO uint16_t UEN                       : 1;  /**< \brief 向上计数EVT使能*/
    __IO uint16_t ZEN                       : 1;  /**< \brief 下溢点EVT使能*/
} stc_tmr4_scsr_field_t;

/**
 * \brief TMR4 专用模式控制寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t AMC                       : 4;  /**< \brief 专用事件输出周期间隔值*/
    uint16_t RESERVED4                      : 2;  /**< \brief 保留*/
    __IO uint16_t MZCE                      : 1;  /**< \brief 周期间隔响应使能*/
    __IO uint16_t MPCE                      : 1;  /**< \brief 周期间隔响应使能*/
    uint16_t RESERVED8                      : 8;  /**< \brief 保留*/
} stc_tmr4_scmr_field_t;

/**
 * \brief TMR4 EMB控制状态寄存器
 */
typedef struct
{
    uint16_t RESERVED0                      : 7;  /**< \brief 保留*/
    __IO uint16_t HOLD                      : 1;  /**< \brief PWM保持*/
    uint16_t RESERVED8                      : 8;  /**< \brief 保留*/
} stc_tmr4_ecsr_field_t;

/**
 * \brief TMR4 EMB扩展控制寄存器
 */
typedef struct
{
    __IO uint16_t EMBVAL                    : 2;  /**< \brief EMB状态控制*/
    uint16_t RESERVED2                      :14;  /**< \brief 保留*/
} stc_tmr4_cr_ecer1_field_t;

/**
 * \brief TMR4 EMB扩展控制寄存器
 */
typedef struct
{
    __IO uint16_t EMBVAL                    : 2;  /**< \brief EMB状态控制*/
    uint16_t RESERVED2                      :14;  /**< \brief 保留*/
} stc_tmr4_cr_ecer2_field_t;

/**
 * \brief TMR4 EMB扩展控制寄存器
 */
typedef struct
{
    __IO uint16_t EMBVAL                    : 2;  /**< \brief EMB状态控制*/
    uint16_t RESERVED2                      :14;  /**< \brief 保留*/
} stc_tmr4_cr_ecer3_field_t;

typedef struct
{
    uint8_t RESERVED0[2];               /**< \brief 保留*/
    __IO uint16_t OCCRUH;               /**< \brief 通用比较基准寄存器UH*/
    uint8_t RESERVED1[2];               /**< \brief 保留*/
    __IO uint16_t OCCRUL;               /**< \brief 通用比较基准寄存器UL*/
    uint8_t RESERVED2[2];               /**< \brief 保留*/
    __IO uint16_t OCCRVH;               /**< \brief 通用比较基准寄存器VH*/
    uint8_t RESERVED3[2];               /**< \brief 保留*/
    __IO uint16_t OCCRVL;               /**< \brief 通用比较基准寄存器VL*/
    uint8_t RESERVED4[2];               /**< \brief 保留*/
    __IO uint16_t OCCRWH;               /**< \brief 通用比较基准寄存器WH*/
    uint8_t RESERVED5[2];               /**< \brief 保留*/
    __IO uint16_t OCCRWL;               /**< \brief 通用比较基准寄存器WL*/
    union
    {
        __IO uint16_t OCSRU;            /**< \brief 通用控制状态寄存器U*/
        stc_tmr4_ocsr_field_t OCSRU_f;
    };
    union
    {
        __IO uint16_t OCERU;            /**< \brief 通用扩展控制寄存器U*/
        stc_tmr4_ocer_field_t OCERU_f;
    };
    union
    {
        __IO uint16_t OCSRV;            /**< \brief 通用控制状态寄存器V*/
        stc_tmr4_ocsr_field_t OCSRV_f;
    };
    union
    {
        __IO uint16_t OCERV;            /**< \brief 通用扩展控制寄存器V*/
        stc_tmr4_ocer_field_t OCERV_f;
    };
    union
    {
        __IO uint16_t OCSRW;            /**< \brief 通用控制状态寄存器W*/
        stc_tmr4_ocsr_field_t OCSRW_f;
    };
    union
    {
        __IO uint16_t OCERW;            /**< \brief 通用扩展控制寄存器W*/
        stc_tmr4_ocer_field_t OCERW_f;
    };
    union
    {
        __IO uint16_t OCMRHUH;          /**< \brief 通用模式控制寄存器UH*/
        stc_tmr4_ocmrh_field_t OCMRHUH_f;
    };
    uint8_t RESERVED13[2];              /**< \brief 保留*/
    union
    {
        __IO uint32_t OCMRLUL;          /**< \brief 通用模式控制寄存器UL*/
        stc_tmr4_ocmrl_field_t OCMRLUL_f;
    };
    union
    {
        __IO uint16_t OCMRHVH;          /**< \brief 通用模式控制寄存器VH*/
        stc_tmr4_ocmrh_field_t OCMRHVH_f;
    };
    uint8_t RESERVED15[2];              /**< \brief 保留*/
    union
    {
        __IO uint32_t OCMRLVL;          /**< \brief 通用模式控制寄存器VL*/
        stc_tmr4_ocmrl_field_t OCMRLVL_f;
    };
    union
    {
        __IO uint16_t OCMRHWH;          /**< \brief 通用模式控制寄存器WH*/
        stc_tmr4_ocmrh_field_t OCMRHWH_f;
    };
    uint8_t RESERVED17[2];              /**< \brief 保留*/
    union
    {
        __IO uint32_t OCMRLWL;          /**< \brief 通用模式控制寄存器WL*/
        stc_tmr4_ocmrl_field_t OCMRLWL_f;
    };
    uint8_t RESERVED18[6];              /**< \brief 保留*/
    __IO uint16_t CPSR;                 /**< \brief 周期基准寄存器*/
    uint8_t RESERVED19[2];              /**< \brief 保留*/
    __IO uint16_t CNTR;                 /**< \brief 计数值寄存器*/
    union
    {
        __IO uint16_t CCSR;             /**< \brief 控制状态寄存器*/
        stc_tmr4_ccsr_field_t CCSR_f;
    };
    union
    {
        __IO uint16_t CVPR;             /**< \brief 有效周期寄存器*/
        stc_tmr4_cvpr_field_t CVPR_f;
    };
    uint8_t RESERVED22[54];             /**< \brief 保留*/
    __IO uint16_t PFSRU;                /**< \brief PWM滤波控制寄存器U*/
    __IO uint16_t PDARU;                /**< \brief PWM死区控制寄存器AU*/
    __IO uint16_t PDBRU;                /**< \brief PWM死区控制寄存器BU*/
    uint8_t RESERVED25[2];              /**< \brief 保留*/
    __IO uint16_t PFSRV;                /**< \brief PWM滤波控制寄存器V*/
    __IO uint16_t PDARV;                /**< \brief PWM死区控制寄存器AU*/
    __IO uint16_t PDBRV;                /**< \brief PWM死区控制寄存器BU*/
    uint8_t RESERVED28[2];              /**< \brief 保留*/
    __IO uint16_t PFSRW;                /**< \brief PWM滤波控制寄存器V*/
    __IO uint16_t PDARW;                /**< \brief PWM死区控制寄存器AW*/
    __IO uint16_t PDBRW;                /**< \brief PWM死区控制寄存器BW*/
    union
    {
        __IO uint16_t POCRU;            /**< \brief PWM基本控制寄存器U*/
        stc_tmr4_pocr_field_t POCRU_f;
    };
    uint8_t RESERVED32[2];              /**< \brief 保留*/
    union
    {
        __IO uint16_t POCRV;            /**< \brief PWM基本控制寄存器V*/
        stc_tmr4_pocr_field_t POCRV_f;
    };
    uint8_t RESERVED33[2];              /**< \brief 保留*/
    union
    {
        __IO uint16_t POCRW;            /**< \brief PWM基本控制寄存器W*/
        stc_tmr4_pocr_field_t POCRW_f;
    };
    uint8_t RESERVED34[2];              /**< \brief 保留*/
    union
    {
        __IO uint16_t RCSR;             /**< \brief 重载控制状态寄存器*/
        stc_tmr4_rcsr_field_t RCSR_f;
    };
    uint8_t RESERVED35[12];             /**< \brief 保留*/
    __IO uint16_t SCCRUH;               /**< \brief 专用比较基准寄存器UH*/
    uint8_t RESERVED36[2];              /**< \brief 保留*/
    __IO uint16_t SCCRUL;               /**< \brief 专用比较基准寄存器UL*/
    uint8_t RESERVED37[2];              /**< \brief 保留*/
    __IO uint16_t SCCRVH;               /**< \brief 专用比较基准寄存器VH*/
    uint8_t RESERVED38[2];              /**< \brief 保留*/
    __IO uint16_t SCCRVL;               /**< \brief 专用比较基准寄存器VL*/
    uint8_t RESERVED39[2];              /**< \brief 保留*/
    __IO uint16_t SCCRWH;               /**< \brief 专用比较基准寄存器WH*/
    uint8_t RESERVED40[2];              /**< \brief 保留*/
    __IO uint16_t SCCRWL;               /**< \brief 专用比较基准寄存器WL*/
    union
    {
        __IO uint16_t SCSRUH;           /**< \brief 专用控制状态寄存器UH*/
        stc_tmr4_scsr_field_t SCSRUH_f;
    };
    union
    {
        __IO uint16_t SCMRUH;           /**< \brief 专用模式控制寄存器UH*/
        stc_tmr4_scmr_field_t SCMRUH_f;
    };
    union
    {
        __IO uint16_t SCSRUL;           /**< \brief 专用控制状态寄存器UL*/
        stc_tmr4_scsr_field_t SCSRUL_f;
    };
    union
    {
        __IO uint16_t SCMRUL;           /**< \brief 专用模式控制寄存器UL*/
        stc_tmr4_scmr_field_t SCMRUL_f;
    };
    union
    {
        __IO uint16_t SCSRVH;           /**< \brief 专用控制状态寄存器VH*/
        stc_tmr4_scsr_field_t SCSRVH_f;
    };
    union
    {
        __IO uint16_t SCMRVH;           /**< \brief 专用模式控制寄存器VH*/
        stc_tmr4_scmr_field_t SCMRVH_f;
    };
    union
    {
        __IO uint16_t SCSRVL;           /**< \brief 专用控制状态寄存器VL*/
        stc_tmr4_scsr_field_t SCSRVL_f;
    };
    union
    {
        __IO uint16_t SCMRVL;           /**< \brief 专用模式控制寄存器VL*/
        stc_tmr4_scmr_field_t SCMRVL_f;
    };
    union
    {
        __IO uint16_t SCSRWH;           /**< \brief 专用控制状态寄存器WH*/
        stc_tmr4_scsr_field_t SCSRWH_f;
    };
    union
    {
        __IO uint16_t SCMRWH;           /**< \brief 专用模式控制寄存器WH*/
        stc_tmr4_scmr_field_t SCMRWH_f;
    };
    union
    {
        __IO uint16_t SCSRWL;           /**< \brief 专用控制状态寄存器WL*/
        stc_tmr4_scsr_field_t SCSRWL_f;
    };
    union
    {
        __IO uint16_t SCMRWL;           /**< \brief 专用模式控制寄存器WL*/
        stc_tmr4_scmr_field_t SCMRWL_f;
    };
    uint8_t RESERVED53[16];             /**< \brief 保留*/
    union
    {
        __IO uint16_t ECSR;             /**< \brief EMB控制状态寄存器*/
        stc_tmr4_ecsr_field_t ECSR_f;
    };
}amhw_hc32f460_tim4_t;
;

typedef struct
{
    union
    {
        __IO uint16_t ECER1;              /**< \brief EMB扩展控制寄存器1*/
        stc_tmr4_cr_ecer1_field_t ECER1_f;
    };
    uint8_t RESERVED1[2];                 /**< \brief 保留*/
    union
    {
        __IO uint16_t ECER2;              /**< \brief EMB扩展控制寄存器2*/
        stc_tmr4_cr_ecer2_field_t ECER2_f;
    };
    uint8_t RESERVED2[2];                 /**< \brief 保留*/
    union
    {
        __IO uint16_t ECER3;              /**< \brief EMB扩展控制寄存器3*/
        stc_tmr4_cr_ecer3_field_t ECER3_f;
    };
} amhw_hc32f460_tim4_cr_t;

typedef enum timer4_cnt_int_mask
{
    TIMER4_CNT_INT_MASK0  = 0u,                 
    TIMER4_CNT_INT_MASK1  = 1u,                
    TIMER4_CNT_INT_MASK2  = 2u,                
    TIMER4_CNT_INT_MASK3  = 3u,                
    TIMER4_CNT_INT_MASK4  = 4u,                
    TIMER4_CNT_INT_MASK5  = 5u,                
    TIMER4_CNT_INT_MASK6  = 6u,                
    TIMER4_CNT_INT_MASK7  = 7u,                
    TIMER4_CNT_INT_MASK8  = 8u,                
    TIMER4_CNT_INT_MASK9  = 9u,                
    TIMER4_CNT_INT_MASK10 = 10u,               
    TIMER4_CNT_INT_MASK11 = 11u,               
    TIMER4_CNT_INT_MASK12 = 12u,               
    TIMER4_CNT_INT_MASK13 = 13u,               
    TIMER4_CNT_INT_MASK14 = 14u,               
    TIMER4_CNT_INT_MASK15 = 15u,               
} timer4_cnt_int_mask_t;

typedef enum timer4_oco_ch
{
    TIMER4_OCO_OUH = 0U,                       
    TIMER4_OCO_OUL = 1U,                      
    TIMER4_OCO_OVH = 2U,                      
    TIMER4_OCO_OVL = 3U,                      
    TIMER4_OCO_OWH = 4U,                      
    TIMER4_OCO_OWL = 5U,                      
} timer4_oco_ch_t;

typedef enum en_timer4_oco_occr_buf
{
    OccrBufDisable                              = 0u,  
    OccrBufTrsfByCntZero                        = 1u, 
    OccrBufTrsfByCntPeak                        = 2u, 
    OccrBufTrsfByCntZeroOrCntPeak               = 3u, 
    OccrBufTrsfByCntZeroZicZero                 = 4u, 
    OccrBufTrsfByCntPeakPicZero                 = 5u, 
    OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero = 6u,                                                       
} en_timer4_oco_occr_buf_t;

typedef enum en_timer4_oco_ocmr_buf
{
    OcmrBufDisable                              = 0u,  
    OcmrBufTrsfByCntZero                        = 1u, 
    OcmrBufTrsfByCntPeak                        = 2u, 
    OcmrBufTrsfByCntZeroOrCntPeak               = 3u,  
    OcmrBufTrsfByCntZeroZicZero                 = 4u, 
    OcmrBufTrsfByCntPeakPicZero                 = 5u, 
    OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero = 6u, 
                                                      
} en_timer4_oco_ocmr_buf_t;

typedef enum en_timer4_oco_port_level
{
    OcPortLevelLow  = 0u,                   
    OcPortLevelHigh = 1u,                   
} en_timer4_oco_port_level_t;

typedef struct stc_timer4_oco_init
{
    en_timer4_oco_occr_buf_t   enOccrBufMode;  

    en_timer4_oco_ocmr_buf_t   enOcmrBufMode;  

    en_timer4_oco_port_level_t enPortLevel;    

    tim4_functional_state_t      enOcoIntCmd;    
} stc_timer4_oco_init_t;

#define TMR4_OCCRx(__TMR4x__, __CH__)       ((uint32_t)&(__TMR4x__)->OCCRUH + ((uint32_t)(__CH__))*4ul)
#define TMR4_OCMRx(__TMR4x__, __CH__)       ((uint32_t)&(__TMR4x__)->OCMRHUH + ((uint32_t)(__CH__))*4ul)
#define TMR4_OCERx(__TMR4x__, __CH__)       ((uint32_t)&(__TMR4x__)->OCERU + (((uint32_t)(__CH__))/2ul)*4ul)
#define TMR4_OCSRx(__TMR4x__, __CH__)       ((uint32_t)&(__TMR4x__)->OCSRU + (((uint32_t)(__CH__))/2ul)*4ul)

#define IS_VALID_OCO_HIGH_CH(x)                                                \
(   (TIMER4_OCO_OUH == (x))                   ||                              \
    (TIMER4_OCO_OVH == (x))                   ||                              \
    (TIMER4_OCO_OWH == (x)))

#define IS_VALID_OCO_LOW_CH(x)                                                 \
(   (TIMER4_OCO_OUL == (x))                   ||                                 \
    (TIMER4_OCO_OVL == (x))                   ||                                 \
    (TIMER4_OCO_OWL == (x)))

typedef enum en_timer4_oco_op_state
{
    OcoOpOutputHold    = 0u,               
    OcoOpOutputHigh    = 1u,               
    OcoOpOutputLow     = 2u,               
    OcoOpOutputReverse = 3u,               
} en_timer4_oco_op_state_t;

typedef enum en_timer4_oco_ocf_state
{
    OcoOcfHold = 0u,                       
    OcoOcfSet  = 1u,                       
} en_timer4_oco_ocf_state_t;

typedef struct stc_oco_low_ch_compare_mode
{
    en_timer4_oco_op_state_t enCntZeroLowMatchHighMatchLowChOpState;         
    en_timer4_oco_op_state_t enCntZeroLowMatchHighNotMatchLowChOpState;      
    en_timer4_oco_op_state_t enCntZeroLowNotMatchHighMatchLowChOpState;      
    en_timer4_oco_op_state_t enCntZeroLowNotMatchHighNotMatchLowChOpState;   

    en_timer4_oco_op_state_t enCntUpCntLowMatchHighMatchLowChOpState;        
    en_timer4_oco_op_state_t enCntUpCntLowMatchHighNotMatchLowChOpState;     
    en_timer4_oco_op_state_t enCntUpCntLowNotMatchHighMatchLowChOpState;     

    en_timer4_oco_op_state_t enCntPeakLowMatchHighMatchLowChOpState;         
    en_timer4_oco_op_state_t enCntPeakLowMatchHighNotMatchLowChOpState;    
    en_timer4_oco_op_state_t enCntPeakLowNotMatchHighMatchLowChOpState;      
    en_timer4_oco_op_state_t enCntPeakLowNotMatchHighNotMatchLowChOpState;   

    en_timer4_oco_op_state_t enCntDownLowMatchHighMatchLowChOpState;         
    en_timer4_oco_op_state_t enCntDownLowMatchHighNotMatchLowChOpState;      
    en_timer4_oco_op_state_t enCntDownLowNotMatchHighMatchLowChOpState;      

    en_timer4_oco_ocf_state_t enCntZeroMatchOcfState;       
    en_timer4_oco_ocf_state_t enCntUpCntMatchOcfState;      
    en_timer4_oco_ocf_state_t enCntPeakMatchOcfState;       
    en_timer4_oco_ocf_state_t enCntDownCntMatchOcfState;    

    tim4_functional_state_t     enMatchConditionExtendCmd;                                                                
} stc_oco_low_ch_compare_mode_t;

typedef struct stc_oco_high_ch_compare_mode
{
    en_timer4_oco_op_state_t  enCntZeroMatchOpState;        
    en_timer4_oco_op_state_t  enCntZeroNotMatchOpState;     

    en_timer4_oco_op_state_t  enCntUpCntMatchOpState;       

    en_timer4_oco_op_state_t  enCntPeakMatchOpState;        
    en_timer4_oco_op_state_t  enCntPeakNotMatchOpState;     

    en_timer4_oco_op_state_t  enCntDownCntMatchOpState;     

    en_timer4_oco_ocf_state_t enCntZeroMatchOcfState;       
    en_timer4_oco_ocf_state_t enCntUpCntMatchOcfState;      
    en_timer4_oco_ocf_state_t enCntPeakMatchOcfState;       
    en_timer4_oco_ocf_state_t enCntDownCntMatchOcfState;    

    tim4_functional_state_t     enMatchConditionExtendCmd;    
                                                            
} stc_oco_high_ch_compare_mode_t;

typedef enum en_timer4_pwm_mode
{
    PwmThroughMode         = 0u,         
    PwmDeadTimerMode       = 1u,        
    PwmDeadTimerFilterMode = 2u,        
} en_timer4_pwm_mode_t;

typedef enum en_timer4_pwm_timer_clk_div
{
    PwmPlckDiv1   = 0u,                
    PwmPlckDiv2   = 1u,                
    PwmPlckDiv4   = 2u,                
    PwmPlckDiv8   = 3u,                
    PwmPlckDiv16  = 4u,                
    PwmPlckDiv32  = 5u,                
    PwmPlckDiv64  = 6u,                
    PwmPlckDiv128 = 7u,                
} en_timer4_pwm_timer_clk_div_t;

typedef enum en_timer4_pwm_output_state
{
    PwmHPwmLHold        = 0u,           
    PwmHPwmLReverse     = 1u,           
    PwmHReversePwmLHold = 2u,           
    PwmHHoldPwmLReverse = 3u,           
} en_timer4_pwm_output_state_t;

typedef struct stc_timer4_pwm_init
{
    en_timer4_pwm_mode_t          enMode;           

    en_timer4_pwm_timer_clk_div_t enClkDiv;         

    en_timer4_pwm_output_state_t  enOutputState;    

    tim4_functional_state_t         enRtIntMaskCmd;   
} stc_timer4_pwm_init_t;

typedef enum en_timer4_pwm_ch
{
    TIMER4_PWM_U = 0u,                    /**< \brief Timer4 PWM couple channel: U */
    TIMER4_PWM_V = 1u,                    /**< \brief Timer4 PWM couple channel: V */
    TIMER4_PWM_W = 2u,                    /**< \brief Timer4 PWM couple channel: W */
} en_timer4_pwm_ch_t;
/**
 * \brief ARR-16位模式重载寄存器         得到当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return ARR重载寄存器的值
 */
am_static_inline
uint16_t amhw_hc32f460_tim4_arr_count_get (amhw_hc32f460_tim4_t *p_hw_tim,
                                           uint8_t               chan)
{
    return p_hw_tim->CPSR;
}


/**
 * \brief ARR-16位模式重载寄存器       设置当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] value    : 设置的计数器的值
 *
 *\ return 无
 */
am_static_inline
void amhw_hc32f460_tim4_arr_count_set (amhw_hc32f460_tim4_t *p_hw_tim, uint8_t chan, uint16_t value)
{
    p_hw_tim->CPSR = value;
}

/**
 * \brief CNT-16位模式计数寄存器         得到当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return CNT计数寄存器的值
 */
am_static_inline
uint16_t amhw_hc32f460_tim4_cnt16_count_get (amhw_hc32f460_tim4_t *p_hw_tim,
                                             uint8_t               chan)
{
    return (uint16_t)p_hw_tim->CNTR;
}

am_static_inline
void amhw_hc32f460_tim4_cnt16_count_set (amhw_hc32f460_tim4_t *p_hw_tim,
                                         uint8_t               chan,
                                         uint16_t              value)
{
    p_hw_tim->CNTR = value;
}

am_static_inline
am_bool_t amhw_hc32f460_tim4_oco_write_occr(amhw_hc32f460_tim4_t *p_hw_tim,
                                            timer4_oco_ch_t       enCh,
                                            uint16_t              u16Occr)
{
    __IO uint16_t *pu16OCCR = NULL;
    am_bool_t enRet = AM_FALSE;

    /* Check TMR4x pointer */
    if (NULL != p_hw_tim)
    {
        /* Get pointer of current channel OCO register address */
        pu16OCCR = (__IO uint16_t*)TMR4_OCCRx(p_hw_tim, enCh);
        /* set register */
        *pu16OCCR = u16Occr;

        enRet = AM_TRUE;
    }

    return enRet;
}

am_static_inline
am_bool_t amhw_hc32f460_tim4_oco_output_cmp_cmd(amhw_hc32f460_tim4_t   *p_hw_tim,
                                                timer4_oco_ch_t         enCh,
                                                tim4_functional_state_t enCmd)
{
    __IO stc_tmr4_ocsr_field_t *pstcOCSR = NULL;
    am_bool_t enRet = AM_FALSE;

    /* Check TMR4x pointer */
    if (NULL != p_hw_tim)
    {
        /* Get pointer of current channel OCO register address */
        pstcOCSR = (__IO stc_tmr4_ocsr_field_t*)TMR4_OCSRx(p_hw_tim, enCh);

        /* set register */
        IS_VALID_OCO_HIGH_CH(enCh) ? (pstcOCSR->OCEH = (uint16_t)enCmd) : (pstcOCSR->OCEL = (uint16_t)enCmd);
        enRet = AM_TRUE;
    }

    return enRet;
}

/**
 * \brief 定时器模式枚举
 */

/**
 * \brief MODE 工作模式  获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 定时器不同模式的选择控制位固定，再根据联合体结构的特性，只对某一寄存器操作即可。
 *
 * \return 设置的工作模式（amhw_hc32_tim_mode_t枚举量）
 */
am_static_inline
tim4_counter_mode_t amhw_hc32f460_tim4_mode_get (amhw_hc32f460_tim4_t *p_hw_tim)
{
    return (tim4_counter_mode_t)(p_hw_tim->CCSR_f.MODE);
}

/**
 * \brief CMODE 工作模式  设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] mode     : 设置的工作模式（amhw_hc32_tim_mode_t枚举量）
 *
 * \note 定时器不同模式的选择控制位固定，再根据联合体结构的特性，只对某一寄存器操作即可。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_tim4_mode_set (amhw_hc32f460_tim4_t     *p_hw_tim,
                                  tim4_counter_mode_t       mode)
{
    p_hw_tim->CCSR_f.MODE = mode;
}

am_static_inline
void amhw_hc32f460_tim4_buf_set (amhw_hc32f460_tim4_t     *p_hw_tim,
                                 am_bool_t                 value)
{
    p_hw_tim->CCSR_f.BUFEN = value;
}

am_static_inline
void amhw_hc32f460_tim4_ext_clk_set (amhw_hc32f460_tim4_t     *p_hw_tim,
                                     am_bool_t                 value)
{
    p_hw_tim->CCSR_f.ECKEN = value;
}

/**
 * \brief CTEN 定时器使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 定时器不同模式的选择控制位固定，再根据联合体结构的特性，只对某一寄存器操作即可。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_tim4_disable (amhw_hc32f460_tim4_t *p_hw_tim, uint8_t chan)
{
    p_hw_tim->CCSR_f.STOP = 1;
}

/**
 * \brief CTEN 定时器使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 定时器不同模式的选择控制位固定，再根据联合体结构的特性，只对某一寄存器操作即可。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_tim4_enable (amhw_hc32f460_tim4_t *p_hw_tim, uint8_t chan)
{
    p_hw_tim->CCSR_f.STOP = 0;
}

/**
 * \brief 内部时钟分频选择
 */
typedef enum {
    AMHW_HC32F460_TIM4_CLK_DIV0 = 0,    /**< \brief 1分频 */
    AMHW_HC32F460_TIM4_CLK_DIV2,        /**< \brief 2分频 */
    AMHW_HC32F460_TIM4_CLK_DIV4,        /**< \brief 4分频 */
    AMHW_HC32F460_TIM4_CLK_DIV8,        /**< \brief 8分频 */
    AMHW_HC32F460_TIM4_CLK_DIV16,       /**< \brief 16分频 */
    AMHW_HC32F460_TIM4_CLK_DIV32,       /**< \brief 32分频 */
    AMHW_HC32F460_TIM4_CLK_DIV64,       /**< \brief 64分频 */
    AMHW_HC32F460_TIM4_CLK_DIV128,      /**< \brief 128分频 */
    AMHW_HC32F460_TIM4_CLK_DIV256,      /**< \brief 256分频 */
    AMHW_HC32F460_TIM4_CLK_DIV512,      /**< \brief 512分频 */
    AMHW_HC32F460_TIM4_CLK_DIV1024,     /**< \brief 1024分频 */
}amhw_hc32f460_tim4_clkdiv_t;

/**
 * \brief PRS 内部时钟分频选择   获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 设置的分频系数（amhw_hc32_tim_clkdiv_t枚举量）
 */
am_static_inline
amhw_hc32f460_tim4_clkdiv_t
    amhw_hc32f460_tim4_mode_clkdiv_get (amhw_hc32f460_tim4_t *p_hw_tim, uint8_t chan)
{
    amhw_hc32f460_tim4_clkdiv_t clkdiv;
    clkdiv = (amhw_hc32f460_tim4_clkdiv_t)p_hw_tim->CCSR_f.CKDIV;
    return clkdiv;
}

#define TMR4_RCSRx(__TMR4x__)               ((uint32_t)&(__TMR4x__)->RCSR)
#define TMR4_POCRx(__TMR4x__, __CH__)       ((uint32_t)&(__TMR4x__)->POCRU + ((uint32_t)(__CH__))*4ul)
#define TMR4_PDARx(__TMR4x__, __CH__)       ((uint32_t)&(__TMR4x__)->PDARU + ((uint32_t)(__CH__))*8ul)
#define TMR4_PDBRx(__TMR4x__, __CH__)       ((uint32_t)&(__TMR4x__)->PDBRU + ((uint32_t)(__CH__))*8ul)
#define TMR4_PFSRx(__TMR4x__, __CH__)       ((uint32_t)&(__TMR4x__)->PFSRU + ((uint32_t)(__CH__))*8ul)

am_static_inline
am_bool_t amhw_hc32f460_tim4_pwm_init(amhw_hc32f460_tim4_t        *p_hw_tim,
                                      en_timer4_pwm_ch_t           enCh,
                                      const stc_timer4_pwm_init_t *pstcInitCfg)
{
    __IO stc_tmr4_pocr_field_t *pstcPOCR_f = NULL;
    __IO stc_tmr4_rcsr_field_t *pstcRCSR_f = NULL;
    am_bool_t enRet = AM_TRUE;

    /* Check TMR4x && pstcInitCfg pointer */
    if ((NULL != p_hw_tim) && (NULL != pstcInitCfg))
    {
        /* Get pointer of current channel PWM register address */
        pstcRCSR_f = (__IO stc_tmr4_rcsr_field_t*)TMR4_RCSRx(p_hw_tim);
        pstcPOCR_f = (__IO stc_tmr4_pocr_field_t*)TMR4_POCRx(p_hw_tim, enCh);

        /* Configure PWM mode */
        pstcPOCR_f->PWMMD = (uint16_t)(pstcInitCfg->enMode);

        /* Configure PWM mode */
        pstcPOCR_f->LVLS = (uint16_t)(pstcInitCfg->enOutputState);

        /* Set timer clock division */
        pstcPOCR_f->DIVCK = (uint16_t)(pstcInitCfg->enClkDiv);

        /* Set interrupt mask */
        switch (enCh)
        {
            case TIMER4_PWM_U:
                pstcRCSR_f->RTIDU = (uint16_t)(pstcInitCfg->enRtIntMaskCmd);
                break;
            case TIMER4_PWM_V:
                pstcRCSR_f->RTIDV = (uint16_t)(pstcInitCfg->enRtIntMaskCmd);
                break;
            case TIMER4_PWM_W:
                pstcRCSR_f->RTIDW = (uint16_t)(pstcInitCfg->enRtIntMaskCmd);
                break;
            default:
                enRet = AM_FALSE;
                break;
        }
    }
    else
    {
        enRet = AM_FALSE;
    }

    return enRet;
}


/**
 * \brief PRS 内部时钟分频选择   设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] clkdiv   : 设置的分频系数（amhw_hc32_tim_clkdiv_t枚举量）
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_tim4_mode_clkdiv_set (amhw_hc32f460_tim4_t       *p_hw_tim,
                                         uint8_t                     chan,
                                         amhw_hc32f460_tim4_clkdiv_t clkdiv)
{
    p_hw_tim->CCSR_f.CKDIV = clkdiv;
}

am_static_inline
void amhw_hc32f460_tim4_intz_set (amhw_hc32f460_tim4_t *p_hw_tim,
                                  am_bool_t             value)
{
    p_hw_tim->CCSR_f.IRQZEN = value;
}

am_static_inline
void amhw_hc32f460_tim4_intp_set (amhw_hc32f460_tim4_t *p_hw_tim,
                                  am_bool_t             value)
{
    p_hw_tim->CCSR_f.IRQPEN = value;
}

am_static_inline
void amhw_hc32f460_tim4_zim_set (amhw_hc32f460_tim4_t *p_hw_tim,
                                 timer4_cnt_int_mask_t mask)
{
    p_hw_tim->CVPR_f.ZIM = mask;
}

am_static_inline
void amhw_hc32f460_tim4_pim_set (amhw_hc32f460_tim4_t *p_hw_tim,
                                 timer4_cnt_int_mask_t mask)
{
    p_hw_tim->CVPR_f.PIM = mask;
}

am_static_inline
am_bool_t amhw_hc32f460_tim4_oco_init(amhw_hc32f460_tim4_t        *p_hw_tim,
                                      timer4_oco_ch_t              enCh,
                                      const stc_timer4_oco_init_t *pstcInitCfg)
{
    __IO stc_tmr4_ocsr_field_t* pstcOCSR = NULL;
    __IO stc_tmr4_ocer_field_t* pstcOCER = NULL;
    am_bool_t enRet = AM_FALSE;

    /* Check TMR4x && pstcInitCfg pointer */
    if ((NULL != p_hw_tim) && (NULL != pstcInitCfg))
    {
        enRet = AM_TRUE;
        /* Get pointer of current channel OCO register address */
        pstcOCER = (__IO stc_tmr4_ocer_field_t*)TMR4_OCERx(p_hw_tim, enCh);
        pstcOCSR = (__IO stc_tmr4_ocsr_field_t*)TMR4_OCSRx(p_hw_tim, enCh);

        /* Set OCMR and OCCR buffer mode */
        if (IS_VALID_OCO_HIGH_CH(enCh)) /* channel: Timer4OcoOuh, Timer4OcoOvh, Timer4OcoOwh */
        {
            pstcOCSR->OCEH = (uint16_t)0u;
            pstcOCSR->OCFH = (uint16_t)0u;

            /* OCMR buffer */
            switch (pstcInitCfg->enOcmrBufMode)
            {
                case OcmrBufDisable:
                    pstcOCER->LMMH = (uint16_t)0u;
                    pstcOCER->MHBUFEN = (uint16_t)0u;
                    break;
                case OcmrBufTrsfByCntZero:
                    pstcOCER->LMMH = (uint16_t)0u;
                    pstcOCER->MHBUFEN = (uint16_t)1u;
                    break;
                case OcmrBufTrsfByCntPeak:
                    pstcOCER->LMMH = (uint16_t)0u;
                    pstcOCER->MHBUFEN = (uint16_t)2u;
                    break;
                case OcmrBufTrsfByCntZeroOrCntPeak:
                    pstcOCER->LMMH = (uint16_t)0u;
                    pstcOCER->MHBUFEN = (uint16_t)3u;
                    break;
                case OcmrBufTrsfByCntZeroZicZero:
                    pstcOCER->LMMH = (uint16_t)1u;
                    pstcOCER->MHBUFEN = (uint16_t)1u;
                    break;
                case OcmrBufTrsfByCntPeakPicZero:
                    pstcOCER->LMMH = (uint16_t)1u;
                    pstcOCER->MHBUFEN = (uint16_t)2u;
                    break;
                case OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                    pstcOCER->LMMH = (uint16_t)1u;
                    pstcOCER->MHBUFEN = (uint16_t)3u;
                    break;
                default:
                    enRet = AM_FALSE;
                    break;
            }

            if (enRet == AM_TRUE)
            {
                /* OCCR buffer */
                switch (pstcInitCfg->enOccrBufMode)
                {
                    case OccrBufDisable:
                        pstcOCER->LMCH = (uint16_t)0u;
                        pstcOCER->CHBUFEN = (uint16_t)0u;
                        break;
                    case OccrBufTrsfByCntZero:
                        pstcOCER->LMCH = (uint16_t)0u;
                        pstcOCER->CHBUFEN = (uint16_t)1u;
                        break;
                    case OccrBufTrsfByCntPeak:
                        pstcOCER->LMCH = (uint16_t)0u;
                        pstcOCER->CHBUFEN = (uint16_t)2u;
                        break;
                    case OccrBufTrsfByCntZeroOrCntPeak:
                        pstcOCER->LMCH = (uint16_t)0u;
                        pstcOCER->CHBUFEN = (uint16_t)3u;
                        break;
                    case OccrBufTrsfByCntZeroZicZero:
                        pstcOCER->LMCH = (uint16_t)1u;
                        pstcOCER->CHBUFEN = (uint16_t)1u;
                        break;
                    case OccrBufTrsfByCntPeakPicZero:
                        pstcOCER->LMCH = (uint16_t)1u;
                        pstcOCER->CHBUFEN = (uint16_t)2u;
                        break;
                    case OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                        pstcOCER->LMCH = (uint16_t)1u;
                        pstcOCER->CHBUFEN = (uint16_t)3u;
                        break;
                    default:
                        enRet = AM_FALSE;
                        break;
                }
            }

            if (enRet == AM_TRUE)
            {
                /* Set initial OP level */
                pstcOCSR->OCPH = (uint16_t)(pstcInitCfg->enPortLevel);
                /* set interrupt enable */
                pstcOCSR->OCIEH = (uint16_t)(pstcInitCfg->enOcoIntCmd);
            }
        }/* channel: Timer4OcoOul, Timer4OcoOvl, Timer4OcoOwl */
        else if (IS_VALID_OCO_LOW_CH(enCh))
        {
            pstcOCSR->OCEL = (uint16_t)0u;
            pstcOCSR->OCFL = (uint16_t)0u;

            /* OCMR buffer */
            switch (pstcInitCfg->enOcmrBufMode)
            {
                case OcmrBufDisable:
                    pstcOCER->LMML = (uint16_t)0u;
                    pstcOCER->MLBUFEN = (uint16_t)0u;
                    break;
                case OcmrBufTrsfByCntZero:
                    pstcOCER->LMML = (uint16_t)0u;
                    pstcOCER->MLBUFEN = (uint16_t)1u;
                    break;
                case OcmrBufTrsfByCntPeak:
                    pstcOCER->LMML = (uint16_t)0u;
                    pstcOCER->MLBUFEN = (uint16_t)2u;
                    break;
                case OcmrBufTrsfByCntZeroOrCntPeak:
                    pstcOCER->LMML = (uint16_t)0u;
                    pstcOCER->MLBUFEN = (uint16_t)3u;
                    break;
                case OcmrBufTrsfByCntZeroZicZero:
                    pstcOCER->LMML = (uint16_t)1u;
                    pstcOCER->MLBUFEN = (uint16_t)1u;
                    break;
                case OcmrBufTrsfByCntPeakPicZero:
                    pstcOCER->LMML = (uint16_t)1u;
                    pstcOCER->MLBUFEN = (uint16_t)2u;
                    break;
                case OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                    pstcOCER->LMML = (uint16_t)1u;
                    pstcOCER->MLBUFEN = (uint16_t)3u;
                    break;
                default:
                    enRet = AM_FALSE;
                    break;
            }

            if (enRet == AM_TRUE)
            {
                /* OCCR buffer */
                switch (pstcInitCfg->enOccrBufMode)
                {
                    case OccrBufDisable:
                        pstcOCER->LMCL = (uint16_t)0u;
                        pstcOCER->CLBUFEN = (uint16_t)0u;
                        break;
                    case OccrBufTrsfByCntZero:
                        pstcOCER->LMCL = (uint16_t)0u;
                        pstcOCER->CLBUFEN = (uint16_t)1u;
                        break;
                    case OccrBufTrsfByCntPeak:
                        pstcOCER->LMCL = (uint16_t)0u;
                        pstcOCER->CLBUFEN = (uint16_t)2u;
                        break;
                    case OccrBufTrsfByCntZeroOrCntPeak:
                        pstcOCER->LMCL = (uint16_t)0u;
                        pstcOCER->CLBUFEN = (uint16_t)3u;
                        break;
                    case OccrBufTrsfByCntZeroZicZero:
                        pstcOCER->LMCL = (uint16_t)1u;
                        pstcOCER->CLBUFEN = (uint16_t)1u;
                        break;
                    case OccrBufTrsfByCntPeakPicZero:
                        pstcOCER->LMCL = (uint16_t)1u;
                        pstcOCER->CLBUFEN = (uint16_t)2u;
                        break;
                    case OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                        pstcOCER->LMCL = (uint16_t)1u;
                        pstcOCER->CLBUFEN = (uint16_t)3u;
                        break;
                    default:
                        enRet = AM_FALSE;
                        break;
                }
            }

            if (enRet == AM_TRUE)
            {
                /* Set initial OP level */
                pstcOCSR->OCPL = (uint16_t)(pstcInitCfg->enPortLevel);
                /* set interrupt enable */
                pstcOCSR->OCIEL = (uint16_t)(pstcInitCfg->enOcoIntCmd);
            }
        }
        else
        {
            enRet = AM_FALSE;
        }
    }

    return enRet;
}


/**
 * \brief UIF 溢出中断 标志获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return AM_TRUE  ： 标志有效
 *         AM_FALSE ： 标志无效
 */
am_static_inline
am_bool_t amhw_hc32f460_tim4_int_flag_check(amhw_hc32f460_tim4_t *p_hw_tim,
                                            timer4_cnt_int_t      flag)
{
    uint8_t value = 0;
    if(TIMER4_CNT_ZERO_MATCH_INT == flag)
    {
        value = p_hw_tim->CCSR_f.IRQZF;        
    } else if (TIMER4_CNT_PEAK_MATCH_INT == flag)
    {
        value = p_hw_tim->CCSR_f.IRQPF;
    }else {
        ;
    }
    return value ? AM_TRUE : AM_FALSE;
}

am_static_inline
void amhw_hc32f460_tim4_int_flag_clr(amhw_hc32f460_tim4_t *p_hw_tim,
                                     timer4_cnt_int_t      flag)
{
    if(TIMER4_CNT_ZERO_MATCH_INT == flag)
    {
        p_hw_tim->CCSR_f.IRQZF = 0;        
    } else if (TIMER4_CNT_PEAK_MATCH_INT == flag)
    {
        p_hw_tim->CCSR_f.IRQPF = 0;
    }else {
        ;
    }
}

/**
 * \brief UIF 溢出中断 标志清除
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_tim4_cnt_value_clr(amhw_hc32f460_tim4_t *p_hw_tim)
{
    p_hw_tim->CCSR_f.CLEAR = (uint16_t)1u;
}

am_static_inline
am_bool_t amhw_hc32f460_tim4_oco_set_h_ch_cmp_mode(amhw_hc32f460_tim4_t                 *p_hw_tim,
                                                   timer4_oco_ch_t                       enCh,
                                                   const stc_oco_high_ch_compare_mode_t *pstcMode)
{
    uint16_t u16OCMR = 0u;
    __IO uint16_t *pu16OCMR = NULL;
    __IO stc_tmr4_ocer_field_t *pstcOCER = NULL;
    am_bool_t enRet = AM_FALSE;

    /* Check TMR4x && pstcMode pointer */
    if ((NULL != p_hw_tim) && (NULL != pstcMode))
    {
        /* Get pointer of current channel OCO register address */
        pu16OCMR = (__IO uint16_t*)TMR4_OCMRx(p_hw_tim, enCh);
        pstcOCER = (__IO stc_tmr4_ocer_field_t*)TMR4_OCERx(p_hw_tim, enCh);

        pstcOCER->MCECH = (uint16_t)(pstcMode->enMatchConditionExtendCmd);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntZeroMatchOpState    << 10u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntZeroNotMatchOpState << 14u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntUpCntMatchOpState   << 8u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntPeakMatchOpState    << 6u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntPeakNotMatchOpState << 12u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntDownCntMatchOpState << 4u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntZeroMatchOcfState    << 3u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntUpCntMatchOcfState   << 2u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntPeakMatchOcfState    << 1u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntDownCntMatchOcfState << 0u);

        *pu16OCMR = u16OCMR;
        enRet = AM_TRUE;
    }

    return enRet;
}

am_static_inline
am_bool_t  amhw_hc32f460_tim4_oco_set_l_ch_cmp_mode(amhw_hc32f460_tim4_t                *p_hw_tim,
                                                    timer4_oco_ch_t                      enCh,
                                                    const stc_oco_low_ch_compare_mode_t *pstcMode)
{
    uint32_t u32OCMR = 0ul;
    __IO uint32_t *pu32OCMR = NULL;
    __IO stc_tmr4_ocer_field_t *pstcOCER = NULL;
    am_bool_t enRet = AM_FALSE;

    /* Check TMR4x pointer and pstcMode pointer */
    if ((NULL != p_hw_tim) && (NULL != pstcMode))
    {
        /* Get pointer of current channel OCO register address */
        pu32OCMR = (__IO uint32_t*)TMR4_OCMRx(p_hw_tim, enCh);
        pstcOCER = (__IO stc_tmr4_ocer_field_t*)TMR4_OCERx(p_hw_tim, enCh);;

        pstcOCER->MCECL = (uint16_t)(pstcMode->enMatchConditionExtendCmd);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroLowMatchHighMatchLowChOpState        << 26u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroLowMatchHighNotMatchLowChOpState     << 10u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroLowNotMatchHighMatchLowChOpState     << 30u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroLowNotMatchHighNotMatchLowChOpState  << 14u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntUpCntLowMatchHighMatchLowChOpState       << 24u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntUpCntLowMatchHighNotMatchLowChOpState    << 8u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntUpCntLowNotMatchHighMatchLowChOpState    << 18u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakLowMatchHighMatchLowChOpState        << 22u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakLowMatchHighNotMatchLowChOpState     << 6u) ;
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakLowNotMatchHighMatchLowChOpState     << 28u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakLowNotMatchHighNotMatchLowChOpState  << 12u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntDownLowMatchHighMatchLowChOpState        << 20u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntDownLowMatchHighNotMatchLowChOpState     << 4u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntDownLowNotMatchHighMatchLowChOpState     << 16u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroMatchOcfState     << 3u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntUpCntMatchOcfState    << 2u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakMatchOcfState     << 1u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntDownCntMatchOcfState  << 0u);

        *pu32OCMR = u32OCMR;
        enRet = AM_TRUE;
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

#endif /* __AMHW_HC32F460_TIM4_H */

/* end of file */
