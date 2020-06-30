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
 * \brief HC32F460 初始化配置（ICG）
 *
 * \internal
 * \par Modification History
 * - 1.00 20-06-02  cds, first implementation
 * \endinternal
 */

#ifndef __HC32F460_ICG_H
#define __HC32F460_ICG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup h32f460_if_icg
 * \copydoc hc32f460_icg.h
 * @{
 */

#include "ametal.h"

/**
 *******************************************************************************
 ** \brief ICG start configure function on/off
 ******************************************************************************/
#ifndef ICG_FUNCTION_ON
#define ICG_FUNCTION_ON                         (1u)
#endif

#ifndef ICG_FUNCTION_OFF
#define ICG_FUNCTION_OFF                        (0u)
#endif

/**
 *******************************************************************************
 ** \brief SWDT running state after reset
 ******************************************************************************/
#define HC32F460_ICG_SWDT_AUTO_START_AFTER_RESET  ((uint16_t)0x0000)  /**< \brief SWDT Auto Start after reset */
#define HC32F460_ICG_SWDT_STOP_AFTER_RESET        ((uint16_t)0x0001)  /**< \brief SWDT stop after reset */

/**
 *******************************************************************************
 ** \brief SWDT count underflow or refresh error trigger event type
 ******************************************************************************/
#define HC32F460_ICG_SWDT_INTERRUPT_REQUEST       ((uint16_t)0x0000)  /**< \brief WDT trigger interrupt request */
#define HC32F460_ICG_SWDT_RESET_REQUEST           ((uint16_t)0x0002)  /**< \brief WDT trigger reset request */

/**
 *******************************************************************************
 ** \brief SWDT count underflow cycle
 ******************************************************************************/
#define HC32F460_ICG_SWDT_COUNT_UNDERFLOW_CYCLE_256          ((uint16_t)0x0000)  /**< \brief 256 clock cycle */
#define HC32F460_ICG_SWDT_COUNT_UNDERFLOW_CYCLE_4096         ((uint16_t)0x0004)  /**< \brief 4096 clock cycle */
#define HC32F460_ICG_SWDT_COUNT_UNDERFLOW_CYCLE_16384        ((uint16_t)0x0008)  /**< \brief 16384 clock cycle */
#define HC32F460_ICG_SWDT_COUNT_UNDERFLOW_CYCLE_65536        ((uint16_t)0x000C)  /**< \brief 65536 clock cycle */

/**
 *******************************************************************************
 ** \brief SWDT count clock division
 ******************************************************************************/
#define HC32F460_ICG_SWDT_COUNT_SWDTCLK_DIV1                 ((uint16_t)0x0000)  /**< \brief SWDTCLK */
#define HC32F460_ICG_SWDT_COUNT_SWDTCLK_DIV16                ((uint16_t)0x0040)  /**< \brief SWDTCLK/16 */
#define HC32F460_ICG_SWDT_COUNT_SWDTCLK_DIV32                ((uint16_t)0x0050)  /**< \brief SWDTCLK/32 */
#define HC32F460_ICG_SWDT_COUNT_SWDTCLK_DIV64                ((uint16_t)0x0060)  /**< \brief SWDTCLK/64 */
#define HC32F460_ICG_SWDT_COUNT_SWDTCLK_DIV128               ((uint16_t)0x0070)  /**< \brief SWDTCLK/128 */
#define HC32F460_ICG_SWDT_COUNT_SWDTCLK_DIV256               ((uint16_t)0x0080)  /**< \brief SWDTCLK/256 */
#define HC32F460_ICG_SWDT_COUNT_SWDTCLK_DIV2048              ((uint16_t)0x00B0)  /**< \brief SWDTCLK/2048 */

/**
 *******************************************************************************
 ** \brief SWDT allow refresh percent range
 ******************************************************************************/
#define HC32F460_ICG_SWDT_100PCT                             ((uint16_t)0x0000)  /**< \brief 100% */
#define HC32F460_ICG_SWDT_0To25PCT                           ((uint16_t)0x0100)  /**< \brief 0%~25% */
#define HC32F460_ICG_SWDT_25To50PCT                          ((uint16_t)0x0200)  /**< \brief 25%~50% */
#define HC32F460_ICG_SWDT_0To50PCT                           ((uint16_t)0x0300)  /**< \brief 0%~50% */
#define HC32F460_ICG_SWDT_50To75PCT                          ((uint16_t)0x0400)  /**< \brief 50%~75% */
#define HC32F460_ICG_SWDT_0To25PCT_50To75PCT                 ((uint16_t)0x0500)  /**< \brief 0%~25% & 50%~75% */
#define HC32F460_ICG_SWDT_25To75PCT                          ((uint16_t)0x0600)  /**< \brief 25%~75% */
#define HC32F460_ICG_SWDT_0To75PCT                           ((uint16_t)0x0700)  /**< \brief 0%~75% */
#define HC32F460_ICG_SWDT_75To100PCT                         ((uint16_t)0x0800)  /**< \brief 75%~100% */
#define HC32F460_ICG_SWDT_0To25PCT_75To100PCT                ((uint16_t)0x0900)  /**< \brief 0%~25% & 75%~100% */
#define HC32F460_ICG_SWDT_25To50PCT_75To100PCT               ((uint16_t)0x0A00)  /**< \brief 25%~50% & 75%~100% */
#define HC32F460_ICG_SWDT_0To50PCT_75To100PCT                ((uint16_t)0x0B00)  /**< \brief 0%~50% & 75%~100% */
#define HC32F460_ICG_SWDT_50To100PCT                         ((uint16_t)0x0C00)  /**< \brief 50%~100% */
#define HC32F460_ICG_SWDT_0To25PCT_50To100PCT                ((uint16_t)0x0D00)  /**< \brief 0%~25% & 50%~100% */
#define HC32F460_ICG_SWDT_25To100PCT                         ((uint16_t)0x0E00)  /**< \brief 25%~100% */
#define HC32F460_ICG_SWDT_0To100PCT                          ((uint16_t)0x0F00)  /**< \brief 0%~100% */

/**
 *******************************************************************************
 ** \brief SWDT count control in the sleep/stop mode
 ******************************************************************************/
#define HC32F460_ICG_SWDT_SPECIAL_MODE_COUNT_CONTINUE        ((uint16_t)0x0000)  /**< \brief SWDT count continue in the sleep/stop mode */
#define HC32F460_ICG_SWDT_SPECIAL_MODE_COUNT_STOP            ((uint16_t)0x1000)  /**< \brief SWDT count stop in the sleep/stop mode */



/**
 *******************************************************************************
 ** \brief WDT running state after reset
 ******************************************************************************/
/** \brief 复位后，WDT自启动（硬件启动） */
#define HC32F460_ICG_WDT_AUTO_START_AFTER_RESET    ((uint16_t)0x0000)
/** \brief 复位后，WDT停止 */
#define HC32F460_ICG_WDT_STOP_AFTER_RESET          ((uint16_t)0x0001)

/**
 *******************************************************************************
 ** \brief WDT count underflow or refresh error trigger event type
 ******************************************************************************/
/** \brief WDT计数溢出后产生中断请求 */
#define HC32F460_ICG_WDT_INTERRUPT_REQUEST          ((uint16_t)0x0000)
/** \brief WDT计数溢出后产生复位请求 */
#define HC32F460_ICG_WDT_RESET_REQUEST              ((uint16_t)0x0002)

/**
 *******************************************************************************
 ** \brief WDT count underflow cycle（计数溢出周期）
 ******************************************************************************/
/** \brief 256周期 */
#define HC32F460_ICG_WDT_COUNT_UNDERFLOW_CYCLE_256           ((uint16_t)0x0000)
/** \brief 4096周期 */
#define HC32F460_ICG_WDT_COUNT_UNDERFLOW_CYCLE_4096          ((uint16_t)0x0004)
/** \brief 16384周期 */
#define HC32F460_ICG_WDT_COUNT_UNDERFLOW_CYCLE_16384         ((uint16_t)0x0008)
/** \brief 65536周期 */
#define HC32F460_ICG_WDT_COUNT_UNDERFLOW_CYCLE_65536         ((uint16_t)0x000C)

/**
 *******************************************************************************
 ** \brief WDT count clock division（WDT计数时钟）
 ******************************************************************************/
#define HC32F460_ICG_WDT_COUNT_PCLK3_DIV4                    ((uint16_t)0x0020)
#define HC32F460_ICG_WDT_COUNT_PCLK3_DIV64                   ((uint16_t)0x0060)
#define HC32F460_ICG_WDT_COUNT_PCLK3_DIV128                  ((uint16_t)0x0070)
#define HC32F460_ICG_WDT_COUNT_PCLK3_DIV256                  ((uint16_t)0x0080)
#define HC32F460_ICG_WDT_COUNT_PCLK3_DIV512                  ((uint16_t)0x0090)
#define HC32F460_ICG_WDT_COUNT_PCLK3_DIV1024                 ((uint16_t)0x00A0)
#define HC32F460_ICG_WDT_COUNT_PCLK3_DIV2048                 ((uint16_t)0x00B0)
#define HC32F460_ICG_WDT_COUNT_PCLK3_DIV8192                 ((uint16_t)0x00D0)

/**
 *******************************************************************************
 ** \brief WDT allow refresh percent range（刷新允许区域计数值百分比）
 ******************************************************************************/
#define HC32F460_ICG_WDT_100PCT                ((uint16_t)0x0000)  /**< \brief 100% */
#define HC32F460_ICG_WDT_0To25PCT              ((uint16_t)0x0100)  /**< \brief 0%~25% */
#define HC32F460_ICG_WDT_25To50PCT             ((uint16_t)0x0200)  /**< \brief 25%~50% */
#define HC32F460_ICG_WDT_0To50PCT              ((uint16_t)0x0300)  /**< \brief 0%~50% */
#define HC32F460_ICG_WDT_50To75PCT             ((uint16_t)0x0400)  /**< \brief 50%~75% */
#define HC32F460_ICG_WDT_0To25PCT_50To75PCT    ((uint16_t)0x0500)  /**< \brief 0%~25% & 50%~75% */
#define HC32F460_ICG_WDT_25To75PCT             ((uint16_t)0x0600)  /**< \brief 25%~75% */
#define HC32F460_ICG_WDT_0To75PCT              ((uint16_t)0x0700)  /**< \brief 0%~75% */
#define HC32F460_ICG_WDT_75To100PCT            ((uint16_t)0x0800)  /**< \brief 75%~100% */
#define HC32F460_ICG_WDT_0To25PCT_75To100PCT   ((uint16_t)0x0900)  /**< \brief 0%~25% & 75%~100% */
#define HC32F460_ICG_WDT_25To50PCT_75To100PCT  ((uint16_t)0x0A00)  /**< \brief 25%~50% & 75%~100% */
#define HC32F460_ICG_WDT_0To50PCT_75To100PCT   ((uint16_t)0x0B00)  /**< \brief 0%~50% & 75%~100% */
#define HC32F460_ICG_WDT_50To100PCT            ((uint16_t)0x0C00)  /**< \brief 50%~100% */
#define HC32F460_ICG_WDT_0To25PCT_50To100PCT   ((uint16_t)0x0D00)  /**< \brief 0%~25% & 50%~100% */
#define HC32F460_ICG_WDT_25To100PCT            ((uint16_t)0x0E00)  /**< \brief 25%~100% */
#define HC32F460_ICG_WDT_0To100PCT             ((uint16_t)0x0F00)  /**< \brief 0%~100% */

/**
 *******************************************************************************
 ** \brief WDT count control in the sleep mode
 ******************************************************************************/
/** \brief SWDT在sleep,stop模式下计数不停止 */
#define HC32F460_ICG_WDT_SPECIAL_MODE_COUNT_CONTINUE     ((uint16_t)0x0000)
/** \brief SWDT在sleep,stop模式下计数停止 */
#define HC32F460_ICG_WDT_SPECIAL_MODE_COUNT_STOP         ((uint16_t)0x1000)


/**
 *******************************************************************************
 ** \brief SWDT hardware start configuration
 ******************************************************************************/
/** \brief Enable or disable WDT hardware start */
#define HC32F460_ICG0_SWDT_HARDWARE_START                 (ICG_FUNCTION_OFF)

/** \brief WDT register config */
#define HC32F460_ICG0_SWDT_AUTS        (HC32F460_ICG_SWDT_AUTO_START_AFTER_RESET)
#define HC32F460_ICG0_SWDT_ITS         (HC32F460_ICG_SWDT_INTERRUPT_REQUEST)
#define HC32F460_ICG0_SWDT_PERI        (HC32F460_ICG_SWDT_COUNT_UNDERFLOW_CYCLE_256)
#define HC32F460_ICG0_SWDT_CKS         (HC32F460_ICG_SWDT_COUNT_SWDTCLK_DIV32)
#define HC32F460_ICG0_SWDT_WDPT        (HC32F460_ICG_SWDT_0To100PCT)
#define HC32F460_ICG0_SWDT_SLPOFF      (HC32F460_ICG_SWDT_SPECIAL_MODE_COUNT_STOP)

/** \brief SWDT register config value */
#if HC32F460_ICG0_SWDT_HARDWARE_START == ICG_FUNCTION_ON
#define HC32F460_ICG0_SWDT_REG_CONFIG                    (HC32F460_ICG0_SWDT_AUTS | HC32F460_ICG0_SWDT_ITS | HC32F460_ICG0_SWDT_PERI | \
                                                          HC32F460_ICG0_SWDT_CKS | HC32F460_ICG0_SWDT_WDPT | HC32F460_ICG0_SWDT_SLPOFF)
