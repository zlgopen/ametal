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
 * - 1.00 17-04-12  ari, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_STM32F103RBT6_ADC_H
#define __AMHW_STM32F103RBT6_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_stm32f103rbt6_if_adc
 * \copydoc amhw_stm32f103rbt6_adc.h
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
 * \brief ADC - Register Layout Typedef
 */
typedef struct amhw_stm32f103rbt6_adc {
    __IO uint32_t sr;        /**< \brief 状态寄存器 */
    __IO uint32_t cr1;       /**< \brief 控制寄存器1 */
    __IO uint32_t cr2;       /**< \brief 控制寄存器2 */
    __IO uint32_t smpr[2];   /**< \brief 采样时间寄存器1、2 */
    __IO uint32_t jofr[4];   /**< \brief 注入通道数据偏移寄存器1、2、3、4 */
    __IO uint32_t htr;       /**< \brief 看门狗高阈值寄存器 */
    __IO uint32_t ltr;       /**< \brief 看门狗低阈值寄存器 */
    __IO uint32_t sqr[3];    /**< \brief 规则序列寄存器1、2、3 */
    __IO uint32_t jsqr;      /**< \brief 注入序列寄存器 */
    __I  uint32_t jdr[4];    /**< \brief 注入数据寄存器1、2、3、4 */
    __I  uint32_t dr;        /**< \brief 规则数据寄存器 */
} amhw_stm32f103rbt6_adc_t;


/**
 * \brief ADC 状态寄存器控制位
 * \@{
 */
#define AMHW_STM32F103RBT6_ADC_DATA_VALID_12BIT          12    /* 12bit有效ADC数据 */

/**
 * \brief ADC 数据对齐方式
 * \@{
 */
#define AMHW_STM32F103RBT6_ADC_DATA_RIGHT                0     /**< \brief 数据右对齐 */
#define AMHW_STM32F103RBT6_ADC_DATA_LEFT                 1     /**< \brief 数据左对齐 */
/** @}*/

/**
 * \brief ADC 单次、连续转换设置
 * \@{
 */
#define AMHW_STM32F103RBT6_ADC_CONVERSION_SINGLE         0     /**< \brief 单次转换 */
#define AMHW_STM32F103RBT6_ADC_CONVERSION_CONTINUOUS     1     /**< \brief 连续转换 */
/** @}*/

/**
 * \brief ADC 状态寄存器控制位
 * \@{
 */
#define AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_STRT_FLAG       (1ul << 4)  /**< \brief 规则通道转换开始标志 */
#define AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_STRT_FLAG      (1ul << 3)  /**< \brief 注入通道转换开始标志 */
#define AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_END_FLAG       (1ul << 2)  /**< \brief 注入通道转换结束标志 */
#define AMHW_STM32F103RBT6_ADC_END_FLAG                     (1ul << 1)  /**< \brief 转换结束标志 */
#define AMHW_STM32F103RBT6_ADC_ANALOG_WDOG_FLAG             (1ul << 0)  /**< \brief 模拟看门狗事件标志 */
#define AMHW_STM32F103RBT6_ADC_ALL_FLAG                  (0x1ful << 0)  /**< \brief 模拟看门狗事件标志 */
/** @}*/

/**
 * \brief ADC 模拟看门狗控制选择
 * \@{
 */
#define AMHW_STM32F103RBT6_ADC_AWDG_NONE_CHAN            0     /**< \brief 不开启模拟看门狗 */
#define AMHW_STM32F103RBT6_ADC_AWDG_INJECTED_CHAN        1     /**< \brief 开启注入通道上的模拟看门狗 */
#define AMHW_STM32F103RBT6_ADC_AWDG_REGULAR_CHAN         2     /**< \brief 开启规则通道上的模拟看门狗 */
#define AMHW_STM32F103RBT6_ADC_AWDG_ALL_CHAN             3     /**< \brief 同时开启注入通道、规则通道上的模拟看门狗 */
/** @}*/

/**
 * \brief ADC 中断使能控制位
 * \@{
 */
#define AMHW_STM32F103RBT6_ADC_INT_INJECTED_END          (0x1ul << 7)  /**< \brief 注入通道转换结束中断 */
#define AMHW_STM32F103RBT6_ADC_INT_AWDG                  (0x1ul << 6)  /**< \brief 模拟看门狗中断 */
#define AMHW_STM32F103RBT6_ADC_INT_END                   (0x1ul << 5)  /**< \brief 转换结束中断 */
#define AMHW_STM32F103RBT6_ADC_INT_ALL                   (0x7ul << 5)  /**< \brief 全部中断 */
/** @}*/

/**
 * \brief ADC 选择启动规则通道组转换的外部事件
 * \@{
 */
