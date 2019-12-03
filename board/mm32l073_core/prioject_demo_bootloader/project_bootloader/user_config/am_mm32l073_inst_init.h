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
 * \brief MM32L073 外设实例初始化函数声明
 *
 * 所有外设设备（I2C、SPI、ADC...）的实例初始化函数均在本头文件中声明，使用实例
 * 初始化函数，可以轻松获得一个外设的句柄，然后使用相关的接口函数对外设进行操作。
 *
 * \sa am_mm32l073_inst_init.h
 * \internal
 * \par Modification history
 * - 1.00 16-10-26  nwt, first implementation.
 * \endinternal
 */
 
#ifndef __AM_MM32L073_INST_INIT_H
#define __AM_MM32L073_INST_INIT_H

#include "ametal.h"
#include "am_adc.h"
#include "am_crc.h"
#include "am_i2c.h"
#include "am_i2c_slv.h"
#include "am_timer.h"
#include "am_cap.h"
#include "am_pwm.h"
#include "am_wdt.h"
#include "am_spi.h"
#include "am_spi_slv.h"
#include "am_uart.h"
#include "am_mm32l073_pwr.h"
#include "am_hwconf_led_gpio.h"
#include "am_hwconf_key_gpio.h"
#include "am_hwconf_buzzer_pwm.h"
#include "am_hwconf_debug_uart.h"
#include "am_hwconf_system_tick_softimer.h"
#include "am_hwconf_lm75.h"
#include "am_hwconf_microport.h"
#include "am_hwconf_miniport_zlg72128.h"
#include "am_hwconf_miniport_595.h"
#include "am_hwconf_miniport_view.h"
#include "am_hwconf_miniport_led.h"
#include "am_hwconf_miniport_key.h"
#include "am_hwconf_miniport_view_key.h"

/**
 * \addtogroup am_if_mm32l073_inst_init
 * \copydoc am_mm32l073_inst_init.h
 * @{
 */

/**
 * \brief 中断实例初始化，初始化中断驱动
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_mm32l073_nvic_inst_init (void);

/**
 * \brief 中断实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_mm32l073_nvic_inst_deinit (void);

/**
 * \brief ADC 实例初始化，获得 ADC 标准服务句柄
 *
 * \param 无
 *
 * \return ADC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_adc_handle_t am_mm32l073_adc_inst_init (void);

/**
 * \brief ADC 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_adc_inst_init() 函数获得的 ADC 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_adc_inst_deinit (am_adc_handle_t handle);

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_mm32l073_clk_inst_init (void);

/**
 * \brief DMA 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_mm32l073_dma_inst_init (void);

/**
 * \brief DMA 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_mm32l073_dma_inst_deinit (void);

/**
 * \brief GPIO 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_mm32l073_gpio_inst_init (void);

/**
 * \brief GPIO 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_mm32l073_gpio_inst_deinit (void);

/**
 * \brief I2C1 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_mm32l073_i2c1_inst_init (void);

/**
 * \brief I2C1 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_i2c1_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_i2c1_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C1 从机实例初始化，获得 I2C 从机标准服务句柄
 *
 * \param 无
 *
 * \return I2C 从机标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_slv_handle_t  am_mm32l073_i2c1_slv_inst_init (void);

/**
 * \brief I2C1 从机实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_i2c1_slv_inst_init() 函数
 *                   获得的 I2C 从机标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_i2c1_slv_inst_deinit (am_i2c_slv_handle_t handle);

/**
 * \brief IWDT 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_mm32l073_iwdg_inst_init (void);

/**
 * \brief IWDT 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_iwdg_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_iwdg_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief PWR 实例初始化
 *
 * \param 无
 *
 * \return PWR 标准服务句柄，若为 NULL，表明初始化失败
 */
am_mm32l073_pwr_handle_t am_mm32l073_pwr_inst_init (void);

