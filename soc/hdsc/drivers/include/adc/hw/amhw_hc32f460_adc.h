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
 * \brief ADC Peripheral Access Layer
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-16  cds, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_HC32F460_ADC_H
#define __AMHW_HC32F460_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"
#include "hc32f460_regbase.h"

/**
 * \addtogroup amhw_hc32f460_if_adc
 * \copydoc amhw_hc32f460_adc.h
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
 * \brief ADC - 启动寄存器位域结构体
 */
typedef struct
{
    __IO uint8_t STRT                       : 1;   /**< \brief 转换开始*/
    uint8_t RESERVED1                       : 7;   /**< \brief 保留*/
} stc_adc_str_field_t;

/**
 * \brief ADC - 控制寄存器0位域结构体
 */
typedef struct
{
    __IO uint16_t MS                        : 2;  /**< \brief 模式选择*/
    uint16_t RESERVED2                      : 2;  /**< \brief 保留*/
    __IO uint16_t ACCSEL                    : 2;  /**< \brief 分辨率选择*/
    __IO uint16_t CLREN                     : 1;  /**< \brief 数据寄存器自动清除*/
    __IO uint16_t DFMT                      : 1;  /**< \brief 数据格式*/
    __IO uint16_t AVCNT                     : 3;  /**< \brief 次数选择*/
    uint16_t RESERVED11                     : 5;  /**< \brief 保留*/
} stc_adc_cr0_field_t;

/**
 * \brief ADC - 控制寄存器1位域结构体
 */
typedef struct
{
    uint16_t RESERVED0                      : 2;  /**< \brief 保留*/
    __IO uint16_t RSCHSEL                   : 1;  /**< \brief 序列A重启通道选择*/
    uint16_t RESERVED3                      :13;  /**< \brief 保留*/
} stc_adc_cr1_field_t;

/**
 * \brief ADC - 转换开始触发寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t TRGSELA                   : 3;  /**< \brief 序列A触发条件选择*/
    uint16_t RESERVED3                      : 4;  /**< \brief 保留*/
    __IO uint16_t TRGENA                    : 1;  /**< \brief 序列A触发使能*/
    __IO uint16_t TRGSELB                   : 3;  /**< \brief 序列B触发条件选择*/
    uint16_t RESERVED11                     : 4;  /**< \brief 保留*/
    __IO uint16_t TRGENB                    : 1;  /**< \brief 序列B触发使能*/
} stc_adc_trgsr_field_t;

/**
 * \brief ADC - 转换通道选择寄存器A位域结构体
 */
typedef struct
{
    __IO uint16_t CHSELA16                  : 1;  /**< \brief 转换通道选择*/
    uint16_t RESERVED1                      :15;  /**< \brief 保留*/
} stc_adc_chselra1_field_t;

/**
 * \brief ADC - 转换通道选择寄存器B位域结构体
 */
typedef struct
{
    __IO uint16_t CHSELB16                  : 1;  /**< \brief 转换通道选择*/
    uint16_t RESERVED1                      :15;  /**< \brief 保留*/
} stc_adc_chselrb1_field_t;

/**
 * \brief ADC - 平均通道选择寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t AVCHSEL16                 : 1;  /**< \brief 平均通道选择*/
    uint16_t RESERVED1                      :15;  /**< \brief 保留*/
} stc_adc_avchselr1_field_t;

/**
 * \brief ADC - AD通道映射控制寄存器0位域结构体
 */
typedef struct
{
    __IO uint16_t CH00MUX                   : 4;  /**< \brief 通道0映射选择*/
    __IO uint16_t CH01MUX                   : 4;  /**< \brief 通道1映射选择*/
    __IO uint16_t CH02MUX                   : 4;  /**< \brief 通道2映射选择*/
    __IO uint16_t CH03MUX                   : 4;  /**< \brief 通道3映射选择*/
} stc_adc_chmuxr0_field_t;

/**
 * \brief ADC - AD通道映射控制寄存器1位域结构体
 */
typedef struct
{
    __IO uint16_t CH04MUX                   : 4;  /**< \brief 通道4映射选择*/
    __IO uint16_t CH05MUX                   : 4;  /**< \brief 通道5映射选择*/
    __IO uint16_t CH06MUX                   : 4;  /**< \brief 通道6映射选择*/
    __IO uint16_t CH07MUX                   : 4;  /**< \brief 通道7映射选择*/
} stc_adc_chmuxr1_field_t;

/**
 * \brief ADC - AD通道映射控制寄存器2位域结构体
 */
typedef struct
{
    __IO uint16_t CH08MUX                   : 4;  /**< \brief 通道8映射选择*/
    __IO uint16_t CH09MUX                   : 4;  /**< \brief 通道9映射选择*/
    __IO uint16_t CH10MUX                   : 4;  /**< \brief 通道10映射选择*/
    __IO uint16_t CH11MUX                   : 4;  /**< \brief 通道11映射选择*/
} stc_adc_chmuxr2_field_t;

/**
 * \brief ADC - AD通道映射控制寄存器3位域结构体
 */
typedef struct
{
    __IO uint16_t CH12MUX                   : 4;  /**< \brief 通道12映射选择*/
    __IO uint16_t CH13MUX                   : 4;  /**< \brief 通道13映射选择*/
    __IO uint16_t CH14MUX                   : 4;  /**< \brief 通道14映射选择*/
    __IO uint16_t CH15MUX                   : 4;  /**< \brief 通道15映射选择*/
} stc_adc_chmuxr3_field_t;

/**
 * \brief ADC - AD中断状态寄存器位域结构体
 */
typedef struct
{
    __IO uint8_t EOCAF                      : 1;  /**< \brief 序列A转换完成标志*/
    __IO uint8_t EOCBF                      : 1;  /**< \brief 序列B转换完成标志*/
    uint8_t RESERVED2                       : 6;  /**< \brief 保留*/
} stc_adc_isr_field_t;

/**
 * \brief ADC - AD中断使能寄存器位域结构体
 */
typedef struct
{
    __IO uint8_t EOCAIEN                    : 1;  /**< \brief 序列A转换完成中断使能*/
    __IO uint8_t EOCBIEN                    : 1;  /**< \brief 序列A转换完成中断使能*/
    uint8_t RESERVED2                       : 6;  /**< \brief 保留*/
} stc_adc_icr_field_t;

/**
 * \brief ADC - 协同模式控制寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t SYNCEN                    : 1;  /**< \brief 同步模式许可*/
    uint16_t RESERVED1                      : 3;  /**< \brief 保留*/
    __IO uint16_t SYNCMD                    : 3;  /**< \brief 同步模式选择*/
    uint16_t RESERVED7                      : 1;  /**< \brief 保留*/
    __IO uint16_t SYNCDLY                   : 8;  /**< \brief 同步延迟时间*/
} stc_adc_synccr_field_t;