/* ADC1/ADC2触发配置 */
#define AMHW_STM32F103RBT6_ADC12_REGULAR_TIM1_CC1                 0     /**< \brief 定时器1的CC1事件 */
#define AMHW_STM32F103RBT6_ADC12_REGULAR_TIM1_CC2                 1     /**< \brief 定时器1的CC2事件 */
#define AMHW_STM32F103RBT6_ADC12_REGULAR_TIM1_CC3                 2     /**< \brief 定时器1的CC3事件 */
#define AMHW_STM32F103RBT6_ADC12_REGULAR_TIM2_CC2                 3     /**< \brief 定时器2的CC2事件 */
#define AMHW_STM32F103RBT6_ADC12_REGULAR_TIM3_TRGO                4     /**< \brief 定时器3的TRGO事件 */
#define AMHW_STM32F103RBT6_ADC12_REGULAR_TIM4_CC4                 5     /**< \brief 定时器4的CC4事件 */
#define AMHW_STM32F103RBT6_ADC12_REGULAR_TIM8_TRGO_OR_EXTI11      6     /**< \brief EXTI线 11/TIM8_TRGO事 件 */
#define AMHW_STM32F103RBT6_ADC12_REGULAR_SWSTART                  7     /**< \brief SWSTART */
/* ADC3触发配置 */
#define AMHW_STM32F103RBT6_ADC3_REGULAR_TIM3_CC1                  0     /**< \brief 定时器3的CC1事件 */
#define AMHW_STM32F103RBT6_ADC3_REGULAR_TIM2_CC3                  1     /**< \brief 定时器3的CC1事件 */
#define AMHW_STM32F103RBT6_ADC3_REGULAR_TIM1_CC3                  2     /**< \brief 定时器1的CC3事件 */
#define AMHW_STM32F103RBT6_ADC3_REGULAR_TIM8_CC1                  3     /**< \brief 定时器8的CC1事件 */
#define AMHW_STM32F103RBT6_ADC3_REGULAR_TIM8_TRGO                 4     /**< \brief 定时器8的TRGO事件 */
#define AMHW_STM32F103RBT6_ADC3_REGULAR_TIM5_CC1                  5     /**< \brief 定时器5的CC1事件 */
#define AMHW_STM32F103RBT6_ADC3_REGULAR_TIM5_CC3                  6     /**< \brief 定时器5的CC3事件 */
#define AMHW_STM32F103RBT6_ADC3_REGULAR_SWSTART                   7     /**< \brief SWSTART */
/** @}*/

/**
 * \brief ADC 选择启动注入通道转换的外部事件
 * \@{
 */
/* ADC1/ADC2触发配置 */
#define AMHW_STM32F103RBT6_ADC12_INJECTED_TIM1_TRGO               0     /**< \brief 定时器1的TRGO事件 */
#define AMHW_STM32F103RBT6_ADC12_INJECTED_TIM1_CC4                1     /**< \brief 定时器1的CC4事件 */
#define AMHW_STM32F103RBT6_ADC12_INJECTED_TIM2_TRGO               2     /**< \brief 定时器2的TRGO事件 */
#define AMHW_STM32F103RBT6_ADC12_INJECTED_TIM2_CC1                3     /**< \brief 定时器2的CC1事件 */
#define AMHW_STM32F103RBT6_ADC12_INJECTED_TIM3_CC4                4     /**< \brief 定时器3的CC4事件 */
#define AMHW_STM32F103RBT6_ADC12_INJECTED_TIM4_TRGO               5     /**< \brief 定时器4的TRGO事件 */
#define AMHW_STM32F103RBT6_ADC12_INJECTED_TIM8_CC4_OR_EXTI15      6     /**< \brief EXTI线 15/TIM8_CC4事 件 */
#define AMHW_STM32F103RBT6_ADC12_INJECTED_JSWSTART                7     /**< \brief JSWSTART */
/* ADC3触发配置 */
#define AMHW_STM32F103RBT6_ADC3_INJECTED_TIM1_TRGO                0     /**< \brief 定时器1的TRG事件 */
#define AMHW_STM32F103RBT6_ADC3_INJECTED_TIM1_CC4                 1     /**< \brief 定时器1的CC4事件 */
#define AMHW_STM32F103RBT6_ADC3_INJECTED_TIM4_CC3                 2     /**< \brief 定时器4的CC3事件 */
#define AMHW_STM32F103RBT6_ADC3_INJECTED_TIM8_CC2                 3     /**< \brief 定时器8的CC2事件 */
#define AMHW_STM32F103RBT6_ADC3_INJECTED_TIM8_CC4                 4     /**< \brief 定时器8的CC4事件 */
#define AMHW_STM32F103RBT6_ADC3_INJECTED_TIM5_TRGO                5     /**< \brief 定时器5的TRGO事件 */
#define AMHW_STM32F103RBT6_ADC3_INJECTED_TIM5_CC4                 6     /**< \brief 定时器5的CC4事件 */
#define AMHW_STM32F103RBT6_ADC3_INJECTED_JSWSTART                 7     /**< \brief JSWSTART */
/** @}*/

/**
 * \brief ADC channel
 */
