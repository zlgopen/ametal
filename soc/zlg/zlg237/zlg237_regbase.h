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
 * \brief ZLG237 芯片外设寄存器基址定义
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-26  yrh, first implementation
 * \endinternal
 */

#ifndef __ZLG237_REGBASE_H
#define __ZLG237_REGBASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup zlg237_if_regbase
 * \copydoc zlg237_regbase.h
 * @{
 */

/**
 * \name 外设内存映射地址定义
 * @{
 */

/** \brief GPIO基地址 */
#define ZLG237_GPIO_BASE              (0x40010800UL)

/** \brief GPIOA基地址 */
#define ZLG237_GPIOA_BASE             (0x40010800UL)

/** \brief GPIOB基地址 */
#define ZLG237_GPIOB_BASE             (0x40010C00UL)

/** \brief GPIOC基地址 */
#define ZLG237_GPIOC_BASE             (0x40011000UL)

/** \brief GPIOD基地址 */
#define ZLG237_GPIOD_BASE             (0x40011400UL)

/** \brief AFIOA基地址 */
#define ZLG237_AFIO_BASE              (0x40010000UL)

/** \brief 高级控制定时器1基地址 */
#define ZLG237_TIM1_BASE              (0x40012C00UL)

/** \brief 通用定时器2基地址 */
#define ZLG237_TIM2_BASE              (0x40000000UL)

/** \brief 通用定时器3基地址 */
#define ZLG237_TIM3_BASE              (0x40000400UL)

/** \brief 通用定时器4基地址 */
#define ZLG237_TIM4_BASE              (0x40000800UL)

/** \brief RTC基地址 */
#define ZLG237_RTC_BASE               (0x40002800UL)

/** \brief 电源控制PWR基地址 */
#define ZLG237_PWR_BASE               (0x40007000UL)

/** \brief 备份控制BKP基地址 */
#define ZLG237_BKP_BASE               (0x40006C00UL)

/** \brief 比较器基地址 */
#define ZLG237_CMP_BASE               (0x40013C00UL)

/** \brief UART1 基地址 */
#define ZLG237_USART1_BASE            (0x40013800UL)

/** \brief UART2 基地址 */
#define ZLG237_USART2_BASE            (0x40004400UL)

/** \brief UART3 基地址 */
#define ZLG237_USART3_BASE            (0x40004800UL)

/** \brief ADC1 基地址 */
#define ZLG237_ADC1_BASE              (0x40012400UL)

/** \brief ADC2 基地址 */
#define ZLG237_ADC2_BASE              (0x40012800UL)

/** \brief DAC 基地址 */
#define ZLG237_DAC_BASE               (0x40007400UL)

/** \brief DMA 基地址 */
#define ZLG237_DMA_BASE               (0x40020000UL)

/** \brief RCC控制器基地址 */
#define ZLG237_RCC_BASE               (0x40021000UL)

/** \brief SPI1控制器基地址 */
#define ZLG237_SPI1_BASE              (0x40013000UL)

/** \brief SPI2控制器基地址 */
#define ZLG237_SPI2_BASE              (0x40003800UL)

/** \brief I2C1基地址 */
#define ZLG237_I2C1_BASE              (0x40005400UL)

/** \brief I2C2基地址 */
#define ZLG237_I2C2_BASE              (0x40005800UL)

/** \brief IWDG控制器基地址 */
#define ZLG237_IWDG_BASE              (0x40003000UL)

/** \brief WWDG控制器基地址 */
#define ZLG237_WWDG_BASE              (0x40002C00UL)

/** \brief CAN控制器基地址 */
#define ZLG237_CAN_BASE               (0x40006400UL)

/** \brief USB控制器基地址 */
#define ZLG237_USB_BASE               (0x40005C00UL)

/** \brief 外部中断(事件)控制器EXTI基地址 */
#define ZLG237_EXTI_BASE              (0x40010400UL)

/** \brief FLASH基地址 */
#define ZLG237_FLASH_BASE             (0x40022000UL)

/** \brief SRAM基地址 */
#define ZLG237_SRAM_BASE              (0x20000000UL)

/** \brief FLASH基地址 */
#define ZLG237_FLASH_UID_BASE         (0x1FFFF7E8UL)

/** \brief CRC基地址 */
#define ZLG237_CRC_BASE               (0x40023000UL)

/** \brief SYSTICK基地址 */
#define ZLG237_SYSTICK                (0xE000E010UL)

/** @} */

/**
 * @} zlg237_if_regbase
 */

#ifdef __cplusplus
}
#endif

#endif/* __ZLG237_REGBASE_H */

/* end of file */
