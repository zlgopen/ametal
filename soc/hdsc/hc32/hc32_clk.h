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
 * \brief HC32 时钟ID定义
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-23  lqy, first implementation
 * \endinternal
 */

#ifndef __HC32_CLK_H
#define __HC32_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup hc32_if_clockid
 * \copydoc hc32_clk.h
 * @{
 */

/**
 * \name 系统或外设的时钟ID
 * \anchor grp_clk_id
 * @{
 */

/* 系统初始相关 */
#define CLK_XTHOSC    (0ul)  /**< \brief 外部高速时钟 */
#define CLK_XTLOSC    (1ul)  /**< \brief 外部低速时钟 */
#define CLK_RCH       (2ul)  /**< \brief 内部高速时钟 */
#define CLK_RCL       (3ul)  /**< \brief 内部低速时钟 */
#define CLK_PLLIN     (4ul)  /**< \brief PLL输入时钟 */
#define CLK_PLLOUT    (5ul)  /**< \brief PLL输出时钟 */
#define CLK_SYSCLK    (6ul)  /**< \brief SYS时钟 */
#define CLK_HCLK      (7ul)  /**< \brief 主时钟 */
#define CLK_PCLK      (8ul)  /**< \brief 外设时钟 */

/* hc32所有外设时钟均挂载在PCLK时钟下 */
#define CLK_FLASH     ((0x1ul << CLK_PCLK) | 31ul)
#define CLK_DMA       ((0x1ul << CLK_PCLK) | 29ul)
#define CLK_GPIO      ((0x1ul << CLK_PCLK) | 28ul)
#define CLK_AES       ((0x1ul << CLK_PCLK) | 27ul)
#define CLK_CRC       ((0x1ul << CLK_PCLK) | 26ul)
#define CLK_SWD       ((0x1ul << CLK_PCLK) | 25ul)
#define CLK_TICK      ((0x1ul << CLK_PCLK) | 24ul)
#define CLK_LCD       ((0x1ul << CLK_PCLK) | 22ul)
#define CLK_CLOCKTRIM ((0x1ul << CLK_PCLK) | 21ul)
#define CLK_RTC       ((0x1ul << CLK_PCLK) | 20ul)
#define CLK_PCNT      ((0x1ul << CLK_PCLK) | 19ul)
#define CLK_RNG       ((0x1ul << CLK_PCLK) | 18ul)
#define CLK_VC_LVD    ((0x1ul << CLK_PCLK) | 17ul)
#define CLK_ADC_BGR   ((0x1ul << CLK_PCLK) | 16ul)
#define CLK_WDT       ((0x1ul << CLK_PCLK) | 15ul)
#define CLK_PCA       ((0x1ul << CLK_PCLK) | 14ul)
#define CLK_OPA       ((0x1ul << CLK_PCLK) | 13ul)
#define CLK_TIM3      ((0x1ul << CLK_PCLK) | 11ul)
#define CLK_TIM456    ((0x1ul << CLK_PCLK) | 10ul)
#define CLK_LPTIM0    ((0x1ul << CLK_PCLK) | 9ul)
#define CLK_TIM012    ((0x1ul << CLK_PCLK) | 8ul)
#define CLK_SPI1      ((0x1ul << CLK_PCLK) | 7ul)
#define CLK_SPI0      ((0x1ul << CLK_PCLK) | 6ul)
#define CLK_I2C1      ((0x1ul << CLK_PCLK) | 5ul)
#define CLK_I2C0      ((0x1ul << CLK_PCLK) | 4ul)
#define CLK_LPUART1   ((0x1ul << CLK_PCLK) | 3ul)
#define CLK_LPUART0   ((0x1ul << CLK_PCLK) | 2ul)
#define CLK_UART1     ((0x1ul << CLK_PCLK) | 1ul)
#define CLK_UART0     ((0x1ul << CLK_PCLK) | 0ul)
#define CLK_UART3     ((0x1ul << CLK_PCLK) | 40ul)
#define CLK_UART2     ((0x1ul << CLK_PCLK) | 39ul)
#define CLK_LPTIM1    ((0x1ul << CLK_PCLK) | 35ul)
#define CLK_DAC       ((0x1ul << CLK_PCLK) | 34ul)

/**
 * @}
 */

/**
 * @} hc32_if_clk
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_CLK_H */

/* end of file */
