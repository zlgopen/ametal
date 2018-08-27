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
 * \brief KL26 外设映射
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-13  mkr, first implementation.
 * \endinternal
 */

#ifndef __KL26_PERIPH_MAP_H
#define __KL26_PERIPH_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../kl26/kl26_regbase.h"

#include "hw/amhw_fsl_adc.h"
#include "hw/amhw_fsl_bme.h"
#include "hw/amhw_fsl_cmp.h"
#include "hw/amhw_fsl_dac.h"
#include "hw/amhw_kl26_dma.h"
#include "hw/amhw_fsl_ftfa.h"
#include "hw/amhw_fsl_i2c.h"
#include "hw/amhw_fsl_i2s.h"
#include "hw/amhw_fsl_llwu.h"
#include "hw/amhw_fsl_lptmr.h"
#include "hw/amhw_kl26_mcm.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "hw/amhw_fsl_pit.h"
#include "hw/amhw_fsl_pmc.h"
#include "hw/amhw_fsl_rcm.h"
#include "hw/amhw_fsl_rtc.h"
#include "hw/amhw_fsl_smc.h"
#include "hw/amhw_fsl_spi.h"
#include "hw/amhw_fsl_tpm.h"
#include "hw/amhw_fsl_tsi.h"
#include "hw/amhw_fsl_uart.h"


/**
 * \addtogroup kl26_if_periph_map
 * \copydoc kl26_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */
#define KL26_SIM      ((amhw_kl26_sim_t     *)KL26_SIM_BASE)    /**< \brief SIM */
#define KL26_TPM0     ((amhw_fsl_tpm_t      *)KL26_TPM0_BASE)   /**< \brief TPM0 */
#define KL26_TPM1     ((amhw_fsl_tpm_t      *)KL26_TPM1_BASE)   /**< \brief TPM1 */
#define KL26_TPM2     ((amhw_fsl_tpm_t      *)KL26_TPM2_BASE)   /**< \brief TPM2 */
#define KL26_MCG      ((amhw_kl26_mcg_t     *)KL26_MCG_BASE)    /**< \brief MCG */
#define KL26_OSC      ((amhw_kl26_osc_t     *)KL26_OSC_BASE)    /**< \brief OSC */
#define KL26_GPIO     ((amhw_kl26_gpio_t    *)KL26_GPIO_BASE)   /**< \brief GPIO */
#define KL26_FGPIO    ((amhw_kl26_fgpio_t   *)KL26_FGPIO_BASE)  /**< \brief FGPIO */
#define KL26_PORT     ((amhw_kl26_port_t    *)KL26_PORT_BASE)   /**< \brief PORT */
#define KL26_UART0    ((amhw_fsl_uart_t    *)KL26_UART0_BASE)   /**< \brief UART0 */
#define KL26_UART1    ((amhw_fsl_uart_t    *)KL26_UART1_BASE)   /**< \brief UART1 */
#define KL26_UART2    ((amhw_fsl_uart_t    *)KL26_UART2_BASE)   /**< \brief UART2 */
#define KL26_USBOTG   ((amhw_fsl_usbotg_t  *)KL26_USBOTG_BASE)  /**< \brief USBOTG */
#define KL26_DAC0     ((amhw_fsl_dac_t      *)KL26_DAC0_BASE)   /**< \brief DAC0 */
#define KL26_ADC0     ((amhw_fsl_adc_t      *)KL26_ADC0_BASE)   /**< \brief ADC0 */
#define KL26_SPI0     ((amhw_fsl_spi_t     *)KL26_SPI0_BASE)    /**< \brief SPI0 */
#define KL26_SPI1     ((amhw_fsl_spi_t     *)KL26_SPI1_BASE)    /**< \brief SPI1 */
#define KL26_I2C0     ((amhw_fsl_i2c_t     *)KL26_I2C0_BASE)    /**< \brief I2C0 */
#define KL26_I2C1     ((amhw_fsl_i2c_t     *)KL26_I2C1_BASE)    /**< \brief I2C1 */
#define KL26_CMP0     ((amhw_fsl_cmp_t      *)KL26_CMP_BASE)    /**< \brief CMP0  */
#define KL26_DMAMUX   ((amhw_kl26_dmamux_t  *)KL26_DMAMUX_BASE) /**< \brief DMAMUX */
#define KL26_DMA      ((amhw_kl26_dma_t     *)KL26_DMA_BASE)    /**< \brief DMA  */
#define KL26_MCM      ((amhw_kl26_mcm_t     *)KL26_MCM_BASE)    /**< \brief MCM  */
#define KL26_RTC      ((amhw_fsl_rtc_t      *)KL26_RTC_BASE)    /**< \brief RTC */
#define KL26_FTFA     ((amhw_fsl_ftfa_t    *)KL26_FTFA_BASE)    /**< \brief FTFA */
#define KL26_PIT      ((amhw_fsl_pit_t      *)KL26_PIT_BASE)    /**< \brief PIT */
#define KL26_TSI      ((amhw_fsl_tsi_t     *)KL26_TSI_BASE)     /**< \brief TSI */
#define KL26_LPTMR    ((amhw_fsl_lptmr_t    *)KL26_LPTMR_BASE)  /**< \brief LPTMR */
#define KL26_SMC      ((amhw_fsl_smc_t     *)KL26_SMC_BASE)     /**< \brief SMC */
#define KL26_PMC      ((amhw_fsl_pmc_t     *)KL26_PMC_BASE)     /**< \brief PMC */
#define KL26_LLWU     ((amhw_fsl_llwu_t    *)KL26_LLWU_BASE)    /**< \brief LLWU */
#define KL26_RCM      ((amhw_fsl_rcm_t      *)KL26_RCM_BASE)    /**< \brief RCM */
#define KL26_I2S0     ((amhw_fsl_i2s_t     *)KL26_I2S0_BASE)    /**< \brief I2S0 */

/** @} */


/**
 * @} kl26_if_periph_map
 */

#ifdef __cplusplus
}
#endif

#endif /* __KL26_REG_BASE_H */

/* end of file */
