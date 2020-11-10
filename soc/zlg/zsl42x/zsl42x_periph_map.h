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
 * \brief ZSL42x 外设映射
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-03  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_PERIPH_MAP_H
#define __AMHW_ZSL42x_PERIPH_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "zsl42x_regbase.h"

#ifdef ZSL42xX3X
#include "hw/amhw_zsl42xx3x_opa.h"
#else
#include "hw/amhw_zsl42x_opa.h"
#endif

#include "hw/amhw_zsl42x_adc.h"
#include "hw/amhw_zsl42x_adtim.h"
#include "hw/amhw_zsl42x_aes.h"
#include "hw/amhw_zsl42x_clktrim.h"
#include "hw/amhw_zsl42x_crc.h"
#include "hw/amhw_zsl42x_dac.h"
#include "hw/amhw_zsl42x_dma.h"
#include "hw/amhw_zsl42x_flash.h"
#include "hw/amhw_zsl42x_gpio.h"
#include "hw/amhw_zsl42x_i2c.h"
#include "hw/amhw_zsl42x_lcd.h"
#include "hw/amhw_zsl42x_lpuart.h"
#include "hw/amhw_zsl42x_lvd.h"
#include "hw/amhw_zsl42x_pca.h"
#include "hw/amhw_zsl42x_pcnt.h"
#include "hw/amhw_zsl42x_ram.h"
#include "hw/amhw_zsl42x_rcc.h"
#include "hw/amhw_zsl42x_rtc.h"
#include "hw/amhw_zsl42x_spi.h"
#include "hw/amhw_zsl42x_tim.h"
#include "hw/amhw_zsl42x_trng.h"
#include "hw/amhw_zsl42x_uart.h"
#include "hw/amhw_zsl42x_vc.h"
#include "hw/amhw_zsl42x_wdt.h"
/**
 * \addtogroup amhw_zsl42x_if_periph_map
 * \copydoc amhw_zsl42x_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */

#define ZSL42x_UART0          ((amhw_zsl42x_uart_t *)ZSL42x_UART0_BASE)
#define ZSL42x_UART1          ((amhw_zsl42x_uart_t *)ZSL42x_UART1_BASE)
#define ZSL42x_UART2          ((amhw_zsl42x_uart_t *)ZSL42x_UART2_BASE)
#define ZSL42x_UART3          ((amhw_zsl42x_uart_t *)ZSL42x_UART3_BASE)
#define ZSL42x_LPUART0        ((amhw_zsl42x_lpuart_t *)ZSL42x_LPUART0_BASE)
#define ZSL42x_LPUART1        ((amhw_zsl42x_lpuart_t *)ZSL42x_LPUART1_BASE)
#define ZSL42x_I2C0           ((amhw_zsl42x_i2c_t *)ZSL42x_I2C0_BASE)
#define ZSL42x_I2C1           ((amhw_zsl42x_i2c_t *)ZSL42x_I2C1_BASE)
#define ZSL42x_SPI0           ((amhw_zsl42x_spi_t *)ZSL42x_SPI0_BASE)
#define ZSL42x_SPI1           ((amhw_zsl42x_spi_t *)ZSL42x_SPI1_BASE)
#define ZSL42x_TIM0           ((amhw_zsl42x_tim_t *)ZSL42x_TIM0_BASE)
#define ZSL42x_TIM1           ((amhw_zsl42x_tim_t *)ZSL42x_TIM1_BASE)
#define ZSL42x_TIM2           ((amhw_zsl42x_tim_t *)ZSL42x_TIM2_BASE)
#define ZSL42x_TIM3           ((amhw_zsl42x_tim_t *)ZSL42x_TIM3_BASE)
#define ZSL42x_TIM4           ((amhw_zsl42x_adtim_t *)ZSL42x_TIM4_BASE)
#define ZSL42x_TIM5           ((amhw_zsl42x_adtim_t *)ZSL42x_TIM5_BASE)
#define ZSL42x_TIM6           ((amhw_zsl42x_adtim_t *)ZSL42x_TIM6_BASE)
#define ZSL42x_LPTIM0         ((amhw_zsl42x_lptim_t *)ZSL42x_LPTIM0_BASE)
#define ZSL42x_LPTIM1         ((amhw_zsl42x_lptim_t *)ZSL42x_LPTIM1_BASE)
#define ZSL42x_AES            ((amhw_zsl42x_aes_t *)ZSL42x_AES_BASE)
#define ZSL42x_ADC            ((amhw_zsl42x_adc_t *)ZSL42x_ADC_BASE)
#define ZSL42x_CLKTRIM        ((amhw_zsl42x_clktrim_t *)ZSL42x_CLKTRIM_BASE)
#define ZSL42x_CRC            ((amhw_zsl42x_crc_t *)ZSL42x_CRC_BASE)
#define ZSL42x_DAC            ((amhw_zsl42x_dac_t *)ZSL42x_DAC_BASE)
#define ZSL42x_DMA            ((amhw_zsl42x_dma_t *)ZSL42x_DMA_BASE)
#define ZSL42x_FLASH          ((amhw_zsl42x_flash_t *)ZSL42x_FLASH_BASE)
#define ZSL42x_GPIO0          ((amhw_zsl42x_gpio_t *)ZSL42x_PORT0_BASE)
#define ZSL42x_LCD            ((amhw_zsl42x_lcd_t *)ZSL42x_LCD_BASE)
#define ZSL42x_LVD            ((amhw_zsl42x_lvd_t *)ZSL42x_LVD_BASE)
#define ZSL42x_RCC            ((amhw_zsl42x_rcc_t *)ZSL42x_RCC_BASE)
#define ZSL42x_OPA            ((amhw_zsl42x_opa_t *)ZSL42x_OPA_BASE)
#define ZSL42x_PCNT           ((amhw_zsl42x_pcnt_t *)ZSL42x_PCNT_BASE)
#define ZSL42x_RAM            ((amhw_zsl42x_ram_t *)ZSL42x_RAM_BASE)
#define ZSL42x_PCA            ((amhw_zsl42x_pca_t *)ZSL42x_PCA_BASE)
#define ZSL42x_RTC            ((amhw_zsl42x_rtc_t *)ZSL42x_RTC_BASE)
#define ZSL42x_TRNG           ((amhw_zsl42x_trng_t *)ZSL42x_TRNG_BASE)
#define ZSL42x_VC             ((amhw_zsl42x_vc_t *)ZSL42x_VC_BASE)
#define ZSL42x_WDT            ((amhw_zsl42x_wdt_t *)ZSL42x_WDT_BASE)
/** @} */


typedef struct {
    __IO uint32_t cr;  /**< \brief 基本配置寄存器 */
} amhw_zsl42x_bgr_t;

#define ZSL42x_BGR     ((amhw_zsl42x_bgr_t *)ZSL42x_BGR_BASE)

/**
 * \brief 内置温度传感器使能控制
 *
 * \param[in] stat : AM_TRUE : 使能
 *                   AM_FALSE: 禁能
 *
 * \return none
 */
am_static_inline
void amhw_zsl42x_bgr_temp_sensor_enable (am_bool_t stat)
{
    ZSL42x_BGR->cr |= (stat << 1);
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
void amhw_zsl42x_bgr_enable (am_bool_t stat)
{
    ZSL42x_BGR->cr |= (stat << 0);
}

/**
 * @} amhw_zsl42x_if_periph_map
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZSL42x_REG_BASE_H */

/* end of file */