typedef enum amhw_stm32f103rbt6_adc_channel {
    AMHW_STM32F103RBT6_ADC_CHAN_0 = 0,
    AMHW_STM32F103RBT6_ADC_CHAN_1,
    AMHW_STM32F103RBT6_ADC_CHAN_2,
    AMHW_STM32F103RBT6_ADC_CHAN_3,
    AMHW_STM32F103RBT6_ADC_CHAN_4,
    AMHW_STM32F103RBT6_ADC_CHAN_5,
    AMHW_STM32F103RBT6_ADC_CHAN_6,
    AMHW_STM32F103RBT6_ADC_CHAN_7,
    AMHW_STM32F103RBT6_ADC_CHAN_8,
    AMHW_STM32F103RBT6_ADC_CHAN_9,
    AMHW_STM32F103RBT6_ADC_CHAN_10,
    AMHW_STM32F103RBT6_ADC_CHAN_11,
    AMHW_STM32F103RBT6_ADC_CHAN_12,
    AMHW_STM32F103RBT6_ADC_CHAN_13,
    AMHW_STM32F103RBT6_ADC_CHAN_14,
    AMHW_STM32F103RBT6_ADC_CHAN_15,
    AMHW_STM32F103RBT6_ADC_CHAN_16,
    AMHW_STM32F103RBT6_ADC_CHAN_17,
} amhw_stm32f103rbt6_adc_channel_t;

/**
 * \brief ADC 双模式选择
 */
typedef enum amhw_stm32f103rbt6_adc_dul_mode {
    AMHW_STM32F103RBT6_ADC_DUL_MODE_0 = 0,    /**< \brief 独立模式 */
    AMHW_STM32F103RBT6_ADC_DUL_MODE_1,        /**< \brief 混合的同步规则+注入同步模式 */
    AMHW_STM32F103RBT6_ADC_DUL_MODE_2,        /**< \brief 混合的同步规则+交替触发模式 */
    AMHW_STM32F103RBT6_ADC_DUL_MODE_3,        /**< \brief 混合同步注入+快速交叉模式 */
    AMHW_STM32F103RBT6_ADC_DUL_MODE_4,        /**< \brief 混合同步注入+慢速交叉模式 */
    AMHW_STM32F103RBT6_ADC_DUL_MODE_5,        /**< \brief 注入同步模式 */
    AMHW_STM32F103RBT6_ADC_DUL_MODE_6,        /**< \brief 规则同步模式 */
    AMHW_STM32F103RBT6_ADC_DUL_MODE_7,        /**< \brief 快速交叉模式 */
    AMHW_STM32F103RBT6_ADC_DUL_MODE_8,        /**< \brief 慢速交叉模式 */
    AMHW_STM32F103RBT6_ADC_DUL_MODE_9,        /**< \brief 交替触发模式狗 */
} amhw_stm32f103rbt6_adc_dul_mode_t;

/**
 * \brief ADC间断模式通道数量
 */
typedef enum amhw_stm32f103rbt6_adc_disc_num {
    AMHW_STM32F103RBT6_ADC_DISC_NUM_1 = 0,    /**< \brief 1个通道 */
    AMHW_STM32F103RBT6_ADC_DISC_NUM_2,        /**< \brief 2个通道 */
    AMHW_STM32F103RBT6_ADC_DISC_NUM_3,        /**< \brief 3个通道 */
    AMHW_STM32F103RBT6_ADC_DISC_NUM_4,        /**< \brief 4个通道 */
    AMHW_STM32F103RBT6_ADC_DISC_NUM_5,        /**< \brief 5个通道 */
    AMHW_STM32F103RBT6_ADC_DISC_NUM_6,        /**< \brief 6个通道 */
    AMHW_STM32F103RBT6_ADC_DISC_NUM_7,        /**< \brief 7个通道 */
    AMHW_STM32F103RBT6_ADC_DISC_NUM_8,        /**< \brief 8个通道 */
} amhw_stm32f103rbt6_adc_disc_num_t;

/**
 * \brief ADC 模拟看门狗通道选择位
 */
typedef enum amhw_stm32f103rbt6_adc_awdg_channel {
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_0 = 0,        /**< \brief ADC模拟输入通道 0 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_1,            /**< \brief ADC模拟输入通道 1 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_2,            /**< \brief ADC模拟输入通道 2 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_3,            /**< \brief ADC模拟输入通道 3 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_4,            /**< \brief ADC模拟输入通道 4 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_5,            /**< \brief ADC模拟输入通道 5 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_6,            /**< \brief ADC模拟输入通道 6 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_7,            /**< \brief ADC模拟输入通道 7 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_8,            /**< \brief ADC模拟输入通道 8 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_9,            /**< \brief ADC模拟输入通道 9 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_10,           /**< \brief ADC模拟输入通道 10 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_11,           /**< \brief ADC模拟输入通道 11 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_12,           /**< \brief ADC模拟输入通道 12 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_13,           /**< \brief ADC模拟输入通道 13 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_14,           /**< \brief ADC模拟输入通道 14 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_15,           /**< \brief ADC模拟输入通道 15 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_16,           /**< \brief ADC模拟输入通道 16 */
    AMHW_STM32F103RBT6_ADC_AWDG_CHAN_17,           /**< \brief ADC模拟输入通道 17 */
} amhw_stm32f103rbt6_adc_awdg_channel_t;

/**
 * \brief ADC sample time
 */
