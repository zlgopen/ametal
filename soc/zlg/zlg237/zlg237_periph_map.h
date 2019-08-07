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
 * \brief ZLG237 外设映射
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-26  yrh, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG237_PERIPH_MAP_H
#define __AMHW_ZLG237_PERIPH_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "zlg237_regbase.h"
#include "hw/amhw_zlg237_adc.h"
#include "hw/amhw_zlg237_bkp.h"
#include "hw/amhw_zlg_cmp.h"
#include "hw/amhw_zlg_crc.h"
#include "hw/amhw_zlg237_flash.h"
#include "hw/amhw_zlg_i2c.h"
#include "hw/amhw_zlg237_rtc.h"
#include "hw/amhw_zlg237_spi.h"
#include "hw/amhw_zlg_tim.h"
#include "hw/amhw_zlg237_usart.h"
#include "hw/amhw_zlg_iwdg.h"
#include "hw/amhw_zlg_wwdg.h"
#include "hw/amhw_zlg237_pwr.h"
#include "hw/amhw_zlg237_exti.h"
#include "hw/amhw_zlg_gpio.h"
#include "hw/amhw_zlg_dma.h"
//#include "hw/amhw_zlg237_usbd.h"

/**
 * \addtogroup amhw_zlg237_if_periph_map
 * \copydoc amhw_zlg237_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */

/** \brief GPIO端口A寄存器块指针 */
#define ZLG237_GPIOA   ((amhw_zlg237_gpio_reg_t  *)ZLG237_GPIOA_BASE)

/** \brief GPIO端口B寄存器块指针 */
#define ZLG237_GPIOB   ((amhw_zlg237_gpio_reg_t  *)ZLG237_GPIOB_BASE)

/** \brief GPIO端口C寄存器块指针 */
#define ZLG237_GPIOC   ((amhw_zlg237_gpio_reg_t  *)ZLG237_GPIOC_BASE)

/** \brief GPIO端口D寄存器块指针 */
#define ZLG237_GPIOD   ((amhw_zlg237_gpio_reg_t  *)ZLG237_GPIOD_BASE)

/** \brief GPIO寄存器块指针 */
#define ZLG237_GPIO    ((amhw_zlg237_gpio_t      *)ZLG237_GPIO_BASE)

/** \brief AFIO寄存器块指针 */
#define ZLG237_AFIO    ((amhw_zlg237_afio_t      *)ZLG237_AFIO_BASE)

/** \brief TIM1寄存器块指针 */
#define ZLG237_TIM1    ((amhw_zlg_tim_t          *)ZLG237_TIM1_BASE)

/** \brief 通用定时器2寄存器块指针 */
#define ZLG237_TIM2    ((amhw_zlg_tim_t          *)ZLG237_TIM2_BASE)

/** \brief 通用定时器3寄存器块指针 */
#define ZLG237_TIM3    ((amhw_zlg_tim_t          *)ZLG237_TIM3_BASE)

/** \brief 通用定时器4寄存器块指针 */
#define ZLG237_TIM4    ((amhw_zlg_tim_t          *)ZLG237_TIM4_BASE)

/** \brief RTC寄存器块指针 */
#define ZLG237_RTC     ((amhw_zlg237_rtc_t       *)ZLG237_RTC_BASE)

/** \brief 电源控制PWR寄存器块指针 */
#define ZLG237_PWR     ((amhw_zlg237_pwr_t       *)ZLG237_PWR_BASE)

/** \brief 备份控制BKP寄存器块指针 */
#define ZLG237_BKP     ((amhw_zlg237_bkp_t       *)ZLG237_BKP_BASE)

/** \brief UART1 寄存器块指针 */
#define ZLG237_USART1  ((amhw_zlg237_usart_t     *)ZLG237_USART1_BASE)

/** \brief UART2 寄存器块指针 */
#define ZLG237_USART2  ((amhw_zlg237_usart_t     *)ZLG237_USART2_BASE)

/** \brief UART3 寄存器块指针 */
#define ZLG237_USART3  ((amhw_zlg237_usart_t     *)ZLG237_USART3_BASE)

/** \brief ADC1 寄存器块指针 */
#define ZLG237_ADC1    ((amhw_zlg237_adc_t       *)ZLG237_ADC1_BASE)

/** \brief ADC2 寄存器块指针 */
#define ZLG237_ADC2    ((amhw_zlg237_adc_t       *)ZLG237_ADC2_BASE)

/** \brief DMA 寄存器块指针 */
#define ZLG237_DMA     ((amhw_zlg237_dma_t       *)ZLG237_DMA_BASE)

/** \brief RCC控制器寄存器块指针 */
#define ZLG237_RCC     ((amhw_zlg237_rcc_t       *)ZLG237_RCC_BASE)

/** \brief SPI1控制器寄存器块指针 */
#define ZLG237_SPI1    ((amhw_zlg237_spi_t       *)ZLG237_SPI1_BASE)

/** \brief SPI2控制器寄存器块指针 */
#define ZLG237_SPI2    ((amhw_zlg237_spi_t       *)ZLG237_SPI2_BASE)

/** \brief I2C1寄存器块指针 */
#define ZLG237_I2C1    ((amhw_zlg237_i2c_t       *)ZLG237_I2C1_BASE)

/** \brief I2C2寄存器块指针 */
#define ZLG237_I2C2    ((amhw_zlg237_i2c_t       *)ZLG237_I2C2_BASE)

/** \brief IWDG控制器寄存器块指针 */
#define ZLG237_IWDG    ((amhw_zlg_iwdg_t         *)ZLG237_IWDG_BASE)

/** \brief WWDG控制器寄存器块指针 */
#define ZLG237_WWDG    ((amhw_zlg_wwdg_t         *)ZLG237_WWDG_BASE)

/** \brief CAN控制器寄存器块指针 */
#define ZLG237_CAN     ((amhw_zlg237_can_t       *)ZLG237_CAN_BASE)

/** \brief USB控制器寄存器块指针 */
#define ZLG237_USB     ((amhw_zlg237_usb_t       *)ZLG237_USB_BASE)

/** \brief 外部中断(事件)控制器EXTI寄存器块指针 */
#define ZLG237_EXTI    ((amhw_zlg237_exti_t      *)ZLG237_EXTI_BASE)

/** \brief FLASH寄存器块指针 */
#define ZLG237_FLASH   ((amhw_zlg237_flash_t     *)ZLG237_FLASH_BASE)

/** \brief CRC寄存器块指针 */
#define ZLG237_CRC     (amhw_zlg_crc_t           *)(ZLG237_CRC_BASE)

/** \brief CMP 寄存器块指针 */
#define ZLG237_CMP     ((amhw_zlg_cmp_t           *)ZLG237_CMP_BASE)
/** @} */

/**
 * @} amhw_zlg237_if_periph_map
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG237_REG_BASE_H */

/* end of file */
