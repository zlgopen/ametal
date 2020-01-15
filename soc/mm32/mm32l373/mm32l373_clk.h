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
 * \brief MM32L373 时钟ID定义
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-23  lqy, first implementation
 * \endinternal
 */

#ifndef __MM32L373_CLK_H
#define __MM32L373_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup mm32l373_if_clockid
 * \copydoc mm32l373_clk.h
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
#define CLK_TIM4    (0x01ul << 8 | 2ul)   /**< \brief TIM4定时器 时钟 */
#define CLK_WWDG    (0x01ul << 8 | 11ul)  /**< \brief WWDG窗口看门狗 时钟 */
#define CLK_SPI2    (0x01ul << 8 | 14ul)  /**< \brief SPI2 时钟 */
#define CLK_UART2   (0x01ul << 8 | 17ul)  /**< \brief UART2 时钟 */
#define CLK_UART3   (0x01ul << 8 | 18ul)  /**< \brief UART3 时钟 */
#define CLK_I2C1    (0x01ul << 8 | 21ul)  /**< \brief I2C1 时钟 */
#define CLK_I2C2    (0x01ul << 8 | 22ul)  /**< \brief I2C2 时钟 */
#define CLK_USB     (0x01ul << 8 | 23ul)  /**< \brief USB 时钟 */
#define CLK_CAN     (0x01ul << 8 | 25ul)  /**< \brief CAN 时钟 */
#define CLK_BKP     (0x01ul << 8 | 27ul)  /**< \brief CRS 时钟 */
#define CLK_PWR     (0x01ul << 8 | 28ul)  /**< \brief 电源接口 时钟 */
#define CLK_DAC     (0x01ul << 8 | 29ul)  /**< \brief 电源接口 时钟 */

/* APB2外设时钟 */
#define CLK_AFIO    (0x02ul << 8 | 0ul)  /**< \brief 系统配置寄存器 时钟 */
#define CLK_IOPA    (0x02ul << 8 | 2ul)  /**< \brief 系统配置寄存器 时钟 */
#define CLK_IOPB    (0x02ul << 8 | 3ul)  /**< \brief 系统配置寄存器 时钟 */
#define CLK_IOPC    (0x02ul << 8 | 4ul)  /**< \brief 系统配置寄存器 时钟 */
#define CLK_IOPD    (0x02ul << 8 | 5ul)  /**< \brief 系统配置寄存器 时钟 */
#define CLK_IOPE    (0x02ul << 8 | 6ul)  /**< \brief 系统配置寄存器 时钟 */
#define CLK_ADC1    (0x02ul << 8 | 9ul)  /**< \brief ADC1接口 时钟 */
#define CLK_ADC2    (0x02ul << 8 | 10ul) /**< \brief ADC2接口 时钟 */
#define CLK_TIM1    (0x02ul << 8 | 11ul) /**< \brief TIM1定时器 时钟 */
#define CLK_SPI1    (0x02ul << 8 | 12ul) /**< \brief SPI1 时钟 */
#define CLK_UART1   (0x02ul << 8 | 14ul) /**< \brief UART1 时钟 */

/* AHB外设时钟 */
#define CLK_DMA    (0x03ul << 8 | 0ul)   /**< \brief DMA 时钟 */
#define CLK_SRAM   (0x03ul << 8 | 2ul)   /**< \brief SRAM 时钟 */
#define CLK_FLITF  (0x03ul << 8 | 4ul)   /**< \brief FLITF 时钟 */
#define CLK_CRC    (0x03ul << 8 | 6ul)   /**< \brief AES 时钟 */
#define CLK_AES    (0x03ul << 8 | 7ul)   /**< \brief AES 时钟 */

/* 其他时钟 */
#define CLK_PLLIN  (0x04ul << 8 | 0ul)  /**< \brief PLL输入 时钟 */
#define CLK_PLLOUT (0x04ul << 8 | 1ul)  /**< \brief PLL输出 时钟 */
#define CLK_AHB    (0x04ul << 8 | 2ul)  /**< \brief AHB 时钟 */
#define CLK_APB1   (0x04ul << 8 | 3ul)  /**< \brief APB1 时钟 */
#define CLK_APB2   (0x04ul << 8 | 4ul)  /**< \brief APB2 时钟 */
#define CLK_HSEOSC (0x04ul << 8 | 5ul)  /**< \brief 外部晶振 时钟 */
#define CLK_LSEOSC (0x04ul << 8 | 6ul)  /**< \brief 外部晶振 时钟 */
#define CLK_LSI    (0x04ul << 8 | 7ul)  /**< \brief LSI 时钟 */
#define CLK_HSI    (0x04ul << 8 | 8ul)  /**< \brief HSI 时钟 */

/* 非总线时钟 */
#define CLK_SYS    (0x05ul << 8 | 0ul)  /**< \brief 系统 时钟 */
#define CLK_RTC    (0x05ul << 8 | 1ul)  /**< \brief 系统 时钟 */
/**
 * @}
 */

/**
 * @} mm32l373_if_clk
 */

#ifdef __cplusplus
}
#endif

#endif /* __MM32L373_CLK_H */

/* end of file */
