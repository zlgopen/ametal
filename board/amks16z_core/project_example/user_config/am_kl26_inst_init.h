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
 * \brief 实例初始化函数声明
 *
 * 所有外设设备（I2C、SPI、ADC...）的实例初始化函数均在本头文件中声明，使用实例
 * 初始化函数，可以轻松获得一个外设的句柄，然后使用相关的接口函数对外设进行操作。
 *
 * \sa am_kl26_inst_init.h
 * \internal
 * \par Modification history
 * - 1.00 16-09-13  nwt, first implementation.
 * \endinternal
 */
 
#ifndef __AM_KL26_INST_INIT_H
#define __AM_KL26_INST_INIT_H
 
#include "ametal.h"
#include "am_adc.h"
#include "am_crc.h"
#include "am_i2c.h"
#include "am_timer.h"
#include "am_cap.h"
#include "am_pwm.h"
#include "am_wdt.h"
#include "am_spi.h"
#include "am_uart.h"
#include "am_baudrate_detect.h"
#include "am_hwconf_led_gpio.h"
#include "am_hwconf_key_gpio.h"
#include "am_hwconf_buzzer_pwm.h"
#include "am_hwconf_debug_uart.h"
#include "am_hwconf_system_tick_softimer.h"
#include "am_hwconf_lm75.h"
#include "am_hwconf_microport.h"
#include "am_hwconf_miniport_595.h"
#include "am_hwconf_miniport_view.h"
#include "am_hwconf_miniport_led.h"
#include "am_hwconf_miniport_key.h"
#include "am_hwconf_miniport_view_key.h"
#include "am_gpio.h"
#include "am_rtc.h"
#include "am_dac.h"

/**
 * \addtogroup am_kl26_if_inst_init
 * \copydoc am_kl26_inst_init.h
 * @{
 */

/**
 * \brief  中断实例初始化，初始化中断驱动
 * \retval AM_OK为实例化成功，其他失败
 */
int am_kl26_nvic_inst_init (void);

/**
 * \brief  中断实例解初始化
 * \retval 无
 */
void am_kl26_nvic_inst_deinit (void);

/**
 * \brief  tpm0 Timer 实例初始化 获取Timer标准服务句柄
 * \retval Timer标准服务句柄  若为NULL，表明初始化失败
 */
am_timer_handle_t am_kl26_tpm0_timing_inst_init (void);

/**
 * \brief  tpm0 Timer 实例解初始化
 * \param[in] handle : 通过 am_kl26_tpm0_timing_inst_init() 函数获得的Timer句柄
 * \retval 无
 */
void am_kl26_tpm0_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief  tpm1 Timer 实例初始化 获取Timer标准服务句柄
 * \retval Timer标准服务句柄  若为NULL，表明初始化失败
 */
am_timer_handle_t am_kl26_tpm1_timing_inst_init (void);

/**
 * \brief  tpm1 Timer 实例解初始化
 * \param[in] handle : 通过 am_kl26_tpm0_timing_inst_init() 函数获得的Timer句柄
 * \retval 无
 */
void am_kl26_tpm1_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief  tpm2 Timer 实例初始化 获取Timer标准服务句柄
 * \retval Timer标准服务句柄  若为NULL，表明初始化失败
 */
am_timer_handle_t am_kl26_tpm2_timing_inst_init (void);


/**
 * \brief  tpm2 Timer 实例解初始化
 * \param[in] handle : 通过 am_kl26_tpm0_timing_inst_init() 函数获得的Timer句柄
 * \retval 无
 */
void am_kl26_tpm2_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief  tpm0 pwm 实例初始化 获取Timer标准服务句柄
 * \retval pwm标准服务句柄  若为NULL，表明初始化失败
 */
am_pwm_handle_t am_kl26_tpm0_pwm_inst_init (void);

/**
 * \brief  tpm0 pwm 实例解初始化
 * \param[in] handle : 通过 am_kl26_tpm0_pwm_inst_init() 函数获得的pwm句柄
 * \retval 无
 */
void am_kl26_tpm0_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief  tpm1 pwm 实例初始化 获取Timer标准服务句柄
 * \retval pwm标准服务句柄  若为NULL，表明初始化失败
 */
am_pwm_handle_t am_kl26_tpm1_pwm_inst_init (void);

/**
 * \brief  tpm1 pwm 实例解初始化
 * \param[in] handle : 通过 am_kl26_tpm1_pwm_inst_init() 函数获得的pwm句柄
 * \retval 无
 */
void am_kl26_tpm1_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief  tpm2 pwm 实例初始化 获取Timer标准服务句柄
 * \retval pwm标准服务句柄  若为NULL，表明初始化失败
 */
am_pwm_handle_t am_kl26_tpm2_pwm_inst_init (void);

/**
 * \brief  tpm2 pwm 实例解初始化
 * \param[in] handle : 通过 am_kl26_tpm2_pwm_inst_init() 函数获得的pwm句柄
 * \retval 无
 */
