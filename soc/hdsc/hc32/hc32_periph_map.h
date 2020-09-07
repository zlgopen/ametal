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
 * \brief HC32 外设映射
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-03  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32_PERIPH_MAP_H
#define __AMHW_HC32_PERIPH_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hc32_regbase.h"

#ifdef HC32X3X
#include "hw/amhw_hc32x3x_opa.h"
#else
#include "hw/amhw_hc32_opa.h"
#endif

#include "hw/amhw_hc32_adc.h"
#include "hw/amhw_hc32_adtim.h"
#include "hw/amhw_hc32_aes.h"
#include "hw/amhw_hc32_clktrim.h"
#include "hw/amhw_hc32_crc.h"
#include "hw/amhw_hc32_dac.h"
#include "hw/amhw_hc32_dma.h"
#include "hw/amhw_hc32_flash.h"
#include "hw/amhw_hc32_gpio.h"
#include "hw/amhw_hc32_i2c.h"
#include "hw/amhw_hc32_lcd.h"
#include "hw/amhw_hc32_lptim.h"
#include "hw/amhw_hc32_lpuart.h"
#include "hw/amhw_hc32_lvd.h"
#include "hw/amhw_hc32_pca.h"
#include "hw/amhw_hc32_pcnt.h"
#include "hw/amhw_hc32_ram.h"
#include "hw/amhw_hc32_rcc.h"
#include "hw/amhw_hc32_rtc.h"
#include "hw/amhw_hc32_spi.h"
#include "hw/amhw_hc32_tim.h"
#include "hw/amhw_hc32_trng.h"
#include "hw/amhw_hc32_uart.h"
#include "hw/amhw_hc32_vc.h"
#include "hw/amhw_hc32_wdt.h"
/**
 * \addtogroup amhw_hc32_if_periph_map
 * \copydoc amhw_hc32_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */

#define HC32_UART0          ((amhw_hc32_uart_t *)HC32_UART0_BASE)
#define HC32_UART1          ((amhw_hc32_uart_t *)HC32_UART1_BASE)
#define HC32_UART2          ((amhw_hc32_uart_t *)HC32_UART2_BASE)
#define HC32_UART3          ((amhw_hc32_uart_t *)HC32_UART3_BASE)
#define HC32_LPUART0        ((amhw_hc32_lpuart_t *)HC32_LPUART0_BASE)
#define HC32_LPUART1        ((amhw_hc32_lpuart_t *)HC32_LPUART1_BASE)
#define HC32_I2C0           ((amhw_hc32_i2c_t *)HC32_I2C0_BASE)
#define HC32_I2C1           ((amhw_hc32_i2c_t *)HC32_I2C1_BASE)
#define HC32_SPI0           ((amhw_hc32_spi_t *)HC32_SPI0_BASE)
#define HC32_SPI1           ((amhw_hc32_spi_t *)HC32_SPI1_BASE)
#define HC32_TIM0           ((amhw_hc32_tim_t *)HC32_TIM0_BASE)
#define HC32_TIM1           ((amhw_hc32_tim_t *)HC32_TIM1_BASE)
#define HC32_TIM2           ((amhw_hc32_tim_t *)HC32_TIM2_BASE)
#define HC32_TIM3           ((amhw_hc32_tim_t *)HC32_TIM3_BASE)
#define HC32_TIM4           ((amhw_hc32_adtim_t *)HC32_TIM4_BASE)
#define HC32_TIM5           ((amhw_hc32_adtim_t *)HC32_TIM5_BASE)
#define HC32_TIM6           ((amhw_hc32_adtim_t *)HC32_TIM6_BASE)
#define HC32_LPTIM0         ((amhw_hc32_lptim_t *)HC32_LPTIM0_BASE)
#define HC32_LPTIM1         ((amhw_hc32_lptim_t *)HC32_LPTIM1_BASE)
#define HC32_AES            ((amhw_hc32_aes_t *)HC32_AES_BASE)
#define HC32_ADC            ((amhw_hc32_adc_t *)HC32_ADC_BASE)
#define HC32_CLKTRIM        ((amhw_hc32_clktrim_t *)HC32_CLKTRIM_BASE)
#define HC32_CRC            ((amhw_hc32_crc_t *)HC32_CRC_BASE)
#define HC32_DAC            ((amhw_hc32_dac_t *)HC32_DAC_BASE)
#define HC32_DMA            ((amhw_hc32_dma_t *)HC32_DMA_BASE)
#define HC32_FLASH          ((amhw_hc32_flash_t *)HC32_FLASH_BASE)
#define HC32_GPIO0          ((amhw_hc32_gpio_t *)HC32_PORT0_BASE)
#define HC32_LCD            ((amhw_hc32_lcd_t *)HC32_LCD_BASE)
#define HC32_LVD            ((amhw_hc32_lvd_t *)HC32_LVD_BASE)
#define HC32_RCC            ((amhw_hc32_rcc_t *)HC32_RCC_BASE)
#define HC32_OPA            ((amhw_hc32_opa_t *)HC32_OPA_BASE)
#define HC32_PCNT           ((amhw_hc32_pcnt_t *)HC32_PCNT_BASE)
#define HC32_RAM            ((amhw_hc32_ram_t *)HC32_RAM_BASE)
#define HC32_PCA            ((amhw_hc32_pca_t *)HC32_PCA_BASE)
#define HC32_RTC            ((amhw_hc32_rtc_t *)HC32_RTC_BASE)
#define HC32_TRNG           ((amhw_hc32_trng_t *)HC32_TRNG_BASE)
#define HC32_VC             ((amhw_hc32_vc_t *)HC32_VC_BASE)
#define HC32_WDT            ((amhw_hc32_wdt_t *)HC32_WDT_BASE)
/** @} */


typedef struct {
    __IO uint32_t cr;  /**< \brief 基本配置寄存器 */
} amhw_hc32_bgr_t;

#define HC32_BGR     ((amhw_hc32_bgr_t *)HC32_BGR_BASE)

/**
 * \brief 内置温度传感器使能控制
 *
 * \param[in] stat : AM_TRUE : 使能
 *                   AM_FALSE: 禁能
 *
 * \return none
 */
am_static_inline
void amhw_hc32_bgr_temp_sensor_enable (am_bool_t stat)
{
    HC32_BGR->cr |= (stat << 1);
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
void amhw_hc32_bgr_enable (am_bool_t stat)
{
    HC32_BGR->cr |= (stat << 0);
}

/**
 * @} amhw_hc32_if_periph_map
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32_REG_BASE_H */

/* end of file */