typedef enum amhw_stm32f103rbt6_adc_sample_time {
    AMHW_STM32F103RBT6_ADC_CHAN_ST1_5 = 0,
    AMHW_STM32F103RBT6_ADC_CHAN_ST7_5,
    AMHW_STM32F103RBT6_ADC_CHAN_ST13_5,
    AMHW_STM32F103RBT6_ADC_CHAN_ST28_5,
    AMHW_STM32F103RBT6_ADC_CHAN_ST41_5,
    AMHW_STM32F103RBT6_ADC_CHAN_ST55_5,
    AMHW_STM32F103RBT6_ADC_CHAN_ST71_5,
    AMHW_STM32F103RBT6_ADC_CHAN_ST239_5,
} amhw_stm32f103rbt6_adc_sample_time_t;

/**
 * \brief ADC 注入通道数据偏移设定（1、2、3、4）
 */
typedef enum amhw_stm32f103rbt6_adc_injected_data_channel {
    AMHW_STM32F103RBT6_ADC_INJECTED_DATA_CHAN_1 = 0,
    AMHW_STM32F103RBT6_ADC_INJECTED_DATA_CHAN_2,
    AMHW_STM32F103RBT6_ADC_INJECTED_DATA_CHAN_3,
    AMHW_STM32F103RBT6_ADC_INJECTED_DATA_CHAN_4,
} amhw_stm32f103rbt6_adc_injected_data_channel_t;

/**
 * \brief ADC 规则通道序列长度
 */
typedef enum amhw_stm32f103rbt6_adc_regular_channel_length {
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_1 = 0,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_2 = 1,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_3,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_4,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_5,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_6,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_7,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_8,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_9,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_10,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_11,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_12,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_13,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_14,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_15,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_LENGTH_16,
} amhw_stm32f103rbt6_adc_regular_channel_length_t;

/**
 * \brief ADC Regular channel sequence 次序设置（如，3通道在序列中处于第4个进行转换）
 */
typedef enum amhw_stm32f103rbt6_adc_regular_channel_order {
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_1st = 0,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_2nd = 1,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_3rd,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_4th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_5th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_6th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_7th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_8th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_9th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_10th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_11th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_12th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_13th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_14th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_15th,
    AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_ORDER_16th,
} amhw_stm32f103rbt6_adc_regular_channel_order_t;

/**
 * \brief ADC 注入通道序列长度
 */
typedef enum amhw_stm32f103rbt6_adc_injected_channel_length {
    AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_LENGTH_1 = 0,
    AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_LENGTH_2,
    AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_LENGTH_3,
    AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_LENGTH_4,
} amhw_stm32f103rbt6_adc_injected_channel_length_t;

/**
 * \brief ADC Regular channel sequence 次序设置（如，3通道在序列中处于第4个进行转换）
 */
typedef enum amhw_stm32f103rbt6_adc_injected_channel_order {
    AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_ORDER_1st = 0,
    AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_ORDER_2nd,
    AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_ORDER_3rd,
    AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_ORDER_4th,
} amhw_stm32f103rbt6_adc_injected_channel_order_t;


/**
 * \brief 状态寄存器标志位获取
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_STRT_FLAG    或
 *                            AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_STRT_FLAG   或
 *                            AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_END_FLAG    或
 *                            AMHW_STM32F103RBT6_ADC_END_FLAG                  或
 *                            AMHW_STM32F103RBT6_ADC_ANALOG_WDOG_FLAG
 *
 * \return none
 */
