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
 * \brief ZSN700 芯片外设寄存器基址定义
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-18  YRZ, first implementation
 * \endinternal
 */

#ifndef __ZSN700_REGBASE_H
#define __ZSN700_REGBASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup zsn700_if_regbase
 * \copydoc zsn700_regbase.h
 * @{
 */

/**
 * \name 外设内存映射地址定义
 * @{
 */

#define ZSN700_UART0_BASE              (0x40000000UL)  /** \brief UART0基地址 */
#define ZSN700_UART1_BASE              (0x40000100UL)  /** \brief UART1基地址 */
#define ZSN700_UART2_BASE              (0x40006000UL)  /** \brief UART2基地址 */
#define ZSN700_UART3_BASE              (0x40006400UL)  /** \brief UART3基地址 */
#define ZSN700_LPUART0_BASE            (0x40000200UL)  /** \brief LPUART0基地址 */
#define ZSN700_LPUART1_BASE            (0x40004000UL)  /** \brief LPUART1基地址 */
#define ZSN700_I2C0_BASE               (0x40000400UL)  /** \brief I2C0基地址 */
#define ZSN700_I2C1_BASE               (0x40004400UL)  /** \brief I2C1基地址 */
#define ZSN700_SPI0_BASE               (0x40000800UL)  /** \brief SPI0基地址 */
#define ZSN700_SPI1_BASE               (0x40004800UL)  /** \brief SPI1基地址 */
#define ZSN700_TIM0_BASE               (0x40000C00UL)  /** \brief TIM0基地址 */
#define ZSN700_TIM1_BASE               (0x40000D00UL)  /** \brief TIM1基地址 */
#define ZSN700_TIM2_BASE               (0x40000E00UL)  /** \brief TIM2基地址 */
#define ZSN700_TIM3_BASE               (0x40005800UL)  /** \brief TIM3基地址 */
#define ZSN700_TIM4_BASE               (0x40003000UL)  /** \brief TIM4基地址 */
#define ZSN700_TIM5_BASE               (0x40003400UL)  /** \brief TIM5基地址 */
#define ZSN700_TIM6_BASE               (0x40003800UL)  /** \brief TIM6基地址 */
#define ZSN700_LPTIM0_BASE             (0x40000F00UL)  /** \brief LPTIM0基地址 */
#define ZSN700_LPTIM1_BASE             (0x40000F40UL)  /** \brief LPTIM1基地址 */
#define ZSN700_AES_BASE                (0x40021400UL)  /** \brief AES基地址 */
#define ZSN700_ADC_BASE                (0x40002400UL)  /** \brief ADC基地址 */
#define ZSN700_CLKTRIM_BASE            (0x40001800UL)  /** \brief CLKTRIM基地址 */
#define ZSN700_CRC_BASE                (0x40020900UL)  /** \brief CRC基地址 */
#define ZSN700_DAC_BASE                (0x40002500UL)  /** \brief DAC基地址 */
#define ZSN700_DMA_BASE                (0x40021000UL)  /** \brief DMA基地址 */
#define ZSN700_FLASH_BASE              (0x40020000UL)  /** \brief FLASH基地址 */
#define ZSN700_PORT0_BASE              (0x40020C00UL)  /** \brief PORT0基地址 */
#define ZSN700_PORT1_BASE              (0x40022000UL)  /** \brief PORT1基地址 */
#define ZSN700_LCD_BASE                (0x40005C00UL)  /** \brief LCDCTRL基地址 */
#define ZSN700_LVD_BASE                (0x40002400UL)  /** \brief LVD基地址 */
#define ZSN700_RCC_BASE                (0x40002000UL)  /** \brief RCC基地址 */
#define ZSN700_OPA_BASE                (0x40002400UL)  /** \brief OPA基地址 */
#define ZSN700_PCNT_BASE               (0x40005400UL)  /** \brief PCNT基地址 */
#define ZSN700_RAM_BASE                (0x40020400UL)  /** \brief RAM基地址 */
#define ZSN700_PCA_BASE                (0x40001000UL)  /** \brief PCA基地址 */
#define ZSN700_RTC_BASE                (0x40001400UL)  /** \brief RTC基地址 */
#define ZSN700_TRNG_BASE               (0x40004C00UL)  /** \brief TRNG基地址 */
#define ZSN700_VC_BASE                 (0x40002410UL)  /** \brief VC基地址 */
#define ZSN700_WDT_BASE                (0x40000F80UL)  /** \brief WDT基地址 */
#define ZSN700_BGR_BASE                (0x40002400UL)  /** \brief BGR基地址 */
#define ZSN700_SYSTICK                 (0xE000E010UL)  /** \brief SYSTICK基地址 */
/** @} */

/**
 * @} zsn700_if_regbase
 */

#ifdef __cplusplus
}
#endif

#endif/* __ZSN700_REGBASE_H */

/* end of file */