/**
 * \brief PWR 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_mm32l073_pwr_inst_deinit (void);

/**
 * \brief SPI1 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_mm32l073_spi1_dma_inst_init (void);

/**
 * \brief SPI1 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_spi1_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_spi1_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI2 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_mm32l073_spi2_dma_inst_init (void);

/**
 * \brief SPI2 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_spi2_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_spi2_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_mm32l073_spi1_int_inst_init (void);

/**
 * \brief SPI1 INT 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_spi1_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_spi1_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI2 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_mm32l073_spi2_int_inst_init (void);

/**
 * \brief SPI2 INT 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_spi2_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_spi2_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 从机实例初始化，获得 SPI 从机标准服务句柄
 *
 * \param 无
 *
 * \return SPI 从机标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_slv_handle_t am_mm32l073_spi1_slv_dma_inst_init (void);

/**
 * \brief SPI1 从机实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_spi1_slv_dma_inst_init() 函数
 *                   获得的 SPI 从机标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_spi1_slv_dma_inst_deinit (am_spi_slv_handle_t handle);

/**
 * \brief SPI1 从机实例初始化，获得 SPI 从机标准服务句柄
 *
 * \param 无
 *
 * \return SPI 从机标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_slv_handle_t am_mm32l073_spi2_slv_dma_inst_init (void);

/**
 * \brief SPI1 从机实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_spi2_slv_dma_inst_init() 函数
 *                   获得的 SPI 从机标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_spi2_slv_dma_inst_deinit (am_spi_slv_handle_t handle);

/**
 * \brief SYSTICK 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_mm32l073_systick_inst_init (void);

/**
 * \brief SYSTICK 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_systick_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_systick_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM1 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_mm32l073_tim1_cap_inst_init (void);

/**
 * \brief TIM1 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim1_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim1_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM2 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_mm32l073_tim2_cap_inst_init (void);

/**
 * \brief TIM2 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim2_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim2_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM3 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_mm32l073_tim3_cap_inst_init (void);

/**
 * \brief TIM3 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim3_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim3_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM14 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_mm32l073_tim14_cap_inst_init (void);

/**
 * \brief TIM14 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim14_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim14_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM16 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_mm32l073_tim16_cap_inst_init (void);

/**
 * \brief TIM16 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim16_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim16_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM17 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_mm32l073_tim17_cap_inst_init (void);

/**
 * \brief TIM17 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim17_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim17_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM1 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_mm32l073_tim1_pwm_inst_init (void);

/**
 * \brief TIM1 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim1_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim1_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM2 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_mm32l073_tim2_pwm_inst_init (void);

/**
 * \brief TIM2 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim2_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim2_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM3 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_mm32l073_tim3_pwm_inst_init (void);

/**
 * \brief TIM3 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim3_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim3_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM14 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_mm32l073_tim14_pwm_inst_init (void);

/**
 * \brief TIM14 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim14_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim14_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM16 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_mm32l073_tim16_pwm_inst_init (void);

/**
 * \brief TIM16 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim16_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim16_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM17 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_mm32l073_tim17_pwm_inst_init (void);

/**
 * \brief TIM17 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim17_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim17_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM1 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_mm32l073_tim1_timing_inst_init (void);

/**
 * \brief TIM1 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim1_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim1_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM2 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_mm32l073_tim2_timing_inst_init (void);

/**
 * \brief TIM2 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim2_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim2_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM3 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_mm32l073_tim3_timing_inst_init (void);

/**
 * \brief TIM3 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim3_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim3_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM14 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_mm32l073_tim14_timing_inst_init (void);

/**
 * \brief TIM14 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim14_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim14_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM16 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_mm32l073_tim16_timing_inst_init (void);

/**
 * \brief TIM16 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim16_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim16_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM17 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_mm32l073_tim17_timing_inst_init (void);

/**
 * \brief TIM17 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_tim17_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_tim17_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief UART1 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_mm32l073_uart1_inst_init (void);

/**
 * \brief UART1 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_uart1_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_uart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART2 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_mm32l073_uart2_inst_init (void);

/**
 * \brief UART2 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_uart2_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_uart2_inst_deinit (am_uart_handle_t handle);

/**
 * \brief WWDG 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_mm32l073_wwdg_inst_init (void);

/**
 * \brief WWDG 实例解初始化
 *
 * \param[in] handle 通过 am_mm32l073_wwdg_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_mm32l073_wwdg_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief SPI0 实例初始化，获得SPI标准服务句柄(POLL方式)
 * \return SPI0句柄值，若为NULL，表明初始化失败
 */
am_spi_handle_t am_mm32l073_spi1_poll_inst_init (void);

/**
 * \brief SPI0 实例解初始化(POLL方式)
 * \param[in] handle : 通过 am_mm32l073_spi0_poll_inst_init() 函数获得的SPI0句柄
 * \retval 无
 */
void am_mm32l073_spi1_poll_inst_deinit (am_spi_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AM_MM32L073_INST_INIT_H */

/* end of file */
