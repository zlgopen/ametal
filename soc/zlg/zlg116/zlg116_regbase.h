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
 * \brief ZLG116 芯片外设寄存器基址定义
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-07  nwt, first implementation
 * \endinternal
 */

#ifndef __ZLG116_REGBASE_H
#define __ZLG116_REGBASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup zlg116_if_regbase
 * \copydoc zlg116_regbase.h
 * @{
 */

/**
 * \name 外设内存映射地址定义
 * @{
 */

/** \brief GPIO基地址 */
#define ZLG116_GPIO_BASE              (0x48000000UL)

/** \brief GPIOA基地址 */
#define ZLG116_GPIOA_BASE             (0x48000000UL)

/** \brief GPIOB基地址 */
#define ZLG116_GPIOB_BASE             (0x48000400UL)

/** \brief GPIOC基地址 */
#define ZLG116_GPIOC_BASE             (0x48000800UL)

/** \brief GPIOD基地址 */
#define ZLG116_GPIOD_BASE             (0x48000C00UL)

/** \brief 高级控制定时器1基地址 */
#define ZLG116_TIM1_BASE              (0x40012C00UL)

/** \brief 通用定时器2基地址 */
#define ZLG116_TIM2_BASE              (0x40000000UL)

/** \brief 通用定时器3基地址 */
#define ZLG116_TIM3_BASE              (0x40000400UL)

/** \brief 16位通用定时器14基地址 */
#define ZLG116_TIM14_BASE             (0x40014000UL)

/** \brief 16位通用定时器16基地址 */
#define ZLG116_TIM16_BASE             (0x40014400UL)

/** \brief 16位通用定时器17基地址 */
#define ZLG116_TIM17_BASE             (0x40014800UL)

/** \brief SYSCFG基地址 */
#define ZLG116_SYSCFG_BASE            (0x40010000UL)

/** \brief 电源控制PWR基地址 */
#define ZLG116_PWR_BASE               (0x40007000UL)

/** \brief 备份控制BKP基地址 */
#define ZLG116_BKP_BASE               (0x40002800UL)

/** \brief UART1 基地址 */
#define ZLG116_UART1_BASE             (0x40013800UL)

/** \brief UART2 基地址 */
#define ZLG116_UART2_BASE             (0x40004400UL)

/** \brief ADC 基地址 */
#define ZLG116_ADC_BASE               (0x40012400UL)

/** \brief CMP 基地址 */
#define ZLG116_CMP_BASE               (0x40013C00UL)

/** \brief DMA 基地址 */
#define ZLG116_DMA_BASE               (0x40020000UL)

/** \brief FLASH控制器基地址 */
#define ZLG116_FLASHCTR_BASE          (0x40022000UL)

/** \brief RCC控制器基地址 */
#define ZLG116_RCC_BASE               (0x40021000UL)

/** \brief CRS控制器基地址 */
#define ZLG116_CRS_BASE               (0x40006C00UL)

/** \brief SPI1控制器基地址 */
#define ZLG116_SPI1_BASE              (0x40013000UL)

/** \brief SPI2控制器基地址 */
#define ZLG116_SPI2_BASE              (0x40003800UL)

/** \brief I2C1基地址 */
#define ZLG116_I2C1_BASE              (0x40005400UL)

/** \brief IWDG控制器基地址 */
#define ZLG116_IWDG_BASE              (0x40003000UL)

/** \brief WWDG控制器基地址 */
#define ZLG116_WWDG_BASE              (0x40002C00UL)

/** \brief DBGMCU 基地址 */
#define ZLG116_DBGMCU_BASE            (0x40005400UL)

/** \brief CAN控制器基地址 */
#define ZLG116_CAN_BASE               (0x40006400UL)

/** \brief USB控制器基地址 */
#define ZLG116_USB_BASE               (0x40005C00UL)

/** \brief 外部中断(事件)控制器EXTI基地址 */
#define ZLG116_EXTI_BASE              (0x40010400UL)

/** \brief NVIC基地址 */
#define ZLG116_NVIC_BASE              (0xE000E100UL)

/** \brief SYSTICK基地址 */
#define ZLG116_SYSTICK_BASE           (0xE000E010UL)

/** \brief FLASH基地址 */
#define ZLG116_FLASH_BASE             (0x40022000UL)

/** \brief SRAM基地址 */
#define ZLG116_SRAM_BASE              (0x20000000UL)

/** \brief DEVICE基地址 */
#define ZLG116_DEVICE_BASE            (0x1FFFF7E0UL)
/** @} */

/**
 * @} zlg116_if_regbase
 */

#ifdef __cplusplus
}
#endif

#endif /* __ZLG116_REGBASE_H */

/* end of file */