#else
#define HC32F460_ICG0_SWDT_REG_CONFIG                    ((uint16_t)0xFFFF)
#endif

/**
 *******************************************************************************
 ** \brief WDT hardware start configuration
 ******************************************************************************/
/** \brief Enable or disable WDT hardware start */
#define HC32F460_ICG0_WDT_HARDWARE_START                 (ICG_FUNCTION_OFF)

/** \brief WDT register config */
#define HC32F460_ICG0_WDT_AUTS        (HC32F460_ICG_WDT_AUTO_START_AFTER_RESET)
#define HC32F460_ICG0_WDT_ITS         (HC32F460_ICG_WDT_RESET_REQUEST)
#define HC32F460_ICG0_WDT_PERI        (HC32F460_ICG_WDT_COUNT_UNDERFLOW_CYCLE_16384)
#define HC32F460_ICG0_WDT_CKS         (HC32F460_ICG_WDT_COUNT_PCLK3_DIV8192)
#define HC32F460_ICG0_WDT_WDPT        (HC32F460_ICG_WDT_0To100PCT)
#define HC32F460_ICG0_WDT_SLPOFF      (HC32F460_ICG_WDT_SPECIAL_MODE_COUNT_STOP)

/** \brief WDT register config value */
#if HC32F460_ICG0_WDT_HARDWARE_START == ICG_FUNCTION_ON
#define HC32F460_ICG0_WDT_REG_CONFIG                     (HC32F460_ICG0_WDT_AUTS | HC32F460_ICG0_WDT_ITS | HC32F460_ICG0_WDT_PERI | \
                                                          HC32F460_ICG0_WDT_CKS | HC32F460_ICG0_WDT_WDPT | HC32F460_ICG0_WDT_SLPOFF)