am_static_inline
am_bool_t amhw_stm32f103rbt6_adc_status_flag_check (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint32_t flag)
{
    return (p_hw_adc->sr & (uint32_t)flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 状态寄存器标志位清除
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_STM32F103RBT6_ADC_REGULAR_CHAN_STRT_FLAG    或
 *                             AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_STRT_FLAG   或
 *                             AMHW_STM32F103RBT6_ADC_INJECTED_CHAN_END_FLAG    或
 *                             AMHW_STM32F103RBT6_ADC_END_FLAG                  或
 *                             AMHW_STM32F103RBT6_ADC_ANALOG_WDOG_FLAG
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_status_flag_clr (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint32_t flag)
{
    p_hw_adc->sr &= ~((uint32_t)flag);
}


/**
 * \brief 模拟看门狗设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_STM32F103RBT6_ADC_AWDG_NONE_CHAN       或
 *                             AMHW_STM32F103RBT6_ADC_AWDG_INJECTED_CHAN   或
 *                             AMHW_STM32F103RBT6_ADC_AWDG_REGULAR_CHAN    或
                               AMHW_STM32F103RBT6_ADC_AWDG_REGULAR_CHAN
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_awdg_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint32_t flag)
{
    p_hw_adc->cr1 = (p_hw_adc->cr1 & (~(0x3ul << 22))) | (flag << 22);
}

/**
 * \brief ADC双模式选择
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag     : 传入参数应为   amhw_stm32f103rbt6_adc_dul_mode_t 中的枚举变量
 *
 *    \note : 在 ADC2 和 ADC3 中这些位为保留位。
 *
 *                            在双模式中，改变通道的配置会产生一个重新开始的条件，这将导致同步丢失。
 *                            建议在进行任何配置改变前关闭双模式。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_dul_mode_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, amhw_stm32f103rbt6_adc_dul_mode_t flag)
{
    p_hw_adc->cr1 = (p_hw_adc->cr1 & (~(0xful << 16))) | ((uint32_t)flag << 16);
}



/**
 * \brief ADC间断模式通道数量设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag     : 传入参数应为   amhw_stm32f103rbt6_adc_disc_num_t 中的枚举变量
 *
 *    \note : 软件通过这些位定义在间断模式下，收到外部触发后转换规则通道的数目。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_disc_num_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, amhw_stm32f103rbt6_adc_disc_num_t flag)
{
    p_hw_adc->cr1 = (p_hw_adc->cr1 & (~(0x7ul << 13))) | ((uint32_t)flag << 13);
}

/**
 * \brief JDISCEN 在注入通道上的间断模式禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_injected_disc_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 &= ~(1ul << 12);
}

/**
 * \brief JDISCEN 在注入通道上的间断模式使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_injected_disc_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 |= (1ul << 12);
}

/**
 * \brief DISCEN 在规则通道上的间断模式禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_regular_disc_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 &= ~(1ul << 11);
}

/**
 * \brief DISCEN 在规则通道上的间断模式使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_regular_disc_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 |= (1ul << 11);
}

/**
 * \brief JAUTO 自动注入通道组转换禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或关闭规则通道组转换结束后自动的注入通道组转换
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_jauto_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 &= ~(1ul << 10);
}

/**
 * \brief JAUTO 自动注入通道组转换使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或关闭规则通道组转换结束后自动的注入通道组转换
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_jauto_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 |= (1ul << 10);
}

/**
 * \brief AWDSGL 扫描模式中在一个单一的通道上看门狗禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或关闭由 AWDCH[4:0]位指定的通道上的模拟看门狗功能
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_awdg_single_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 &= ~(1ul << 9);
}

/**
 * \brief AWDSGL 扫描模式中在一个单一的通道上看门狗使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或关闭由 AWDCH[4:0]位指定的通道上的模拟看门狗功能
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_awdg_single_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 |= (1ul << 9);
}

/**
 * \brief 扫描模式禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 在扫描模式中，转换由 ADC_SQRx或 ADC_JSQRx寄存器选中的通道.
 *
 *         如果分别设置了 EOCIE或 JEOCIE位，只在最后一个通道转换完毕后才会产生 EOC或 JEOC中断。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_scan_mode_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 &= ~(1ul << 8);
}

/**
 * \brief 扫描模式使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 在扫描模式中，转换由 ADC_SQRx或 ADC_JSQRx寄存器选中的通道.
 *
 *         如果分别设置了 EOCIE或 JEOCIE位，只在最后一个通道转换完毕后才会产生 EOC或 JEOC中断。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_scan_mode_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr1 |= (1ul << 8);
}

/**
 * \brief ADC中断禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_STM32F103RBT6_ADC_INT_INJECTED_EOC    或
 *                             AMHW_STM32F103RBT6_ADC_INT_AWDG            或
 *                             AMHW_STM32F103RBT6_ADC_INT_EOC
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_int_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint32_t flag)
{
    p_hw_adc->cr1 &= ~flag;
}

/**
 * \brief ADC中断使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_STM32F103RBT6_ADC_INT_INJECTED_EOC    或
 *                            AMHW_STM32F103RBT6_ADC_INT_AWDG            或
 *                            AMHW_STM32F103RBT6_ADC_INT_EOC
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_int_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint32_t flag)
{
    p_hw_adc->cr1 |= flag;
}



/**
 * \brief AWDCH[4:0] ADC模拟看门狗通道选择位
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   amhw_stm32f103rbt6_adc_awdg_channel_t 中的枚举变量
 *
 * \note : 用于选择模拟看门狗保护的输入通道.
 *
 *         ADC1的模拟输入通道 16和通道 17在芯片内部分别连到了温度传感器和 Vrefint.
 *         ADC2的模拟输入通道 16和通道 17在芯片内部连到了 Vss。
 *         ADC3的模拟输入通道 9、14、15、16、17与 Vss相连。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_awdg_channel_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, amhw_stm32f103rbt6_adc_awdg_channel_t flag)
{
    p_hw_adc->cr1 = (p_hw_adc->cr1 & (~(0x1ful << 0))) | (flag << 0);
}

/**
 * \brief TSVREFE 温度传感器和 Vrefint禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或禁止温度传感器和Vrefint通道。在多于1个 ADC的器件中，该位仅出现在 ADC1 中。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_tsvrefe_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 &= ~(1ul << 23);
}

/**
 * \brief TSVREFE 温度传感器和Vrefint使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或禁止温度传感器和Vrefint通道。在多于1个 ADC的器件中，该位仅出现在ADC1中。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_tsvrefe_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 |= (1ul << 23);
}

/**
 * \brief SWSTART 开始转换规 则通道禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 由软件设置该位以启动转换，转换开始后硬件马上清除此位.
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_swstart_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 &= ~(1ul << 22);
}

/**
 * \brief SWSTART 开始转换规 则通道使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 由软件设置该位以启动转换，转换开始后硬件马上清除此位.
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_swstart_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 |= (1ul << 22);
}

/**
 * \brief JSWSTART 开始转换注入通道禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 由软件设置该位以启动转换，软件可清除此位或在转换开始后硬件马上清除此位。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_jswstart_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 &= ~(1ul << 21);
}

/**
 * \brief JSWSTART 开始转换注入通道使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 由软件设置该位以启动转换，软件可清除此位或在转换开始后硬件马上清除此位。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_jswstart_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 |= (1ul << 21);
}

/**
 * \brief JSWSTART 注入通道的外部触发转换模式禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或禁止可以启动规则通道组转换的外部触发事件。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_extirig_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 &= ~(1ul << 20);
}

/**
 * \brief JSWSTART 规则通道的外部触发转换模式使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或禁止可以启动规则通道组转换的外部触发事件。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_extirig_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 |= (1ul << 20);
}

/**
 * \brief JSWSTART 设置启动规则通道组转换的外部事件
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_STM32F103RBT6_ADC12_REGULAR_TIM1_CC1
 *                            .
 *                            .
 *                            .
 *                            AMHW_STM32F103RBT6_ADC12_REGULAR_SWSTART
 *
 *                            或
 *
 *                            AMHW_STM32F103RBT6_ADC3_REGULAR_TIM3_CC1
 *                            .
 *                            .
 *                            .
 *                            AMHW_STM32F103RBT6_ADC3_REGULAR_SWSTART
 *
 * \note : 在adc12中，仅 大 容 量 产 品 具 有TIM8_TRGO功能
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_extsel_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint32_t flag)
{
    p_hw_adc->cr2 = (p_hw_adc->cr2 & (~(0x7ul << 17))) | (flag << 17);
}

/**
 * \brief JSWSTART 注入通道的外部触发转换模式禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或禁止可以启动规则通道组转换的外部触发事件。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_jextirig_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 &= ~(1ul << 15);
}

/**
 * \brief JSWSTART 注入通道的外部触发转换模式使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 用于开启或禁止可以启动规则通道组转换的外部触发事件。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_jextirig_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 |= (1ul << 15);
}

/**
 * \brief JEXTSEL[2:0] 选择启动规则通道组转换的外部事件
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_STM32F103RBT6_ADC12_INJECTED_TIM1_TRGO
 *                            .
 *                            .
 *                            .
 *                            AMHW_STM32F103RBT6_ADC12_INJECTED_JSWSTART
 *
 *                            或
 *
 *                            AMHW_STM32F103RBT6_ADC3_INJECTED_TIM1_TRGO
 *                            .
 *                            .
 *                            .
 *                            AMHW_STM32F103RBT6_ADC3_INJECTED_JSWSTART
 *
 * \note : 在adc12中，仅大容量产品具有 TIM8_CC4
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_jextsel_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint32_t flag)
{
    p_hw_adc->cr2 = (p_hw_adc->cr2 & (~(0x7ul << 12))) | (flag << 12);
}


/**
 * \brief ALIGN 数据对齐方式设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_STM32F103RBT6_ADC_DATA_RIGHT 或
 *                            AMHW_STM32F103RBT6_ADC_DATA_LEFT
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_data_alignment_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint32_t flag)
{
    p_hw_adc->cr2 = (p_hw_adc->cr2 & (~(0x1ul << 11))) | (flag << 11);
}

/**
 * \brief DMA 直接存储器访问模式禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 只有 ADC1和 ADC3能产生 DMA请求。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_dma_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 &= ~(1ul << 8);
}

/**
 * \brief DMA 直接存储器访问模式使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 只有 ADC1和 ADC3能产生 DMA请求。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_dma_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 |= (1ul << 8);
}

/**
 * \brief RSTCAL 复位校准使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除.
 *
 *         如果正在进行转换时设置 RSTCAL，清除校准寄存器需要额外的周期.
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_rstcal_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 |= (1ul << 3);
}

/**
 * \brief RSTCAL 复位校准结束判断
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \return  AM_FALSE：正在校准      ；    AM_TRUE：已校准
 */
am_static_inline
am_bool_t amhw_stm32f103rbt6_adc_rstcal_check (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    return (p_hw_adc->cr2 & (1ul << 3)) ?  AM_FALSE : AM_TRUE;
}

/**
 * \brief CAL AD校准使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 该位由软件设置以开始校准，并在校准结束时由硬件清除。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_cal_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 |= (1ul << 2);
}

/**
 * \brief CAL AD校准结束判断
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \return  AM_FALSE：正在校准      ；    AM_TRUE：已校准
 */
am_static_inline
am_bool_t amhw_stm32f103rbt6_adc_cal_check (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    return (p_hw_adc->cr2 & (1ul << 2)) ?  AM_FALSE : AM_TRUE;
}

/**
 * \brief CONT 单次、连续转换设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_STM32F103RBT6_ADC_CONVERSION_SINGLE      或
 *                             AMHW_STM32F103RBT6_ADC_CONVERSION_CONTINUOUS
 *
 * \note : 如果设置了此位，则转换将连续进行直到该位被清除。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_cont_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint32_t flag)
{
    p_hw_adc->cr2 = (p_hw_adc->cr2 & (~(0x1ul << 1))) | (flag << 1);
}

/**
 * \brief ADON ADC禁能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 当该位为’0’时，写入’1’将把 ADC 从断电模式下唤醒。
 *         当该位为’1’时，写入’1’将启动转换
 *
 *         应用程序需注意，在转换器上电至转换开始有一个延迟 Tstab。
 *
 *         ADC上电延迟一段时间后(tSTAB)，再次设置ADON位时开始进行转换。
 *
 *         如果在这个寄存器中与 ADON 一起还有其他位被改变，则转换不被触发。这是为了防止触发错误的转换。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_disable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 &= ~(1ul << 0);
}

/**
 * \brief ADC使能
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : 当该位为’0’时，写入’1’将把 ADC 从断电模式下唤醒。
 *         当该位为’1’时，写入’1’将启动转换
 *
 *         应用程序需注意，在转换器上电至转换开始有一个延迟 Tstab。
 *
 *         ADC上电延迟一段时间后(tSTAB)，再次设置ADON位时开始进行转换。
 *
 *         如果在这个寄存器中与 ADON 一起还有其他位被改变，则转换不被触发。这是为了防止触发错误的转换。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_enable (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    p_hw_adc->cr2 |= (1ul << 0);
}



/**
 * \brief 某一通道的采样时间设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] time     : 传入参数应为   amhw_stm32f103rbt6_adc_sample_time_t 中的枚举变量
 * \param[in] channel  : 传入参数应为   amhw_stm32f103rbt6_adc_channel_t 中的枚举变量
 *
 * \note : 这些位用于独立地选择每个通道的采样时间。在采样周期中通道选择位必须保持不变。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_smpr_set (amhw_stm32f103rbt6_adc_t             *p_hw_adc,
                               amhw_stm32f103rbt6_adc_sample_time_t  time,
                               amhw_stm32f103rbt6_adc_channel_t      channel)
{
    if((channel / 10) == 1) {

        /* 10~17通道，采样周期设定位于_SMPR1 寄存器 */
        p_hw_adc->smpr[0] = (p_hw_adc->smpr[0] & (~(0x7ul << ((channel - 10) * 3)))) | (time << ((channel - 10) * 3));
    }else {

        /* 0~9通道，采样周期设定位于_SMPR0 寄存器 */
        p_hw_adc->smpr[1] = (p_hw_adc->smpr[1] & (~(0x7ul << (channel * 3)))) | (time << (channel * 3));
    }
}

