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
 * \brief MM32L073 外设映射
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-07  nwt, first implementation
 * \endinternal
 */

#ifndef __MM32L073_PERIPH_MAP_H
#define __MM32L073_PERIPH_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mm32l073_regbase.h"

#include "hw/amhw_mm32_adc.h"
#include "hw/amhw_mm32_bkp.h"
#include "hw/amhw_mm32_cmp.h"
#include "hw/amhw_mm32_crc.h"
#include "hw/amhw_mm32_dac.h"
#include "hw/amhw_mm32_flash.h"
#include "hw/amhw_mm32_i2c.h"
#include "hw/amhw_mm32_spi.h"
#include "hw/amhw_mm32_tim.h"
#include "hw/amhw_mm32_uart.h"
#include "hw/amhw_mm32_iwdg.h"
#include "hw/amhw_mm32_wwdg.h"
#include "hw/amhw_mm32_pwr.h"
#include "hw/amhw_mm32_exti.h"
#include "hw/amhw_mm32_syscfg.h"
#include "hw/amhw_mm32_gpio.h"
#include "hw/amhw_mm32_dma.h"

#include "hw/amhw_mm32l073_rcc.h"


/**
 * \addtogroup mm32l073_if_periph_map
 * \copydoc mm32l073_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */

/** \brief 电源管理单元(PWR)寄存器块指针 */
#define MM32L073_PWR     ((amhw_mm32_pwr_t           *)MM32L073_PWR_BASE)

/** \brief 备份区域(BKP)寄存器块指针 */
#define MM32L073_BKP     ((amhw_mm32_bkp_t           *)MM32L073_BKP_BASE)

/** \brief 系统配置(SYSCFG)寄存器块指针 */
#define MM32L073_SYSCFG  ((amhw_mm32_syscfg_t        *)MM32L073_SYSCFG_BASE)

/** \brief GPIO端口A寄存器块指针 */
#define MM32L073_GPIOA   ((amhw_mm32_gpio_t          *)MM32L073_GPIOA_BASE)

/** \brief GPIO端口B寄存器块指针 */
#define MM32L073_GPIOB   ((amhw_mm32_gpio_t          *)MM32L073_GPIOB_BASE)

/** \brief GPIO端口C寄存器块指针 */
#define MM32L073_GPIOC   ((amhw_mm32_gpio_t          *)MM32L073_GPIOC_BASE)

/** \brief GPIO端口D寄存器块指针 */
#define MM32L073_GPIOD   ((amhw_mm32_gpio_t          *)MM32L073_GPIOD_BASE)

/** \brief GPIO寄存器块指针 */
#define MM32L073_GPIO    ((amhw_mm32_gpio_t          *)MM32L073_GPIO_BASE)

/** \brief 16位高级定时器1寄存器指针 */
#define MM32L073_TIM1    ((amhw_mm32_tim_t           *)MM32L073_TIM1_BASE)

/** \brief 16位通用定时器14寄存器指针 */
#define MM32L073_TIM14   ((amhw_mm32_tim_t           *)MM32L073_TIM14_BASE)

/** \brief 16位通用定时器16寄存器指针 */
#define MM32L073_TIM16   ((amhw_mm32_tim_t           *)MM32L073_TIM16_BASE)

/** \brief 16位通用定时器17寄存器指针 */
#define MM32L073_TIM17   ((amhw_mm32_tim_t           *)MM32L073_TIM17_BASE)

/** \brief 16位通用定时器2寄存器指针 */
#define MM32L073_TIM2    ((amhw_mm32_tim_t           *)MM32L073_TIM2_BASE)

/** \brief 16位通用定时器3寄存器指针 */
#define MM32L073_TIM3    ((amhw_mm32_tim_t           *)MM32L073_TIM3_BASE)

/** \brief I2C总线控制器(I2C0)寄存器块指针 */
#define MM32L073_I2C     ((amhw_mm32_i2c_t           *)MM32L073_I2C1_BASE )

/** \brief 独立看门狗(IWDG)寄存器块指针 */
#define MM32L073_IWDG    ((amhw_mm32_iwdg_t          *)MM32L073_IWDG_BASE)

/** \brief 窗口看门狗(WWDG)寄存器块指针 */
#define MM32L073_WWDG    ((amhw_mm32_wwdg_t          *)MM32L073_WWDG_BASE)

/** \brief 串口(UART2)寄存器块指针 */
#define MM32L073_UART2   ((amhw_mm32_uart_t          *)MM32L073_UART2_BASE)

/** \brief 串口(UART1)寄存器块指针 */
#define MM32L073_UART1   ((amhw_mm32_uart_t          *)MM32L073_UART1_BASE)

/** \brief 模数转换(ADC)寄存器块指针 */
#define MM32L073_ADC     ((amhw_mm32_adc_t           *)MM32L073_ADC_BASE)

/** \brief SPI1 寄存器块指针 */
#define MM32L073_SPI1    ((amhw_mm32_spi_t           *)MM32L073_SPI1_BASE)

/** \brief SPI2 寄存器块指针 */
#define MM32L073_SPI2    ((amhw_mm32_spi_t           *)MM32L073_SPI2_BASE)

/** \brief DMA 控制器(DMA)寄存器块指针 */
#define MM32L073_DMA     ((amhw_mm32_dma_t           *)MM32L073_DMA_BASE)

/** \brief CMP 寄存器块指针 */
#define MM32L073_CMP     ((amhw_mm32_cmp_t           *)MM32L073_CMP_BASE)

/** \brief NVIC 寄存器块指针 */
#define MM32L073_INT     ((amhw_arm_nvic_t          *)MM32L073_NVIC_BASE)

/** \brief SYSTICK 寄存器块指针 */
#define MM32L073_SYSTICK ((amhw_arm_systick_t       *)MM32L073_SYSTICK_BASE)

/** \brief FLASH 寄存器块指针 */
#define MM32L073_FLASH   ((amhw_mm32_flash_t         *)MM32L073_FLASHCTR_BASE)

/** \brief RCC 寄存器块指针 */
#define MM32L073_RCC     ((amhw_mm32l073_rcc_t        *)MM32L073_RCC_BASE)

/** \brief EXTI 寄存器块指针 */
#define MM32L073_EXTI    ((amhw_mm32_exti_t          *)MM32L073_EXTI_BASE)

/** @} */

/**
 * @} mm32l073_if_periph_map
 */

#ifdef __cplusplus
}
#endif

#endif /* __MM32L073_PERIPH_MAP_H */

/* end of file */