#else
#define HC32F460_ICG0_WDT_REG_CONFIG                     ((uint16_t)0xFFFF)
#endif



/**
 *******************************************************************************
 ** \brief HRC frequency select
 ******************************************************************************/
#define HC32F460_ICG_HRC_FREQUENCY_20MHZ                     ((uint16_t)0x0000)  /**< \brief HRC frequency 20MHZ */
#define HC32F460_ICG_HRC_FREQUENCY_16MHZ                     ((uint16_t)0x0001)  /**< \brief HRC frequency 16MHZ */

/**
 *******************************************************************************
 ** \brief HRC oscillation state control
 ******************************************************************************/
#define HC32F460_ICG_HRC_OSCILLATION_START                   ((uint16_t)0x0000)  /**< \brief HRC oscillation start */
#define HC32F460_ICG_HRC_OSCILLATION_STOP                    ((uint16_t)0x0100)  /**< \brief HRC oscillation stop */

/**
 *******************************************************************************
 ** \brief VDU0 threshold voltage select
 ******************************************************************************/
#define HC32F460_ICG_VDU0_VOLTAGE_THRESHOLD_1P5              ((uint8_t)0x00)  /**< \brief VDU0 voltage threshold 1.9V */
#define HC32F460_ICG_VDU0_VOLTAGE_THRESHOLD_2P0              ((uint8_t)0x01)  /**< \brief VDU0 voltage threshold 2.0V */
#define HC32F460_ICG_VDU0_VOLTAGE_THRESHOLD_2P1              ((uint8_t)0x02)  /**< \brief VDU0 voltage threshold 2.1V */
#define HC32F460_ICG_VDU0_VOLTAGE_THRESHOLD_2P3              ((uint8_t)0x03)  /**< \brief VDU0 voltage threshold 2.3V */