am_static_inline
uint32_t amhw_stm32f103rbt6_adc_smpr_get (amhw_stm32f103rbt6_adc_t         *p_hw_adc,
                                   amhw_stm32f103rbt6_adc_channel_t  channel)
{
    if((channel / 10) == 1) {

        /* 10~17通道，采样周期设定位于_SMPR1 寄存器 */
        return (p_hw_adc->smpr[1] >> ((channel - 10) * 3)) & 0x7ul ;
    }else {

        /* 0~9通道，采样周期设定位于_SMPR0 寄存器 */
        return (p_hw_adc->smpr[0] >> (channel * 3)) & 0x7ul ;
    }
}



/**
 * \brief 注入通道数据偏移寄存器
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] channel  : 传入参数应为   amhw_stm32f103rbt6_adc_injected_data_channel_t 中的枚举变量
 * \param[in] value    : 数据偏移设定值(0~4095)
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_jofr_set (amhw_stm32f103rbt6_adc_t                       *p_hw_adc,
                               amhw_stm32f103rbt6_adc_injected_data_channel_t  channel,
                               uint16_t                                 value)
{
    p_hw_adc->jofr[channel] = value & (~(0xffful));
}

/**
 * \brief 看门狗高阈值设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] value    : 看门狗上限设定值(0~4095)
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_awdg_htr_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint16_t value)
{
    p_hw_adc->htr = value & (~(0xffful));
}

/**
 * \brief 看门狗低阈值设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] value    : 看门狗下限设定值(0~4095)
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_awdg_ltr_set (amhw_stm32f103rbt6_adc_t *p_hw_adc, uint16_t value)
{
    p_hw_adc->ltr = value & (~(0xffful));
}


/**
 * \brief 规则通道序列长度设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] length   : 传入参数应为   amhw_stm32f103rbt6_adc_regular_channel_length_t 中的枚举变量
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_regular_channel_length_set (amhw_stm32f103rbt6_adc_t                        *p_hw_adc,
                                                 amhw_stm32f103rbt6_adc_regular_channel_length_t  length)
{
    p_hw_adc->sqr[0] = (p_hw_adc->sqr[0] & (~(0xful << 20))) | ((uint32_t)length << 20);
}


/**
 * \brief 规则通道序列中通道设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] order    : 传入参数应为   amhw_stm32f103rbt6_adc_regular_channel_order_t 中的枚举变量
 * \param[in] channel  : 传入参数应为   amhw_stm32f103rbt6_adc_channel_t 中的枚举变量
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_regular_channel_order_set (amhw_stm32f103rbt6_adc_t                       *p_hw_adc,
                                                amhw_stm32f103rbt6_adc_regular_channel_order_t  order,
                                                amhw_stm32f103rbt6_adc_channel_t                channel)
{
    if ((order / 6) == 0) {

        /* 第1~6个转换，在 SQR3  寄存器中设定 */
        p_hw_adc->sqr[2] = (p_hw_adc->sqr[2] & (~(0x1ful << (order * 5)))) | (channel << (order * 5));
    }else if ((order / 6) == 1) {

        /* 第7~12个转换，在 SQR2  寄存器中设定 */
        p_hw_adc->sqr[1] = (p_hw_adc->sqr[1] & (~(0x1ful << ((order - 6) * 5)))) | (channel << ((order - 6) * 5));
    }
    else {

        /* 第13~16个转换，在 SQR1  寄存器中设定 */
        p_hw_adc->sqr[0] = (p_hw_adc->sqr[0] & (~(0x1ful << ((order - 12) * 5)))) | (channel << ((order - 12) * 5));
    }
}