/**
 * \brief ADC - 模拟看门狗控制寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t AWDEN                     : 1;  /**< \brief 看门狗比较功能使能*/
    uint16_t RESERVED1                      : 3;  /**< \brief 保留*/
    __IO uint16_t AWDMD                     : 1;  /**< \brief 看门狗比较模式*/
    uint16_t RESERVED5                      : 1;  /**< \brief 保留*/
    __IO uint16_t AWDSS                     : 2;  /**< \brief 看门狗序列选择*/
    __IO uint16_t AWDIEN                    : 1;  /**< \brief 看门狗中断使能*/
    uint16_t RESERVED9                      : 7;  /**< \brief 保留*/
} stc_adc_awdcr_field_t;

/**
 * \brief ADC - 模拟看门狗比较通道选择寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t AWDCH16                   : 1;  /**< \brief 看门狗比较功能通道选择*/
    uint16_t RESERVED1                      :15;  /**< \brief 保留*/
} stc_adc_awdchsr1_field_t;

/**
 * \brief ADC - 模拟看门狗状态寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t AWDF16                    : 1;  /**< \brief 看门狗比较状态位*/
    uint16_t RESERVED1                      :15;  /**< \brief 保留*/
} stc_adc_awdsr1_field_t;

/**
 * \brief ADC - 可编程增益放大器控制寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t PGACTL                    : 4;  /**< \brief 放大器控制*/
    uint16_t RESERVED4                      :12;  /**< \brief 保留*/
} stc_adc_pgacr_field_t;

/**
 * \brief ADC - 可编程增益倍数寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t GAIN                      : 4;  /**< \brief 放大器增益设定*/
    uint16_t RESERVED4                      :12;  /**< \brief 保留*/
} stc_adc_pgagsr_field_t;

/**
 * \brief ADC - 可编程增益放大器输入选择寄存器0位域结构体
 */
typedef struct
{
    __IO uint16_t PGAINSEL                  : 9;  /**< \brief 放大器模拟输入选择*/
    uint16_t RESERVED9                      : 7;  /**< \brief 保留*/
} stc_adc_pgainsr0_field_t;

/**
 * \brief ADC - 可编程增益放大器输入选择寄存器1位域结构体
 */
typedef struct
{
    __IO uint16_t PGAVSSEN                  : 1;  /**< \brief 放大器地切除控制选择*/
    uint16_t RESERVED1                      :15;  /**< \brief 保留*/
} stc_adc_pgainsr1_field_t;


/**
 * \brief ADC - Register Layout Typedef
 */