/**
 *******************************************************************************
 ** \brief VDU0 running state after reset
 ******************************************************************************/
#define HC32F460_ICG_VDU0_START_AFTER_RESET                  ((uint8_t)0x00)   /**< \brief VDU0 start after reset */
#define HC32F460_ICG_VDU0_STOP_AFTER_RESET                   ((uint8_t)0x04)   /**< \brief VDU0 stop after reset */

/**
 *******************************************************************************
 ** \brief NMI pin filter sample clock division
 ******************************************************************************/
#define HC32F460_ICG_NMI_PIN_FILTER_PCLK3_DIV1               ((uint8_t)0x00)   /**< \brief PCLK3 */
#define HC32F460_ICG_NMI_PIN_FILTER_PCLK3_DIV8               ((uint8_t)0x04)   /**< \brief PCLK3/8 */
#define HC32F460_ICG_NMI_PIN_FILTER_PCLK3_DIV32              ((uint8_t)0x08)   /**< \brief PCLK3/32 */
#define HC32F460_ICG_NMI_PIN_FILTER_PCLK3_DIV64              ((uint8_t)0x0C)   /**< \brief PCLK3/64 */

/**
 *******************************************************************************
 ** \brief NMI pin trigger edge type
 ******************************************************************************/
#define HC32F460_ICG_NMI_PIN_TRIGGER_EDGE_FALLING            ((uint8_t)0x00)   /**< \brief Falling edge trigger */
#define HC32F460_ICG_NMI_PIN_TRIGGER_EDGE_RISING             ((uint8_t)0x10)   /**< \brief Rising edge trigger */

