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
 * \brief HC32F460 芯片外设寄存器基址定义
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-23  cds, first implementation
 * \endinternal
 */

#ifndef __HC32F460_REGBASE_H
#define __HC32F460_REGBASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup hc32f460_if_regbase
 * \copydoc hc32f460_regbase.h
 * @{
 */

#define HC32F460_SYSC_BASE          (0x40054000UL)  /** \brief SYSC基地址 */
#define HC32F460_MSTP_BASE          (0x40048000UL)  /** \brief MSTP基地址 */
#define HC32F460_GPIO_BASE          (0x40053800UL)  /** \brief GPIO基地址 */
#define HC32F460_EFM_BASE           (0x40010400UL)  /** \brief 嵌入式FLASH（EFM）基地址 */
#define HC32F460_INTC_BASE          (0x40051000UL)  /** \brief INTC基地址 */
#define HC32F460_UART1_BASE         (0x4001D000UL)  /** \brief UART1基地址 */
#define HC32F460_UART2_BASE         (0x4001D400UL)  /** \brief UART2基地址 */
#define HC32F460_UART3_BASE         (0x40021000UL)  /** \brief UART3基地址 */
#define HC32F460_UART4_BASE         (0x40021400UL)  /** \brief UART4基地址 */
#define HC32F460_EVENT_PORT_BASE    (0x40010800UL)  /** \brief EVENT_PORT基地址 */
#define HC32F460_SYSTICK_BASE       (0xE000E010UL)  /** \brief SYSTICK基地址 */
#define HC32F460_DMA1_BASE          (0x40053000UL)  /** \brief DMA1基地址 */
#define HC32F460_DMA2_BASE          (0x40053400UL)  /** \brief DMA2基地址 */
#define HC32F460_AOS_BASE           (0x40010800UL)  /** \brief AOS基地址 */
#define HC32F460_TIMER0_1_BASE      (0x40024000UL)  /** \brief TIMER01基地址 */
#define HC32F460_TIMER0_2_BASE      (0x40024400UL)  /** \brief TIMER02基地址 */
#define HC32F460_TMRA1_BASE         (0x40015000UL)  /** \brief TIMERA01基地址 */
#define HC32F460_TMRA2_BASE         (0x40015400UL)  /** \brief TIMERA02基地址 */
#define HC32F460_TMRA3_BASE         (0x40015800UL)  /** \brief TIMERA03基地址 */
#define HC32F460_TMRA4_BASE         (0x40015C00UL)  /** \brief TIMERA04基地址 */
#define HC32F460_TMRA5_BASE         (0x40016000UL)  /** \brief TIMERA05基地址 */
#define HC32F460_TMRA6_BASE         (0x40016400UL)  /** \brief TIMERA06基地址 */
#define HC32F460_TMR41_BASE         (0x40017000UL)  /** \brief TIMER401基地址 */
#define HC32F460_TMR42_BASE         (0x40024800UL)  /** \brief TIMER402基地址 */
#define HC32F460_TMR43_BASE         (0x40024C00UL)  /** \brief TIMER403基地址 */
#define HC32F460_TMR4_CR            (0x40055408UL)  /** \brief TIMER4_CR基地址 */
#define HC32F460_TMR61_BASE         (0x40018000UL)  /** \brief TIMER601基地址 */
#define HC32F460_TMR62_BASE         (0x40018400UL)  /** \brief TIMER602基地址 */
#define HC32F460_TMR63_BASE         (0x40018800UL)  /** \brief TIMER603基地址 */
#define HC32F460_TMR6_CR            (0x40018000UL)  /** \brief TIMER6_CR基地址 */
#define HC32F460_I2C1_BASE          (0x4004E000UL)  /** \brief I2C1基地址 */
#define HC32F460_I2C2_BASE          (0x4004E400UL)  /** \brief I2C2基地址 */
#define HC32F460_I2C3_BASE          (0x4004E800UL)  /** \brief I2C3基地址 */
#define HC32F460_RTC_BASE           (0x4004C000UL)  /** \brief RTC基地址 */
#define HC32F460_CAN_BASE           (0x40070400UL)  /** \brief CAN基地址 */
#define HC32F460_QSPI_BASE          (0x9C000000UL)  /** \brief QSPI基地址 */
#define HC32F460_SPI1_BASE          (0x4001C000UL)  /** \brief SPI1基地址 */
#define HC32F460_SPI2_BASE          (0x4001C400UL)  /** \brief SPI2基地址 */
#define HC32F460_SPI3_BASE          (0x40020000UL)  /** \brief SPI3基地址 */
#define HC32F460_SPI4_BASE          (0x40020400UL)  /** \brief SPI4基地址 */
#define HC32F460_ADC1_BASE          (0x40040000UL)  /** \brief ADC1基地址 */
#define HC32F460_ADC2_BASE          (0x40040400UL)  /** \brief ADC2基地址 */
#define HC32F460_SDIOC1_BASE        (0x4006FC00UL)  /** \brief SDIOC1基地址 */
#define HC32F460_SDIOC2_BASE        (0x40070000UL)  /** \brief SDIOC2基地址 */
#define HC32F460_WDT_BASE           (0x40049000UL)  /** \brief WDT基地址 */
#define HC32F460_SWDT_BASE          (0x40049400UL)  /** \brief SWDT基地址 */
#define HC32F460_MPU_BASE           (0x40050000UL)  /** \brief MPU基地址 */
#define HC32F460_HASH_BASE          (0x40008400UL)  /** \brief HASH基地址 */
#define HC32F460_TRNG_BASE          (0x40041000UL)  /** \brief TRNG基地址 */
#define HC32F460_AES_BASE           (0x40008000UL)  /** \brief AES基地址 */
#define HC32F460_DCU1_BASE          (0x40052000UL)  /** \brief DCU1基地址 */
#define HC32F460_DCU2_BASE          (0x40052400UL)  /** \brief DCU2基地址 */
#define HC32F460_DCU3_BASE          (0x40052800UL)  /** \brief DCU3基地址 */
#define HC32F460_DCU4_BASE          (0x40052C00UL)  /** \brief DCU4基地址 */
#define HC32F460_CMP1_BASE          (0x4004A000UL)  /** \brief CMP1基地址 */
#define HC32F460_CMP2_BASE          (0x4004A010UL)  /** \brief CMP2基地址 */
#define HC32F460_CMP3_BASE          (0x4004A020UL)  /** \brief CMP3基地址 */
#define HC32F460_CMP_CR_BASE        (0x4004A000UL)  /** \brief CMP_CR基地址 */
#define HC32F460_USB_OTG_FS_BASE    (0x400C0000UL)  /** \brief USB基地址 */
#define HC32F460_CRC_BASE           (0x40008C00UL)  /** \brief CRC基地址 */
/**
 * \name 外设内存映射地址定义
 * @{
 */


/** @} */

/**
 * @} hc32f460_if_regbase
 */

#ifdef __cplusplus
}
#endif

#endif/* __HC32F460_REGBASE_H */

/* end of file */
