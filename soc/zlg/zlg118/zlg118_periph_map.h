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
 * \brief ZLG118 外设映射
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-03  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG118_PERIPH_MAP_H
#define __AMHW_ZLG118_PERIPH_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "zlg118_regbase.h"

#include "hw/amhw_zlg118_adc.h"
#include "hw/amhw_zlg118_adtim.h"
#include "hw/amhw_zlg118_aes.h"
#include "hw/amhw_zlg118_clktrim.h"
#include "hw/amhw_zlg118_crc.h"
#include "hw/amhw_zlg118_dac.h"
#include "hw/amhw_zlg118_dma.h"
#include "hw/amhw_zlg118_flash.h"
#include "hw/amhw_zlg118_gpio.h"
#include "hw/amhw_zlg118_i2c.h"
#include "hw/amhw_zlg118_lcd.h"
#include "hw/amhw_zlg118_lptim.h"
#include "hw/amhw_zlg118_lpuart.h"
#include "hw/amhw_zlg118_lvd.h"
#include "hw/amhw_zlg118_opa.h"
#include "hw/amhw_zlg118_pca.h"
#include "hw/amhw_zlg118_pcnt.h"
#include "hw/amhw_zlg118_ram.h"
#include "hw/amhw_zlg118_rcc.h"
#include "hw/amhw_zlg118_rtc.h"
#include "hw/amhw_zlg118_spi.h"
#include "hw/amhw_zlg118_tim.h"
#include "hw/amhw_zlg118_trng.h"
#include "hw/amhw_zlg118_uart.h"
#include "hw/amhw_zlg118_vc.h"
#include "hw/amhw_zlg118_wdt.h"
/**
 * \addtogroup amhw_zlg118_if_periph_map
 * \copydoc amhw_zlg118_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */

#define ZLG118_UART0          ((amhw_zlg118_uart_t *)ZLG118_UART0_BASE)
#define ZLG118_UART1          ((amhw_zlg118_uart_t *)ZLG118_UART1_BASE)
#define ZLG118_UART2          ((amhw_zlg118_uart_t *)ZLG118_UART2_BASE)
#define ZLG118_UART3          ((amhw_zlg118_uart_t *)ZLG118_UART3_BASE)
#define ZLG118_LPUART0        ((amhw_zlg118_lpuart_t *)ZLG118_LPUART0_BASE)
#define ZLG118_LPUART1        ((amhw_zlg118_lpuart_t *)ZLG118_LPUART1_BASE)
#define ZLG118_I2C0           ((amhw_zlg118_i2c_t *)ZLG118_I2C0_BASE)
#define ZLG118_I2C1           ((amhw_zlg118_i2c_t *)ZLG118_I2C1_BASE)
#define ZLG118_SPI0           ((amhw_zlg118_spi_t *)ZLG118_SPI0_BASE)
#define ZLG118_SPI1           ((amhw_zlg118_spi_t *)ZLG118_SPI1_BASE)
#define ZLG118_TIM0           ((amhw_zlg118_tim_t *)ZLG118_TIM0_BASE)
#define ZLG118_TIM1           ((amhw_zlg118_tim_t *)ZLG118_TIM1_BASE)
#define ZLG118_TIM2           ((amhw_zlg118_tim_t *)ZLG118_TIM2_BASE)
#define ZLG118_TIM3           ((amhw_zlg118_tim_t *)ZLG118_TIM3_BASE)
#define ZLG118_TIM4           ((amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE)
#define ZLG118_TIM5           ((amhw_zlg118_adtim_t *)ZLG118_TIM5_BASE)
#define ZLG118_TIM6           ((amhw_zlg118_adtim_t *)ZLG118_TIM6_BASE)
#define ZLG118_LPTIM0         ((amhw_zlg118_lptim_t *)ZLG118_LPTIM0_BASE)
#define ZLG118_LPTIM1         ((amhw_zlg118_lptim_t *)ZLG118_LPTIM1_BASE)
#define ZLG118_AES            ((amhw_zlg118_aes_t *)ZLG118_AES_BASE)
#define ZLG118_ADC            ((amhw_zlg118_adc_t *)ZLG118_ADC_BASE)
#define ZLG118_CLKTRIM        ((amhw_zlg118_clktrim_t *)ZLG118_CLKTRIM_BASE)
#define ZLG118_CRC            ((amhw_zlg118_crc_t *)ZLG118_CRC_BASE)
#define ZLG118_DAC            ((amhw_zlg118_dac_t *)ZLG118_DAC_BASE)
#define ZLG118_DMA            ((amhw_zlg118_dma_t *)ZLG118_DMA_BASE)
#define ZLG118_FLASH          ((amhw_zlg118_flash_t *)ZLG118_FLASH_BASE)
#define ZLG118_GPIO0          ((amhw_zlg118_gpio_t *)ZLG118_PORT0_BASE)
#define ZLG118_LCD            ((amhw_zlg118_lcd_t *)ZLG118_LCD_BASE)
#define ZLG118_LVD            ((amhw_zlg118_lvd_t *)ZLG118_LVD_BASE)
#define ZLG118_RCC            ((amhw_zlg118_rcc_t *)ZLG118_RCC_BASE)
#define ZLG118_OPA            ((amhw_zlg118_opa_t *)ZLG118_OPA_BASE)
#define ZLG118_PCNT           ((amhw_zlg118_pcnt_t *)ZLG118_PCNT_BASE)
#define ZLG118_RAM            ((amhw_zlg118_ram_t *)ZLG118_RAM_BASE)
#define ZLG118_PCA            ((amhw_zlg118_pca_t *)ZLG118_PCA_BASE)
#define ZLG118_RTC            ((amhw_zlg118_rtc_t *)ZLG118_RTC_BASE)
#define ZLG118_TRNG           ((amhw_zlg118_trng_t *)ZLG118_TRNG_BASE)
#define ZLG118_VC             ((amhw_zlg118_vc_t *)ZLG118_VC_BASE)
#define ZLG118_WDT            ((amhw_zlg118_wdt_t *)ZLG118_WDT_BASE)
/** @} */


typedef struct {
    __IO uint32_t cr;  /**< \brief 基本配置寄存器 */
} amhw_zlg118_bgr_t;

#define ZLG118_BGR     ((amhw_zlg118_bgr_t *)ZLG118_BGR_BASE)

/**
 * \brief 内置温度传感器使能控制
 *
 * \param[in] stat : AM_TRUE : 使能
 *                   AM_FALSE: 禁能
 *
 * \return none
 */
am_static_inline
void amhw_zlg118_bgr_temp_sensor_enable (am_bool_t stat)
{
    ZLG118_BGR->cr |= (stat << 1);
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
void amhw_zlg118_bgr_enable (am_bool_t stat)
{
    ZLG118_BGR->cr |= (stat << 0);
}

/**
 * @} amhw_zlg118_if_periph_map
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG118_REG_BASE_H */

/* end of file */