/**
 *******************************************************************************
 ** \brief Enable or disable NMI pin interrupt request
 ******************************************************************************/
#define HC32F460_ICG_NMI_PIN_IRQ_DISABLE                     ((uint8_t)0x00)   /**< \brief Disable NMI pin interrupt request */
#define HC32F460_ICG_NMI_PIN_IRQ_ENABLE                      ((uint8_t)0x20)   /**< \brief Enable NMI pin interrupt request */

/**
 *******************************************************************************
 ** \brief Enable or disable NMI digital filter function
 ******************************************************************************/
#define HC32F460_ICG_NMI_DIGITAL_FILTER_DISABLE              ((uint8_t)0x00)   /**< \brief Disable NMI digital filter */
#define HC32F460_ICG_NMI_DIGITAL_FILTER_ENABLE               ((uint8_t)0x40)   /**< \brief Enable NMI digital filter */

/**
 *******************************************************************************
 ** \brief Enable or disable NMI pin ICG function
 ******************************************************************************/
#define HC32F460_ICG_NMI_PIN_ICG_FUNCTION_DISABLE            ((uint8_t)0x80)   /**< \brief Disable NMI pin ICG function */
#define HC32F460_ICG_NMI_PIN_ICG_FUNCTION_ENABLE             ((uint8_t)0x00)   /**< \brief Enable NMI pin ICG function */



/**
 *******************************************************************************
 ** \brief HRC hardware start configuration
 ******************************************************************************/
/** \brief Enable or disable HRC hardware start */
#define HC32F460_ICG1_HRC_HARDWARE_START                 (ICG_FUNCTION_OFF)

/** \brief HRC register config */
#define HC32F460_ICG1_HRC_FREQSEL                        (HC32F460_ICG_HRC_FREQUENCY_16MHZ)
#define HC32F460_ICG1_HRC_STOP                           (HC32F460_ICG_HRC_OSCILLATION_START)

/** \brief HRC register config value */
#if HC32F460_ICG1_HRC_HARDWARE_START == ICG_FUNCTION_ON
#define HC32F460_ICG1_HRC_REG_CONFIG                     (HC32F460_ICG1_HRC_FREQSEL | HC32F460_ICG1_HRC_STOP)
#else
#define HC32F460_ICG1_HRC_REG_CONFIG                     ((uint16_t)0xFFFF)
#endif

/**
 *******************************************************************************
 ** \brief VDU0 hardware start configuration
 ******************************************************************************/
