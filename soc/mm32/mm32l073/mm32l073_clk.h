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
 * \brief ZLG 时钟ID定义
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-07  nwt, first implementation
 * \endinternal
 */

#ifndef __MM32L073_CLK_H
#define __MM32L073_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup mm32l073_if_clockid
 * \copydoc mm32l073_clk.h
 * @{
 */

/**
 * \name 系统或外设的时钟ID
 * \anchor grp_clk_id
 * @{
 */

/* APB1外设时钟 */
#define CLK_TIM2    (0x01ul << 8 | 0ul)   /**< \brief TIM2定时器 时钟 */
#define CLK_TIM3    (0x01ul << 8 | 1ul)   /**< \brief TIM3定时器 时钟 */
#define CLK_WWDG    (0x01ul << 8 | 11ul)  /**< \brief WWDG窗口看门狗 时钟 */
#define CLK_SPI2    (0x01ul << 8 | 14ul)  /**< \brief SPI2 时钟 */
#define CLK_UART2   (0x01ul << 8 | 17ul)  /**< \brief UART2 时钟 */
#define CLK_I2C1    (0x01ul << 8 | 21ul)  /**< \brief I2C1 时钟 */
#define CLK_USB     (0x01ul << 8 | 23ul)  /**< \brief USB 时钟 */
#define CLK_CAN     (0x01ul << 8 | 25ul)  /**< \brief CAN 时钟 */
#define CLK_CRS     (0x01ul << 8 | 27ul)  /**< \brief CRS 时钟 */
#define CLK_PWR     (0x01ul << 8 | 28ul)  /**< \brief 电源接口 时钟 */

/* APB2外设时钟 */
#define CLK_SYSCFG  (0x02ul << 8 | 0ul)  /**< \brief 系统配置寄存器 时钟 */
#define CLK_ADC1    (0x02ul << 8 | 9ul)  /**< \brief ADC1接口 时钟 */
#define CLK_TIM1    (0x02ul << 8 | 11ul) /**< \brief TIM1定时器 时钟 */
#define CLK_SPI1    (0x02ul << 8 | 12ul) /**< \brief SPI1 时钟 */
#define CLK_UART1   (0x02ul << 8 | 14ul) /**< \brief UART1 时钟 */
#define CLK_CPT     (0x02ul << 8 | 15ul) /**< \brief 比较器 时钟 */
#define CLK_TIM14   (0x02ul << 8 | 16ul) /**< \brief TIM14 时钟 */
#define CLK_TIM16   (0x02ul << 8 | 17ul) /**< \brief TIM16 时钟 */
#define CLK_TIM17   (0x02ul << 8 | 18ul) /**< \brief TIM17 时钟 */
#define CLK_DBGMCU  (0x02ul << 8 | 22ul) /**< \brief DBGMCU 时钟 */

/* AHB外设时钟 */
#define CLK_DMA    (0x03ul << 8 | 0ul)   /**< \brief DMA 时钟 */
#define CLK_SRAM   (0x03ul << 8 | 2ul)   /**< \brief SRAM 时钟 */
#define CLK_FLITF  (0x03ul << 8 | 4ul)   /**< \brief FLITF 时钟 */
#define CLK_AES    (0x03ul << 8 | 7ul)   /**< \brief AES 时钟 */
#define CLK_GPIOA  (0x03ul << 8 | 17ul)  /**< \brief GPIOA 时钟 */
#define CLK_GPIOB  (0x03ul << 8 | 18ul)  /**< \brief GPIOB 时钟 */
#define CLK_GPIOC  (0x03ul << 8 | 19ul)  /**< \brief GPIOC 时钟 */
#define CLK_GPIOD  (0x03ul << 8 | 20ul)  /**< \brief GPIOD 时钟 */

/* 其他时钟 */
#define CLK_PLLIN  (0x04ul << 8 | 0ul)  /**< \brief PLL输入 时钟 */
#define CLK_PLLOUT (0x04ul << 8 | 1ul)  /**< \brief PLL输出 时钟 */
#define CLK_AHB    (0x04ul << 8 | 2ul)  /**< \brief AHB 时钟 */
#define CLK_APB1   (0x04ul << 8 | 3ul)  /**< \brief APB1 时钟 */
#define CLK_APB2   (0x04ul << 8 | 4ul)  /**< \brief APB2 时钟 */
#define CLK_HSEOSC (0x04ul << 8 | 5ul)  /**< \brief 外部晶振 时钟 */
#define CLK_LSI    (0x04ul << 8 | 6ul)  /**< \brief LSI 时钟 */
#define CLK_HSI    (0x04ul << 8 | 7ul)  /**< \brief HSI 时钟 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __MM32L073_CLK_H */

/* end of file */
