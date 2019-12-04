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
 * \brief ZLG GPIO 引脚配置参数定义
 *
 * \note: 配置引脚功能的时候一定要同时配置引脚的模式，如上下拉模式、推挽模式等
 *
 * \internal
 * \par Modification History
 * - 1.00 19-07-3  zc, first implementation
 * \endinternal
 */

#ifndef __HK32F103RBT6_PIN_H
#define __HK32F103RBT6_PIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_hk32f103rbt6_gpio_util.h"

 /**
 * \addtogroup hk32f103rbt6_if_pin
 * \copydoc hk32f103rbt6_pin.h
 * @{
 */

/**
 * \name HK32F103RBT6 GPIO 引脚编号
 * \anchor grp_gpio_pin_number
 * @{
 */

#define PIOA            0         /**< \brief 端口A引脚起始号 */
#define PIOA_0          0         /**< \brief PIOA_0 引脚号 */
#define PIOA_1          1         /**< \brief PIOA_1 引脚号 */
#define PIOA_2          2         /**< \brief PIOA_2 引脚号 */
#define PIOA_3          3         /**< \brief PIOA_3 引脚号 */
#define PIOA_4          4         /**< \brief PIOA_4 引脚号 */
#define PIOA_5          5         /**< \brief PIOA_5 引脚号 */
#define PIOA_6          6         /**< \brief PIOA_6 引脚号 */
#define PIOA_7          7         /**< \brief PIOA_7 引脚号 */
#define PIOA_8          8         /**< \brief PIOA_8 引脚号 */
#define PIOA_9          9         /**< \brief PIOA_9 引脚号 */
#define PIOA_10         10        /**< \brief PIOA_10 引脚号 */
#define PIOA_11         11        /**< \brief PIOA_11 引脚号 */
#define PIOA_12         12        /**< \brief PIOA_13 引脚号 */
#define PIOA_13         13        /**< \brief PIOA_13 引脚号 */
#define PIOA_14         14        /**< \brief PIOA_14 引脚号 */
#define PIOA_15         15        /**< \brief PIOA_15 引脚号 */

#define PIOB            16        /**< \brief 端口B引脚起始号 */
#define PIOB_0          16        /**< \brief PIOB_8 引脚号 */
#define PIOB_1          17        /**< \brief PIOB_1 引脚号 */
#define PIOB_2          18        /**< \brief PIOB_2 引脚号 */
#define PIOB_3          19        /**< \brief PIOB_3 引脚号 */
#define PIOB_4          20        /**< \brief PIOB_4 引脚号 */
#define PIOB_5          21        /**< \brief PIOB_5 引脚号 */
#define PIOB_6          22        /**< \brief PIOB_3 引脚号 */
#define PIOB_7          23        /**< \brief PIOB_4 引脚号 */
#define PIOB_8          24        /**< \brief PIOB_5 引脚号 */
#define PIOB_9          25        /**< \brief PIOB_5 引脚号 */
#define PIOB_10         26        /**< \brief PIOB_10引脚号 */
#define PIOB_11         27        /**< \brief PIOB_11引脚号 */
#define PIOB_12         28        /**< \brief PIOB_12引脚号 */
#define PIOB_13         29        /**< \brief PIOB_13引脚号 */
#define PIOB_14         30        /**< \brief PIOB_14引脚号 */
#define PIOB_15         31        /**< \brief PIOB_15引脚号 */

#define PIOC            32        /**< \brief 端口C引脚起始号 */
#define PIOC_0          32        /**< \brief PIOC_13引脚号 */
#define PIOC_1          33        /**< \brief PIOC_13引脚号 */
#define PIOC_2          34        /**< \brief PIOC_13引脚号 */
#define PIOC_3          35        /**< \brief PIOC_13引脚号 */
#define PIOC_4          36        /**< \brief PIOC_13引脚号 */
#define PIOC_5          37        /**< \brief PIOC_13引脚号 */
#define PIOC_6          38        /**< \brief PIOC_13引脚号 */
#define PIOC_7          39        /**< \brief PIOC_13引脚号 */
#define PIOC_8          40        /**< \brief PIOC_13引脚号 */
#define PIOC_9          41        /**< \brief PIOC_13引脚号 */
#define PIOC_10         42        /**< \brief PIOC_13引脚号 */
#define PIOC_11         43        /**< \brief PIOC_13引脚号 */
#define PIOC_12         44        /**< \brief PIOC_13引脚号 */
#define PIOC_13         45        /**< \brief PIOC_13引脚号 */
#define PIOC_14         46        /**< \brief PIOC_14引脚号 */
#define PIOC_15         47        /**< \brief PIOC_15引脚号 */

#define PIOD            48        /**< \brief 端口D引脚起始号 */
#define PIOD_0          48        /**< \brief PIOD_0引脚号 */
#define PIOD_1          49        /**< \brief PIOD_1引脚号 */
#define PIOD_2          50        /**< \brief PIOD_2引脚号 */

/**
 * \brief 引脚数目为51
 */
#define PIN_NUM         51

/**
 * \brief 支持同时发生最大引脚中断数是16
 */
#define PIN_INT_MAX     16

/** @} */

/**
 * \name HK32F103RBT6 引脚定义,除调试引脚SWCK、SWIO之外，所有引脚复位时都默认为GPIO功能(浮空输入)
 * @{
 */

/******************************************************************************/
/**
 * \brief TIM5CH4 与 LSI 相连   ADC的规则和注入转换宏

 */

#define TIM5_CH4_LSI_REMAP1                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x10, 1)   /**< \brief TIM5_CH4_LSI */
#define ADC1_ETRGINJ_REMAP0                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x11, 0)   /**< \brief EXTI15 */
#define ADC1_ETRGINJ_REMAP1                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x11, 1)   /**< \brief TIM8_CH4 */
#define ADC1_ETRGREG_REMAP0                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x12, 0)   /**< \brief EXTI11 */
#define ADC1_ETRGREG_REMAP1                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x12, 1)   /**< \brief TIM8_TRGO */
#define ADC2_ETRGINJ_REMAP0                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x13, 0)   /**< \brief EXTI15 */
#define ADC2_ETRGINJ_REMAP1                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x13, 1)   /**< \brief TIM8_CH4 */
#define ADC2_ETRGREG_REMAP0                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x14, 0)   /**< \brief EXTI11 */
#define ADC2_ETRGREG_REMAP1                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x14, 1)   /**< \brief TIM8_TRGO */
/**
 * \name PIOA_0引脚复用功能
 * @{
 */
