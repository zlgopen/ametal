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
 * \brief ZSN700 IO 引脚配置参数定义
 *
 * \note: 配置引脚功能的时候一定要同时配置引脚的模式，如上下拉模式、推挽模式等
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-18  YRZ, first implementation
 * \endinternal
 */

#ifndef __ZSN700_PIN_H
#define __ZSN700_PIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_zsn700_gpio_util.h"

#ifdef PIN_OUTSET
    #include "zsn700_pin_outset.h"
#endif
 /**
 * \addtogroup zsn700_if_pin
 * \copydoc zsn700_pin.h
 * @{
 */

/**
 * \name ZSN700 PIO 引脚编号
 * \anchor grp_gpio_pin_number
 * @{
 */

#define PIOA            0         /**< \brief 端口A引脚起始号 */
#define PIOA_0          0         /**< \brief PIOA_0 引脚号 */
#define PIOA_1          1         /**< \brief PIOA_1 引脚号 */
#define PIOA_2          2         /**< \brief PIOA_2 引脚号 */
#define PIOA_3          3         /**< \brief PIOA_3 引脚号 */
#define PIOA_4          4         /**< \brief PIOA_4 引脚号 */
#define PIOA_8          8         /**< \brief PIOA_8 引脚号 */
#define PIOA_9          9         /**< \brief PIOA_9 引脚号 */
#define PIOA_10         10        /**< \brief PIOA_10 引脚号 */
#define PIOA_11         11        /**< \brief PIOA_11 引脚号 */
#define PIOA_12         12        /**< \brief PIOA_13 引脚号 */
#define PIOA_13         13        /**< \brief PIOA_13 引脚号 */
#define PIOA_14         14        /**< \brief PIOA_14 引脚号 */

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
#define PIOD_2          50        /**< \brief PIOD_2  引脚号 */

#define PIOF            80       /**< \brief 端口F引脚起始号 */
#define PIOF_0          80       /**< \brief PIOF_0   引脚号 */
#define PIOF_1          81       /**< \brief PIOF_1   引脚号 */
#define PIOF_6          86       /**< \brief PIOF_6   引脚号 */
#define PIOF_7          87       /**< \brief PIOF_7   引脚号 */

#define PIOF_11         91       /**< \brief PIOF_11  引脚号 */

/**
 * \brief 引脚数目为96
 */
#define PIN_NUM         92

/**
 * \brief 支持同时发生最大引脚中断数是92
 */
#define PIN_INT_MAX     92

/** @} */

/**
 * \name ZSN700 引脚定义,除调试引脚SWCK、SWIO之外，所有引脚复位时都默认为GPIO功能(上拉输入)
 * @{
 */

/******************************************************************************/


/**
 * \name PIO引脚复用时功能模式(通用)
 * @{
 */