typedef struct amhw_hc32f460_adc{
    union
    {
        __IO uint8_t STR;             /**< \brief AD启动寄存器 */
        stc_adc_str_field_t STR_f;    /**< \brief AD启动寄存器位域结构体 */
    };
    uint8_t RESERVED1[1];             /**< \brief 保留*/
    union
    {
        __IO uint16_t CR0;            /**< \brief AD 控制寄存器0*/
        stc_adc_cr0_field_t CR0_f;    /**< \brief AD 控制寄存器0位域结构体*/
    };
    union
    {
        __IO uint16_t CR1;            /**< \brief AD 控制寄存器1*/
        stc_adc_cr1_field_t CR1_f;    /**< \brief AD 控制寄存器1位域结构体*/
    };
    uint8_t RESERVED3[4];
    union
    {
        __IO uint16_t TRGSR;          /**< \brief AD 转换开始触发寄存器*/
        stc_adc_trgsr_field_t TRGSR_f;/**< \brief AD 转换开始触发寄存器位域结构体*/
    };
    __IO uint16_t CHSELRA0;           /**< \brief AD 通道选择寄存器A0*/
    union
    {
        __IO uint16_t CHSELRA1;       /**< \brief AD 通道选择寄存器A1*/
        stc_adc_chselra1_field_t CHSELRA1_f;  /**< \brief AD 通道选择寄存器A1位域结构体*/
    };
    __IO uint16_t CHSELRB0;           /**< \brief AD 通道选择寄存器B0*/
    union
    {
        __IO uint16_t CHSELRB1;       /**< \brief AD 通道选择寄存器B1*/
        stc_adc_chselrb1_field_t CHSELRB1_f;  /**< \brief AD 通道选择寄存器B1位域结构体*/
    };
    __IO uint16_t AVCHSELR0;          /**< \brief AD 平均通道选择寄存器0*/
    union
    {
        __IO uint16_t AVCHSELR1;      /**< \brief AD 平均通道选择寄存器1*/
        stc_adc_avchselr1_field_t AVCHSELR1_f;  /**< \brief AD 平均通道选择寄存器1位域结构体*/
    };
    uint8_t RESERVED10[8];
    __IO uint8_t SSTR0;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR1;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR2;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR3;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR4;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR5;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR6;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR7;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR8;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR9;               /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR10;              /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR11;              /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR12;              /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR13;              /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR14;              /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTR15;              /**< \brief AD 采样周期寄存器*/
    __IO uint8_t SSTRL;               /**< \brief AD 采样周期寄存器*/
    uint8_t RESERVED27[7];            /**< \brief 保留*/
    union
    {
        __IO uint16_t CHMUXR0;             /**< \brief AD通道映射控制寄存器0*/
        stc_adc_chmuxr0_field_t CHMUXR0_f; /**< \brief AD通道映射控制寄存器0位域结构体*/
    };
    union
    {
        __IO uint16_t CHMUXR1;             /**< \brief AD通道映射控制寄存器1*/
        stc_adc_chmuxr1_field_t CHMUXR1_f; /**< \brief AD通道映射控制寄存器1位域结构体*/
    };
    union
    {
        __IO uint16_t CHMUXR2;             /**< \brief AD通道映射控制寄存器2*/
        stc_adc_chmuxr2_field_t CHMUXR2_f; /**< \brief AD通道映射控制寄存器2位域结构体*/
    };
    union
    {
        __IO uint16_t CHMUXR3;             /**< \brief AD通道映射控制寄存器3*/
        stc_adc_chmuxr3_field_t CHMUXR3_f; /**< \brief AD通道映射控制寄存器3位域结构体*/
    };
    uint8_t RESERVED31[6];                 /**< \brief 保留*/
    union
    {
        __IO uint8_t ISR;                  /**< \brief AD中断状态寄存器*/
        stc_adc_isr_field_t ISR_f;         /**< \brief AD中断状态寄存器位域结构体*/
    };
    union
    {
        __IO uint8_t ICR;                  /**< \brief AD中断许可寄存器*/
        stc_adc_icr_field_t ICR_f;         /**< \brief AD中断许可寄存器位域结构体*/
    };
    uint8_t RESERVED33[4];                 /**< \brief 保留*/
    union
    {
        __IO uint16_t SYNCCR;              /**< \brief AD协同模式控制寄存器*/
        stc_adc_synccr_field_t SYNCCR_f;   /**< \brief AD协同模式控制寄存器位域结构体*/
    };
    uint8_t RESERVED34[2];
    __IO uint16_t DR0;                     /**< \brief ch0 AD数据寄存器*/
    __IO uint16_t DR1;                     /**< \brief ch1 AD数据寄存器*/
    __IO uint16_t DR2;                     /**< \brief ch2 AD数据寄存器*/
    __IO uint16_t DR3;                     /**< \brief ch3 AD数据寄存器*/
    __IO uint16_t DR4;                     /**< \brief ch4 AD数据寄存器*/
    __IO uint16_t DR5;                     /**< \brief ch5 AD数据寄存器*/
    __IO uint16_t DR6;                     /**< \brief ch6 AD数据寄存器*/
    __IO uint16_t DR7;                     /**< \brief ch7 AD数据寄存器*/
    __IO uint16_t DR8;                     /**< \brief ch8 AD数据寄存器*/
    __IO uint16_t DR9;                     /**< \brief ch9 AD数据寄存器*/
    __IO uint16_t DR10;                    /**< \brief ch10 AD数据寄存器*/
    __IO uint16_t DR11;                    /**< \brief ch11 AD数据寄存器*/
    __IO uint16_t DR12;                    /**< \brief ch12 AD数据寄存器*/
    __IO uint16_t DR13;                    /**< \brief ch13 AD数据寄存器*/
    __IO uint16_t DR14;                    /**< \brief ch14 AD数据寄存器*/
    __IO uint16_t DR15;                    /**< \brief ch15 AD数据寄存器*/
    __IO uint16_t DR16;                    /**< \brief ch16 AD数据寄存器*/
    uint8_t RESERVED51[46];                /**< \brief 保留*/
    union
    {
        __IO uint16_t AWDCR;               /**< \brief 模拟看门狗控制寄存器*/
        stc_adc_awdcr_field_t AWDCR_f;     /**< \brief 模拟看门狗控制寄存器位域结构体*/
    };
    uint8_t RESERVED52[2];                 /**< \brief 保留*/
    __IO uint16_t AWDDR0;                  /**< \brief 模拟看门狗阀值寄存器0*/
    __IO uint16_t AWDDR1;                  /**< \brief 模拟看门狗阀值寄存器1*/
    uint8_t RESERVED54[4];                 /**< \brief 保留*/
    __IO uint16_t AWDCHSR0;                /**< \brief 模拟看门狗比较通道选择寄存器0*/
    union
    {
        __IO uint16_t AWDCHSR1;               /**< \brief 模拟看门狗比较通道选择寄存器1*/
        stc_adc_awdchsr1_field_t AWDCHSR1_f;  /**< \brief 模拟看门狗比较通道选择寄存器1位域结构体*/
    };
    __IO uint16_t AWDSR0;                  /**< \brief 模拟看门狗状态寄存器0*/
    union
    {
        __IO uint16_t AWDSR1;              /**< \brief 模拟看门狗状态寄存器1*/
        stc_adc_awdsr1_field_t AWDSR1_f;   /**< \brief 模拟看门狗状态寄存器1位域结构体*/
    };
    uint8_t RESERVED58[12];                /**< \brief 保留*/
    union
    {
        __IO uint16_t PGACR;               /**< \brief AD可编程增益放大控制寄存器*/
        stc_adc_pgacr_field_t PGACR_f;     /**< \brief AD可编程增益放大控制寄存器位域结构体*/
    };
    union
    {
        __IO uint16_t PGAGSR;              /**< \brief AD可编程增益倍数寄存器*/
        stc_adc_pgagsr_field_t PGAGSR_f;   /**< \brief AD可编程增益倍数寄存器位域结构体*/
    };
    uint8_t RESERVED60[8];                 /**< \brief 保留*/
    union
    {
        __IO uint16_t PGAINSR0;               /**< \brief AD可编程增益放大器输入选择寄存器0*/
        stc_adc_pgainsr0_field_t PGAINSR0_f;  /**< \brief AD可编程增益放大器输入选择寄存器0位域结构体*/
    };
    union
    {
        __IO uint16_t PGAINSR1;               /**< \brief AD可编程增益放大器输入选择寄存器1*/
        stc_adc_pgainsr1_field_t PGAINSR1_f;  /**< \brief AD可编程增益放大器输入选择寄存器1位域结构体*/
    };
} amhw_hc32f460_adc_t;

/* 通道类型 */
typedef enum {
    AMHW_HC32F460_ADC_CH_MUX_ADC1_IN0       = 0,
    AMHW_HC32F460_ADC_CH_MUX_ADC1_IN1       = 1,
    AMHW_HC32F460_ADC_CH_MUX_ADC1_IN2       = 2,
    AMHW_HC32F460_ADC_CH_MUX_ADC1_IN3       = 3,
    AMHW_HC32F460_ADC_CH_MUX_ADC12_IN4       = 4,
    AMHW_HC32F460_ADC_CH_MUX_ADC12_IN5       = 5,
    AMHW_HC32F460_ADC_CH_MUX_ADC12_IN6       = 6,
    AMHW_HC32F460_ADC_CH_MUX_ADC12_IN7       = 7,
    AMHW_HC32F460_ADC_CH_MUX_ADC12_IN8       = 8,
    AMHW_HC32F460_ADC_CH_MUX_ADC12_IN9       = 9,
    AMHW_HC32F460_ADC_CH_MUX_ADC12_IN10      = 10,
    AMHW_HC32F460_ADC_CH_MUX_ADC12_IN11      = 11,
    AMHW_HC32F460_ADC_CH_MUX_ADC1_IN12       = 12,
    AMHW_HC32F460_ADC_CH_MUX_ADC1_IN13       = 13,
    AMHW_HC32F460_ADC_CH_MUX_ADC1_IN14       = 14,
    AMHW_HC32F460_ADC_CH_MUX_ADC1_IN15       = 15,
    AMHW_HC32F460_ADC_CH_MUX_ADC12_IN_ANALOG      = 16,
}amhw_hc32f460_adc_chan_sel_t;