/**
 * \brief 注入通道序列长度设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] length   : 传入参数应为   amhw_stm32f103rbt6_adc_injected_channel_length_t 中的枚举变量
 *
 * \note : 不同于规则转换序列，如果 JL[1:0]的长度小于 4，则转换的序列顺序是从(4-JL)开始。
 *         例如：ADC_JSQR[21:0] =   10    00011   00011   00111  00010
 *                               3个转换            3       3       7      2
 *         意味着扫描转换将按下列通道顺序转换：7、 3、3，而不是 2、7、3。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_injected_channel_length_set (amhw_stm32f103rbt6_adc_t                         *p_hw_adc,
                                                  amhw_stm32f103rbt6_adc_injected_channel_length_t  length)
{
    p_hw_adc->jsqr = (p_hw_adc->jsqr & (~(0x3ul << 20))) | ((uint32_t)length << 20);
}



/**
 * \brief 注入通道序列中通道设置
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] order    : 传入参数应为   amhw_stm32f103rbt6_adc_injected_channel_order_t 中的枚举变量
 * \param[in] channel  : 传入参数应为   amhw_stm32f103rbt6_adc_channel_t 中的枚举变量
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_adc_injected_channel_order_set (amhw_stm32f103rbt6_adc_t                        *p_hw_adc,
                                                 amhw_stm32f103rbt6_adc_injected_channel_order_t  order,
                                                 amhw_stm32f103rbt6_adc_channel_t                 channel)
{
    p_hw_adc->jsqr = (p_hw_adc->jsqr & (~(0x1ful << (order * 5)))) | (channel << (order * 5));
}

/**
 * \brief 注入通道adc数据获取
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 * \param[in] channel  : 传入参数应为   amhw_stm32f103rbt6_adc_injected_data_channel_t 中的枚举变量
 *
 * \return 注入通道的转换结果 (数据左对齐或右对齐)
 */
