/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82x 引脚及配置参数定义
 * 
 * \note 1. 可转移功能可以配置所有I/O引脚；
 *          GPIO功能和特殊功能属于固定功能，只能搭配对应的引脚使用 \n
 *          示例：PIO0_0引脚功能配置： \n 
 *          - 配置为GPIO功能使用宏：      PIO0_0_GPIO \n
 *          - 配置为比较器输入功能使用宏：PIO0_0_ACMP_I1 \n
 *          - 配置串口发送引脚功能使用宏：PIO_FUNC_U0_TXD
 *
 * \note 2. 复用功能方向由自身决定，PIOx_x_GPIO_INPUT或PIOx_x_GPIO_OUTPUT
 *          会将引脚配置为普通IO输入输出功能 
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  mem, modified.
 * - 1.02 15-07-28  zxl, modified.
 * - 1.00 15-07-08  zxl, first implementation.
 * \endinternal
 */

#ifndef __LPC82X_PIN_H
#define __LPC82X_PIN_H

#include "am_lpc82x_gpio_util.h"

#ifdef __cplusplus
extern "C" {
#endif

 /** 
 * \addtogroup lpc82x_if_pin
 * \copydoc lpc82x_pin.h
 * @{
 */

/** 
 * \name LPC8xx 引脚编号
 * @{
 */

#define PIO0_0          0        /**< \brief PIO0_0 引脚号 */
#define PIO0_1          1        /**< \brief PIO0_1 引脚号 */
#define PIO0_2          2        /**< \brief PIO0_2 引脚号 */
#define PIO0_3          3        /**< \brief PIO0_3 引脚号 */
#define PIO0_4          4        /**< \brief PIO0_4 引脚号 */
#define PIO0_5          5        /**< \brief PIO0_5 引脚号 */
#define PIO0_6          6        /**< \brief PIO0_6 引脚号 */
#define PIO0_7          7        /**< \brief PIO0_7 引脚号 */
#define PIO0_8          8        /**< \brief PIO0_8 引脚号 */
#define PIO0_9          9        /**< \brief PIO0_9 引脚号 */
#define PIO0_10         10       /**< \brief PIO0_10引脚号 */
#define PIO0_11         11       /**< \brief PIO0_11引脚号 */
#define PIO0_12         12       /**< \brief PIO0_12引脚号 */
#define PIO0_13         13       /**< \brief PIO0_13引脚号 */
#define PIO0_14         14       /**< \brief PIO0_14引脚号 */
#define PIO0_15         15       /**< \brief PIO0_15引脚号 */
#define PIO0_16         16       /**< \brief PIO0_16引脚号 */
#define PIO0_17         17       /**< \brief PIO0_17引脚号 */
#define PIO0_18         18       /**< \brief PIO0_18引脚号 */
#define PIO0_19         19       /**< \brief PIO0_19引脚号 */
#define PIO0_20         20       /**< \brief PIO0_20引脚号 */
#define PIO0_21         21       /**< \brief PIO0_21引脚号 */
#define PIO0_22         22       /**< \brief PIO0_22引脚号 */
#define PIO0_23         23       /**< \brief PIO0_23引脚号 */
#define PIO0_24         24       /**< \brief PIO0_24引脚号 */
#define PIO0_25         25       /**< \brief PIO0_25引脚号 */
#define PIO0_26         26       /**< \brief PIO0_26引脚号 */
#define PIO0_27         27       /**< \brief PIO0_27引脚号 */
#define PIO0_28         28       /**< \brief PIO0_28引脚号 */

/** @} */

/** 
 * \name LPC82x 引脚数
 * @{
 */
 
/** \brief 支持最大引脚数是29 */
#define LPC82X_PIN_NUM         29

/** @} */
 
/** 
 * \name LPC82x 引脚可转移功能
 *
 * 每个I/O引脚都可配置为以下功能
 *
 * @{
 */

/** \brief UART0_TXD  功能 */
#define PIO_FUNC_U0_TXD          AM_LPC82X_GPIO_FUNC_U0_TXD

/** \brief UART0_RXD  功能 */
#define PIO_FUNC_U0_RXD          AM_LPC82X_GPIO_FUNC_U0_RXD

/** \brief UART0_RTS  功能 */
#define PIO_FUNC_U0_RTS          AM_LPC82X_GPIO_FUNC_U0_RTS

/** \brief UART0_CTS  功能 */
#define PIO_FUNC_U0_CTS          AM_LPC82X_GPIO_FUNC_U0_CTS

/** \brief UART0_SCLK 功能 */
#define PIO_FUNC_U0_SCLK         AM_LPC82X_GPIO_FUNC_U0_SCLK

/** \brief UART1_TXD  功能 */
#define PIO_FUNC_U1_TXD          AM_LPC82X_GPIO_FUNC_U1_TXD

/** \brief UART1_RXD  功能 */
#define PIO_FUNC_U1_RXD          AM_LPC82X_GPIO_FUNC_U1_RXD

/** \brief UART1_RTS  功能 */
#define PIO_FUNC_U1_RTS          AM_LPC82X_GPIO_FUNC_U1_RTS

/** \brief UART1_CTS  功能 */
#define PIO_FUNC_U1_CTS          AM_LPC82X_GPIO_FUNC_U1_CTS

/** \brief UART1_SCLK 功能 */
#define PIO_FUNC_U1_SCLK         AM_LPC82X_GPIO_FUNC_U1_SCLK

/** \brief UART2_TXD  功能 */
#define PIO_FUNC_U2_TXD          AM_LPC82X_GPIO_FUNC_U2_TXD

/** \brief UART2_RXD  功能 */
#define PIO_FUNC_U2_RXD          AM_LPC82X_GPIO_FUNC_U2_RXD

/** \brief UART2_RTS  功能 */
#define PIO_FUNC_U2_RTS          AM_LPC82X_GPIO_FUNC_U2_RTS

/** \brief UART2_CTS  功能 */
#define PIO_FUNC_U2_CTS          AM_LPC82X_GPIO_FUNC_U2_CTS

/** \brief UART2_SCLK 功能 */
#define PIO_FUNC_U2_SCLK         AM_LPC82X_GPIO_FUNC_U2_SCLK

/** \brief SPI0_SCK   功能 */
#define PIO_FUNC_SPI0_SCK        AM_LPC82X_GPIO_FUNC_SPI0_SCK

/** \brief SPI0_MOSI  功能 */
#define PIO_FUNC_SPI0_MOSI       AM_LPC82X_GPIO_FUNC_SPI0_MOSI

/** \brief SPI0_MISO  功能 */
#define PIO_FUNC_SPI0_MISO       AM_LPC82X_GPIO_FUNC_SPI0_MISO

/** \brief SPI0_SSEL0 功能 */
#define PIO_FUNC_SPI0_SSEL0      AM_LPC82X_GPIO_FUNC_SPI0_SSEL0

/** \brief SPI0_SSEL1 功能 */
#define PIO_FUNC_SPI0_SSEL1      AM_LPC82X_GPIO_FUNC_SPI0_SSEL1

/** \brief SPI0_SSEL2 功能 */
#define PIO_FUNC_SPI0_SSEL2      AM_LPC82X_GPIO_FUNC_SPI0_SSEL2

/** \brief SPI0_SSEL3 功能 */
#define PIO_FUNC_SPI0_SSEL3      AM_LPC82X_GPIO_FUNC_SPI0_SSEL3

/** \brief SPI1_SCK   功能 */
#define PIO_FUNC_SPI1_SCK        AM_LPC82X_GPIO_FUNC_SPI1_SCK

/** \brief SPI1_MOSI  功能 */
#define PIO_FUNC_SPI1_MOSI       AM_LPC82X_GPIO_FUNC_SPI1_MOSI

/** \brief SPI1_MISO  功能 */
#define PIO_FUNC_SPI1_MISO       AM_LPC82X_GPIO_FUNC_SPI1_MISO

/** \brief SPI1_SSEL0 功能 */
#define PIO_FUNC_SPI1_SSEL0      AM_LPC82X_GPIO_FUNC_SPI1_SSEL0

/** \brief SPI1_SSEL1 功能 */
#define PIO_FUNC_SPI1_SSEL1      AM_LPC82X_GPIO_FUNC_SPI1_SSEL1

/** \brief SCT_PIN_PIN0 功能 */
#define PIO_FUNC_SCT_PIN0        AM_LPC82X_GPIO_FUNC_SCT_PIN0

/** \brief SCT_PIN_PIN1 功能 */
#define PIO_FUNC_SCT_PIN1        AM_LPC82X_GPIO_FUNC_SCT_PIN1

/** \brief SCT_PIN_PIN2 功能 */
#define PIO_FUNC_SCT_PIN2        AM_LPC82X_GPIO_FUNC_SCT_PIN2

/** \brief SCT_PIN_PIN3 功能 */
#define PIO_FUNC_SCT_PIN3        AM_LPC82X_GPIO_FUNC_SCT_PIN3

/** \brief SCT_OUT0 功能 */
#define PIO_FUNC_SCT_OUT0        AM_LPC82X_GPIO_FUNC_SCT_OUT0

/** \brief SCT_OUT1 功能 */
#define PIO_FUNC_SCT_OUT1        AM_LPC82X_GPIO_FUNC_SCT_OUT1

/** \brief SCT_OUT2 功能 */
#define PIO_FUNC_SCT_OUT2        AM_LPC82X_GPIO_FUNC_SCT_OUT2

/** \brief SCT_OUT3 功能 */
#define PIO_FUNC_SCT_OUT3        AM_LPC82X_GPIO_FUNC_SCT_OUT3

/** \brief SCT_OUT4 功能 */
#define PIO_FUNC_SCT_OUT4        AM_LPC82X_GPIO_FUNC_SCT_OUT4

/** \brief SCT_OUT5 功能 */
#define PIO_FUNC_SCT_OUT5        AM_LPC82X_GPIO_FUNC_SCT_OUT5

/** \brief I2C1_SDA 功能 */
#define PIO_FUNC_I2C1_SDA        AM_LPC82X_GPIO_FUNC_I2C1_SDA

/** \brief I2C1_SCL 功能 */
#define PIO_FUNC_I2C1_SCL        AM_LPC82X_GPIO_FUNC_I2C1_SCL

/** \brief I2C2_SDA 功能 */
#define PIO_FUNC_I2C2_SDA        AM_LPC82X_GPIO_FUNC_I2C2_SDA

/** \brief I2C2_SCL 功能 */
#define PIO_FUNC_I2C2_SCL        AM_LPC82X_GPIO_FUNC_I2C2_SCL

/** \brief I2C3_SDA 功能 */
#define PIO_FUNC_I2C3_SDA        AM_LPC82X_GPIO_FUNC_I2C3_SDA

/** \brief I2C3_SCL 功能 */
#define PIO_FUNC_I2C3_SCL        AM_LPC82X_GPIO_FUNC_I2C3_SCL

/** \brief ADC_PINTRIG0 功能 */
#define PIO_FUNC_ADC_PINTRIG0    AM_LPC82X_GPIO_FUNC_ADC_PINTRIG0

/** \brief ADC_PINTRIG1 功能 */
#define PIO_FUNC_ADC_PINTRIG1    AM_LPC82X_GPIO_FUNC_ADC_PINTRIG1

/** \brief ACMP 功能 */
#define PIO_FUNC_ACMP_O          AM_LPC82X_GPIO_FUNC_ACMP_O

/** \brief CLKOUT 功能 */

#define PIO_FUNC_CLKOUT          AM_LPC82X_GPIO_FUNC_CLKOUT
  
/** \brief GPIO_INT_BMAT 功能 */
#define PIO_FUNC_GPIO_INT_BMAT   AM_LPC82X_GPIO_FUNC_GPIO_INT_BMAT

/** @} */

/******************************************************************************/

/**
 * \name PIO0_0 固定功能
 * @{
 */

#define PIO0_0_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_0_ACMP_I1     AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief ACMP_I1  */

/** @} */

/**
 * \name PIO0_0 GPIO方向
 * @{
 */
 
#define PIO0_0_GPIO_INPUT             AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_0_GPIO_OUTPUT_INIT_HIGH  AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_0_GPIO_OUTPUT_INIT_LOW   AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_0 引脚模式
 * @{
 */

#define PIO0_0_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_0_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_0_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_0_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_0_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_0 输入极性
 * @{
 */
 
#define PIO0_0_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_0_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_0 迟滞
 * @{
 */
#define PIO0_0_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_0_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_0 滤波周期
 * @{
 */
 
#define PIO0_0_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_0_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_0_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_0_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_0 滤波分频
 * @{
 */
 
#define PIO0_0_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_0_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_0_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_0_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_0_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_0_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_0_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_1 固定功能
 * @{
 */

#define PIO0_1_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_1_ACMP_I2     AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief ACMP_I2  */
#define PIO0_1_CLKIN       AM_LPC82X_GPIO_FUNC_OTHER2   /**< \brief CLKIN    */

/** @} */

/**
 * \name PIO0_1 GPIO方向
 * @{
 */
 
#define PIO0_1_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_1_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_1_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_1 引脚模式
 * @{
 */

#define PIO0_1_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_1_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_1_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_1_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_1_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_1 输入极性
 * @{
 */

#define PIO0_1_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_1_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_1 迟滞
 * @{
 */
#define PIO0_1_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_1_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_1 滤波周期
 * @{
 */
 
#define PIO0_1_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_1_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_1_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_1_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_1 滤波分频
 * @{
 */
 
#define PIO0_1_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_1_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_1_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_1_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_1_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_1_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_1_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_2 固定功能
 * @{
 */

#define PIO0_2_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_2_SWDIO       AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief SWDIO    */

/** @} */

/**
 * \name PIO0_2 GPIO方向
 * @{
 */

#define PIO0_2_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_2_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_2_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_2 引脚模式
 * @{
 */

#define PIO0_2_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_2_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_2_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_2_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_2_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_2 输入极性
 * @{
 */

#define PIO0_2_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_2_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_2 迟滞
 * @{
 */
#define PIO0_2_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_2_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_2 滤波周期
 * @{
 */

#define PIO0_2_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_2_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_2_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_2_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_2 滤波分频
 * @{
 */

#define PIO0_2_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_2_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_2_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_2_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_2_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_2_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_2_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_3 固定功能
 * @{
 */

#define PIO0_3_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_3_SWCLK       AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief SWCLK    */

/** @} */

/**
 * \name PIO0_3 GPIO方向
 * @{
 */
 
#define PIO0_3_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_3_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_3_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_3 引脚模式
 * @{
 */

#define PIO0_3_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_3_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_3_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_3_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_3_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_3 输入极性
 * @{
 */

#define PIO0_3_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_3_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_3 迟滞
 * @{
 */
#define PIO0_3_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_3_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_3 滤波周期
 * @{
 */

#define PIO0_3_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_3_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_3_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_3_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_3 滤波分频
 * @{
 */

#define PIO0_3_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_3_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_3_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_3_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_3_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_3_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_3_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_4 固定功能
 * @{
 */

#define PIO0_4_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_4_ADC_11      AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief ADC_11   */

/** @} */

/**
 * \name PIO0_4 GPIO方向
 * @{
 */

#define PIO0_4_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_4_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_4_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_4 引脚模式
 * @{
 */

#define PIO0_4_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_4_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_4_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_4_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_4_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_4 输入极性
 * @{
 */

#define PIO0_4_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_4_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_4 迟滞
 * @{
 */
#define PIO0_4_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_4_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_4 滤波周期
 * @{
 */

#define PIO0_4_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_4_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_4_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_4_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_4 滤波分频
 * @{
 */

#define PIO0_4_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_4_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_4_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_4_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_4_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_4_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_4_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_5 固定功能
 * @{
 */

#define PIO0_5_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_5_RESETN      AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief RESETN   */

/** @} */

/**
 * \name PIO0_5 GPIO方向
 * @{
 */

#define PIO0_5_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_5_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_5_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_5 引脚模式
 * @{
 */
#define PIO0_5_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_5_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_5_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_5_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_5_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_5 输入极性
 * @{
 */
#define PIO0_5_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_5_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_5 迟滞
 * @{
 */
#define PIO0_5_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_5_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_5 滤波周期
 * @{
 */

#define PIO0_5_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_5_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_5_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_5_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_5 滤波分频
 * @{
 */

#define PIO0_5_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_5_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_5_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_5_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_5_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_5_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_5_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_6 固定功能
 * @{
 */

#define PIO0_6_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_6_VDDCMP      AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief VDDCMP   */
#define PIO0_6_ADC_1       AM_LPC82X_GPIO_FUNC_OTHER2   /**< \brief ADC_1    */

/** @} */

/**
 * \name PIO0_6 GPIO方向
 * @{
 */

#define PIO0_6_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_6_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_6_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_6 引脚模式
 * @{
 */

#define PIO0_6_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_6_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_6_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_6_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_6_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_6 输入极性
 * @{
 */

#define PIO0_6_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_6_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_6 迟滞
 * @{
 */

#define PIO0_6_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_6_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_6 滤波周期
 * @{
 */

#define PIO0_6_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_6_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_6_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_6_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_6 滤波分频
 * @{
 */

#define PIO0_6_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_6_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_6_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_6_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_6_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_6_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_6_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_7 固定功能
 * @{
 */

#define PIO0_7_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_7_ADC_0       AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief ADC_0    */

/** @} */

/**
 * \name PIO0_7 GPIO方向
 * @{
 */

#define PIO0_7_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_7_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_7_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_7 引脚模式
 * @{
 */

#define PIO0_7_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_7_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_7_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_7_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_7_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_7 输入极性
 * @{
 */

#define PIO0_7_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_7_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_7 迟滞
 * @{
 */

#define PIO0_7_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_7_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_7 滤波周期
 * @{
 */

#define PIO0_7_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_7_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_7_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_7_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_7 滤波分频
 * @{
 */

#define PIO0_7_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_7_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_7_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_7_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_7_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_7_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_7_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_8 固定功能
 * @{
 */

#define PIO0_8_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_8_XTALIN      AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief XTALIN   */

/** @} */

/**
 * \name PIO0_8 GPIO方向
 * @{
 */

#define PIO0_8_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_8_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_8_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_8 引脚模式
 * @{
 */

#define PIO0_8_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_8_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_8_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_8_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_8_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_8 输入极性
 * @{
 */

#define PIO0_8_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_8_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_8 迟滞
 * @{
 */

#define PIO0_8_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_8_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_8 滤波周期
 * @{
 */

#define PIO0_8_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_8_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_8_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_8_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_8 滤波分频
 * @{
 */

#define PIO0_8_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_8_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_8_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_8_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_8_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_8_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_8_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_9 固定功能
 * @{
 */

#define PIO0_9_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO     */
#define PIO0_9_XTALOUT     AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief XTALOUT  */

/** @} */

/**
 * \name PIO0_9 GPIO方向
 * @{
 */

#define PIO0_9_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_9_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_9_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_9 引脚模式
 * @{
 */

#define PIO0_9_INACTIVE    AM_LPC82X_GPIO_INACTIVE      /**< \brief 消极模式 */
#define PIO0_9_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN      /**< \brief 下拉模式 */
#define PIO0_9_PULLUP      AM_LPC82X_GPIO_PULLUP        /**< \brief 上拉模式 */
#define PIO0_9_REPEATER    AM_LPC82X_GPIO_REPEATER      /**< \brief 中继模式 */
#define PIO0_9_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN    /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_9 输入极性
 * @{
 */

#define PIO0_9_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转    */
#define PIO0_9_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转      */

/** @} */

/**
 * \name PIO0_9 迟滞
 * @{
 */

#define PIO0_9_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启    */
#define PIO0_9_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启      */

/** @} */

/**
 * \name PIO0_9 滤波周期
 * @{
 */

#define PIO0_9_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_9_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_9_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_9_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_9 滤波分频
 * @{
 */

#define PIO0_9_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0     /**< \brief 分频值为0 */
#define PIO0_9_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1     /**< \brief 分频值为1 */
#define PIO0_9_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2     /**< \brief 分频值为2 */
#define PIO0_9_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3     /**< \brief 分频值为3 */
#define PIO0_9_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4     /**< \brief 分频值为4 */
#define PIO0_9_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5     /**< \brief 分频值为5 */
#define PIO0_9_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6     /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_10 固定功能
 * @{
 */

#define PIO0_10_GPIO        AM_LPC82X_GPIO_FUNC_GPIO     /**< \brief GPIO    */
#define PIO0_10_I2C0_SCL    AM_LPC82X_GPIO_FUNC_OTHER1   /**< \brief I2C0_SCL*/

/** @} */

/**
 * \name PIO0_10 GPIO方向
 * @{
 */

#define PIO0_10_GPIO_INPUT            AM_LPC82X_GPIO_INPUT        /**< \brief 输入    */
#define PIO0_10_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH  /**< \brief 输出高  */
#define PIO0_10_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW   /**< \brief 输出低  */

/** @} */

/**
 * \name PIO0_10 输入极性
 * @{
 */

#define PIO0_10_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_10_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_10 迟滞
 * @{
 */
#define PIO0_10_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_10_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_10 滤波周期
 * @{
 */

#define PIO0_10_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE  /**< \brief 滤波禁能  */
#define PIO0_10_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE   /**< \brief 滤波1周期 */
#define PIO0_10_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE   /**< \brief 滤波2周期 */
#define PIO0_10_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE   /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_10 滤波分频
 * @{
 */

#define PIO0_10_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_10_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_10_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_10_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_10_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_10_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_10_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/**
 * \name PIO0_10 I2C模式
 * @{
 */

/** \brief 标准/快速模式 */
#define PIO0_10_FAST_STD_MODE   AM_LPC82X_GPIO_I2C_CODE(0x0)

/** \brief GPIO模式      */
#define PIO0_10_FAST_IO_MODE    AM_LPC82X_GPIO_I2C_CODE(0x1)

/** \brief 超快速模式    */
#define PIO0_10_FAST_PLUS_MODE  AM_LPC82X_GPIO_I2C_CODE(0x2)

/** @} */

/******************************************************************************/

/**
 * \name PIO0_11 固定功能
 * @{
 */

#define PIO0_11_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_11_I2C0_SDA    AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief I2C0_SDA */

/** @} */

/**
 * \name PIO0_11 GPIO方向
 * @{
 */

#define PIO0_11_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_11_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_11_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_11 输入极性
 * @{
 */

#define PIO0_11_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE /**< \brief 不反转   */
#define PIO0_11_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE  /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_11 迟滞
 * @{
 */

#define PIO0_11_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE /**< \brief 不开启   */
#define PIO0_11_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE  /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_11 滤波周期
 * @{
 */

#define PIO0_11_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_11_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_11_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_11_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_11 滤波分频
 * @{
 */

#define PIO0_11_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_11_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_11_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_11_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_11_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_11_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_11_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/**
 * \name PIO0_11 I2C模式
 * @{
 */

/** \brief 标准/快速模式 */
#define PIO0_11_FAST_STD_MODE  AM_LPC82X_GPIO_I2C_CODE(0x0)

/** \brief GPIO模式      */
#define PIO0_11_FAST_IO_MODE   AM_LPC82X_GPIO_I2C_CODE(0x1)

/** \brief 超快速模式    */
#define PIO0_11_FAST_PLUS_MODE AM_LPC82X_GPIO_I2C_CODE(0x2)

/** @} */

/******************************************************************************/

/**
 * \name PIO0_12 固定功能
 * @{
 */

#define PIO0_12_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */

/** @} */

/**
 * \name PIO0_12 GPIO方向
 * @{
 */

#define PIO0_12_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_12_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_12_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_12 引脚模式
 * @{
 */

#define PIO0_12_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_12_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_12_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_12_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_12_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_12 输入极性
 * @{
 */

#define PIO0_12_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_12_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_12 迟滞
 * @{
 */
#define PIO0_12_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_12_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_12 滤波周期
 * @{
 */

#define PIO0_12_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_12_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_12_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_12_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_12 滤波分频
 * @{
 */
 
#define PIO0_12_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_12_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_12_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_12_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_12_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_12_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_12_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_13 固定功能
 * @{
 */

#define PIO0_13_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_13_ADC_10      AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief ADC_10   */

/** @} */

/**
 * \name PIO0_13 GPIO方向
 * @{
 */

#define PIO0_13_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_13_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_13_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_13 引脚模式
 * @{
 */

#define PIO0_13_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_13_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_13_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_13_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_13_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_13 输入极性
 * @{
 */

#define PIO0_13_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_13_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_13 迟滞
 * @{
 */

#define PIO0_13_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_13_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_13 滤波周期
 * @{
 */

#define PIO0_13_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_13_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_13_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_13_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_13 滤波分频
 * @{
 */

#define PIO0_13_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_13_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_13_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_13_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_13_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_13_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_13_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_14 固定功能
 * @{
 */

#define PIO0_14_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_14_ACMP_I3     AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief ACMP_I3  */
#define PIO0_14_ADC_2       AM_LPC82X_GPIO_FUNC_OTHER2  /**< \brief ADC_2    */

/** @} */

/**
 * \name PIO0_14 GPIO方向
 * @{
 */

#define PIO0_14_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_14_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_14_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_14 引脚模式
 * @{
 */

#define PIO0_14_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_14_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_14_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_14_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_14_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_14 输入极性
 * @{
 */

#define PIO0_14_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_14_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_14 迟滞
 * @{
 */

#define PIO0_14_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_14_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_14 滤波周期
 * @{
 */

#define PIO0_14_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_14_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_14_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_14_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_14 滤波分频
 * @{
 */

#define PIO0_14_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_14_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_14_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_14_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_14_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_14_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_14_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_15 固定功能
 * @{
 */

#define PIO0_15_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */

/** @} */

/**
 * \name PIO0_15 GPIO方向
 * @{
 */

#define PIO0_15_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_15_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_15_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_15 引脚模式
 * @{
 */

#define PIO0_15_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_15_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_15_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_15_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_15_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_15 输入极性
 * @{
 */

#define PIO0_15_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_15_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_15 迟滞
 * @{
 */

#define PIO0_15_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_15_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_15 滤波周期
 * @{
 */

#define PIO0_15_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_15_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_15_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_15_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_15 滤波分频
 * @{
 */

#define PIO0_15_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_15_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_15_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_15_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_15_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_15_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_15_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_16 固定功能
 * @{
 */

#define PIO0_16_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */

/** @} */

/**
 * \name PIO0_16 GPIO方向
 * @{
 */

#define PIO0_16_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_16_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_16_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_16 引脚模式
 * @{
 */

#define PIO0_16_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_16_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_16_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_16_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_16_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_16 输入极性
 * @{
 */

#define PIO0_16_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_16_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_16 迟滞
 * @{
 */

#define PIO0_16_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_16_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_16 滤波周期
 * @{
 */

#define PIO0_16_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_16_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_16_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_16_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_16 滤波分频
 * @{
 */

#define PIO0_16_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_16_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_16_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_16_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_16_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_16_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_16_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_17 固定功能
 * @{
 */

#define PIO0_17_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_17_ADC_9       AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief ADC_9    */

/** @} */

/**
 * \name PIO0_17 GPIO方向
 * @{
 */

#define PIO0_17_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_17_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_17_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_17 引脚模式
 * @{
 */

#define PIO0_17_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_17_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_17_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_17_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_17_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_17 输入极性
 * @{
 */

#define PIO0_17_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_17_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_17 迟滞
 * @{
 */

#define PIO0_17_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_17_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_17 滤波周期
 * @{
 */

#define PIO0_17_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_17_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_17_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_17_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_17 滤波分频
 * @{
 */

#define PIO0_17_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_17_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_17_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_17_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_17_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_17_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_17_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_18 固定功能
 * @{
 */

#define PIO0_18_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_18_ADC_8       AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief ADC_8    */

/** @} */

/**
 * \name PIO0_18 GPIO方向
 * @{
 */

#define PIO0_18_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_18_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_18_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_18 引脚模式
 * @{
 */

#define PIO0_18_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_18_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_18_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_18_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_18_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_18 输入极性
 * @{
 */

#define PIO0_18_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_18_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_18 迟滞
 * @{
 */

#define PIO0_18_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_18_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_18 滤波周期
 * @{
 */

#define PIO0_18_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_18_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_18_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_18_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_18 滤波分频
 * @{
 */

#define PIO0_18_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_18_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_18_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_18_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_18_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_18_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_18_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_19 固定功能
 * @{
 */

#define PIO0_19_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_19_ADC_7       AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief ADC_7    */

/** @} */

/**
 * \name PIO0_19 GPIO方向
 * @{
 */

#define PIO0_19_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_19_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_19_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_19 引脚模式
 * @{
 */

#define PIO0_19_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_19_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_19_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_19_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_19_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_19 输入极性
 * @{
 */

#define PIO0_19_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_19_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_19 迟滞
 * @{
 */
#define PIO0_19_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_19_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_19 滤波周期
 * @{
 */
 
#define PIO0_19_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_19_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_19_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_19_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_19 滤波分频
 * @{
 */
 
#define PIO0_19_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_19_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_19_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_19_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_19_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_19_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_19_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_20 固定功能
 * @{
 */

#define PIO0_20_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_20_ADC_6       AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief ADC_6    */

/** @} */

/**
 * \name PIO0_20 GPIO方向
 * @{
 */

#define PIO0_20_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_20_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_20_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_20 引脚模式
 * @{
 */

#define PIO0_20_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_20_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_20_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_20_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_20_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_20 输入极性
 * @{
 */

#define PIO0_20_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_20_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_20 迟滞
 * @{
 */

#define PIO0_20_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_20_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_20 滤波周期
 * @{
 */

#define PIO0_20_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_20_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_20_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_20_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_20 滤波分频
 * @{
 */

#define PIO0_20_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_20_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_20_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_20_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_20_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_20_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_20_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_21 固定功能
 * @{
 */

#define PIO0_21_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_21_ADC_5       AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief ADC_5    */

/** @} */

/**
 * \name PIO0_21 GPIO方向
 * @{
 */

#define PIO0_21_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_21_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_21_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_21 引脚模式
 * @{
 */

#define PIO0_21_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_21_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_21_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_21_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_21_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_21 输入极性
 * @{
 */

#define PIO0_21_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_21_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_21 迟滞
 * @{
 */

#define PIO0_21_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_21_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_21 滤波周期
 * @{
 */

#define PIO0_21_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_21_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_21_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_21_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_21 滤波分频
 * @{
 */

#define PIO0_21_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_21_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_21_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_21_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_21_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_21_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_21_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_22 固定功能
 * @{
 */

#define PIO0_22_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_22_ADC_4       AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief ADC_4    */

/** @} */

/**
 * \name PIO0_22 GPIO方向
 * @{
 */

#define PIO0_22_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_22_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_22_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_22 引脚模式
 * @{
 */

#define PIO0_22_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_22_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_22_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_22_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_22_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_22 输入极性
 * @{
 */

#define PIO0_22_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_22_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_22 迟滞
 * @{
 */

#define PIO0_22_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_22_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_22 滤波周期
 * @{
 */

#define PIO0_22_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_22_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_22_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_22_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_22 滤波分频
 * @{
 */

#define PIO0_22_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_22_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_22_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_22_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_22_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_22_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_22_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_23 固定功能
 * @{
 */

#define PIO0_23_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */
#define PIO0_23_ACMP_I4     AM_LPC82X_GPIO_FUNC_OTHER1  /**< \brief ACMP_I4  */
#define PIO0_23_ADC_3       AM_LPC82X_GPIO_FUNC_OTHER2  /**< \brief ADC_3    */

/** @} */

/**
 * \name PIO0_23 GPIO方向
 * @{
 */

#define PIO0_23_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_23_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_23_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_23 引脚模式
 * @{
 */

#define PIO0_23_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_23_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_23_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_23_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_23_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_23 输入极性
 * @{
 */

#define PIO0_23_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_23_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_23 迟滞
 * @{
 */

#define PIO0_23_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_23_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_23 滤波周期
 * @{
 */

#define PIO0_23_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_23_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_23_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_23_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_23 滤波分频
 * @{
 */

#define PIO0_23_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_23_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_23_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_23_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_23_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_23_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_23_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_24 固定功能
 * @{
 */

#define PIO0_24_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */

/** @} */

/**
 * \name PIO0_24 GPIO方向
 * @{
 */

#define PIO0_24_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_24_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_24_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_24 引脚模式
 * @{
 */

#define PIO0_24_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_24_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_24_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_24_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_24_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_24 输入极性
 * @{
 */
 
#define PIO0_24_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_24_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_24 迟滞
 * @{
 */

#define PIO0_24_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_24_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_24 滤波周期
 * @{
 */

#define PIO0_24_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_24_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_24_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_24_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_24 滤波分频
 * @{
 */

#define PIO0_24_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_24_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_24_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_24_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_24_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_24_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_24_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_25 固定功能
 * @{
 */

#define PIO0_25_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */

/** @} */

/**
 * \name PIO0_25 GPIO方向
 * @{
 */
#define PIO0_25_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_25_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_25_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_25 引脚模式
 * @{
 */

#define PIO0_25_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_25_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_25_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_25_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_25_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_25 输入极性
 * @{
 */

#define PIO0_25_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_25_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_25 迟滞
 * @{
 */

#define PIO0_25_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_25_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_25 滤波周期
 * @{
 */

#define PIO0_25_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_25_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_25_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_25_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_25 滤波分频
 * @{
 */

#define PIO0_25_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_25_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_25_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_25_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_25_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_25_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_25_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_26 固定功能
 * @{
 */

#define PIO0_26_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */

/** @} */

/**
 * \name PIO0_26 GPIO方向
 * @{
 */
 
#define PIO0_26_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_26_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_26_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_26 引脚模式
 * @{
 */

#define PIO0_26_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_26_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_26_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_26_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_26_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_26 输入极性
 * @{
 */

#define PIO0_26_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_26_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_26 迟滞
 * @{
 */

#define PIO0_26_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_26_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_26 滤波周期
 * @{
 */

#define PIO0_26_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_26_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_26_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_26_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_26 滤波分频
 * @{
 */

#define PIO0_26_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_26_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_26_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_26_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_26_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_26_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_26_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_27 固定功能
 * @{
 */

#define PIO0_27_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */

/** @} */

/**
 * \name PIO0_27 GPIO方向
 * @{
 */

#define PIO0_27_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_27_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_27_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_27 引脚模式
 * @{
 */

#define PIO0_27_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_27_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_27_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_27_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_27_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_27 输入极性
 * @{
 */

#define PIO0_27_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_27_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_27 迟滞
 * @{
 */

#define PIO0_27_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_27_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_27 滤波周期
 * @{
 */

#define PIO0_27_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_27_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_27_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_27_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_27 滤波分频
 * @{
 */

#define PIO0_27_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_27_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_27_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_27_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_27_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_27_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_27_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */

/******************************************************************************/

/**
 * \name PIO0_28 固定功能
 * @{
 */

#define PIO0_28_GPIO        AM_LPC82X_GPIO_FUNC_GPIO    /**< \brief GPIO     */

/** @} */

/**
 * \name PIO0_28 GPIO方向
 * @{
 */

#define PIO0_28_GPIO_INPUT            AM_LPC82X_GPIO_INPUT       /**< \brief 输入   */
#define PIO0_28_GPIO_OUTPUT_INIT_HIGH AM_LPC82X_GPIO_OUTPUT_HIGH /**< \brief 输出高 */
#define PIO0_28_GPIO_OUTPUT_INIT_LOW  AM_LPC82X_GPIO_OUTPUT_LOW  /**< \brief 输出低 */

/** @} */

/**
 * \name PIO0_28 引脚模式
 * @{
 */

#define PIO0_28_INACTIVE    AM_LPC82X_GPIO_INACTIVE     /**< \brief 消极模式 */
#define PIO0_28_PULLDOWN    AM_LPC82X_GPIO_PULLDOWN     /**< \brief 下拉模式 */
#define PIO0_28_PULLUP      AM_LPC82X_GPIO_PULLUP       /**< \brief 上拉模式 */
#define PIO0_28_REPEATER    AM_LPC82X_GPIO_REPEATER     /**< \brief 中继模式 */
#define PIO0_28_OPEN_DRAIN  AM_LPC82X_GPIO_OPEN_DRAIN   /**< \brief 开漏模式 */

/** @} */

/**
 * \name PIO0_28 输入极性
 * @{
 */

#define PIO0_28_INV_DISABLE AM_LPC82X_GPIO_INV_DISABLE  /**< \brief 不反转   */
#define PIO0_28_INV_ENABLE  AM_LPC82X_GPIO_INV_ENABLE   /**< \brief 反转     */

/** @} */

/**
 * \name PIO0_28 迟滞
 * @{
 */

#define PIO0_28_HYS_DISABLE AM_LPC82X_GPIO_HYS_DISABLE  /**< \brief 不开启   */
#define PIO0_28_HYS_ENABLE  AM_LPC82X_GPIO_HYS_ENABLE   /**< \brief 开启     */

/** @} */

/**
 * \name PIO0_28 滤波周期
 * @{
 */

#define PIO0_28_FIL_DISABLE AM_LPC82X_GPIO_FIL_DISABLE /**< \brief 滤波禁能  */
#define PIO0_28_FIL_1CYCLE  AM_LPC82X_GPIO_FIL_1CYCLE  /**< \brief 滤波1周期 */
#define PIO0_28_FIL_2CYCLE  AM_LPC82X_GPIO_FIL_2CYCLE  /**< \brief 滤波2周期 */
#define PIO0_28_FIL_3CYCLE  AM_LPC82X_GPIO_FIL_3CYCLE  /**< \brief 滤波3周期 */

/** @} */

/**
 * \name PIO0_28 滤波分频
 * @{
 */

#define PIO0_28_FIL_DIV0    AM_LPC82X_GPIO_FIL_DIV0    /**< \brief 分频值为0 */
#define PIO0_28_FIL_DIV1    AM_LPC82X_GPIO_FIL_DIV1    /**< \brief 分频值为1 */
#define PIO0_28_FIL_DIV2    AM_LPC82X_GPIO_FIL_DIV2    /**< \brief 分频值为2 */
#define PIO0_28_FIL_DIV3    AM_LPC82X_GPIO_FIL_DIV3    /**< \brief 分频值为3 */
#define PIO0_28_FIL_DIV4    AM_LPC82X_GPIO_FIL_DIV4    /**< \brief 分频值为4 */
#define PIO0_28_FIL_DIV5    AM_LPC82X_GPIO_FIL_DIV5    /**< \brief 分频值为5 */
#define PIO0_28_FIL_DIV6    AM_LPC82X_GPIO_FIL_DIV6    /**< \brief 分频值为6 */

/** @} */


/**
 * @} lpc82x_if_pin
 */

#ifdef __cplusplus
}
#endif

#endif /* __LPC82X_PIN_H */

/* end of file */