/**
 * \brief ADC开始转换
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_convert_start(amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->STR_f.STRT = 1;
}

/**
 * \brief ADC停止转换
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_convert_stop(amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->STR_f.STRT = 0;
}


typedef enum amhw_hc32f460_adc_average_count{
    AMHW_HC32F460_ADC_AVERAGE_COUNT_2   = 0x0,
    AMHW_HC32F460_ADC_AVERAGE_COUNT_4   = 0x1,
    AMHW_HC32F460_ADC_AVERAGE_COUNT_8   = 0x2,
    AMHW_HC32F460_ADC_AVERAGE_COUNT_16  = 0x3,
    AMHW_HC32F460_ADC_AVERAGE_COUNT_32  = 0x4,
    AMHW_HC32F460_ADC_AVERAGE_COUNT_64  = 0x5,
    AMHW_HC32F460_ADC_AVERAGE_COUNT_128 = 0x6,
    AMHW_HC32F460_ADC_AVERAGE_COUNT_256 = 0x7,
}amhw_hc32f460_adc_average_count_t;


/**
 * \brief ADC平均转换设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_average_count_set(amhw_hc32f460_adc_t              *p_hw_adc,
                                         amhw_hc32f460_adc_average_count_t flag)
{
    p_hw_adc->CR0_f.AVCNT = flag;
}


typedef enum amhw_hc32f460_adc_data_align_format{
    AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT = 0x0, /** <brief 数据右对齐 */
    AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_LEFT  = 0x1, /** <brief 数据左对齐 */
}amhw_hc32f460_adc_data_align_format_t;

/**
 * \brief ADC转换结果对齐方式设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_data_align_set(amhw_hc32f460_adc_t                  *p_hw_adc,
                                      amhw_hc32f460_adc_data_align_format_t flag)
{
    p_hw_adc->CR0_f.DFMT = flag;
}

/**
 * \brief ADC数据寄存器自动清除使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_data_autoclr_enable(amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->CR0_f.CLREN = 1;
}

/**
 * \brief ADC数据寄存器自动清除禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_data_autoclr_disable(amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->CR0_f.CLREN = 0;
}

/**
 * \brief ADC转换精度
 */
typedef enum amhw_hc32f460_adc_resolution{
    AMHW_HC32F460_ADC_RESOLUTION_12BIT = 12, /** <brief 12位分辨率 */
    AMHW_HC32F460_ADC_RESOLUTION_10BIT = 10, /** <brief 10位分辨率 */
    AMHW_HC32F460_ADC_RESOLUTION_8BIT  = 8,  /** <brief 8位分辨率 */
}amhw_hc32f460_adc_resolution_t;

/**
 * \brief ADC分辨率设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_resolution_set(amhw_hc32f460_adc_t           *p_hw_adc,
                                      amhw_hc32f460_adc_resolution_t flag)
{
    p_hw_adc->CR0_f.ACCSEL = flag;
}

typedef enum amhw_hc32f460_adc_mode{
    AMHW_HC32F460_ADC_SEQ_A_ONCE              = 0x0, /** <brief 序列A单次扫描 */
    AMHW_HC32F460_ADC_SEQ_A_CONTINUOUS        = 0x1, /** <brief 序列A连续扫描 */
    AMHW_HC32F460_ADC_SEQ_A_ONCE_B_ONCE       = 0x2, /** <brief 序列A单次扫描，B单次扫描 */
    AMHW_HC32F460_ADC_SEQ_A_CONTINUOUS_B_ONCE = 0x3, /** <brief 序列A连续扫描，B单次扫描 */
}amhw_hc32f460_adc_mode_t;
/**
 * \brief ADC模式设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_mode_set(amhw_hc32f460_adc_t      *p_hw_adc,
                                amhw_hc32f460_adc_mode_t  flag)
{
    p_hw_adc->CR0_f.MS = flag;
}

typedef enum amhw_hc32f460_adc_seq_a_restart_position{
    AMHW_HC32F460_ADC_SEQ_A_RESTART_POSITION_CONTINUOUS = 0x0, /** <brief 被序列B中断后，序列A重启时从被中断通道开始继续扫描 */
    AMHW_HC32F460_ADC_SEQ_A_RESTART_POSITION_RESTART    = 0x1, /** <brief 被序列B中断后，序列A重启时从第一个通道开始重新扫描 */
}amhw_hc32f460_adc_seq_a_restart_position_t;

/**
 * \brief ADC序列A重启方式设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_seq_a_restart_position_set(amhw_hc32f460_adc_t                       *p_hw_adc,
                                                  amhw_hc32f460_adc_seq_a_restart_position_t flag)
{
    p_hw_adc->CR1_f.RSCHSEL = 0;
}

/**
 * \brief ADC序列B片内或外部引脚触发许可
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_seq_b_trigger_enable(amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->TRGSR_f.TRGENB = 1;
}

/**
 * \brief ADC序列B片内或外部引脚触发禁止
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_seq_b_trigger_disable(amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->TRGSR_f.TRGENB = 0;
}

/**
 * \brief ADC序列A片内或外部引脚触发许可
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_seq_a_trigger_enable(amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->TRGSR_f.TRGENA = 1;
}

/**
 * \brief ADC序列A片内或外部引脚触发禁止
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_seq_a_trigger_disable(amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->TRGSR_f.TRGENA = 0;
}

typedef enum amhw_hc32f460_adc_seq_trigger_source{
    AMHW_HC32F460_ADC_SEQ_TRIGGER_ADTRx                 = 0x0,
    AMHW_HC32F460_ADC_SEQ_TRIGGER_IN_TRGx0              = 0x1,
    AMHW_HC32F460_ADC_SEQ_TRIGGER_IN_TRGx1              = 0x2,
    AMHW_HC32F460_ADC_SEQ_TRIGGER_IN_TRGx0_AND_IN_TRGx1 = 0x3,
}amhw_hc32f460_adc_seq_trigger_source_t;

/**
 * \brief ADC序列B触发条件选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_seq_b_trigger_source_sel(amhw_hc32f460_adc_t *p_hw_adc,
                                                amhw_hc32f460_adc_seq_trigger_source_t flag)
{
    p_hw_adc->TRGSR_f.TRGSELB = flag;
}

/**
 * \brief ADC序列A触发条件选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_seq_a_trigger_source_sel(amhw_hc32f460_adc_t                   *p_hw_adc,
                                                amhw_hc32f460_adc_seq_trigger_source_t flag)
{
    p_hw_adc->TRGSR_f.TRGSELA = flag;
}

#define HC32F460_AOS_ADC_ITRGSELR0_BASE    (0x40010858UL)
#define HC32F460_AOS_ADC_ITRGSELR1_BASE    (0x40010860UL)
/**
 * \brief ADC片内触发源（IN_TRGx0）选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_intrgx0_sel(int32_t flag)
{
    *(volatile uint32_t *)((uint32_t)(HC32F460_AOS_ADC_ITRGSELR0_BASE)) = flag;
}

/**
 * \brief ADC片内触发源（IN_TRGx1）选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_intrgx1_sel(int32_t flag)
{
    *(volatile uint32_t *)((uint32_t)(HC32F460_AOS_ADC_ITRGSELR1_BASE)) = flag;
}

/**
 * \brief ADC序列A通道选择(CHSELA[15:0])
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 *
 * \note 不要在序列A和序列B中选择相同的通道
 */
