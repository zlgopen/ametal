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
 * \brief KL26芯片外设寄存器基址定义
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-13  mkr, first implementation.
 * \endinternal
 */

#ifndef __KL26_REGBASE_H
#define __KL26_REGBASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup kl26_if_regbase
 * \copydoc kl26_regbase.h
 * @{
 */

/**
 * \name 外设内存映射地址定义
 * @{
 */
#define KL26_NVIC_BASE           (0xE000E100UL)  /**< \brief NVIC基地址 */
#define KL26_SIM_BASE            (0x40047000UL)  /**< \brief SIM基地址 */
#define KL26_TPM0_BASE           (0x40038000UL)  /**< \brief TPM0基地址 */
#define KL26_TPM1_BASE           (0x40039000UL)  /**< \brief TPM1基地址 */
#define KL26_TPM2_BASE           (0x4003A000UL)  /**< \brief TPM2基地址 */
#define KL26_MCG_BASE            (0x40064000UL)  /**< \brief MCG基地址 */
#define KL26_OSC_BASE            (0x40065000UL)  /**< \brief OSC基地址 */
#define KL26_PORT_BASE           (0x40049000UL)  /**< \brief PORT端口基地址 */
#define KL26_GPIO_BASE           (0x400FF000UL)  /**< \brief GPIO通道基地址 */
#define KL26_FGPIO_BASE          (0xF8000000UL)  /**< \brief FGPIO通道基地址 */
#define KL26_UART0_BASE          (0x4006A000UL)  /**< \brief UART0基地址 */
#define KL26_UART1_BASE          (0x4006B000UL)  /**< \brief UART1基地址 */
#define KL26_UART2_BASE          (0x4006C000UL)  /**< \brief UART2基地址 */
#define KL26_USBOTG_BASE         (0x40072000UL)  /**< \brief USBOTG基地址 */
#define KL26_DAC0_BASE           (0x4003F000UL)  /**< \brief DAC0基地址 */
#define KL26_ADC0_BASE           (0x4003B000UL)  /**< \brief ADC0基地址 */
#define KL26_SPI0_BASE           (0x40076000UL)  /**< \brief SPI0基地址 */
#define KL26_SPI1_BASE           (0x40077000UL)  /**< \brief SPI1基地址 */
#define KL26_I2C0_BASE           (0x40066000UL)  /**< \brief I2C0基地址 */
#define KL26_I2C1_BASE           (0x40067000UL)  /**< \brief I2C1基地址 */
#define KL26_CMP_BASE            (0x40073000UL)  /**< \brief CMP基地址 */
#define KL26_DMAMUX_BASE         (0x40021000UL)  /**< \brief DMAMUX基地址 */
#define KL26_DMA_BASE            (0x40008100UL)  /**< \brief DMA基地址 */
#define KL26_MCM_BASE            (0xf0003000UL)  /**< \brief MCM基地址 */
#define KL26_RTC_BASE            (0x4003D000UL)  /**< \brief RTC基地址 */
#define KL26_FTFA_BASE           (0x40020000UL)  /**< \brief FTFA基地址 */
#define KL26_PIT_BASE            (0x40037000UL)  /**< \brief PIT基地址 */
#define KL26_TSI_BASE            (0x40045000UL)  /**< \brief TSI基地址 */
#define KL26_LPTMR_BASE          (0x40040000UL)  /**< \brief LPTMR基地址 */
#define KL26_LLWU_BASE           (0x4007C000UL)  /**< \brief LLWU基地址 */
#define KL26_PMC_BASE            (0x4007D000UL)  /**< \brief PMC基地址 */
#define KL26_SMC_BASE            (0x4007E000UL)  /**< \brief SMC基地址 */
#define KL26_RCM_BASE            (0x4007F000UL)  /**< \brief RCM基地址 */
#define KL26_I2S0_BASE           (0x4002F000UL)  /**< \brief I2S0基地址 */
#define KL26_SYSTICK_BASE        (0xE000E010UL)  /**< \brief SYSTICK基地址 */

/** @} */

/**
 * @} kl26_if_regbase
 */

#ifdef __cplusplus
}
#endif


#endif  /* __KL26_REGBASE_H */

/* end of file */