am_static_inline
uint16_t amhw_stm32f103rbt6_adc_injected_data_get (amhw_stm32f103rbt6_adc_t                       *p_hw_adc,
                                            amhw_stm32f103rbt6_adc_injected_data_channel_t  channel)
{
    return p_hw_adc->jdr[channel];
}

/**
 * \brief 规则通道adc数据获取
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \return 规则通道的转换结果 (数据左对齐或右对齐)
 */
am_static_inline
uint16_t amhw_stm32f103rbt6_adc_regular_data_get (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    return (p_hw_adc->dr & 0xffff);
}

/**
 * \brief 双模式下，规则通道adc1数据获取
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : [31:16]  在 ADC1 中：双模式下，这些位包含了 ADC2转换的规则通道数据。
 *         [31:16]  在 ADC2和 ADC3中：不使用这些位。
 *
 * \return 规则通道的转换结果 (数据左对齐或右对齐)
 */
am_static_inline
uint16_t amhw_stm32f103rbt6_adc_regular_dul_mode_adc1_data_get (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    return (p_hw_adc->dr & 0xffff);
}

/**
 * \brief 双模式下，规则通道adc2数据获取
 * \param[in] p_hw_adc : 指向ADC结构体的指针
 *
 * \note : [31:16]  在 ADC1 中：双模式下，这些位包含了 ADC2转换的规则通道数据。
 *         [31:16]  在 ADC2和 ADC3中：不使用这些位。
 *
 * \return 规则通道的转换结果 (数据左对齐或右对齐)
 */
am_static_inline
uint16_t amhw_stm32f103rbt6_adc_regular_dul_mode_adc2_data_get (amhw_stm32f103rbt6_adc_t *p_hw_adc)
{
    return ((p_hw_adc->dr >> 16) & 0xffff);
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

#endif /* __AMHW_STM32F103RBT6_ADC_H */

/* end of file */