am_static_inline
void amhw_hc32f460_adc_seq_a_channel_sel0(amhw_hc32f460_adc_t *p_hw_adc,
                                          uint32_t             chan)
{
    p_hw_adc->CHSELRA0 |= (uint16_t)((1 << chan) & 0xFFFF);
}

/**
 * \brief ADC序列A通道选择(CHSELA[31:16])
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 *
 * \note 不要在序列A和序列B中选择相同的通道
 */
am_static_inline
void amhw_hc32f460_adc_seq_a_channel_sel1(amhw_hc32f460_adc_t *p_hw_adc,
                                          uint32_t             chan)
{
    p_hw_adc->CHSELRA1 |= (uint16_t)((1 << (chan - 16)) & 0xFFFF);
}

/**
 * \brief ADC序列B通道选择(CHSELB[15:0])
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 *
 * \note 不要在序列A和序列B中选择相同的通道
 */
am_static_inline
void amhw_hc32f460_adc_seq_b_channel_sel0(amhw_hc32f460_adc_t *p_hw_adc,
                                          uint32_t             chan)
{
    p_hw_adc->CHSELRB0 |= (uint16_t)((1 << chan) & 0xFFFF);
}

/**
 * \brief ADC序列B通道选择(CHSELB[31:16])
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 *
 * \note 不要在序列A和序列B中选择相同的通道
 */
am_static_inline
void amhw_hc32f460_adc_seq_b_channel_sel1(amhw_hc32f460_adc_t *p_hw_adc,
                                          uint32_t             chan)
{
    p_hw_adc->CHSELRB1 |= (uint16_t)((1 << (chan - 16)) & 0xFFFF);
}

typedef enum {
    AMHW_HC32F460_ADC_CHAN0_MASK = 1 << 0,
    AMHW_HC32F460_ADC_CHAN1_MASK = 1 << 1,
    AMHW_HC32F460_ADC_CHAN2_MASK = 1 << 2,
    AMHW_HC32F460_ADC_CHAN3_MASK = 1 << 3,
    AMHW_HC32F460_ADC_CHAN4_MASK = 1 << 4,
    AMHW_HC32F460_ADC_CHAN5_MASK = 1 << 5,
    AMHW_HC32F460_ADC_CHAN6_MASK = 1 << 6,
    AMHW_HC32F460_ADC_CHAN7_MASK = 1 << 7,
    AMHW_HC32F460_ADC_CHAN8_MASK = 1 << 8,
    AMHW_HC32F460_ADC_CHAN9_MASK = 1 << 9,
    AMHW_HC32F460_ADC_CHAN10_MASK = 1 << 10,
    AMHW_HC32F460_ADC_CHAN11_MASK = 1 << 11,
    AMHW_HC32F460_ADC_CHAN12_MASK = 1 << 12,
    AMHW_HC32F460_ADC_CHAN13_MASK = 1 << 13,
    AMHW_HC32F460_ADC_CHAN14_MASK = 1 << 14,
    AMHW_HC32F460_ADC_CHAN15_MASK = 1 << 15,
    AMHW_HC32F460_ADC_CHAN16_MASK = 1 << 16,
}amhw_hc32f460_adc_chan_mask_t;


/**
 * \brief ADC序列平均通道选择(AVCHSEL[15:0])
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_average_channel_sel0(amhw_hc32f460_adc_t *p_hw_adc,
                                            uint32_t             chan)
{
    p_hw_adc->AVCHSELR0 |= (uint16_t)(1 << chan);
}

/**
 * \brief ADC序列平均通道选择(AVCHSEL[31:16])
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_average_channel_sel1(amhw_hc32f460_adc_t *p_hw_adc,
                                            uint32_t             chan)
{
	p_hw_adc->AVCHSELR1 |= (uint16_t)((1 << (chan - 16)));
}

/**
 * \brief ADC序列平均通道清除(AVCHSEL[15:0])
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_average_channel_clr0(amhw_hc32f460_adc_t *p_hw_adc,
                                            uint32_t             chan)
{
    p_hw_adc->AVCHSELR0 &= ~(uint16_t)(1 << chan);
}

/**
 * \brief ADC序列平均通道清除(AVCHSEL[31:16])
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_average_channel_clr1(amhw_hc32f460_adc_t *p_hw_adc,
                                            uint32_t             chan)
{
	p_hw_adc->AVCHSELR1 &= ~(uint16_t)((1 << (chan - 16)));
}



/**
 * \brief ADC采样周期设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] channel  : 通道号
 * \param[in] count    : 采样周期数（5~255）
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_sampling_period_set(amhw_hc32f460_adc_t *p_hw_adc,
                                           uint8_t              channel,
                                           uint8_t              count)
{
    if (channel <= 15) {
        *(uint8_t *)((&p_hw_adc->SSTR0) + channel) = count;
    } else {
        p_hw_adc->SSTRL = count;
    }
}

/**
 * \brief ADC采样周期获取
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] channel  : 通道号
 *
 * \return none
 */
am_static_inline
uint8_t amhw_hc32f460_adc_sampling_period_get(amhw_hc32f460_adc_t *p_hw_adc,
                                              uint8_t              channel)
{
    if (channel <= 15) {
        return *(uint8_t *)((&p_hw_adc->SSTR0) + channel);
    } else {
        return p_hw_adc->SSTRL;
    }
}


typedef struct amhw_adc_chmux{
    uint8_t                           chan;
    uint8_t                           pin;
    amhw_hc32f460_adc_chan_sel_t      type;
}amhw_adc_chmux_t;