#define PIO_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIO_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIO_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIO_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIO_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIO_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIO_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_0引脚复用功能
 * @{
 */
#define PIOA_0_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_0_UART1_CTS      AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_0_LPUART1_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_0_TIM0_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_0_VC0_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 4)
#define PIOA_0_TIM1_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_0_TIM3_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_0_TIM0_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_0引脚复用时功能模式
 * @{
 */
#define PIOA_0_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_0_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_0_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_0_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_0_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_0_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_0_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_1引脚复用功能
 * @{
 */
#define PIOA_1_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_1_UART1_RTS      AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_1_LPUART1_RXD    AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_1_TIM0_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_1_TIM1_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x01, 4)
#define PIOA_1_TIM1_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_1_HCLK_OUT       AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_1_SPI1_MOSI      AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_1引脚复用时功能模式
 * @{
 */
#define PIOA_1_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_1_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_1_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_1_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_1_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_1_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_1_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_2引脚复用功能
 * @{
 */
#define PIOA_2_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_2_UART1_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_2_TIM0_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_2_VC1_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_2_TIM1_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 4)
#define PIOA_2_TIM2_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_2_PCLK_OUT       AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_2_SPI1_MISO      AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_2引脚复用时功能模式
 * @{
 */

#define PIOA_2_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_2_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_2_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_2_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_2_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_2_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_2_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_3引脚复用功能
 * @{
 */
#define PIOA_3_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_3_UART1_RXD      AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_3_TIM0_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_3_TIM1_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_3_TIM2_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x01, 4)
#define PIOA_3_SPI1_CS        AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_3_TIM3_CH1A      AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_3_TIM5_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_3引脚复用时功能模式
 * @{
 */
#define PIOA_3_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_3_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_3_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_3_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_3_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_3_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_3_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_4引脚复用功能
 * @{
 */
#define PIOA_4_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_4_SPI0_CS        AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_4_UART1_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_4_PCA_CH4        AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_4_TIM2_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x01, 4)
#define PIOA_4_TIM5_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_4_LVD_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_4_TIM3_CH2B      AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_4引脚复用时功能模式
 * @{
 */
#define PIOA_4_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_4_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_4_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_4_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_4_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_4_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_4_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_8引脚复用功能
 * @{
 */
#define PIOA_8_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_8_ART0_TXD       AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_8_TIM3_CH0A      AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_8_TIM1_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_8_TIM4_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_8_TIM3_BK        AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_8引脚复用时功能模式
 * @{
 */
#define PIOA_8_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_8_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_8_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_8_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_8_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_8_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_8_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_9引脚复用功能
 * @{
 */
#define PIOA_9_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_9_UART0_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_9_TIM3_CH1A      AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_9_TIM0_BK        AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_9_I2C0_SCL       AM_ZSN700_GPIO_REMAP_CODE(0x01, 4)
#define PIOA_9_HCLK_OUT       AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_9_TIM5_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_9引脚复用时功能模式
 * @{
 */
#define PIOA_9_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_9_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_9_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_9_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_9_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_9_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_9_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_10引脚复用功能
 * @{
 */
#define PIOA_10_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_10_UART0_RXD      AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_10_TIM3_CH2A      AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_10_TIM2_BK        AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_10_I2C0_SDA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 4)
#define PIOA_10_TIM2_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_10_PCLK_OUT       AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_10_TIM6_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_10引脚复用时功能模式
 * @{
 */
#define PIOA_10_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_10_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_10_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_10_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_10_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_10_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_10_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_11引脚复用功能
 * @{
 */
#define PIOA_11_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_11_UART0_CTS      AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_11_TIM3_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_11_I2C1_SCL       AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_11_VC0_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_11_SPI0_MISO      AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_11_TIM4_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_11引脚复用时功能模式
 * @{
 */
#define PIOA_11_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_11_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_11_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_11_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_11_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_11_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_11_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_12引脚复用功能
 * @{
 */
#define PIOA_12_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_12_UART0_RTS      AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_12_TIM3_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_12_I2C1_SDA       AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_12_VC1_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_12_SPI0_MOSI      AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_12_PCNT_S0        AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_12引脚复用时功能模式
 * @{
 */
#define PIOA_12_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_12_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_12_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_12_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_12_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_12_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_12_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_13引脚复用功能
 * @{
 */
#define PIOA_13_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_13_IR_OUT         AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_13_UART0_RXD      AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_13_LVD_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_13_TIM3_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x01, 4)
#define PIOA_13_RTC_1HZ        AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_13_PCNT_S1        AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_13_VC2_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_13引脚复用时功能模式
 * @{
 */
#define PIOA_13_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_13_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_13_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_13_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_13_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_13_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_13_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOA_14引脚复用功能
 * @{
 */
#define PIOA_14_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x01, 0)
#define PIOA_14_UART1_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x01, 1)
#define PIOA_14_UART0_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x01, 2)
#define PIOA_14_TIM3_CH2A      AM_ZSN700_GPIO_REMAP_CODE(0x01, 3)
#define PIOA_14_LVD_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 4)
#define PIOA_14_RCH_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 5)
#define PIOA_14_RCL_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 6)
#define PIOA_14_PLL_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x01, 7)

/** @} */

/**
 * \name PIOA_14引脚复用时功能模式
 * @{
 */
#define PIOA_14_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOA_14_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOA_14_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOA_14_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOA_14_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOA_14_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOA_14_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_0引脚复用功能
 * @{
 */
#define PIOB_0_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_0_PCA_CH2        AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_0_TIM3_CH1B      AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_0_LPUART0_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_0_TIM5_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_0_RCH_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_0_RCL_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_0_PLL_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_0引脚复用时功能模式
 * @{
 */
#define PIOB_0_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_0_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_0_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_0_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_0_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_0_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_0_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_1引脚复用功能
 * @{
 */
#define PIOB_1_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_1_PCA_CH3        AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_1_PCLK_OUT       AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_1_TIM3_CH2B      AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_1_TIM6_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_1_LPUART0_RTS    AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_1_VC2_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_1_TCLK_OUT       AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_1引脚复用时功能模式
 * @{
 */
#define PIOB_1_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_1_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_1_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_1_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_1_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_1_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_1_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_2引脚复用功能
 * @{
 */
#define PIOB_2_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_2_LPTIM0_TOG     AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_2_PCA_ECI        AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_2_LPUART1_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_2_TIM4_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_2_TIM1_BK        AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_2_TIM0_BK        AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_2_TIM2_BK        AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_2引脚复用时功能模式
 * @{
 */
#define PIOB_2_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_2_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_2_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_2_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_2_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_2_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_2_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_3引脚复用功能
 * @{
 */
#define PIOB_3_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_3_SPI0_SCK       AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_3_TIM0_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_3_TIM1_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_3_TIM3_CH0A      AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_3_LPTIM0_GATE    AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_3_XTL_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_3_XTH_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_3引脚复用时功能模式
 * @{
 */
#define PIOB_3_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_3_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_3_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_3_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_3_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_3_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_3_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_4引脚复用功能
 * @{
 */
#define PIOB_4_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_4_SPI0_MISO      AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_4_PCA_CH0        AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_4_TIM2_BK        AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_4_UART0_CTS      AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_4_TIM2_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_4_TIM3_CH0B      AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_4_LPTIM0_ETR     AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_4引脚复用时功能模式
 * @{
 */
#define PIOB_4_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_4_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_4_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_4_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_4_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_4_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_4_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_5引脚复用功能
 * @{
 */
#define PIOB_5_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_5_SPI0_MOSI      AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_5_TIM1_BK        AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_5_PCA_CH1        AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_5_LPTIM0_GATE    AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_5_PCNT_S0        AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_5_UART0_RTS      AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_5引脚复用时功能模式
 * @{
 */
#define PIOB_5_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_5_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_5_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_5_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_5_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_5_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_5_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_6引脚复用功能
 * @{
 */
#define PIOB_6_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_6_I2C0_SCL       AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_6_UART0_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_6_TIM1_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_6_TIM0_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_6_LPTIM0_ETR     AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_6_TIM3_CH0A      AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_6_LPTIM0_TOG     AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_6引脚复用时功能模式
 * @{
 */
#define PIOB_6_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_6_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_6_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_6_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_6_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_6_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_6_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_7引脚复用功能
 * @{
 */
#define PIOB_7_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_7_I2C0_SDA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_7_UART0_RXD      AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_7_TIM2_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_7_LPUART1_CTS    AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_7_TIM0_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_7_LPTIM0_TOGN    AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_7_PCNT_S1        AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_7引脚复用时功能模式
 * @{
 */
#define PIOB_7_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_7_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_7_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_7_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_7_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_7_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_7_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_8引脚复用功能
 * @{
 */
#define PIOB_8_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_8_I2C0_SCL       AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_8_TIM1_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_8_TIM2_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_8_TIM0_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_8_TIM3_CH2A      AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_8_UART0_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_8引脚复用时功能模式
 * @{
 */
#define PIOB_8_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_8_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_8_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_8_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_8_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_8_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_8_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_9引脚复用功能
 * @{
 */
#define PIOB_9_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_9_I2C0_SDA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_9_IR_OUT         AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_9_SPI1_CS        AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_9_TIM2_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_9_TIM2_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_9_UART0_RXD      AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_9引脚复用时功能模式
 * @{
 */
#define PIOB_9_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_9_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_9_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_9_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_9_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_9_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_9_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_10引脚复用功能
 * @{
 */
#define PIOB_10_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_10_I2C1_SCL       AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_10_SPI1_SCK       AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_10_TIM1_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_10_LPUART0_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_10_TIM3_CH1A      AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_10_LPUART1_RTS    AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_10_UART1_RTS      AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_10引脚复用时功能模式
 * @{
 */
#define PIOB_10_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_10_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_10_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_10_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_10_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_10_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_10_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_11引脚复用功能
 * @{
 */
#define PIOB_11_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_11_I2C1_SDA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_11_TIM1_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_11_LPUART0_RXD    AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_11_TIM2_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_11_TIM6_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_11_LPUART1_CTS    AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_11_UART1_CTS      AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_11引脚复用时功能模式
 * @{
 */
#define PIOB_11_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_11_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_11_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_11_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_11_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_11_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_11_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_12引脚复用功能
 * @{
 */
#define PIOB_12_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_12_SPI1_CS        AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_12_TIM3_BK        AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_12_LPUART0_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_12_TIM0_BK        AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_12_LPUART0_RTS    AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_12_TIM6_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_12引脚复用时功能模式
 * @{
 */
#define PIOB_12_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_12_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_12_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_12_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_12_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_12_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_12_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_13引脚复用功能
 * @{
 */
#define PIOB_13_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_13_SPI1_SCK       AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_13_I2C1_SCL       AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_13_TIM3_CH0B      AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_13_LPUART0_CTS    AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_13_TIM1_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_13_TIM1_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_13_TIM6_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_13引脚复用时功能模式
 * @{
 */
#define PIOB_13_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_13_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_13_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_13_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_13_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_13_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_13_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_14引脚复用功能
 * @{
 */
#define PIOB_14_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_14_SPI1_MISO      AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_14_I2C1_SDA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_14_TIM3_CH1B      AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_14_TIM0_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_14_RTC_1HZ        AM_ZSN700_GPIO_REMAP_CODE(0x02, 5)
#define PIOB_14_LPUART0_RTS    AM_ZSN700_GPIO_REMAP_CODE(0x02, 6)
#define PIOB_14_TIM1_BK        AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_14引脚复用时功能模式
 * @{
 */
#define PIOB_14_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_14_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_14_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_14_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_14_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_14_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_14_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOB_15引脚复用功能
 * @{
 */
#define PIOB_15_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x02, 0)
#define PIOB_15_SPI1_MOSI      AM_ZSN700_GPIO_REMAP_CODE(0x02, 1)
#define PIOB_15_TIM3_CH2B      AM_ZSN700_GPIO_REMAP_CODE(0x02, 2)
#define PIOB_15_TIM0_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x02, 3)
#define PIOB_15_TIM0_GATE      AM_ZSN700_GPIO_REMAP_CODE(0x02, 4)
#define PIOB_15_LPUART1_RXD    AM_ZSN700_GPIO_REMAP_CODE(0x02, 7)

/** @} */

/**
 * \name PIOB_15引脚复用时功能模式
 * @{
 */
#define PIOB_15_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOB_15_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOB_15_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOB_15_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOB_15_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOB_15_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOB_15_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_0引脚复用功能
 * @{
 */
#define PIOC_0_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_0_LPTIM0_GATE    AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_0_PCNT_S0        AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_0_UART1_CTS      AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_0_UART2_RTS      AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)

/** @} */

/**
 * \name PIOC_0引脚复用时功能模式
 * @{
 */
#define PIOC_0_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_0_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_0_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_0_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_0_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_0_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_0_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_1引脚复用功能
 * @{
 */
#define PIOC_1_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_1_LPTIM0_TOG     AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_1_TIM5_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_1_UART1_RTS      AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_1_PCNT_S0FO      AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)
#define PIOC_1_UART2_CTS      AM_ZSN700_GPIO_REMAP_CODE(0x03, 6)

/** @} */

/**
 * \name PIOC_1引脚复用时功能模式
 * @{
 */
#define PIOC_1_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_1_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_1_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_1_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_1_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_1_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_1_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_2引脚复用功能
 * @{
 */
#define PIOC_2_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_2_SPI1_MISO      AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_2_LPTIM0_TOGN    AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_2_PCNT_S1        AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_2_UART2_RXD      AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)

/** @} */

/**
 * \name PIOC_2引脚复用时功能模式
 * @{
 */
#define PIOC_2_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_2_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_2_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_2_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_2_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_2_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_2_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_3引脚复用功能
 * @{
 */
#define PIOC_3_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_3_SPI1_MOSI      AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_3_LPTIM0_ETR     AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_3_LPTIM0_TOGN    AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_3_PCNT_S1FO      AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)
#define PIOC_3_UART2_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x03, 5)

/** @} */

/**
 * \name PIOC_3引脚复用时功能模式
 * @{
 */
#define PIOC_3_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_3_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_3_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_3_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_3_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_3_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_3_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_4引脚复用功能
 * @{
 */
#define PIOC_4_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_4_LPUART0_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_4_TIM2_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_4_IR_OUT         AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_4_VC2_OUT        AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)

/** @} */

/**
 * \name PIOC_4引脚复用时功能模式
 * @{
 */
#define PIOC_4_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_4_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_4_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_4_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_4_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_4_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_4_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_5引脚复用功能
 * @{
 */
#define PIOC_5_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_5_LPUART0_RXD    AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_5_TIM6_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_5_PCA_CH4        AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)

/** @} */

/**
 * \name PIOC_5引脚复用时功能模式
 * @{
 */
#define PIOC_5_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_5_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_5_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_5_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_5_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_5_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_5_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_6引脚复用功能
 * @{
 */
#define PIOC_6_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_6_PCA_CH0        AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_6_TIM4_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_6_TIM2_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_6_LPTIM1_GATE    AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)
#define PIOC_6_UART3_RXD      AM_ZSN700_GPIO_REMAP_CODE(0x03, 6)

/** @} */

/**
 * \name PIOC_6引脚复用时功能模式
 * @{
 */
#define PIOC_6_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_6_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_6_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_6_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_6_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_6_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_6_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_7引脚复用功能
 * @{
 */
#define PIOC_7_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_7_PCA_CH1        AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_7_TIM5_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_7_TIM2_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_7_LPTIM1_ETR     AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)
#define PIOC_7_UART3_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x03, 6)

/** @} */

/**
 * \name PIOC_7引脚复用时功能模式
 * @{
 */
#define PIOC_7_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_7_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_7_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_7_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_7_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_7_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_7_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_8引脚复用功能
 * @{
 */
#define PIOC_8_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_8_PCA_CH2        AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_8_TIM6_CHA       AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_8_TIM2_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_8_LPTIM1_TOG     AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)
#define PIOC_8_UART3_CTS      AM_ZSN700_GPIO_REMAP_CODE(0x03, 6)

/** @} */

/**
 * \name PIOC_8引脚复用时功能模式
 * @{
 */
#define PIOC_8_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_8_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_8_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_8_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_8_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_8_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_8_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_9引脚复用功能
 * @{
 */
#define PIOC_9_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_9_PCA_CH3        AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_9_TIM4_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_9_TIM1_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_9_LPTIM1_TOGN    AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)
#define PIOC_9_UART3_RTS      AM_ZSN700_GPIO_REMAP_CODE(0x03, 6)

/** @} */

/**
 * \name PIOC_9引脚复用时功能模式
 * @{
 */
#define PIOC_9_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_9_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_9_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_9_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_9_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_9_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_9_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_10引脚复用功能
 * @{
 */
#define PIOC_10_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_10_LPUART1_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_10_LPUART0_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_10_PCA_CH2        AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)

/** @} */

/**
 * \name PIOC_10引脚复用时功能模式
 * @{
 */
#define PIOC_10_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_10_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_10_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_10_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_10_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_10_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_10_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_11引脚复用功能
 * @{
 */
#define PIOC_11_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_11_LPUART1_RXD    AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_11_LPUART0_RXD    AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_11_PCA_CH3        AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_11_PCNT_S0FO      AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)

/** @} */

/**
 * \name PIOC_11引脚复用时功能模式
 * @{
 */
#define PIOC_11_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_11_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_11_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_11_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_11_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_11_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_11_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_12引脚复用功能
 * @{
 */
#define PIOC_12_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_12_LPUART0_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x03, 1)
#define PIOC_12_LPUART1_TXD    AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_12_PCA_CH4        AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)
#define PIOC_12_PCNT_S1FO      AM_ZSN700_GPIO_REMAP_CODE(0x03, 4)

/** @} */

/**
 * \name PIOC_12引脚复用时功能模式
 * @{
 */
#define PIOC_12_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_12_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_12_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_12_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_12_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_12_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_12_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_13引脚复用功能
 * @{
 */
#define PIOC_13_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)
#define PIOC_13_RTC_1HZ        AM_ZSN700_GPIO_REMAP_CODE(0x03, 2)
#define PIOC_13_TIM3_CH1B      AM_ZSN700_GPIO_REMAP_CODE(0x03, 3)

/** @} */

/**
 * \name PIOC_13引脚复用时功能模式
 * @{
 */
#define PIOC_13_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_13_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_13_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_13_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_13_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_13_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_13_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_14引脚复用功能
 * @{
 */
#define PIOC_14_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)

/** @} */

/**
 * \name PIOC_14引脚复用时功能模式
 * @{
 */
#define PIOC_14_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_14_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_14_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_14_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_14_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_14_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_14_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOC_15引脚复用功能
 * @{
 */
#define PIOC_15_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x03, 0)

/** @} */

/**
 * \name PIOC_15引脚复用时功能模式
 * @{
 */
#define PIOC_15_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOC_15_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOC_15_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOC_15_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOC_15_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOC_15_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOC_15_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOD_2引脚复用功能
 * @{
 */
#define PIOD_2_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x04, 0)
#define PIOD_2_PCA_ECI        AM_ZSN700_GPIO_REMAP_CODE(0x04, 1)
#define PIOD_2_LPUART0_RTS    AM_ZSN700_GPIO_REMAP_CODE(0x04, 2)
#define PIOD_2_TIM1_ETR       AM_ZSN700_GPIO_REMAP_CODE(0x04, 3)

/** @} */

/**
 * \name PIOD_2引脚复用时功能模式
 * @{
 */
#define PIOD_2_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOD_2_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOD_2_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOD_2_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOD_2_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOD_2_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOD_2_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOF_0引脚复用功能
 * @{
 */
#define PIOF_0_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x06, 0)
#define PIOF_0_I2C0_SDA       AM_ZSN700_GPIO_REMAP_CODE(0x06, 1)
#define PIOF_0_UART1_TXD      AM_ZSN700_GPIO_REMAP_CODE(0x06, 3)

/** @} */

/**
 * \name PIOF_0引脚复用时功能模式
 * @{
 */
#define PIOF_0_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOF_0_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOF_0_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOF_0_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOF_0_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOF_0_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOF_0_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOF_1引脚复用功能
 * @{
 */
#define PIOF_1_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x06, 0)
#define PIOF_1_I2C0_SCL       AM_ZSN700_GPIO_REMAP_CODE(0x06, 1)
#define PIOF_1_TIM4_CHB       AM_ZSN700_GPIO_REMAP_CODE(0x06, 2)
#define PIOF_1_UART1_RXD      AM_ZSN700_GPIO_REMAP_CODE(0x06, 3)

/** @} */

/**
 * \name PIOF_1引脚复用时功能模式
 * @{
 */
#define PIOF_1_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOF_1_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOF_1_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOF_1_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOF_1_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOF_1_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOF_1_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOF_6引脚复用功能
 * @{
 */
#define PIOF_6_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x06, 0)
#define PIOF_6_I2C1_SCL       AM_ZSN700_GPIO_REMAP_CODE(0x06, 1)
#define PIOF_6_LPUART1_CTS    AM_ZSN700_GPIO_REMAP_CODE(0x06, 2)
#define PIOF_6_UART0_CTS      AM_ZSN700_GPIO_REMAP_CODE(0x06, 3)

/** @} */

/**
 * \name PIOF_6引脚复用时功能模式
 * @{
 */
#define PIOF_6_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOF_6_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOF_6_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOF_6_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOF_6_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOF_6_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOF_6_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOF_7引脚复用功能
 * @{
 */
#define PIOF_7_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x06, 0)
#define PIOF_7_I2C1_SDA       AM_ZSN700_GPIO_REMAP_CODE(0x06, 1)
#define PIOF_7_LPUART1_RTS    AM_ZSN700_GPIO_REMAP_CODE(0x06, 2)
#define PIOF_7_UART0_RTS      AM_ZSN700_GPIO_REMAP_CODE(0x06, 3)

/** @} */

/**
 * \name PIOF_7引脚复用时功能模式
 * @{
 */
#define PIOF_7_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOF_7_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOF_7_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOF_7_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOF_7_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOF_7_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOF_7_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/**
 * \name PIOF_11引脚复用功能
 * @{
 */
#define PIOF_11_GPIO           AM_ZSN700_GPIO_REMAP_CODE(0x06, 0)

/** @} */

/**
 * \name PIOF_11引脚复用时功能模式
 * @{
 */
#define PIOF_11_INPUT_PU       AM_ZSN700_GPIO_MODE_CODE(0x0)   /**< \brief 上拉输入模式 (默认) */
#define PIOF_11_INPUT_PD       AM_ZSN700_GPIO_MODE_CODE(0x1)   /**< \brief 下拉输入模式 */
#define PIOF_11_AIN            AM_ZSN700_GPIO_MODE_CODE(0x2)   /**< \brief 模拟输入模式 */
#define PIOF_11_INPUT_FLOAT    AM_ZSN700_GPIO_MODE_CODE(0x3)   /**< \brief 浮空输入模式 */
#define PIOF_11_OUT_PP         AM_ZSN700_GPIO_MODE_CODE(0x4)   /**< \brief 推挽输出模式 */
#define PIOF_11_OUT_OD         AM_ZSN700_GPIO_MODE_CODE(0x5)   /**< \brief 开漏输出模式 */
#define PIOF_11_AOUT           AM_ZSN700_GPIO_MODE_CODE(0x6)   /**< \brief 模拟输出模式 */
/** @} */

/******************************************************************************/

/**
 * @}
 */

/**
 * @}if_zsn700_pin
 */

#ifdef __cplusplus
}
#endif

#endif /* __ZSN700_PIN_H */

/* end of file */