void am_kl26_tpm2_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief  tpm0 cap 实例初始化 获取Timer标准服务句柄
 * \retval cap标准服务句柄  若为NULL，表明初始化失败
 */
am_cap_handle_t am_kl26_tpm0_cap_inst_init (void);

/**
 * \brief  tpm0 cap 实例解初始化
 * \param[in] handle : 通过 am_kl26_tpm0_cap_inst_init() 函数获得的cap句柄
 * \retval 无
 */
void am_kl26_tpm0_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief  tpm1 cap 实例初始化 获取Timer标准服务句柄
 * \retval cap标准服务句柄  若为NULL，表明初始化失败
 */
am_cap_handle_t am_kl26_tpm1_cap_inst_init (void);

/**
 * \brief  tpm1 cap 实例解初始化
 * \param[in] handle : 通过 am_kl26_tpm1_cap_inst_init() 函数获得的cap句柄
 * \retval 无
 */
void am_kl26_tpm1_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief  tpm2 cap 实例初始化 获取Timer标准服务句柄
 * \retval cap标准服务句柄  若为NULL，表明初始化失败
 */
am_cap_handle_t am_kl26_tpm2_cap_inst_init (void);

/**
 * \brief  tpm2 cap 实例解初始化
 * \param[in] handle : 通过 am_kl26_tpm2_cap_inst_init() 函数获得的cap句柄
 * \retval 无
 */
void am_kl26_tpm2_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief  pit Timer 实例初始化 获取Timer标准服务句柄
 * \retval cap标准服务句柄  若为NULL，表明初始化失败
 */
am_timer_handle_t am_kl26_pit_timing_inst_init (void);

/**
 * \brief  pit Timer 实例解初始化
 * \param[in] handle : 通过 am_kl26_pit_timing_inst_init() 函数获得的Timer句柄
 * \retval 无
 */
void am_kl26_pit_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief  CLK 实例初始化，初始化系统时钟
 * \retval AM_OK : 时钟成功初始化
 */
int am_kl26_clk_inst_init (void);

/**
 * \brief  PMU 实例初始化，初始化系统时钟
 * \retval AM_OK : 成功初始化
 */
int am_kl26_pmu_inst_init (void);

/** \brief PMU实例解初始化 */
void am_kl26_pmu_inst_deinit (void);

/**
 * \brief  GPIO 实例初始化
 * \retval AM_OK    : 初始化成功
 */
int am_kl26_gpio_inst_init (void);

/**
 * \brief GPIO 实例解初始化
 * \return 无
 */
void am_kl26_gpio_inst_deinit (void);

/**
 * \brief  USART0 实例初始化，获得UART标准服务句柄
 * \return UART标准服务句柄，若为NULL，表明初始化失败
 */
am_uart_handle_t am_kl26_uart0_inst_init (void);

/**
 * \brief USART0 实例解初始化
 * \param[in] handle : 通过 am_kl26_uart0_inst_init() 函数获得的UART句柄
 * \return 无
 */
void am_kl26_uart0_inst_deinit (am_uart_handle_t handle);

/**
 * \brief  USART1 实例初始化，获得UART标准服务句柄
 * \return UART标准服务句柄，若为NULL，表明初始化失败
 */
am_uart_handle_t am_kl26_uart1_inst_init (void);

/**
 * \brief     USART1 实例解初始化
 * \param[in] handle : 通过 am_kl26_uart1_inst_init() 函数获得的UART句柄
 * \return 无
 */
void am_kl26_uart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief  USART2 实例初始化，获得UART标准服务句柄
 * \return UART标准服务句柄，若为NULL，表明初始化失败
 */
am_uart_handle_t am_kl26_uart2_inst_init (void);

/**
 * \brief USART2 实例解初始化
 * \param[in] handle : 通过 am_kl26_uart2_inst_init() 函数获得的UART句柄
 * \return 无
 */
void am_kl26_uart2_inst_deinit (am_uart_handle_t handle);

/**
 * \brief  RTC 实例初始化，获得RTC标准服务句柄
 * \return RTC 标准服务句柄，若为NULL，表明初始化失败
 */
am_rtc_handle_t am_kl26_rtc_inst_init (void);

/**
 * \brief RTC 实例解初始化
 * \param[in] handle : 通过 am_kl26_rtc_inst_init() 函数获得的RTC句柄
 * \return 无
 */
void am_kl26_rtc_inst_deinit (am_rtc_handle_t handle);


/**
 * \brief  LPTMR Timer 实例初始化，获得Timer标准服务句柄
 * \return Timer 标准服务句柄，若为NULL，表明初始化失败
 */
am_timer_handle_t am_kl26_lptmr_timing_inst_init (void);

/**
 * \brief LPTMR Timer 实例解初始化
 * \param[in] handle : 通过 am_kl26_lptmr_timing_inst_init() 函数获得的Timer句柄
 * \return 无
 */
