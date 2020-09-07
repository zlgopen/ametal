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
 * \brief HC32 芯片外设寄存器基址定义
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-03  zp, first implementation
 * \endinternal
 */

#ifndef __HC32F07X_REGBASE_H
#define __HC32F07X_REGBASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup hc32_if_regbase
 * \copydoc hc32_regbase.h
 * @{
 */

/**
 * \name 外设内存映射地址定义
 * @{
 */

#define HC32F07X_UART0_BASE              (0x40000000UL)  /** \brief UART0基地址 */
#define HC32F07X_UART1_BASE              (0x40000100UL)  /** \brief UART1基地址 */
#define HC32F07X_UART2_BASE              (0x40006000UL)  /** \brief UART2基地址 */
#define HC32F07X_UART3_BASE              (0x40006400UL)  /** \brief UART3基地址 */
#define HC32F07X_LPUART0_BASE            (0x40000200UL)  /** \brief LPUART0基地址 */
#define HC32F07X_LPUART1_BASE            (0x40004000UL)  /** \brief LPUART1基地址 */
#define HC32F07X_I2C0_BASE               (0x40000400UL)  /** \brief I2C0基地址 */
#define HC32F07X_I2C1_BASE               (0x40004400UL)  /** \brief I2C1基地址 */
#define HC32F07X_SPI0_BASE               (0x40000800UL)  /** \brief SPI0基地址 */
#define HC32F07X_SPI1_BASE               (0x40004800UL)  /** \brief SPI1基地址 */
#define HC32F07X_TIM0_BASE               (0x40000C00UL)  /** \brief TIM0基地址 */
#define HC32F07X_TIM1_BASE               (0x40000D00UL)  /** \brief TIM1基地址 */
#define HC32F07X_TIM2_BASE               (0x40000E00UL)  /** \brief TIM2基地址 */
#define HC32F07X_TIM3_BASE               (0x40005800UL)  /** \brief TIM3基地址 */
#define HC32F07X_TIM4_BASE               (0x40003000UL)  /** \brief TIM4基地址 */
#define HC32F07X_TIM5_BASE               (0x40003400UL)  /** \brief TIM5基地址 */
#define HC32F07X_TIM6_BASE               (0x40003800UL)  /** \brief TIM6基地址 */
#define HC32F07X_LPTIM0_BASE             (0x40000F00UL)  /** \brief LPTIM0基地址 */
#define HC32F07X_LPTIM1_BASE             (0x40000F40UL)  /** \brief LPTIM1基地址 */
#define HC32F07X_AES_BASE                (0x40021400UL)  /** \brief AES基地址 */
#define HC32F07X_ADC_BASE                (0x40002400UL)  /** \brief ADC基地址 */
#define HC32F07X_CLKTRIM_BASE            (0x40001800UL)  /** \brief CLKTRIM基地址 */
#define HC32F07X_CRC_BASE                (0x40020900UL)  /** \brief CRC基地址 */
#define HC32F07X_DAC_BASE                (0x40002500UL)  /** \brief DAC基地址 */
#define HC32F07X_DMA_BASE                (0x40021000UL)  /** \brief DMA基地址 */
#define HC32F07X_FLASH_BASE              (0x40020000UL)  /** \brief FLASH基地址 */
#define HC32F07X_PORT0_BASE              (0x40020C00UL)  /** \brief PORT0基地址 */
#define HC32F07X_PORT1_BASE              (0x40022000UL)  /** \brief PORT1基地址 */
#define HC32F07X_LCD_BASE                (0x40005C00UL)  /** \brief LCDCTRL基地址 */
#define HC32F07X_LVD_BASE                (0x40002400UL)  /** \brief LVD基地址 */
#define HC32F07X_RCC_BASE                (0x40002000UL)  /** \brief RCC基地址 */
#define HC32F07X_OPA_BASE                (0x40002400UL)  /** \brief OPA基地址 */
#define HC32F07X_PCNT_BASE               (0x40005400UL)  /** \brief PCNT基地址 */
#define HC32F07X_RAM_BASE                (0x40020400UL)  /** \brief RAM基地址 */
#define HC32F07X_PCA_BASE                (0x40001000UL)  /** \brief PCA基地址 */
#define HC32F07X_RTC_BASE                (0x40001400UL)  /** \brief RTC基地址 */
#define HC32F07X_TRNG_BASE               (0x40004C00UL)  /** \brief TRNG基地址 */
#define HC32F07X_VC_BASE                 (0x40002410UL)  /** \brief VC基地址 */
#define HC32F07X_WDT_BASE                (0x40000F80UL)  /** \brief WDT基地址 */
#define HC32F07X_BGR_BASE                (0x40002400UL)  /** \brief BGR基地址 */
#define HC32F07X_SYSTICK                 (0xE000E010UL)  /** \brief SYSTICK基地址 */
#define HC32F07X_CAN_BASE                (0x40030000UL)  /** \brief CAN基地址 */
#define HC32F07X_USB_BASE                (0x40040000UL)  /** \brief USB基地址 */
#define HC32F07X_I2S0_BASE               (0x40002800UL)  /** \brief I2S0基地址 */
#define HC32F07X_I2S1_BASE               (0x40002C00UL)  /** \brief I2S1基地址 */
#define HC32F07X_HDIV_BASE               (0x40021800UL)  /** \brief HDIV基地址 */
#define HC32F07X_CTS_BASE                (0x40005000UL)  /** \brief CTS基地址 */

/** @} */

/**
 * @} hc32_if_regbase
 */

#ifdef __cplusplus
}
#endif

#endif/* __HC32_REGBASE_H */

/* end of file */
