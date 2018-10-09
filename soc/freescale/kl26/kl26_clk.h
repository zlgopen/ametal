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
 * \brief KL26 时钟ID定义
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-13  ari first implementation.
 * \endinternal
 */

#ifndef __KL26_CLK_ID_H
#define __KL26_CLK_ID_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup kl26_if_clk
 * \copydoc kl26_clk.h
 * @{
 */


/**
 * \name 系统或外设的时钟ID
 * \anchor grp_clk_id
 * @{
 */

#define CLK_SLOW_IRC    (4 << 8 | 0)  /**< \brief 慢速IRC时钟 */
#define CLK_FAST_IRC    (4 << 8 | 1)  /**< \brief 快速IRC时钟 */
#define CLK_XTAL        (4 << 8 | 2)  /**< \brief XTAL */
#define CLK_CLKIN       (4 << 8 | 3)  /**< \brief 输入时钟 */

#define CLK_CORE        (4 << 8 | 4)  /**< \brief 内核时钟 */
#define CLK_SYSTEM      (4 << 8 | 5)  /**< \brief 系统时钟 */
#define CLK_BUS         (4 << 8 | 6)  /**< \brief 总线时钟 */
#define CLK_FLASH       (4 << 8 | 7)  /**< \brief Flash时钟 */
#define CLK_PERIPH      (4 << 8 | 8)  /**< \brief 外设时钟 */
#define CLK_SYSTICK     (4 << 8 | 9)  /**< \brief 系统滴答时钟 */

#define CLK_OSC32K      (4 << 8 | 10) /**< \brief OSC32K时钟 */
#define CLK_OSCERC      (4 << 8 | 11) /**< \brief OSCERC时钟 */

/** \brief rtc out clocks*/
#define CLK_RTCOUT      (4 << 8 | 12) /**< \brief RTC输出时钟 */

/** \brief mcg clocks*/
#define CLK_MCGFLL      (4 << 8 | 13) /**< \brief FLL时钟 */
#define CLK_MCGPLL      (4 << 8 | 14) /**< \brief PLL时钟 */
#define CLK_MCGEXT      (4 << 8 | 15) /**< \brief EXT时钟 */
#define CLK_MCGIRC      (4 << 8 | 16) /**< \brief IRC时钟 */
#define CLK_MCGOUT      (4 << 8 | 17) /**< \brief MCG输出时钟 */

/** \brief LPO clock */
#define CLK_LPO         (4 << 8 | 18) /**< \brief LPO时钟 */
#define CLK_WDT         (4 << 8 | 19) /**< \brief WDT时钟 */

/** \brief periph clk */
#define CLK_I2C0        (0 << 8 | 6)  /**< \brief I2C0时钟 */
#define CLK_I2C1        (0 << 8 | 7)  /**< \brief I2C1时钟 */
#define CLK_UART0       (0 << 8 | 10) /**< \brief UART0时钟 */
#define CLK_UART1       (0 << 8 | 11) /**< \brief UART1时钟 */
#define CLK_UART2       (0 << 8 | 12) /**< \brief UART2时钟 */
#define CLK_USBOTG      (0 << 8 | 18) /**< \brief USB时钟 */
#define CLK_CMP         (0 << 8 | 19) /**< \brief CMP时钟 */
#define CLK_SPI0        (0 << 8 | 22) /**< \brief SPI0时钟 */
#define CLK_SPI1        (0 << 8 | 23) /**< \brief SPI1时钟 */

/** \brief  SCGC5 */
#define CLK_LPTMR       (1 << 8 | 0)  /**< \brief LPTMR时钟 */
#define CLK_TSI         (1 << 8 | 5)  /**< \brief TSI时钟 */
#define CLK_PORTA       (1 << 8 | 9)  /**< \brief PORTA时钟 */
#define CLK_PORTB       (1 << 8 | 10) /**< \brief PORTB时钟 */
#define CLK_PORTC       (1 << 8 | 11) /**< \brief PORTC时钟 */
#define CLK_PORTD       (1 << 8 | 12) /**< \brief PORTD时钟 */
#define CLK_PORTE       (1 << 8 | 13) /**< \brief PORTE时钟 */

/** \brief  SCGC6 */
#define CLK_FTF         (2 << 8 | 0)  /**< \brief FTF时钟 */
#define CLK_DMAMUX      (2 << 8 | 1)  /**< \brief DMAMUX时钟 */
#define CLK_I2S         (2 << 8 | 15) /**< \brief I2S时钟 */
#define CLK_PIT         (2 << 8 | 23) /**< \brief PIT时钟 */
#define CLK_TPM0        (2 << 8 | 24) /**< \brief TPM0时钟 */
#define CLK_TPM1        (2 << 8 | 25) /**< \brief TPM1时钟 */
#define CLK_TPM2        (2 << 8 | 26) /**< \brief TPM2时钟 */
#define CLK_ADC0        (2 << 8 | 27) /**< \brief ADC0时钟 */
#define CLK_RTC         (2 << 8 | 29) /**< \brief RTC时钟 */
#define CLK_DAC0        (2 << 8 | 31) /**< \brief DAC0时钟 */

/** \brief  SCGC7 */
#define CLK_DMA         (3 << 8 | 8)  /**< \brief DMA时钟 */

#define CLK_PERIPH_ID_MIN CLK_I2C0    /**< \brief 外设时钟ID最小值 */
#define CLK_PERIPH_ID_MAX CLK_DMA     /**< \brief 外设时钟ID最大值 */

/**
 * @}
 */


/**
 * @} kl26_if_clk
 */

#ifdef __cplusplus
}
#endif

#endif /* __KL26_CLK_H */

/* end of file */
