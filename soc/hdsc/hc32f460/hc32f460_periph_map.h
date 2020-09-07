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
 * \brief HC32F460 外设映射
 *
 * \internal
 * \par Modification history
 * - 1.00 20-01-16  cds, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_PERIPH_MAP_H
#define __AMHW_HC32F460_PERIPH_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hc32f460_regbase.h"


#include "hw/amhw_hc32f460_gpio.h"
#include "hw/amhw_hc32f460_flash.h"
#include "hw/amhw_hc32f460_uart.h"
#include "hw/amhw_hc32f460_timea.h"
#include "hw/amhw_hc32f460_i2c.h"
#include "hw/amhw_hc32f460_dma.h"
#include "hw/amhw_hc32f460_qspi.h"
#include "hw/amhw_hc32f460_spi.h"
#include "hw/amhw_hc32f460_adc.h"
#include "hw/amhw_hc32f460_sdioc.h"
#include "hw/amhw_hc32f460_wdt.h"
#include "hw/amhw_hc32f460_swdt.h"
#include "hw/amhw_hc32f460_mpu.h"
#include "hw/amhw_hc32f460_hash.h"
#include "hw/amhw_hc32f460_trng.h"
#include "hw/amhw_hc32f460_aes.h"
#include "hw/amhw_hc32f460_crc.h"
#include "hw/amhw_hc32f460_cmp.h"
/**
 * \addtogroup amhw_hc32f460_if_periph_map
 * \copydoc amhw_hc32f460_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */


#define HC32F460_GPIO           ((amhw_hc32f460_gpio_t *)HC32F460_GPIO_BASE)
#define HC32F460_EFM            ((amhw_hc32f460_flash_t *)HC32F460_EFM_BASE)
#define HC32F460_UART1          ((amhw_hc32f460_uart_t *)HC32F460_UART1_BASE)
#define HC32F460_UART2          ((amhw_hc32f460_uart_t *)HC32F460_UART2_BASE)
#define HC32F460_UART3          ((amhw_hc32f460_uart_t *)HC32F460_UART3_BASE)
#define HC32F460_UART4          ((amhw_hc32f460_uart_t *)HC32F460_UART4_BASE)
#define HC32F460_DMA1           ((amhw_hc32f460_dma_t  *)HC32F460_DMA1_BASE)
#define HC32F460_DMA2           ((amhw_hc32f460_dma_t  *)HC32F460_DMA2_BASE)
#define HC32F460_TMRA1          ((amhw_hc32f460_timea_t *)HC32F460_TMRA1_BASE)
#define HC32F460_TMRA2          ((amhw_hc32f460_timea_t *)HC32F460_TMRA2_BASE)
#define HC32F460_TMRA3          ((amhw_hc32f460_timea_t *)HC32F460_TMRA3_BASE)
#define HC32F460_TMRA4          ((amhw_hc32f460_timea_t *)HC32F460_TMRA4_BASE)
#define HC32F460_TMRA5          ((amhw_hc32f460_timea_t *)HC32F460_TMRA5_BASE)
#define HC32F460_TMRA6          ((amhw_hc32f460_timea_t *)HC32F460_TMRA6_BASE)
#define HC32F460_TMR41          ((amhw_hc32f460_tim4_t *)HC32F460_TMR41_BASE)
#define HC32F460_TMR42          ((amhw_hc32f460_tim4_t *)HC32F460_TMR42_BASE)
#define HC32F460_TMR43          ((amhw_hc32f460_tim4_t *)HC32F460_TMR43_BASE)
#define HC32F460_TMR61          ((amhw_hc32f460_adtim_t *)HC32F460_TMR61_BASE)
#define HC32F460_TMR62          ((amhw_hc32f460_adtim_t *)HC32F460_TMR62_BASE)
#define HC32F460_TMR63          ((amhw_hc32f460_adtim_t *)HC32F460_TMR63_BASE)
#define HC32F460_I2C1           ((amhw_hc32f460_i2c_t *)HC32F460_I2C1_BASE)
#define HC32F460_I2C2           ((amhw_hc32f460_i2c_t *)HC32F460_I2C2_BASE)
#define HC32F460_I2C3           ((amhw_hc32f460_i2c_t *)HC32F460_I2C3_BASE)
#define HC32F460_RTC            ((amhw_hc32f460_rtc_t *)HC32F460_RTC_BASE)
#define HC32F460_QSPI           ((amhw_hc32f460_qspi_t *)HC32F460_QSPI_BASE)
#define HC32F460_SPI1           ((amhw_hc32f460_spi_t *)HC32F460_SPI1_BASE)
#define HC32F460_SPI2           ((amhw_hc32f460_spi_t *)HC32F460_SPI2_BASE)
#define HC32F460_SPI3           ((amhw_hc32f460_spi_t *)HC32F460_SPI3_BASE)
#define HC32F460_SPI4           ((amhw_hc32f460_spi_t *)HC32F460_SPI4_BASE)
#define HC32F460_ADC1           ((amhw_hc32f460_adc_t  *)HC32F460_ADC1_BASE)
#define HC32F460_ADC2           ((amhw_hc32f460_adc_t  *)HC32F460_ADC2_BASE)
#define HC32F460_SDIOC1         ((amhw_hc32f460_sdioc_t  *)HC32F460_SDIOC1_BASE)
#define HC32F460_SDIOC2         ((amhw_hc32f460_sdioc_t  *)HC32F460_SDIOC2_BASE)
#define HC32F460_WDT            ((amhw_hc32f460_wdt_t  *)HC32F460_WDT_BASE)
#define HC32F460_SWDT           ((amhw_hc32f460_swdt_t  *)HC32F460_SWDT_BASE)
#define HC32F460_MPU            ((amhw_hc32f460_mpu_t  *)HC32F460_MPU_BASE)
#define HC32F460_HASH           ((amhw_hc32f460_hash_t *)HC32F460_HASH_BASE)
#define HC32F460_TRNG           ((amhw_hc32f460_trng_t *)HC32F460_TRNG_BASE)
#define HC32F460_AES            ((amhw_hc32f460_aes_t *)HC32F460_AES_BASE)
#define HC32F460_DCU1           ((amhw_hc32f460_dcu_t *)HC32F460_DCU1_BASE)
#define HC32F460_DCU2           ((amhw_hc32f460_dcu_t *)HC32F460_DCU2_BASE)
#define HC32F460_DCU3           ((amhw_hc32f460_dcu_t *)HC32F460_DCU3_BASE)
#define HC32F460_DCU4           ((amhw_hc32f460_dcu_t *)HC32F460_DCU4_BASE)
#define HC32F460_USB            ((amhw_hc32f460_usbd_t *)HC32F460_USB_OTG_FS_BASE)
#define HC32F460_CRC            ((amhw_hc32f460_crc_t *)HC32F460_CRC_BASE)
#define HC32F460_CMP1           ((amhw_hc32f460_cmp_t *)HC32F460_CMP1_BASE)
#define HC32F460_CMP2           ((amhw_hc32f460_cmp_t *)HC32F460_CMP2_BASE)
#define HC32F460_CMP3           ((amhw_hc32f460_cmp_t *)HC32F460_CMP3_BASE)

/** @} */




/**
 * @} amhw_hc32f460_if_periph_map
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_REG_BASE_H */

/* end of file */