/**
 * \brief ADC1通道映射设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] channel  : 通道号
 * \param[in] count    : 映射对象编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_adc_channel_mux_set(amhw_hc32f460_adc_t               *p_hw_adc,
                                       uint8_t                           channel,
                                       amhw_hc32f460_adc_chan_sel_t      type)
{
    if ((uint32_t)p_hw_adc == HC32F460_ADC1_BASE) {
        *(uint16_t *)((&p_hw_adc->CHMUXR0) + (channel / 4)) &= ~(0xf << (4 * (channel % 4)));
        *(uint16_t *)((&p_hw_adc->CHMUXR0) + (channel / 4)) |= (type) << (4 * (channel % 4));
    } else {
        *(uint16_t *)((&p_hw_adc->CHMUXR0) + (channel / 4)) &= ~(((type < 16) ? (type - 4) : 8) << (4 * (channel % 4)));
        *(uint16_t *)((&p_hw_adc->CHMUXR0) + (channel / 4)) = ((type < 16) ? (type - 4) : 8) << (4 * (channel % 4));
    }

}


typedef enum amhw_hc32f460_adc_int_type{
    AMHW_HC32F460_ADC_INT_EOCAF = 0,  /* 序列A转换完成中断*/
    AMHW_HC32F460_ADC_INT_EOCBF = 1,  /* 序列B转换完成中断*/
}amhw_hc32f460_adc_int_type_t;

/**
 * \brief ADC中断状态标志获取
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f460_adc_int_type_t枚举量）
 *
 * \return AM_TRUE  : 1
 *         AM_FALSE : 0
 */