#define PIOA_0_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_0_ADC12_IN0             PIOA_0_NO_REMAP   /**< \brief ADC12_IN0 */
#define PIOA_0_WKUP                  PIOA_0_NO_REMAP   /**< \brief WAKE_UP */
#define PIOA_0_GPIO                  PIOA_0_NO_REMAP   /**< \brief GPIO */

#define PIOA_0_USART2_CTS_REMAP0     AM_HK32F103RBT6_GPIO_REMAP_CODE(0x03, 0)   /**< \brief USART2_CTS */
#define PIOA_0_TIM2_CH1_ETR_REMAP0   AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 0)   /**< \brief TIM2_CH1_ETR */
#define PIOA_0_TIM2_CH1_ETR_REMAP2   AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 2)   /**< \brief TIM2_CH1_ETR */

/** @} */

/**
 * \name PIOA_0引脚复用时功能模式
 * @{
 */

#define PIOA_0_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_0_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_0_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_0_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_0_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_0_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_0_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_0_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_0引脚响应速率
 * @{
 */

#define PIOA_0_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_0_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_0_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_1引脚复用功能
 * @{
 */
#define PIOA_1_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_1_ADC12_IN1             PIOA_1_NO_REMAP   /**< \brief ADC12_IN1 */
#define PIOA_1_GPIO                  PIOA_1_NO_REMAP   /**< \brief GPIO */

#define PIOA_1_TIM2_CH2_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 0)   /**< \brief TIM2_CH2 */
#define PIOA_1_TIM2_CH2_REMAP2       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 2)   /**< \brief TIM2_CH2 */
#define PIOA_1_USART2_RTS_REMAP0     AM_HK32F103RBT6_GPIO_REMAP_CODE(0x03, 0)   /**< \brief USART2_RTS */

/** @} */

/**
 * \name PIOA_1引脚复用时功能模式
 * @{
 */

#define PIOA_1_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_1_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_1_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_1_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_1_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_1_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_1_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_1_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_1引脚响应速率
 * @{
 */

#define PIOA_1_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_1_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_1_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_2引脚复用功能
 * @{
 */
#define PIOA_2_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_2_ADC12_IN2             PIOA_2_NO_REMAP   /**< \brief ADC12_IN2 */
#define PIOA_2_GPIO                  PIOA_2_NO_REMAP   /**< \brief GPIO */

#define PIOA_2_TIM2_CH3_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 0)   /**< \brief TIM2_CH3 */
#define PIOA_2_TIM2_CH3_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 1)   /**< \brief TIM2_CH3 */
#define PIOA_2_USART2_TX_REMAP0      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x03, 0)   /**< \brief USART2_RTS */


/** @} */

/**
 * \name PIOA_2引脚复用时功能模式
 * @{
 */

#define PIOA_2_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_2_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_2_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_2_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_2_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_2_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_2_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_2_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_2引脚响应速率
 * @{
 */

#define PIOA_2_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_2_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_2_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_3引脚复用功能
 * @{
 */
#define PIOA_3_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_3_ADC12_IN3             PIOA_3_NO_REMAP   /**< \brief ADC12_IN3 */
#define PIOA_3_GPIO                  PIOA_3_NO_REMAP   /**< \brief GPIO */

#define PIOA_3_TIM2_CH4_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 0)   /**< \brief TIM2_CH4 */
#define PIOA_3_TIM2_CH4_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 1)   /**< \brief TIM2_CH4 */
#define PIOA_3_USART2_RX_REMAP0      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x03, 0)   /**< \brief USART2_RX */
#define PIOA_3_TIM5_CH4_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x10, 0)   /**< \brief TIM5_CH4 */


/** @} */

/**
 * \name PIOA_3引脚复用时功能模式
 * @{
 */

#define PIOA_3_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_3_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_3_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_3_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_3_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_3_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_3_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_3_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_3引脚响应速率
 * @{
 */

#define PIOA_3_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_3_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_3_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_4引脚复用功能
 * @{
 */
#define PIOA_4_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_4_ADC12_IN4             PIOA_4_NO_REMAP    /**< \brief ADC12_IN4 */
#define PIOA_4_DAC1_OUT              PIOA_4_NO_REMAP    /**< \brief DAC1_OUT */
#define PIOA_4_GPIO                  PIOA_4_NO_REMAP    /**< \brief GPIO */

#define PIOA_4_SPI1_NSS              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x00, 0)     /**< \brief SPI1_NSS */
#define PIOA_4_USART2_CK_REMAP0      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x03, 0)     /**< \brief USART2_CK */

/** @} */

/**
 * \name PIOA_4引脚复用时功能模式
 * @{
 */

#define PIOA_4_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_4_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_4_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_4_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_4_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_4_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_4_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_4_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_4引脚响应速率
 * @{
 */

#define PIOA_4_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_4_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_4_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_5引脚复用功能
 * @{
 */
#define PIOA_5_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_5_ADC12_IN5             PIOA_5_NO_REMAP   /**< \brief ADC12_IN5 */
#define PIOA_5_DAC2_OUT              PIOA_5_NO_REMAP   /**< \brief DAC2_OUT */
#define PIOA_5_GPIO                  PIOA_5_NO_REMAP   /**< \brief GPIO */

#define PIOA_5_SPI1_SCK_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x00, 0)   /**< \brief SPI1_SCK */

/** @} */

/**
 * \name PIOA_5引脚复用时功能模式
 * @{
 */

#define PIOA_5_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_5_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_5_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_5_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_5_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_5_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_5_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_5_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_5引脚响应速率
 * @{
 */

#define PIOA_5_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_5_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_5_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_6引脚复用功能
 * @{
 */
#define PIOA_6_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_6_ADC12_IN6             PIOA_6_NO_REMAP   /**< \brief ADC12_IN6 */
#define PIOA_6_GPIO                  PIOA_6_NO_REMAP   /**< \brief GPIO */

#define PIOA_6_SPI1_MISO_REMAP0      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x00, 0)   /**< \brief SPI1_MISO */
#define PIOA_6_TIM3_CH1_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 0)   /**< \brief TIM3_CH3 */
#define PIOA_6_TIM1_BAKIN_REMAP1      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 1)   /**< \brief TIM1_BAKIN */

/** @} */

/**
 * \name PIOA_6引脚复用时功能模式
 * @{
 */

#define PIOA_6_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_6_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_6_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_6_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_6_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_6_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_6_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_6_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_6引脚响应速率
 * @{
 */

#define PIOA_6_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_6_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_6_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_7引脚复用功能
 * @{
 */
#define PIOA_7_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_7_ADC12_IN7             PIOA_7_NO_REMAP   /**< \brief ADC12_IN7 */
#define PIOA_7_GPIO                  PIOA_7_NO_REMAP   /**< \brief GPIO */

#define PIOA_7_SPI1_MOSI_REMAP0      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x00, 0)   /**< \brief SPI1_MOSI */
#define PIOA_7_TIM3_CH2_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 0)   /**< \brief TIM3_CH2 */
#define PIOA_7_TIM1_CH1N_REMAP1      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 1)   /**< \brief TIM1_CH1N */
/** @} */

/**
 * \name PIOA_7引脚复用时功能模式
 * @{
 */

#define PIOA_7_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_7_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_7_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_7_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_7_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_7_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_7_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_7_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_7引脚响应速率
 * @{
 */

#define PIOA_7_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_7_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_7_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_8引脚复用功能
 * @{
 */
#define PIOA_8_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_8_MCO                   PIOA_8_NO_REMAP   /**< \brief MCO */
#define PIOA_8_GPIO                  PIOA_8_NO_REMAP   /**< \brief GPIO */

#define PIOA_8_TIM1_CH1_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 0)   /**< \brief TIM1_CH1 */
#define PIOA_8_TIM1_CH1_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 1)   /**< \brief TIM1_CH1 */

/** @} */

/**
 * \name PIOA_8引脚复用时功能模式
 * @{
 */

#define PIOA_8_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_8_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_8_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_8_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_8_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_8_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_8_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_8_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_8引脚响应速率
 * @{
 */

#define PIOA_8_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_8_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_8_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_9引脚复用功能
 * @{
 */
#define PIOA_9_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_9_GPIO                  PIOA_9_NO_REMAP   /**< \brief GPIO */

#define PIOA_9_USART1_TX_REMAP0      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x02, 0)   /**< \brief USART1_TX */
#define PIOA_9_TIM1_CH2_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 0)   /**< \brief TIM1_CH2 */
#define PIOA_9_TIM1_CH2_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 1)   /**< \brief TIM1_CH2 */

/** @} */

/**
 * \name PIOA_9引脚复用时功能模式
 * @{
 */

#define PIOA_9_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_9_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_9_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_9_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_9_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_9_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_9_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_9_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_9引脚响应速率
 * @{
 */

#define PIOA_9_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_9_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_9_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_10引脚复用功能
 * @{
 */
#define PIOA_10_NO_REMAP             AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_10_GPIO                 PIOA_10_NO_REMAP   /**< \brief GPIO */

#define PIOA_10_USART1_RX_REMAP0     AM_HK32F103RBT6_GPIO_REMAP_CODE(0x02, 0)   /**< \brief USART1_RX */
#define PIOA_10_TIM1_CH3_REMAP0      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 0)   /**< \brief TIM1_CH3 */
#define PIOA_10_TIM1_CH3_REMAP1      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 1)   /**< \brief TIM1_CH3 */
/** @} */

/**
 * \name PIOA_10引脚复用时功能模式
 * @{
 */

#define PIOA_10_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_10_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_10_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_10_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_10_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_10_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_10_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_10_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_10引脚响应速率
 * @{
 */

#define PIOA_10_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_10_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_10_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_11引脚复用功能
 * @{
 */
#define PIOA_11_NO_REMAP             AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_11_USART1_CTS           PIOA_11_NO_REMAP   /**< \brief USART1_CTS */
#define PIOA_11_USBDM                PIOA_11_NO_REMAP   /**< \brief USBDM */
#define PIOA_11_GPIO                 PIOA_11_NO_REMAP   /**< \brief GPIO */

#define PIOA_11_TIM1_CH4_REMAP0      AM_HK32F103RBT6_GPIO_REMAP_CODE(0X06, 0)  /**< \brief TIM1_CH4 */
#define PIOA_11_TIM1_CH4_REMAP1      AM_HK32F103RBT6_GPIO_REMAP_CODE(0X06, 1)  /**< \brief TIM1_CH4 */
#define PIOA_11_CAN_RX_REMAP0        AM_HK32F103RBT6_GPIO_REMAP_CODE(0X0D, 0)  /**< \brief CAN_RX */
/** @} */

/**
 * \name PIOA_11引脚复用时功能模式
 * @{
 */

#define PIOA_11_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_11_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_11_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_11_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_11_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_11_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_11_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_11_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_11引脚响应速率
 * @{
 */

#define PIOA_11_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_11_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_11_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_12引脚复用功能
 * @{
 */
#define PIOA_12_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_12_USART1_RTS            PIOA_12_NO_REMAP   /**< \brief USART1_RTS */
#define PIOA_12_USBDP                 PIOA_12_NO_REMAP
#define PIOA_12_GPIO                  PIOA_12_NO_REMAP   /**< \brief GPIO */

#define PIOA_12_CAN_TX_REMAP0         AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0D, 0)   /**< \brief CAN_TX */
#define PIOA_12_TIM1_ETR_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 0)   /**< \brief TIM1_ETR */
#define PIOA_12_TIM1_ETR_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 1)   /**< \brief TIM1_ETR */

/** @} */

/**
 * \name PIOA_12引脚复用时功能模式
 * @{
 */

#define PIOA_12_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_12_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_12_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_12_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_12_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_12_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_12_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_12_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_12引脚响应速率
 * @{
 */

#define PIOA_12_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_12_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_12_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_13引脚复用功能
 * @{
 */
#define PIOA_13_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_13_GPIO                  PIOA_13_NO_REMAP   /**< \brief GPIO */

#define PIOA_13_SWDIO                 AM_HK32F103RBT6_GPIO_REMAP_CODE(0x18, 0)   /**< \brief SWDIO */
/** @} */

/**
 * \name PIOA_13引脚复用时功能模式
 * @{
 */

#define PIOA_13_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_13_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_13_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_13_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_13_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_13_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_13_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_13_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_13引脚响应速率
 * @{
 */

#define PIOA_13_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_13_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_13_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_14引脚复用功能
 * @{
 */
#define PIOA_14_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_14_GPIO                  PIOA_14_NO_REMAP   /**< \brief GPIO */

#define PIOA_14_SWDCLK                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x18, 0)   /**< \brief SWDCLK */
/** @} */

/**
 * \name PIOA_14引脚复用时功能模式
 * @{
 */

#define PIOA_14_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_14_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_14_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_14_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_14_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_14_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_14_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_14_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_14引脚响应速率
 * @{
 */

#define PIOA_14_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_14_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_14_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOA_15引脚复用功能
 * @{
 */
#define PIOA_15_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOA_15_GPIO                  PIOA_15_NO_REMAP   /**< \brief GPIO */

#define PIOA_15_JTDI                  AM_HK32F103RBT6_GPIO_REMAP_CODE(0x18, 0)   /**< \brief JTDI */
#define PIOA_15_TIM2_CH1_ETR_REMAP1   AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 1)   /**< \brief TIM2_CH1_ETR */
#define PIOA_15_TIM2_CH1_ETR_REMAP3   AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 3)   /**< \brief TIM2_CH1_ETR */
#define PIOA_15_SPI1_NSS              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x00, 1)   /**< \brief SPI1_NSS */
/** @} */

/**
 * \name PIOA_15引脚复用时功能模式
 * @{
 */

#define PIOA_15_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOA_15_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOA_15_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOA_15_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOA_15_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOA_15_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOA_15_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOA_15_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOA_15引脚响应速率
 * @{
 */

#define PIOA_15_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOA_15_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOA_15_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_0引脚复用功能
 * @{
 */
#define PIOB_0_NO_REMAP               AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_0_ADC12_IN8              PIOB_0_NO_REMAP   /**< \brief ADC12_IN8 */
#define PIOB_0_GPIO                   PIOB_0_NO_REMAP   /**< \brief GPIO */

#define PIOB_0_TIM3_CH3_REMAP0        AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 0)   /**< \brief TIM3_CH3 */
#define PIOB_0_TIM3_CH3_REMAP2        AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 2)   /**< \brief TIM3_CH3 */
#define PIOB_0_TIM1_CH2N_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0X06, 1)   /**< \brief TIM1_CH2N */
/** @} */

/**
 * \name PIOB_8引脚复用时功能模式
 * @{
 */

#define PIOB_0_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_0_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOB_0_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_0_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_0_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_0_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_0_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_0_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_0引脚响应速率
 * @{
 */

#define PIOB_0_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_0_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_0_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_1引脚复用功能
 * @{
 */
#define PIOB_1_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_1_ADC12_IN9             PIOB_1_NO_REMAP   /**< \brief ADC12_IN9 */
#define PIOB_1_GPIO                  PIOB_1_NO_REMAP   /**< \brief GPIO */

#define PIOB_1_TIM3_CH4_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 0)   /**< \brief TIM3_CH4 */
#define PIOB_1_TIM3_CH4_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 2)   /**< \brief TIM3_CH4 */
#define PIOB_1_TIM1_CH3N_REMAP1      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 1)   /**< \brief TIM1_CH3N */

/** @} */

/**
 * \name PIOB_1引脚复用时功能模式
 * @{
 */

#define PIOB_1_AIN                  AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_1_INPUT_FLOAT          AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOB_1_INPUT_PD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_1_INPUT_PU             AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_1_OUT_PP               AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_1_OUT_OD               AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_1_AF_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_1_AF_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_1引脚响应速率
 * @{
 */

#define PIOB_1_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_1_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_1_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_2引脚复用功能
 * @{
 */
#define PIOB_2_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_2_GPIO                  PIOB_2_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOB_2引脚复用时功能模式
 * @{
 */

#define PIOB_2_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_2_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOB_2_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_2_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_2_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_2_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_2_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_2_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_2引脚响应速率
 * @{
 */

#define PIOB_2_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_2_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_2_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_3引脚复用功能
 * @{
 */
#define PIOB_3_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_3_GPIO                  PIOB_3_NO_REMAP   /**< \brief GPIO */

#define PIOB_3_TRACESWO              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x18, 0)   /**< \brief TRACESWO */
#define PIOB_3_TIM2_CH2_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 1)   /**< \brief TIM2_CH2 */
#define PIOB_3_TIM2_CH2_REMAP3       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 3)   /**< \brief TIM2_CH2 */
#define PIOB_3_SPI1_SCK_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x00, 1)   /**< \brief SPI1_SCK */

/** @} */

/**
 * \name PIOB_3引脚复用时功能模式
 * @{
 */

#define PIOB_3_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_3_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOB_3_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_3_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_3_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_3_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_3_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_3_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_3引脚响应速率
 * @{
 */

#define PIOB_3_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_3_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_3_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_4引脚复用功能
 * @{
 */
#define PIOB_4_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_4_GPIO                  PIOB_4_NO_REMAP   /**< \brief GPIO */

#define PIOB_4_TIM3_CH1_REMAP2       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 2)   /**< \brief TIM3_CH1 */
#define PIOB_4_SPI1_MISO_REMAP1      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x00, 1)   /**< \brief SPI1_MISO */
#define PIOB_4_JNTRST                AM_HK32F103RBT6_GPIO_REMAP_CODE(0x18, 0)   /**< \brief JNTRST */
/** @} */

/**
 * \name PIOB_4引脚复用时功能模式
 * @{
 */

#define PIOB_4_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_4_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOB_4_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_4_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_4_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_4_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_4_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_4_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_4引脚响应速率
 * @{
 */

#define PIOB_4_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_4_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_4_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_5引脚复用功能
 * @{
 */

#define PIOB_5_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_5_GPIO                  PIOB_5_NO_REMAP   /**< \brief GPIO */
#define PIOB_5_I2C1_SMBAI            PIOB_5_NO_REMAP   /**< \brief I2C1_SMBAI */

#define PIOB_5_TIM3_CH2_REMAP2       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 2)   /**< \brief TIM3_CH2 */
#define PIOB_5_SPI1_MOSI_REMAP1      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x00, 1)   /**< \brief SPI1_MOSI */

/** @} */

/**
 * \name PIOB_5引脚复用时功能模式
 * @{
 */

#define PIOB_5_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_5_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOB_5_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_5_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_5_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_5_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_5_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_5_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_5引脚响应速率
 * @{
 */

#define PIOB_5_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_5_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_5_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_6引脚复用功能
 * @{
 */

#define PIOB_6_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_6_GPIO                  PIOB_6_NO_REMAP   /**< \brief GPIO */
#define PIOB_6_TIM4_CH1              PIOB_6_NO_REMAP   /**< \brief TIM4_CH1 */

#define PIOB_6_I2C1_SCL_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x01, 0)   /**< \brief I2C1_SCL */
#define PIOB_6_USART1_TX_REMAP1      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x02, 1)   /**< \brief USART1_TX */
#define PIOB_6_TIM4_CH1_REMAP        AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0C, 0)   /**< \brief TIM4_CH1 */

/** @} */

/**
 * \name PIOB_6引脚复用时功能模式
 * @{
 */

#define PIOB_6_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_6_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOB_6_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_6_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_6_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_6_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_6_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_6_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_6引脚响应速率
 * @{
 */

#define PIOB_6_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_6_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_6_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_7引脚复用功能
 * @{
 */

#define PIOB_7_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_7_TIM4_CH2              PIOB_7_NO_REMAP   /**< \brief TIM4_CH2 */
#define PIOB_7_GPIO                  PIOB_7_NO_REMAP   /**< \brief GPIO */

#define PIOB_7_I2C1_SDA_REMAP0       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x01, 0)   /**< \brief I2C1_SDA */
#define PIOB_7_USART1_RX_REMAP1      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x02, 1)   /**< \brief USART1_RX */
#define PIOB_7_TIM4_CH2_REMAP        AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0C, 0)   /**< \brief TIM4_CH2 */

/** @} */

/**
 * \name PIOB_7管脚复用时引脚复用时功能模式
 * @{
 */

#define PIOB_7_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_7_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式(默认) */
#define PIOB_7_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_7_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_7_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_7_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_7_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_7_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_7引脚响应速率
 * @{
 */

#define PIOB_7_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_7_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_7_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_8引脚复用功能
 * @{
 */

#define PIOB_8_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_8_TIM4_CH3              PIOB_8_NO_REMAP   /**< \brief TIM4_CH3 */
#define PIOB_8_GPIO                  PIOB_8_NO_REMAP   /**< \brief GPIO */

#define PIOB_8_I2C1_SCL_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x01, 1)   /**< \brief I2C1_SCL */
#define PIOB_8_CAN_RX_REMAP2         AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0D, 2)   /**< \brief CAN_RX */
#define PIOB_8_TIM4_CH3_REMAP        AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0C, 0)   /**< \brief TIM4_CH3 */

/** @} */

/**
 * \name PIOB_8引脚复用时功能模式
 * @{
 */

#define PIOB_8_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_8_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOB_8_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_8_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_8_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_8_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_8_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_8_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_8引脚响应速率
 * @{
 */

#define PIOB_8_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_8_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_8_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_9引脚复用功能
 * @{
 */

#define PIOB_9_NO_REMAP              AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_9_TIM4_CH4              PIOB_9_NO_REMAP   /**< \brief TIM4_CH4 */
#define PIOB_9_GPIO                  PIOB_9_NO_REMAP   /**< \brief GPIO */

#define PIOB_9_I2C1_SDA_REMAP1       AM_HK32F103RBT6_GPIO_REMAP_CODE(0x01, 1)   /**< \brief I2C1_SDA */
#define PIOB_9_CAN_TX_REMAP2         AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0D, 2)   /**< \brief CAN_TX */
#define PIOB_9_TIM4_CH4_REMAP        AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0C, 0)   /**< \brief TIM4_CH4 */

/** @} */

/**
 * \name PIOB_9引脚复用时功能模式
 * @{
 */

#define PIOB_9_AIN                   AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_9_INPUT_FLOAT           AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOB_9_INPUT_PD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_9_INPUT_PU              AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_9_OUT_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_9_OUT_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_9_AF_PP                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_9_AF_OD                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_9引脚响应速率
 * @{
 */

#define PIOB_9_SPEED_10MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_9_SPEED_2MHz            AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_9_SPEED_50MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_10引脚复用功能
 * @{
 */

#define PIOB_10_NO_REMAP             AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_10_I2C2_SCL             PIOB_10_NO_REMAP   /**< \brief I2C2_SCL */
#define PIOB_10_GPIO                 PIOB_10_NO_REMAP   /**< \brief GPIO */

#define PIOB_10_USART3_TX_REMAP0     AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 0)   /**< \brief USART3_TX */
#define PIOB_10_TIM2_CH3_REMAP2      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 2)   /**< \brief TIM2_CH3 */
#define PIOB_10_TIM2_CH3_REMAP3      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 3)   /**< \brief TIM2_CH3 */

/** @} */

/**
 * \name PIOB_10引脚复用时功能模式
 * @{
 */

#define PIOB_10_AIN                  AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_10_INPUT_FLOAT          AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOB_10_INPUT_PD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_10_INPUT_PU             AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_10_OUT_PP               AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_10_OUT_OD               AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_10_AF_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_10_AF_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_10引脚响应速率
 * @{
 */

#define PIOB_10_SPEED_10MHz          AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_10_SPEED_2MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_10_SPEED_50MHz          AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_11引脚复用功能
 * @{
 */

#define PIOB_11_NO_REMAP             AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_11_I2C2_SDA             PIOB_11_NO_REMAP   /**< \brief I2C2_SDA */
#define PIOB_11_GPIO                 PIOB_11_NO_REMAP   /**< \brief GPIO */

#define PIOB_11_USART3_RX_REMAP0     AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 0)   /**< \brief USART3_RX */
#define PIOB_11_TIM2_CH4_REMAP2      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 2)   /**< \brief TIM2_CH4 */
#define PIOB_11_TIM2_CH4_REMAP3      AM_HK32F103RBT6_GPIO_REMAP_CODE(0x08, 3)   /**< \brief TIM2_CH4 */

/** @} */

/**
 * \name PIOB_11引脚复用时功能模式
 * @{
 */

#define PIOB_11_AIN                  AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_11_INPUT_FLOAT          AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOB_11_INPUT_PD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_11_INPUT_PU             AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_11_OUT_PP               AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_11_OUT_OD               AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_11_AF_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_11_AF_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_11引脚响应速率
 * @{
 */

#define PIOB_11_SPEED_10MHz          AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_11_SPEED_2MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_11_SPEED_50MHz          AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_12引脚复用功能
 * @{
 */

#define PIOB_12_NO_REMAP             AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_12_SPI2_NSS             PIOB_12_NO_REMAP   /**< \brief SPI2_NSS */
#define PIOB_12_I2C2_SMBAI           PIOB_12_NO_REMAP   /**< \brief I2C2_SMBAI */
#define PIOB_12_GPIO                 PIOB_12_NO_REMAP   /**< \brief GPIO */

#define PIOB_12_TIM1_BAKIN_REMAP0    AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 0)   /**< \brief TIM1_BAKIN */
#define PIOB_12_USART3_CK_REMAP0     AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 0)   /**< \brief USART3_CK */

/** @} */

/**
 * \name PIOB_12引脚复用时功能模式
 * @{
 */

#define PIOB_12_AIN                  AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_12_INPUT_FLOAT          AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOB_12_INPUT_PD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_12_INPUT_PU             AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_12_OUT_PP               AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_12_OUT_OD               AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_12_AF_PP                AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_12_AF_OD                AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_12引脚响应速率
 *
 * @{
 */

#define PIOB_12_SPEED_10MHz          AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_12_SPEED_2MHz           AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_12_SPEED_50MHz          AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_13引脚复用功能
 * @{
 */

#define PIOB_13_NO_REMAP            AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_13_SPI2_SCK            PIOB_13_NO_REMAP   /**< \brief SPI2_SCK */
#define PIOB_13_GPIO                PIOB_13_NO_REMAP   /**< \brief GPIO */

#define PIOB_13_TIM1_CH1N_REMAP0    AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 0)   /**< \brief TIM1_CH1N */
#define PIOB_13_USART3_CTS_REMAP0   AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 0)   /**< \brief TIM1_CH1N */
#define PIOB_13_USART3_CTS_REMAP1   AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 1)   /**< \brief TIM1_CH1N */

/** @} */

/**
 * \name PIOB_13引脚复用时功能模式
 * @{
 */

#define PIOB_13_AIN                 AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_13_INPUT_FLOAT         AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOB_13_INPUT_PD            AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_13_INPUT_PU            AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_13_OUT_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_13_OUT_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_13_AF_PP               AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_13_AF_OD               AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_13引脚响应速率
 * @{
 */

#define PIOB_13_SPEED_10MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_13_SPEED_2MHz          AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_13_SPEED_50MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_14引脚复用功能
 * @{
 */

#define PIOB_14_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_14_SPI2_MISO          PIOB_14_NO_REMAP   /**< \brief SPI2_MISO */
#define PIOB_14_GPIO               PIOB_14_NO_REMAP   /**< \brief GPIO */

#define PIOB_14_TIM1_CH2N_REMAP0   AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 0)   /**< \brief TIM1_CH2N */
#define PIOB_14_USART3_RTS_REMAP0  AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 0)   /**< \brief USART3_RTS */
#define PIOB_14_USART3_RTS_REMAP1  AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 1)   /**< \brief USART3_RTS */

/** @} */

/**
 * \name PIOB_14引脚复用时功能模式
 * @{
 */

#define PIOB_14_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_14_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOB_14_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_14_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_14_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_14_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_14_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_14_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_14引脚响应速率
 * @{
 */

#define PIOB_14_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_14_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_14_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOB_15引脚复用功能
 * @{
 */

#define PIOB_15_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOB_15_SPI2_MOSI          PIOB_15_NO_REMAP   /**< \brief SPI2_MOSI */
#define PIOB_15_GPIO               PIOB_15_NO_REMAP   /**< \brief GPIO */

#define PIOB_15_TIM1_CH3N_REMAP0   AM_HK32F103RBT6_GPIO_REMAP_CODE(0x06, 0)   /**< \brief TIM1_CH3N */

/** @} */

/**
 * \name PIOB_15引脚复用时功能模式
 * @{
 */

#define PIOB_15_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOB_15_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOB_15_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOB_15_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOB_15_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOB_15_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOB_15_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOB_15_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOB_15引脚响应速率
 * @{
 */

#define PIOB_15_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOB_15_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOB_15_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_0引脚复用功能
 * @{
 */
#define PIOC_0_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_0_ADC12_IN10         PIOC_0_NO_REMAP   /**< \brief ADC12_IN10 */
#define PIOC_0_GPIO               PIOC_0_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOC_0引脚复用时功能模式
 * @{
 */

#define PIOC_0_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_0_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_0_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_0_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_0_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_0_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_0_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_0_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_0引脚响应速率
 * @{
 */

#define PIOC_0_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_0_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_0_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_1引脚复用功能
 * @{
 */
#define PIOC_1_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_1_ADC12_IN11         PIOC_1_NO_REMAP   /**< \brief ADC12_IN11 */
#define PIOC_1_GPIO               PIOC_1_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOC_1引脚复用时功能模式
 * @{
 */

#define PIOC_1_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_1_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_1_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_1_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_1_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_1_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_1_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_1_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_1引脚响应速率
 * @{
 */

#define PIOC_1_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_1_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_1_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_2引脚复用功能
 * @{
 */
#define PIOC_2_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_2_ADC12_IN12         PIOC_2_NO_REMAP   /**< \brief ADC12_IN12 */
#define PIOC_2_GPIO               PIOC_2_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOC_2引脚复用时功能模式
 * @{
 */

#define PIOC_2_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_2_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_2_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_2_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_2_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_2_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_2_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_2_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_2引脚响应速率
 * @{
 */

#define PIOC_2_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_2_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_2_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_3引脚复用功能
 * @{
 */

#define PIOC_3_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_3_ADC12_IN13         PIOC_3_NO_REMAP   /**< \brief ADC12_IN13 */
#define PIOC_3_GPIO               PIOC_3_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOC_3引脚复用时功能模式
 * @{
 */

#define PIOC_3_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_3_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_3_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_3_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_3_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_3_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_3_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_3_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_3引脚响应速率
 * @{
 */

#define PIOC_3_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_3_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_3_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_4引脚复用功能
 * @{
 */

#define PIOC_4_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_4_ADC12_IN14         PIOC_4_NO_REMAP   /**< \brief ADC12_IN14 */
#define PIOC_4_GPIO               PIOC_4_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOC_4引脚复用时功能模式
 * @{
 */

#define PIOC_4_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_4_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_4_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_4_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_4_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_4_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_4_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_4_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_4引脚响应速率
 * @{
 */

#define PIOC_4_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_4_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_4_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_5引脚复用功能
 * @{
 */

#define PIOC_5_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_5_ADC12_IN15         PIOC_5_NO_REMAP   /**< \brief ADC12_IN15 */
#define PIOC_5_GPIO               PIOC_5_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOC_5引脚复用时功能模式
 * @{
 */

#define PIOC_5_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_5_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_5_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_5_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_5_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_5_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_5_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_5_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_5引脚响应速率
 * @{
 */

#define PIOC_5_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_5_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_5_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_6引脚复用功能
 * @{
 */

#define PIOC_6_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_6_GPIO               PIOC_6_NO_REMAP   /**< \brief GPIO */

#define PIOC_6_TIM3_CH1_REMAP3    AM_HK32F103RBT6_GPIO_REMAP_CODE(0X0A, 3)   /**< \brief TIM3_CH1 */

/** @} */

/**
 * \name PIOC_6引脚复用时功能模式
 * @{
 */

#define PIOC_6_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_6_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_6_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_6_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_6_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_6_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_6_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_6_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_6引脚响应速率
 * @{
 */

#define PIOC_6_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_6_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_6_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_7引脚复用功能
 * @{
 */

#define PIOC_7_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_7_GPIO               PIOC_7_NO_REMAP   /**< \brief GPIO */

#define PIOC_7_TIM3_CH2_REMAP3    AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 3)   /**< \brief TIM3_CH2 */

/** @} */

/**
 * \name PIOC_7引脚复用时功能模式
 * @{
 */

#define PIOC_7_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_7_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_7_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_7_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_7_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_7_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_7_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_7_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_7引脚响应速率
 * @{
 */

#define PIOC_7_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_7_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_7_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_8引脚复用功能
 * @{
 */

#define PIOC_8_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_8_GPIO               PIOC_8_NO_REMAP   /**< \brief GPIO */

#define PIOC_8_TIM3_CH3_REMAP3    AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 3)   /**< \brief TIM3_CH3 */

/** @} */

/**
 * \name PIOC_8引脚复用时功能模式
 * @{
 */

#define PIOC_8_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_8_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_8_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_8_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_8_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_8_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_8_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_8_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_8引脚响应速率
 * @{
 */

#define PIOC_8_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_8_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_8_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_9引脚复用功能
 * @{
 */

#define PIOC_9_NO_REMAP           AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_9_GPIO               PIOC_9_NO_REMAP   /**< \brief GPIO */

#define PIOC_9_TIM3_CH4_REMAP3    AM_HK32F103RBT6_GPIO_REMAP_CODE(0x0A, 3)   /**< \brief TIM3_CH4 */

/** @} */

/**
 * \name PIOC_9引脚复用时功能模式
 * @{
 */

#define PIOC_9_AIN                AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_9_INPUT_FLOAT        AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_9_INPUT_PD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_9_INPUT_PU           AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_9_OUT_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_9_OUT_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_9_AF_PP              AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_9_AF_OD              AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_9引脚响应速率
 * @{
 */

#define PIOC_9_SPEED_10MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_9_SPEED_2MHz         AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_9_SPEED_50MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_10引脚复用功能
 * @{
 */

#define PIOC_10_NO_REMAP          AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_10_GPIO              PIOC_10_NO_REMAP   /**< \brief GPIO */

#define PIOC_10_USART3_TX_REMAP1  AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 1)   /**< \brief USART3_TX */

/** @} */

/**
 * \name PIOC_10引脚复用时功能模式
 * @{
 */

#define PIOC_10_AIN               AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_10_INPUT_FLOAT       AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_10_INPUT_PD          AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_10_INPUT_PU          AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_10_OUT_PP            AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_10_OUT_OD            AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_10_AF_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_10_AF_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_10引脚响应速率
 * @{
 */

#define PIOC_10_SPEED_10MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_10_SPEED_2MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_10_SPEED_50MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_11引脚复用功能
 * @{
 */

#define PIOC_11_NO_REMAP          AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_11_GPIO              PIOC_11_NO_REMAP   /**< \brief GPIO */

#define PIOC_11_USART3_RX_REMAP1  AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 1)   /**< \brief USART3_RX */

/** @} */

/**
 * \name PIOC_11引脚复用时功能模式
 * @{
 */

#define PIOC_11_AIN               AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_11_INPUT_FLOAT       AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_11_INPUT_PD          AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_11_INPUT_PU          AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_11_OUT_PP            AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_11_OUT_OD            AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_11_AF_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_11_AF_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_11引脚响应速率
 * @{
 */

#define PIOC_11_SPEED_10MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_11_SPEED_2MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_11_SPEED_50MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_12引脚复用功能
 * @{
 */

#define PIOC_12_NO_REMAP          AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_12_GPIO              PIOC_12_NO_REMAP   /**< \brief GPIO */

#define PIOC_12_USART3_CK_REMAP1  AM_HK32F103RBT6_GPIO_REMAP_CODE(0x04, 1)   /**< \brief USART3_CK */

/** @} */

/**
 * \name PIOC_12引脚复用时功能模式
 * @{
 */

#define PIOC_12_AIN               AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_12_INPUT_FLOAT       AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_12_INPUT_PD          AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_12_INPUT_PU          AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_12_OUT_PP            AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_12_OUT_OD            AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_12_AF_PP             AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_12_AF_OD             AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_12引脚响应速率
 * @{
 */

#define PIOC_12_SPEED_10MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_12_SPEED_2MHz        AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_12_SPEED_50MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_13引脚复用功能
 * @{
 */

#define PIOC_13_NO_REMAP         AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_13_TAMPER_RTC       PIOC_13_NO_REMAP   /**< \brief TAMPER_RTC */
#define PIOC_13_GPIO             PIOC_13_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOC_13引脚复用时功能模式
 * @{
 */

#define PIOC_13_AIN              AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_13_INPUT_FLOAT      AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_13_INPUT_PD         AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_13_INPUT_PU         AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_13_OUT_PP           AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_13_OUT_OD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_13_AF_PP            AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_13_AF_OD            AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_13引脚响应速率
 * @{
 */

#define PIOC_13_SPEED_10MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_13_SPEED_2MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_13_SPEED_50MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_14引脚复用功能
 * @{
 */

#define PIOC_14_NO_REMAP        AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_14_OSC32_IN        PIOC_14_NO_REMAP   /**< \brief OSC32_IN */
#define PIOC_14_GPIO            PIOC_14_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOC_14引脚复用时功能模式
 * @{
 */

#define PIOC_14_AIN             AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_14_INPUT_FLOAT     AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_14_INPUT_PD        AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_14_INPUT_PU        AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_14_OUT_PP          AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_14_OUT_OD          AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_14_AF_PP           AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_14_AF_OD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_14引脚响应速率
 * @{
 */

#define PIOC_14_SPEED_10MHz     AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_14_SPEED_2MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_14_SPEED_50MHz     AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOC_15引脚复用功能
 * @{
 */

#define PIOC_15_NO_REMAP        AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOC_15_OSC32_OUT       PIOC_15_NO_REMAP   /**< \brief OSC32_OUT */
#define PIOC_15_GPIO            PIOC_15_NO_REMAP   /**< \brief GPIO */

/** @} */

/**
 * \name PIOC_15引脚复用时功能模式
 * @{
 */

#define PIOC_15_AIN             AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOC_15_INPUT_FLOAT     AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOC_15_INPUT_PD        AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOC_15_INPUT_PU        AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOC_15_OUT_PP          AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOC_15_OUT_OD          AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOC_15_AF_PP           AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOC_15_AF_OD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOC_15引脚响应速率
 * @{
 */

#define PIOC_15_SPEED_10MHz     AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOC_15_SPEED_2MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOC_15_SPEED_50MHz     AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOD_0引脚复用功能
 * @{
 */

#define PIOD_0_NO_REMAP         AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOD_0_OSC_IN           PIOD_0_NO_REMAP   /**< \brief OSC_IN */
#define PIOD_0_GPIO             PIOD_0_NO_REMAP    /**< \brief GPIO */

#define PIOD_0_CAN_RX_REMAP3    AM_HK32F103RBT6_GPIO_REMAP_CODE(0xD, 3)   /**< \brief CAN_RX */
#define PIOD_0_OSC_IN_REMAP     AM_HK32F103RBT6_GPIO_REMAP_CODE(0xF, 1)   /**< \brief OSC_IN */
/** @} */

/**
 * \name PIOD_0引脚复用时功能模式
 * @{
 */

#define PIOD_0_AIN              AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOD_0_INPUT_FLOAT      AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOD_0_INPUT_PD         AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOD_0_INPUT_PU         AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOD_0_OUT_PP           AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOD_0_OUT_OD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOD_0_AF_PP            AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOD_0_AF_OD            AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOD_0引脚响应速率
 * @{
 */

#define PIOD_0_SPEED_10MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOD_0_SPEED_2MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOD_0_SPEED_50MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOD_1引脚复用功能
 * @{
 */

#define PIOD_1_NO_REMAP         AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOD_1_OSC_OUT          PIOD_1_NO_REMAP   /**< \brief OSC_OUT */
#define PIOD_1_GPIO             PIOD_1_NO_REMAP   /**< \brief GPIO */

#define PIOD_1_OSC_OUT_REMAP    AM_HK32F103RBT6_GPIO_REMAP_CODE(0xF, 1)   /**< \brief OSC_OUT */
#define PIOD_1_CAN_TX_REMAP3    AM_HK32F103RBT6_GPIO_REMAP_CODE(0xD, 3)   /**< \brief CAN_RX */

/** @} */

/**
 * \name PIOD_1引脚复用时功能模式
 * @{
 */

#define PIOD_1_AIN              AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOD_1_INPUT_FLOAT      AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOD_1_INPUT_PD         AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOD_1_INPUT_PU         AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOD_1_OUT_PP           AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOD_1_OUT_OD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOD_1_AF_PP            AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOD_1_AF_OD            AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOD_1引脚响应速率
 * @{
 */

#define PIOD_1_SPEED_10MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOD_1_SPEED_2MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOD_1_SPEED_50MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * \name PIOD_2引脚复用功能
 * @{
 */

#define PIOD_2_NO_REMAP         AM_HK32F103RBT6_GPIO_REMAP_CODE(0x15, 0)
#define PIOD_2_GPIO             PIOD_2_NO_REMAP   /**< \brief GPIO */
#define PIOD_2_TIM3_ETR         PIOD_2_NO_REMAP   /**< \brief TIM3_ETR */

/** @} */

/**
 * \name PIOD_2引脚复用时功能模式
 * @{
 */

#define PIOD_2_AIN              AM_HK32F103RBT6_GPIO_MODE_CODE(0x0)   /**< \brief 模拟模式 */
#define PIOD_2_INPUT_FLOAT      AM_HK32F103RBT6_GPIO_MODE_CODE(0x1)   /**< \brief 输入浮空模式（默认） */
#define PIOD_2_INPUT_PD         AM_HK32F103RBT6_GPIO_MODE_CODE(0x2)   /**< \brief 输入下拉模式 */
#define PIOD_2_INPUT_PU         AM_HK32F103RBT6_GPIO_MODE_CODE(0x3)   /**< \brief 输入上拉模式 */
#define PIOD_2_OUT_PP           AM_HK32F103RBT6_GPIO_MODE_CODE(0x4)   /**< \brief 通用推挽输出模式 */
#define PIOD_2_OUT_OD           AM_HK32F103RBT6_GPIO_MODE_CODE(0x5)   /**< \brief 通用开漏输出模式 */
#define PIOD_2_AF_PP            AM_HK32F103RBT6_GPIO_MODE_CODE(0x6)   /**< \brief 复用推挽输出模式 */
#define PIOD_2_AF_OD            AM_HK32F103RBT6_GPIO_MODE_CODE(0x7)   /**< \brief 复用开漏输出模式 */

/** @} */

/**
 * \name PIOD_2引脚响应速率
 * @{
 */

#define PIOD_2_SPEED_10MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x01)   /**< \brief 10MHz输出响应速率 */
#define PIOD_2_SPEED_2MHz       AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x02)   /**< \brief 2MHz输出响应速率 */
#define PIOD_2_SPEED_50MHz      AM_HK32F103RBT6_GPIO_OUTRES_RATE_CODE(0x03)   /**< \brief 50MHz输出响应速率 */

/** @} */

/******************************************************************************/

/**
 * @}
 */

/**
 * @}if_hk32f103rbt6_pin
 */

#ifdef __cplusplus
}
#endif

#endif /* __HK32F103RBT6_PIN_H */

/* end of file */