/** \brief Enable or disable VDU0 hardware start */
#define HC32F460_ICG1_VDU0_HARDWARE_START                (HC32F460_ICG_FUNCTION_OFF)

/** \brief VDU0 register config */
#define HC32F460_ICG1_VDU0_BOR_LEV                       (HC32F460_ICG_VDU0_VOLTAGE_THRESHOLD_2P3)
#define HC32F460_ICG1_VDU0_BORDIS                        (HC32F460_ICG_VDU0_STOP_AFTER_RESET)

/** \brief VDU0 register config value */
#if HC32F460_ICG1_VDU0_HARDWARE_START == ICG_FUNCTION_ON
#define HC32F460_ICG1_VDU0_REG_CONFIG                    (HC32F460_ICG1_VDU0_BOR_LEV | HC32F460_ICG1_VDU0_BORDIS)
#else
#define HC32F460_ICG1_VDU0_REG_CONFIG                    ((uint8_t)0xFF)
#endif

/**
 *******************************************************************************
 ** \brief NMI hardware start configuration
 ******************************************************************************/
/** \brief Enable or disable NMI hardware start */
#define HC32F460_ICG1_NMI_HARDWARE_START                 (ICG_FUNCTION_OFF)

/** \brief NMI register config */
#define HC32F460_ICG1_NMI_SMPCLK                         (HC32F460_ICG_NMI_PIN_FILTER_PCLK3_DIV1)
#define HC32F460_ICG1_NMI_TRG                            (HC32F460_ICG_NMI_PIN_TRIGGER_EDGE_RISING)
#define HC32F460_ICG1_NMI_IMR                            (HC32F460_ICG_NMI_PIN_IRQ_DISABLE)
#define HC32F460_ICG1_NMI_NFEN                           (HC32F460_ICG_NMI_DIGITAL_FILTER_DISABLE)
#define HC32F460_ICG1_NMI_ICGENA                         (HC32F460_ICG_NMI_PIN_ICG_FUNCTION_DISABLE)

/** \brief NMI register config value */
#if HC32F460_ICG1_NMI_HARDWARE_START == ICG_FUNCTION_ON
#define HC32F460_ICG1_NMI_REG_CONFIG                     (HC32F460_ICG1_NMI_SMPCLK | HC32F460_ICG1_NMI_TRG | \
                                                          HC32F460_ICG1_NMI_IMR | HC32F460_ICG1_NMI_NFEN | HC32F460_ICG1_NMI_ICGENA)
#else
#define HC32F460_ICG1_NMI_REG_CONFIG                     ((uint8_t)0xFF)
#endif







/**
 *******************************************************************************
 ** \brief ICG registers configuration
 ******************************************************************************/
/** \brief ICG0 register value */
#define HC32F460_ICG0_REGISTER_CONSTANT          (((uint32_t)HC32F460_ICG0_WDT_REG_CONFIG << 16)  | \
                                                 ((uint32_t)HC32F460_ICG0_SWDT_REG_CONFIG))
/** \brief ICG1 register value */
#define HC32F460_ICG1_REGISTER_CONSTANT          (((uint32_t)HC32F460_ICG1_NMI_REG_CONFIG << 24) | \
                                                 ((uint32_t)HC32F460_ICG1_VDU0_REG_CONFIG << 16) | \
                                                 ((uint32_t)HC32F460_ICG1_HRC_REG_CONFIG))
/** \brief ICG2~7 register reserved value */
#define HC32F460_ICG2_REGISTER_CONSTANT                  ((uint32_t)0xFFFFFFFFul)
#define HC32F460_ICG3_REGISTER_CONSTANT                  ((uint32_t)0xFFFFFFFFul)
#define HC32F460_ICG4_REGISTER_CONSTANT                  ((uint32_t)0xFFFFFFFFul)
#define HC32F460_ICG5_REGISTER_CONSTANT                  ((uint32_t)0xFFFFFFFFul)
#define HC32F460_ICG6_REGISTER_CONSTANT                  ((uint32_t)0xFFFFFFFFul)
#define HC32F460_ICG7_REGISTER_CONSTANT                  ((uint32_t)0xFFFFFFFFul)




/**
 * @} hc32f460_if_icg
 */

#ifdef __cplusplus
}
#endif

#endif
/* end of file */