am_static_inline
am_bool_t amhw_hc32f460_adc_int_flag_check (amhw_hc32f460_adc_t          *p_hw_adc,
                                            amhw_hc32f460_adc_int_type_t  type)
{
    return ((p_hw_adc->ISR >> type) & 0x1u) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief ADC中断状态标志清除
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f460_adc_int_type_t枚举量）

 */
am_static_inline
void amhw_hc32f460_adc_int_flag_clr (amhw_hc32f460_adc_t          *p_hw_adc,
                                     amhw_hc32f460_adc_int_type_t  type)
{
    p_hw_adc->ISR &= ~(1 << type);
}

/**
 * \brief ADC中断使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f460_adc_int_type_t枚举量）
 */
am_static_inline
void amhw_hc32f460_adc_int_enable (amhw_hc32f460_adc_t          *p_hw_adc,
                                   amhw_hc32f460_adc_int_type_t  type)
{
    p_hw_adc->ICR |= (1 << type);
}

/**
 * \brief ADC中断禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] type     : 标志类别（amhw_hc32f460_adc_int_type_t枚举量）
 */
am_static_inline
void amhw_hc32f460_adc_int_disable (amhw_hc32f460_adc_t          *p_hw_adc,
                                    amhw_hc32f460_adc_int_type_t  type)
{
    p_hw_adc->ICR &= (~(1 << type));
}

#define AMHW_HC32F460_ADC_DATA_RIGHT_12BIT_MASK  (0XFFF)
#define AMHW_HC32F460_ADC_DATA_RIGHT_10BIT_MASK  (0X3FF)
#define AMHW_HC32F460_ADC_DATA_RIGHT_8BIT_MASK   (0XFF)
#define AMHW_HC32F460_ADC_DATA_LEFT_12BIT_MASK   (0XFFF0)
#define AMHW_HC32F460_ADC_DATA_LEFT_10BIT_MASK   (0XFFC0)
#define AMHW_HC32F460_ADC_DATA_LEFT_8BIT_MASK    (0XFF00)

/**
 * \brief ADC获取通道x的数据
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] channel    : 通道号
 * \param[in] resolution : 分辨率
 * \param[in] format     : 对齐方式
 */
am_static_inline
uint16_t amhw_hc32f460_adc_ch_data_get (amhw_hc32f460_adc_t                  *p_hw_adc,
                                        uint32_t                              channel,
                                        amhw_hc32f460_adc_resolution_t        resolution,
                                        amhw_hc32f460_adc_data_align_format_t format)
{
    if (format == AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_LEFT) {
        if (resolution == AMHW_HC32F460_ADC_RESOLUTION_8BIT) {
            return (*(uint16_t *)(&(p_hw_adc->DR0) + channel) & AMHW_HC32F460_ADC_DATA_LEFT_8BIT_MASK) >> 8;
        } else if(resolution == AMHW_HC32F460_ADC_RESOLUTION_10BIT) {
            return (*(uint16_t *)(&(p_hw_adc->DR0) + channel) & AMHW_HC32F460_ADC_DATA_LEFT_10BIT_MASK) >> 6;
        } else {
            return (*(uint16_t *)(&(p_hw_adc->DR0) + channel) & AMHW_HC32F460_ADC_DATA_LEFT_12BIT_MASK) >> 4;
        }
    } else {
        if (resolution == AMHW_HC32F460_ADC_RESOLUTION_8BIT) {
            return *(uint16_t *)(&(p_hw_adc->DR0) + channel) & AMHW_HC32F460_ADC_DATA_RIGHT_8BIT_MASK;
        } else if(resolution == AMHW_HC32F460_ADC_RESOLUTION_10BIT) {
            return *(uint16_t *)(&(p_hw_adc->DR0) + channel) & AMHW_HC32F460_ADC_DATA_RIGHT_10BIT_MASK;
        } else {
            return *(uint16_t *)(&(p_hw_adc->DR0) + channel) & AMHW_HC32F460_ADC_DATA_RIGHT_12BIT_MASK;
        }
    }
}

/**
 * \brief ADC模拟看门狗中断使能
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 */
am_static_inline
void amhw_hc32f460_adc_awd_int_enable (amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->AWDCR_f.AWDIEN = AM_TRUE;
}

/**
 * \brief ADC模拟看门狗中断禁能
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 */
am_static_inline
void amhw_hc32f460_adc_awd_int_disable (amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->AWDCR_f.AWDIEN = AM_FALSE;
}

/**
 * \brief ADC模拟看门狗低阀值设置
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] value      : 低阀值设定值
 * \param[in] resolution : 转换精度（amhw_hc32f460_adc_resolution_t）
 * \param[in] format     : 数据对齐方式（amhw_hc32f460_adc_data_align_format_t）
 */
am_static_inline
void amhw_hc32f460_adc_awd_low_threshold_set (amhw_hc32f460_adc_t                  *p_hw_adc,
                                              uint16_t                              value,
                                              amhw_hc32f460_adc_resolution_t        resolution,
                                              amhw_hc32f460_adc_data_align_format_t format)
{
    if (format == AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_LEFT) {
        p_hw_adc->AWDDR0 = value << (16 - resolution);
    } else {
        p_hw_adc->AWDDR0 = value;
    }
}

/**
 * \brief ADC模拟看门狗高阀值设置
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] value      : 低阀值设定值
 * \param[in] resolution : 转换精度（amhw_hc32f460_adc_resolution_t）
 * \param[in] format     : 数据对齐方式（amhw_hc32f460_adc_data_align_format_t）
 */
am_static_inline
void amhw_hc32f460_adc_awd_high_threshold_set (amhw_hc32f460_adc_t                  *p_hw_adc,
                                               uint16_t                              value,
                                               amhw_hc32f460_adc_resolution_t        resolution,
                                               amhw_hc32f460_adc_data_align_format_t format)
{
    if (format == AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_LEFT) {
        p_hw_adc->AWDDR1 = value << (16 - resolution);
    } else {
        p_hw_adc->AWDDR1 = value;
    }
}

/**
 * \brief ADC模拟看门狗比较通道设置(ch0~ch15)
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] chan_mask  : 指定通道的掩码（ch0~ch15，amhw_hc32f460_adc_chan_mask_t）
 */
am_static_inline
void amhw_hc32f460_adc_awd_chan0_15_set (amhw_hc32f460_adc_t     *p_hw_adc,
                                         uint16_t                 chan_mask)
{
    p_hw_adc->AWDCHSR0 = chan_mask;
}

/**
 * \brief ADC模拟看门狗比较通道设置(ch16)
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] chan_mask  : 指定通道的掩码（ch16，amhw_hc32f460_adc_chan_mask_t）
 */
am_static_inline
void amhw_hc32f460_adc_awd_chan_16_set (amhw_hc32f460_adc_t     *p_hw_adc,
                                        uint16_t                 chan_mask)
{
    p_hw_adc->AWDCHSR1_f.AWDCH16 = chan_mask;
}

/**
 * \brief ADC模拟看门狗比较状态获取(ch0~ch15)
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] chan_mask  : 指定通道的掩码（ch0~ch15，amhw_hc32f460_adc_chan_mask_t）
 */
am_static_inline
uint16_t amhw_hc32f460_adc_awd_chan0_15_cmpstat_get (amhw_hc32f460_adc_t     *p_hw_adc)
{
    return p_hw_adc->AWDSR0;
}

/**
 * \brief ADC模拟看门狗比较通道获取(ch16)
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] chan_mask  : 指定通道的掩码（ch16，amhw_hc32f460_adc_chan_mask_t）
 */
am_static_inline
uint16_t amhw_hc32f460_adc_awd_chan16_cmpstat_get (amhw_hc32f460_adc_t     *p_hw_adc)
{
    return p_hw_adc->AWDSR1_f.AWDF16;
}

/**
 * \brief ADC模拟看门狗比较状态清除(ch0~ch15)
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] chan_mask  : 指定通道的掩码（ch0~ch15，amhw_hc32f460_adc_chan_mask_t）
 */
am_static_inline
void amhw_hc32f460_adc_awd_chan0_15_cmpstat_clr (amhw_hc32f460_adc_t     *p_hw_adc,
                                                 uint16_t                 chan_mask)
{
    p_hw_adc->AWDSR0 &= (~chan_mask);
}

/**
 * \brief ADC模拟看门狗比较通道清除(ch16)
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] chan_mask  : 指定通道的掩码（ch16，amhw_hc32f460_adc_chan_mask_t）
 */
am_static_inline
void amhw_hc32f460_adc_awd_chan16_cmpstat_clr (amhw_hc32f460_adc_t     *p_hw_adc,
                                               uint16_t                 chan_mask)
{
	p_hw_adc->AWDSR1_f.AWDF16 &= (~chan_mask);
}


/* 看门狗序列选择 */
typedef enum {
    AMHW_HC32F460_ADC_AWD_SEQ_A_AND_B = 0,  /**<brief 序列A和B扫描完成时都输出ADC_SEQCMP中断/事件 */
    AMHW_HC32F460_ADC_AWD_SEQ_A_ONLY  = 1,  /**<brief 序列A扫描完成时输出ADC_SEQCMP中断/事件，序列B不输出 */
    AMHW_HC32F460_ADC_AWD_SEQ_B_ONLY  = 2,  /**<brief 序列B扫描完成时输出ADC_SEQCMP中断/事件，序列A不输出 */
}amhw_hc32f460_adc_awd_seq_sel_t;


/**
 * \brief ADC模拟看门狗序列选择
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 */
am_static_inline
void amhw_hc32f460_adc_awd_seq_set (amhw_hc32f460_adc_t             *p_hw_adc,
                                    amhw_hc32f460_adc_awd_seq_sel_t  flag)
{
    p_hw_adc->AWDCR_f.AWDSS = flag;
}

typedef enum {
    AMHW_HC32F460_ADC_AWD_CMPMODE0 = 0, /**<brief 转换结果<AWDDR0,或转换结果>AWDDR1时，比较条件满足 */
    AMHW_HC32F460_ADC_AWD_CMPMODE1 = 1, /**<brief AWDDR0 <= 转换结果 <= AWDDR1 时，比较条件满足 */
}amhw_hc32f460_adc_awd_cmpmode_t;

/**
 * \brief ADC模拟看门狗比较模式设置
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 */
am_static_inline
void amhw_hc32f460_adc_awd_cmp_mode_set (amhw_hc32f460_adc_t             *p_hw_adc,
                                         amhw_hc32f460_adc_awd_cmpmode_t  flag)
{
    p_hw_adc->AWDCR_f.AWDMD = flag;
}

/**
 * \brief ADC模拟看门狗比较功能使能
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 */
am_static_inline
void amhw_hc32f460_adc_awd_enable (amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->AWDCR_f.AWDEN = AM_TRUE;
}

/**
 * \brief ADC模拟看门狗比较功能禁止
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 */
am_static_inline
void amhw_hc32f460_adc_awd_disable (amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->AWDCR_f.AWDEN = AM_FALSE;
}

/**
 * \brief ADC所有中断禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 */
am_static_inline
void amhw_hc32f460_adc_all_int_disable (amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->ICR = 0;
    p_hw_adc->AWDCR_f.AWDIEN = 0;
}


/**
 * \brief ADC可编程放大控制器使能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 */
am_static_inline
void amhw_hc32f460_adc_pga_enable (amhw_hc32f460_adc_t  *p_hw_adc)
{
    p_hw_adc->PGACR_f.PGACTL = 0xE;
}

/**
 * \brief ADC可编程放大控制器禁能
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 */
am_static_inline
void amhw_hc32f460_adc_pga_disable (amhw_hc32f460_adc_t  *p_hw_adc)
{
    p_hw_adc->PGACR_f.PGACTL = 0x0;
}

typedef enum {
    AMHW_HC32F460_ADC_PGA_GAIN_2_000 = 0, /**<brief × 2.000 */
    AMHW_HC32F460_ADC_PGA_GAIN_2_133,     /**<brief × 2.133 */
    AMHW_HC32F460_ADC_PGA_GAIN_2_286,     /**<brief × 2.286 */
    AMHW_HC32F460_ADC_PGA_GAIN_2_667,     /**<brief × 2.667 */
    AMHW_HC32F460_ADC_PGA_GAIN_2_909,     /**<brief × 2.909 */
    AMHW_HC32F460_ADC_PGA_GAIN_3_200,     /**<brief × 3.2 */
    AMHW_HC32F460_ADC_PGA_GAIN_3_556,     /**<brief × 3.556 */
    AMHW_HC32F460_ADC_PGA_GAIN_4_000,     /**<brief × 4.000 */
    AMHW_HC32F460_ADC_PGA_GAIN_4_571,     /**<brief × 4.571 */
    AMHW_HC32F460_ADC_PGA_GAIN_5_333,     /**<brief × 5.333 */
    AMHW_HC32F460_ADC_PGA_GAIN_6_400,     /**<brief × 6.4 */
    AMHW_HC32F460_ADC_PGA_GAIN_8_000,     /**<brief × 8 */
    AMHW_HC32F460_ADC_PGA_GAIN_10_667,    /**<brief × 10.667 */
    AMHW_HC32F460_ADC_PGA_GAIN_16,        /**<brief × 16 */
    AMHW_HC32F460_ADC_PGA_GAIN_32,        /**<brief × 32 */
} amhw_hc32f460_adc_pga_gain_value_t;


/**
 * \brief ADC可编程放大控制器增益倍数设置
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] gain_value : 增益倍数（amhw_hc32f460_adc_pga_gain_value_t）
 */
am_static_inline
void amhw_hc32f460_adc_pga_gain_set(amhw_hc32f460_adc_t               *p_hw_adc,
                                    amhw_hc32f460_adc_pga_gain_value_t gain_value)
{
    p_hw_adc->PGAGSR_f.GAIN = gain_value;
}

/**
 * \brief ADC可编程放大控制器模拟输入选择
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] adc_in_num : ADC输入编号(选择内部模拟通道时只能是8bitDAC_1)
 */
am_static_inline
void amhw_hc32f460_adc_pga_analog_input_set(amhw_hc32f460_adc_t               *p_hw_adc,
                                            amhw_hc32f460_adc_chan_sel_t       adc_in_num)
{
    p_hw_adc->PGAINSR0_f.PGAINSEL = (1 << adc_in_num);
}

typedef enum {
    AMHW_HC32F460_ADC_PGA_N_INPUT_PGAVSS = 0,  /* 使用外部端口PGAVSS作为PGA负相输入 */
    AMHW_HC32F460_ADC_PGA_N_INPUT_INAVSS = 1,  /* 使用内部模拟地AVSS作为PGA负相输入 */
}amhw_hc32f460_adc_pga_negative_input_t;


/**
 * \brief ADC可编程放大控制器负相输入选择
 *
 * \param[in] p_hw_adc   : 指向ADC寄存器的指针
 * \param[in] adc_in_num : ADC输入编号
 */
am_static_inline
void amhw_hc32f460_adc_pga_negative_input_set(amhw_hc32f460_adc_t                    *p_hw_adc,
                                              amhw_hc32f460_adc_pga_negative_input_t  flag)
{
    p_hw_adc->PGAINSR1_f.PGAVSSEN = flag;
}

/**
 * \brief ADC协同模式同步延迟时间设置
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] time     : 同步延迟时间（t = time * PCLK4, time = 1~255）
 *
 * note : 在SYNCEN为0时设置该寄存器，请不要写入0x00
 */
am_static_inline
void amhw_hc32f460_adc_sync_delaytime_set(amhw_hc32f460_adc_t *p_hw_adc,
                                          uint8_t              time)
{
    p_hw_adc->SYNCCR_f.SYNCDLY = time;
}

#define  AMHW_HC32F460_ADC_SYNC_MODE_SINGLE   (0 << 2) /* 单次触发 */
#define  AMHW_HC32F460_ADC_SYNC_MODE_CYCLE    (1 << 2) /* 循环触发 */

#define  AMHW_HC32F460_ADC_SYNC_MODE_DELAY    (0 << 1) /* 延迟触发模式 */
#define  AMHW_HC32F460_ADC_SYNC_MODE_PARALLEL (1 << 1) /* 并行触发模式 */

#define  AMHW_HC32F460_ADC_SYNC_MODE_ADC0_1   (0 << 0) /* ADC0和ADC1同步工作，ADC2独立工作 */
#define  AMHW_HC32F460_ADC_SYNC_MODE_ADC0_1_2 (1 << 0) /* ADC0和ADC1、ADC2同步工作 */

/**
 * \brief ADC协同模式选择
 *
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 * \param[in] mode     : 协同工作模式
 *
 * note : 在SYNCEN为0时设置该寄存器
 *        使用单次触发时，请将需要同步的ADC设置为序列A单次扫描，或序列A连续扫描模式。使用触发模式时，
 *        请将ADC设置为序列A单次扫描模式。
 */
am_static_inline
void amhw_hc32f460_adc_sync_mode_set(amhw_hc32f460_adc_t *p_hw_adc,
                                     uint8_t              mode)
{
    p_hw_adc->SYNCCR_f.SYNCMD = mode;
}


/**
 * \brief ADC同步模式许可
 * \param[in] p_hw_adc : 指向ADC寄存器的指针
 *
 * note : 同步模式只支持序列A。在SYNCEN写1之前，请将参与同步的几个ADC的序列B关闭，并给序列A选择相同
 *        数目的通道，设置相同的通道采样时间，以避免各ADC扫描时间不一致，引起后续的同步失败。软件强制
 *        停止转换时，SYNCEN自动清零。
 */
am_static_inline
void amhw_hc32f460_adc_sync_mode_enable(amhw_hc32f460_adc_t *p_hw_adc)
{
    p_hw_adc->SYNCCR_f.SYNCEN = AM_TRUE;
}


#define HC32F460_AOS_ADC1_ITRGSEL0    ((uint32_t *)(0X40010858))
#define HC32F460_AOS_ADC1_ITRGSEL1    ((uint32_t *)(0X4001085C))
#define HC32F460_AOS_ADC2_ITRGSEL0    ((uint32_t *)(0X40010860))
#define HC32F460_AOS_ADC2_ITRGSEL1    ((uint32_t *)(0X40010864))

/**
 * \brief ADC内部触发使能
 *
 * \param[in] p_hw_adc : 指向AOS ADC触发寄存器的指针
 * \param[in] event_src: 事件标号（en_event_src_t枚举量）
 */
am_static_inline
void amhw_hc32f460_aos_adc_in_event_trigger_sel(uint32_t       *p_adc_itrgsel,
                                                uint32_t        event_src) {
    *p_adc_itrgsel = event_src;
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

#endif /* __AMHW_HC32F460_ADC_H */

/* end of file */
