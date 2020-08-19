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

#include "hc32f07x_regbase.h"

#include "hw/amhw_hc32_adc.h"
#include "hw/amhw_hc32_adtim.h"
#include "hw/amhw_hc32_aes.h"
#include "hw/amhw_hc32_clktrim.h"
#include "hw/amhw_hc32_crc.h"
#include "hw/amhw_hc32f07x_dac.h"
#include "hw/amhw_hc32_dma.h"
#include "hw/amhw_hc32f07x_flash.h"
#include "hw/amhw_hc32_gpio.h"
#include "hw/amhw_hc32_i2c.h"
#include "hw/amhw_hc32_lcd.h"
#include "hw/amhw_hc32_lptim.h"
#include "hw/amhw_hc32_lpuart.h"
#include "hw/amhw_hc32_lvd.h"
#include "hw/amhw_hc32f07x_opa.h"
#include "hw/amhw_hc32_pca.h"
#include "hw/amhw_hc32_pcnt.h"
#include "hw/amhw_hc32_ram.h"
#include "hw/amhw_hc32f07x_rcc.h"
#include "hw/amhw_hc32_rtc.h"
#include "hw/amhw_hc32_spi.h"
#include "hw/amhw_hc32_tim.h"
#include "hw/amhw_hc32_trng.h"
#include "hw/amhw_hc32_uart.h"
#include "hw/amhw_hc32_vc.h"
#include "hw/amhw_hc32_wdt.h"
#include "hw/amhw_hc32f07x_can.h"
#include "hw/amhw_hc32f07x_usbd.h"
#include "hw/amhw_hc32f07x_cts.h"
#include "hw/amhw_hc32f07x_hdiv.h"
#include "hw/amhw_hc32f07x_cts.h"

/**
 * \addtogroup amhw_hc32_if_periph_map
 * \copydoc amhw_hc32_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */

#define HC32_UART0          ((amhw_hc32_uart_t *)HC32F07X_UART0_BASE)
#define HC32_UART1          ((amhw_hc32_uart_t *)HC32F07X_UART1_BASE)
#define HC32_UART2          ((amhw_hc32_uart_t *)HC32F07X_UART2_BASE)
#define HC32_UART3          ((amhw_hc32_uart_t *)HC32F07X_UART3_BASE)
#define HC32_LPUART0        ((amhw_hc32_lpuart_t *)HC32F07X_LPUART0_BASE)
#define HC32_LPUART1        ((amhw_hc32_lpuart_t *)HC32F07X_LPUART1_BASE)
#define HC32_I2C0           ((amhw_hc32_i2c_t *)HC32F07X_I2C0_BASE)
#define HC32_I2C1           ((amhw_hc32_i2c_t *)HC32F07X_I2C1_BASE)
#define HC32_SPI0           ((amhw_hc32_spi_t *)HC32F07X_SPI0_BASE)
#define HC32_SPI1           ((amhw_hc32_spi_t *)HC32F07X_SPI1_BASE)
#define HC32_TIM0           ((amhw_hc32_tim_t *)HC32F07X_TIM0_BASE)
#define HC32_TIM1           ((amhw_hc32_tim_t *)HC32F07X_TIM1_BASE)
#define HC32_TIM2           ((amhw_hc32_tim_t *)HC32F07X_TIM2_BASE)
#define HC32_TIM3           ((amhw_hc32_tim_t *)HC32F07X_TIM3_BASE)
#define HC32_TIM4           ((amhw_hc32_adtim_t *)HC32F07X_TIM4_BASE)
#define HC32_TIM5           ((amhw_hc32_adtim_t *)HC32F07X_TIM5_BASE)
#define HC32_TIM6           ((amhw_hc32_adtim_t *)HC32F07X_TIM6_BASE)
#define HC32_AES            ((amhw_hc32_aes_t *)HC32F07X_AES_BASE)
#define HC32_ADC            ((amhw_hc32_adc_t *)HC32F07X_ADC_BASE)
#define HC32_CLKTRIM        ((amhw_hc32f07x_clktrim_t *)HC32F07X_CLKTRIM_BASE)
#define HC32_CRC            ((amhw_hc32_crc_t *)HC32F07X_CRC_BASE)
#define HC32_DAC            ((amhw_hc32f07x_dac_t *)HC32F07X_DAC_BASE)
#define HC32_DMA            ((amhw_hc32_dma_t *)HC32F07X_DMA_BASE)
#define HC32_FLASH          ((amhw_hc32f07x_flash_t *)HC32F07X_FLASH_BASE)
#define HC32_GPIO0          ((amhw_hc32_gpio_t *)HC32F07X_PORT0_BASE)
#define HC32_LCD            ((amhw_hc32_lcd_t *)HC32F07X_LCD_BASE)
#define HC32_LVD            ((amhw_hc32_lvd_t *)HC32F07X_LVD_BASE)
#define HC32_RCC            ((amhw_hc32f07x_rcc_t *)HC32F07X_RCC_BASE)
#define HC32_OPA            ((amhw_hc32f07x_opa_t *)HC32F07X_OPA_BASE)
#define HC32_RAM            ((amhw_hc32_ram_t *)HC32F07X_RAM_BASE)
#define HC32_PCA            ((amhw_hc32_pca_t *)HC32F07X_PCA_BASE)
#define HC32_RTC            ((amhw_hc32_rtc_t *)HC32F07X_RTC_BASE)
#define HC32_TRNG           ((amhw_hc32_trng_t *)HC32F07X_TRNG_BASE)
#define HC32_VC             ((amhw_hc32_vc_t *)HC32F07X_VC_BASE)
#define HC32_WDT            ((amhw_hc32_wdt_t *)HC32F07X_WDT_BASE)
#define HC32_CAN            ((amhw_hc32f07x_can_t *)HC32F07X_CAN_BASE)
#define HC32_USB            ((amhw_hc32_usbfs_t *)HC32F07X_USB_BASE)
#define HC32_CTS            ((amhw_hc32f07x_cts_t *)HC32F07X_CTS_BASE)
#define HC32_I2S0            ((amhw_hc32f07x_i2s_t *)HC32F07X_I2S0_BASE)
#define HC32_I2S1            ((amhw_hc32f07x_i2s_t *)HC32F07X_I2S1_BASE)
#define HC32_HDIV            ((amhw_hc32f07x_hdiv_t *)HC32F07X_HDIV_BASE)

/** @} */

typedef struct {
	__IO uint32_t cr; /**< \brief 基本配置寄存器 */
}amhw_hc32_bgr_t;

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
