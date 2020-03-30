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
 * \brief ZSN700 外设映射
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-18  YRZ, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZSN700_PERIPH_MAP_H
#define __AMHW_ZSN700_PERIPH_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "zsn700_regbase.h"

#include "hw/amhw_zsn700_adc.h"
#include "hw/amhw_zsn700_adtim.h"
#include "hw/amhw_zsn700_aes.h"
#include "hw/amhw_zsn700_clktrim.h"
#include "hw/amhw_zsn700_crc.h"
#include "hw/amhw_zsn700_dac.h"
#include "hw/amhw_zsn700_dma.h"
#include "hw/amhw_zsn700_flash.h"
#include "hw/amhw_zsn700_gpio.h"
#include "hw/amhw_zsn700_i2c.h"
#include "hw/amhw_zsn700_lcd.h"
#include "hw/amhw_zsn700_lptim.h"
#include "hw/amhw_zsn700_lpuart.h"
#include "hw/amhw_zsn700_lvd.h"
#include "hw/amhw_zsn700_opa.h"
#include "hw/amhw_zsn700_pca.h"
#include "hw/amhw_zsn700_pcnt.h"
#include "hw/amhw_zsn700_ram.h"
#include "hw/amhw_zsn700_rcc.h"
#include "hw/amhw_zsn700_rtc.h"
#include "hw/amhw_zsn700_spi.h"
#include "hw/amhw_zsn700_tim.h"
#include "hw/amhw_zsn700_trng.h"
#include "hw/amhw_zsn700_uart.h"
#include "hw/amhw_zsn700_vc.h"
#include "hw/amhw_zsn700_wdt.h"
/**
 * \addtogroup amhw_zsn700_if_periph_map
 * \copydoc amhw_zsn700_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */

#define ZSN700_UART0          ((amhw_zsn700_uart_t *)ZSN700_UART0_BASE)
#define ZSN700_UART1          ((amhw_zsn700_uart_t *)ZSN700_UART1_BASE)
#define ZSN700_UART2          ((amhw_zsn700_uart_t *)ZSN700_UART2_BASE)
#define ZSN700_UART3          ((amhw_zsn700_uart_t *)ZSN700_UART3_BASE)
#define ZSN700_LPUART0        ((amhw_zsn700_lpuart_t *)ZSN700_LPUART0_BASE)
#define ZSN700_LPUART1        ((amhw_zsn700_lpuart_t *)ZSN700_LPUART1_BASE)
#define ZSN700_I2C0           ((amhw_zsn700_i2c_t *)ZSN700_I2C0_BASE)
#define ZSN700_I2C1           ((amhw_zsn700_i2c_t *)ZSN700_I2C1_BASE)
#define ZSN700_SPI0           ((amhw_zsn700_spi_t *)ZSN700_SPI0_BASE)
#define ZSN700_SPI1           ((amhw_zsn700_spi_t *)ZSN700_SPI1_BASE)
#define ZSN700_TIM0           ((amhw_zsn700_tim_t *)ZSN700_TIM0_BASE)
#define ZSN700_TIM1           ((amhw_zsn700_tim_t *)ZSN700_TIM1_BASE)
#define ZSN700_TIM2           ((amhw_zsn700_tim_t *)ZSN700_TIM2_BASE)
#define ZSN700_TIM3           ((amhw_zsn700_tim_t *)ZSN700_TIM3_BASE)
#define ZSN700_TIM4           ((amhw_zsn700_adtim_t *)ZSN700_TIM4_BASE)
#define ZSN700_TIM5           ((amhw_zsn700_adtim_t *)ZSN700_TIM5_BASE)
#define ZSN700_TIM6           ((amhw_zsn700_adtim_t *)ZSN700_TIM6_BASE)
#define ZSN700_LPTIM0         ((amhw_zsn700_lptim_t *)ZSN700_LPTIM0_BASE)
#define ZSN700_LPTIM1         ((amhw_zsn700_lptim_t *)ZSN700_LPTIM1_BASE)
#define ZSN700_AES            ((amhw_zsn700_aes_t *)ZSN700_AES_BASE)
#define ZSN700_ADC            ((amhw_zsn700_adc_t *)ZSN700_ADC_BASE)
#define ZSN700_CLKTRIM        ((amhw_zsn700_clktrim_t *)ZSN700_CLKTRIM_BASE)
#define ZSN700_CRC            ((amhw_zsn700_crc_t *)ZSN700_CRC_BASE)
#define ZSN700_DAC            ((amhw_zsn700_dac_t *)ZSN700_DAC_BASE)
#define ZSN700_DMA            ((amhw_zsn700_dma_t *)ZSN700_DMA_BASE)
#define ZSN700_FLASH          ((amhw_zsn700_flash_t *)ZSN700_FLASH_BASE)
#define ZSN700_GPIO0          ((amhw_zsn700_gpio_t *)ZSN700_PORT0_BASE)
#define ZSN700_LCD            ((amhw_zsn700_lcd_t *)ZSN700_LCD_BASE)
#define ZSN700_LVD            ((amhw_zsn700_lvd_t *)ZSN700_LVD_BASE)
#define ZSN700_RCC            ((amhw_zsn700_rcc_t *)ZSN700_RCC_BASE)
#define ZSN700_OPA            ((amhw_zsn700_opa_t *)ZSN700_OPA_BASE)
#define ZSN700_PCNT           ((amhw_zsn700_pcnt_t *)ZSN700_PCNT_BASE)
#define ZSN700_RAM            ((amhw_zsn700_ram_t *)ZSN700_RAM_BASE)
#define ZSN700_PCA            ((amhw_zsn700_pca_t *)ZSN700_PCA_BASE)
#define ZSN700_RTC            ((amhw_zsn700_rtc_t *)ZSN700_RTC_BASE)
#define ZSN700_TRNG           ((amhw_zsn700_trng_t *)ZSN700_TRNG_BASE)
#define ZSN700_VC             ((amhw_zsn700_vc_t *)ZSN700_VC_BASE)
#define ZSN700_WDT            ((amhw_zsn700_wdt_t *)ZSN700_WDT_BASE)
/** @} */


typedef struct {
    __IO uint32_t cr;  /**< \brief 基本配置寄存器 */
} amhw_zsn700_bgr_t;

#define ZSN700_BGR     ((amhw_zsn700_bgr_t *)ZSN700_BGR_BASE)

/**
 * \brief 内置温度传感器使能控制
 *
 * \param[in] stat : AM_TRUE : 使能
 *                   AM_FALSE: 禁能
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_bgr_temp_sensor_enable (am_bool_t stat)
{
    ZSN700_BGR->cr |= (stat << 1);
}

/**
 * \brief BGR使能控制
 *
 * \param[in] stat : AM_TRUE : 使能
 *                   AM_FALSE: 禁能
 *
 * \return none
 */
am_static_inline
void amhw_zsn700_bgr_enable (am_bool_t stat)
{
    ZSN700_BGR->cr |= (stat << 0);
}

/**
 * @} amhw_zsn700_if_periph_map
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZSN700_REG_BASE_H */

/* end of file */