void am_kl26_lptmr_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief ADC0 实例初始化，获得ADC标准服务句柄
 *
 * \return 无
 */
am_adc_handle_t am_kl26_adc0_inst_init (void);

/**
 * \brief ADC0 实例解初始化
 * \param[in] handle : ADC0句柄值
 *
 * \return 无
 */
void am_kl26_adc0_inst_deinit (am_adc_handle_t handle);

/**
 * \brief DMA 实例初始化
 * \return 无
 */
void am_kl26_dma_inst_init (void);

/**
 * \brief DMA 实例解初始化
 * \return 无
 */
void am_kl26_dma_inst_deinit (void);

/**
 * \brief I2C0 实例初始化
 * \return I2C句柄值
 */
am_i2c_handle_t am_kl26_i2c0_inst_init (void);

/**
 * \brief I2C0 实例解初始化
 * \return 无
 */
void am_kl26_i2c0_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C1 实例初始化
 * \return I2C句柄值
 */
am_i2c_handle_t am_kl26_i2c1_inst_init (void);


/**
 * \brief I2C1 实例解初始化
 * \return 无
 */
void am_kl26_i2c1_inst_deinit (am_i2c_handle_t handle);


/**
 * \brief SPI0 实例初始化，获得SPI标准服务句柄(INT方式)
 * \return SPI0句柄值，若为NULL，表明初始化失败
 */
am_spi_handle_t am_kl26_spi0_int_inst_init (void);

/**
 * \brief SPI0 实例解初始化(INT方式)
 * \param[in] handle : 通过 am_kl26_spi0_int_inst_init() 函数获得的SPI0句柄
 * \return 无
 */
void am_kl26_spi0_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 实例初始化，获得SPI标准服务句柄(INT方式)
 * \return SPI1句柄值，若为NULL，表明初始化失败
 */
am_spi_handle_t am_kl26_spi1_int_inst_init (void);

/**
 * \brief SPI1 实例解初始化(INT方式)
 * \param[in] handle : 通过 am_kl26_spi1_int_inst_init() 函数获得的SPI0句柄
 * \return 无
 */
void am_kl26_spi1_int_inst_deinit (am_spi_handle_t handle);


/**
 * \brief SPI0 实例初始化，获得SPI标准服务句柄(DMA方式)
 * \return SPI0句柄值，若为NULL，表明初始化失败
 */
am_spi_handle_t am_kl26_spi0_dma_inst_init (void);

/**
 * \brief SPI0 实例解初始化(DMA方式)
 * \param[in] handle : 通过 am_kl26_spi0_dma_inst_init() 函数获得的SPI0句柄
 * \return 无
 */
void am_kl26_spi0_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 实例初始化，获得SPI标准服务句柄(DMA方式)
 * \return SPI1句柄值，若为NULL，表明初始化失败
 */
am_spi_handle_t am_kl26_spi1_dma_inst_init (void);

/**
 * \brief SPI1 实例解初始化(DMA方式)
 * \param[in] handle : 通过 am_kl26_spi1_dma_inst_init() 函数获得的SPI0句柄
 * \return 无
 */
void am_kl26_spi1_dma_inst_deinit (am_spi_handle_t handle);
/**
 * \brief TSI实例初始化
 *
 * \param[in] 无
 *
 * \retval AM_OK: 初始化操作成功
 */
int am_kl26_tsi_inst_init (void);

/**
 * \brief TSI实例解初始化
 *
 * \return 无
 */
void am_kl26_tsi_inst_deinit (void);

/**
 * \brief DAC0 实例初始化
 * \return 获得DAC标准服务句柄
 */
am_dac_handle_t am_kl26_dac0_inst_init (void);

/**
 * \brief DAC0 实例解初始化
 * \param[in] handle : DAC0句柄值
 *
 * \return 无
 */
void am_kl26_dac0_inst_deinit (am_dac_handle_t handle);

/**
 * \brief  WDT 实例初始化，获得WDT标准服务句柄
 *
 * \return WDT句柄值
 */
am_wdt_handle_t am_kl26_wdt_inst_init (void);

/**
 * \brief WDT 实例解初始化
 *
 * \param[in] handle : WDT看门狗句柄值
 */
void am_kl26_wdt_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief GPIO LED instance init
 * \param[in] none
 * \return AM_OK: 初始化成功
 */
int am_led_gpio_inst_init (void);

/** \brief SYSTICK 实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_kl26_systick_inst_init (void);

/** \brief SYSTICK 实例解初始化 */
void am_kl26_systick_inst_deinit (am_timer_handle_t handle);

/** \brief 实例初始化，获得自动波特率服务句柄 */
am_baudrate_detect_handle_t am_kl26_baudrate_detect_inst_init (void);

/** \brief 实例解初始化*/
void am_kl26_baudrate_detect_inst_deinit(am_baudrate_detect_handle_t handle);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AM_KL26_INST_INIT_H */

/* end of file */
