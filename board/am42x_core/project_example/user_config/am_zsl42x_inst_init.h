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
 * \brief ZSL42x 外设 实例初始化函数声明
 *
 * 所有外设设备（I2C、SPI、ADC...）的实例初始化函数均在本头文件中声明，使用实例
 * 初始化函数，可以轻松获得一个外设的句柄，然后使用相关的接口函数对外设进行操作。
 *
 * \sa am_zsl42x_inst_init.h
 * \internal
 * \par Modification history
 * - 1.00 15-12-11  tee, first implementation
 * \endinternal
 */

#ifndef __am_zsl42x_inst_init_H
#define __am_zsl42x_inst_init_H

#ifdef __cplusplus
extern "C" {
#endif

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
#include "am_led.h"
#include "am_uart.h"
#include "am_dac.h"
#include "am_rtc.h"
#include "am_ftl.h"
#include "am_temp.h"
#include "am_delay.h"
#include "am_hc595.h"
#include "am_ili9341.h"
#include "am_rx8025t.h"
#include "am_digitron_disp.h"
#include "am_ep24cxx.h"
#include "am_mx25xx.h"
#include "am_mtd.h"
#include "am_rngbuf.h"
#include "am_input.h"
#include "am_zsl42x_pca.h"
#include "am_zsl42x_pcnt.h"
#include "am_hwconf_zsl42x_led_gpio.h"
#include "am_hwconf_key_gpio.h"
#include "am_hwconf_buzzer_pwm.h"
#include "am_hwconf_debug_uart.h"
#include "am_hwconf_system_tick_softimer.h"
#include "am_hwconf_miniport_zlg72128.h"
#include "am_hwconf_miniport_595.h"
#include "am_hwconf_miniport_view.h"
#include "am_hwconf_miniport_led.h"
#include "am_hwconf_miniport_key.h"
#include "am_hwconf_miniport_view_key.h"
#include "am_event_input_key.h"
#include "am_event_category_input.h"

/**
 * \addtogroup am_if_zsl42x_inst_init
 * \copydoc am_zsl42x_inst_init.h
 * @{
 */

/**
 * \brief MiniPort-ZLG72128 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_miniport_zlg72128_inst_init (void);

/**
 * \brief 实例初始化函数
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_key_gpio_inst_init (void);

/**
 * \brief 中断实例初始化，初始化中断驱动
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_zsl42x_nvic_inst_init (void);

/**
 * \brief 中断实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_zsl42x_nvic_inst_deinit (void);

/**
 * \brief ILI9341 实例初始化，获得 ILI9341 标准服务句柄
 *
 * \param 无
 *
 * \return ILI9341 标准服务句柄，若为 NULL，表明初始化失败
 */
am_ili9341_handle_t am_zsl42x_ili9341_inst_init (void);

/**
 * \brief ILI9341 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_ili9341_inst_init() 函数获得的 ILI9341
 *                   标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_ili9341_inst_deinit (am_ili9341_handle_t handle);

/**
 * \brief ADC 实例初始化，获得 ADC 标准服务句柄
 *
 * \param 无
 *
 * \return ADC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_adc_handle_t am_zsl42x_adc_inst_init (void);

/**
 * \brief ADC 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_adc1_inst_init() 函数获得的 ADC 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_adc_inst_deinit (am_adc_handle_t handle);

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_zsl42x_clk_inst_init (void);

/**
 * \brief CRC 实例初始化，获得 CRC 标准服务句柄
 *
 * \param 无
 *
 * \return CRC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_crc_handle_t am_zsl42x_crc_inst_init (void);

/**
 * \brief CRC 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_crc_inst_init() 函数获得的 CRC 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_crc_inst_deinit (am_crc_handle_t handle);

/**
 * \brief DAC 实例初始化，获得 DAC 标准服务句柄
 *
 * \param 无
 *
 * \return DAC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_dac_handle_t am_zsl42x_dac_inst_init (void);

/**
 * \brief DAC 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_dac_inst_init() 函数获得的 DAC 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_dac_inst_deinit (am_dac_handle_t handle);

/**
 * \brief DMA 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_zsl42x_dma_inst_init (void);

/**
 * \brief DMA 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_zsl42x_dma_inst_deinit (void);

/**
 * \brief GPIO 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_zsl42x_gpio_inst_init (void);

/**
 * \brief GPIO 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_zsl42x_gpio_inst_deinit (void);

/**
 * \brief I2C1 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_zsl42x_i2c1_inst_init (void);

/**
 * \brief I2C1 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_i2c1_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_i2c1_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C2 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_zsl42x_i2c0_inst_init (void);

/**
 * \brief I2C2 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_i2c2_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_i2c0_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C1 从机实例初始化，获得 I2C 从机标准服务句柄
 *
 * \param 无
 *
 * \return I2C 从机标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_slv_handle_t am_zsl42x_i2c1_slv_inst_init (void);

/**
 * \brief I2C1 从机实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_i2c1_slv_inst_init() 函数
 *                   获得的 I2C 从机标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_i2c1_slv_inst_deinit (am_i2c_slv_handle_t handle);

/**
 * \brief I2C2 从机实例初始化，获得 I2C 从机标准服务句柄
 *
 * \param 无
 *
 * \return I2C 从机标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_slv_handle_t am_zsl42x_i2c0_slv_inst_init (void);

/**
 * \brief I2C2 从机实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_i2c2_slv_inst_init() 函数
 *                   获得的 I2C 从机标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_i2c0_slv_inst_deinit (am_i2c_slv_handle_t handle);

/**
 * \brief IWDG 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_zsl42x_iwdg_inst_init (void);

/**
 * \brief IWDG 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_iwdg_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_iwdg_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief PWR 实例初始化
 *
 * \param 无
 *
 * \return PWR 标准服务句柄，若为 NULL，表明初始化失败
 */
//am_zsl42x_pwr_handle_t am_zsl42x_pwr_inst_init (void);

/**
 * \brief PWR 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_zsl42x_pwr_inst_deinit (void);

/**
 * \brief RTC 实例初始化，获得 RTC 标准服务句柄
 *
 * \param 无
 *
 * \return RTC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_rtc_handle_t am_zsl42x_rtc_inst_init (void);

/**
 * \brief RTC 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_rtc_inst_init() 函数获得的 RTC 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_rtc_inst_deinit (am_rtc_handle_t handle);

/**
 * \brief SPI0 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_zsl42x_spi0_dma_inst_init (void);

/**
 * \brief SPI2 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_spi0_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_spi0_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_zsl42x_spi1_dma_inst_init (void);

/**
 * \brief SPI1 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_spi1_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_spi1_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI0 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_zsl42x_spi0_int_inst_init (void);

/**
 * \brief SPI0 INT 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_spi0_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_spi0_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_zsl42x_spi1_int_inst_init (void);

/**
 * \brief SPI1 INT 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_spi1_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_spi1_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI0 poll 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_zsl42x_spi0_poll_inst_init (void);

/**
 * \brief SPI2 poll 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_spi0_poll_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_spi0_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 poll 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_zsl42x_spi1_poll_inst_init (void);

/**
 * \brief SPI1 poll 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_spi1_poll_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_spi1_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SYSTICK 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_systick_inst_init (void);

/**
 * \brief SYSTICK 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_systick_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_systick_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM0 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_zsl42x_tim0_cap_inst_init (void);

/**
 * \brief TIM0 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim3_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim0_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM1 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_zsl42x_tim1_cap_inst_init (void);

/**
 * \brief TIM1 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim1_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim1_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM2 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_zsl42x_tim2_cap_inst_init (void);

/**
 * \brief TIM2 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim2_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim2_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM3 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_zsl42x_tim3_cap_inst_init (void);

/**
 * \brief TIM3 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim3_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim3_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM0 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_zsl42x_tim0_pwm_inst_init (void);

/**
 * \brief TIM0 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim1_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim0_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM1 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_zsl42x_tim1_pwm_inst_init (void);

/**
 * \brief TIM1 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim1_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim1_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM2 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_zsl42x_tim2_pwm_inst_init (void);

/**
 * \brief TIM2 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim2_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim2_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM3 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_zsl42x_tim3_pwm_inst_init (void);

/**
 * \brief TIM3 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim3_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim3_pwm_inst_deinit (am_pwm_handle_t handle);


/**
 * \brief TIM0 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_tim0_timing_inst_init (void);

/**
 * \brief TIM0 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim0_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim0_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM1 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_tim1_timing_inst_init (void);

/**
 * \brief TIM1 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim1_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim1_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM2 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_tim2_timing_inst_init (void);

/**
 * \brief TIM2 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim2_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim2_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM3 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_tim3_timing_inst_init (void);

/**
 * \brief TIM3 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim3_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim3_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief LPTIM0 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_lptim0_timing_inst_init (void);

/**
 * \brief LPTIM0 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_lptim0_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_lptim0_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief LPTIM1 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_lptim1_timing_inst_init (void);

/**
 * \brief LPTIM1 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_lptim1_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_lptim1_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief ADTIM4 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_zsl42x_tim4_cap_inst_init (void);

/**
 * \brief ADTIM4 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim4_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim4_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief ADTIM5 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_zsl42x_tim5_cap_inst_init (void);

/**
 * \brief ADTIM5 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim5_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim5_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief ADTIM6 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_zsl42x_tim6_cap_inst_init (void);

/**
 * \brief ADTIM6 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim6_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim6_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief ADTIM4 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_zsl42x_tim4_pwm_inst_init (void);

/**
 * \brief ADTIM4 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim4_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim4_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief ADTIM5 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_zsl42x_tim5_pwm_inst_init (void);

/**
 * \brief ADTIM5 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim5_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim5_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief ADTIM6 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_zsl42x_tim6_pwm_inst_init (void);

/**
 * \brief ADTIM6 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim6_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim6_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief ADTIM4 timing 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_tim4_timing_inst_init (void);

/**
 * \brief ADTIM4 timing 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim4_timing_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim4_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief ADTIM5 timing 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_tim5_timing_inst_init (void);

/**
 * \brief ADTIM5 timing 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim5_timing_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim5_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief ADTIM6 timing 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_zsl42x_tim6_timing_inst_init (void);

/**
 * \brief ADTIM6 timing 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_tim6_timing_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_tim6_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief UART0 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_zsl42x_uart0_inst_init (void);

/**
 * \brief UART0 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_uart1_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_uart0_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART1 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_zsl42x_uart1_inst_init (void);

/**
 * \brief UART1 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_uart1_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_uart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART2 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_zsl42x_uart2_inst_init (void);

/**
 * \brief UART2 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_uart2_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_uart2_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART3 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_zsl42x_uart3_inst_init (void);

/**
 * \brief UART3 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_uart3_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_uart3_inst_deinit (am_uart_handle_t handle);

/**
 * \brief LPUART0 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_zsl42x_lpuart0_inst_init (void);

/**
 * \brief LPUART0 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_lpuart0_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_lpuart0_inst_deinit (am_uart_handle_t handle);

/**
 * \brief LPUART1 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_zsl42x_lpuart1_inst_init (void);

/**
 * \brief LPUART1 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_lpuart1_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_lpuart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief WDT 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_zsl42x_wdt_inst_init (void);

/**
 * \brief WDT 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_wdt_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_zsl42x_wdt_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief PCA 实例初始化
 */
am_zsl42x_pca_handle_t am_zsl42x_pca1_inst_init (void);

/**
 * \brief PCA 实例解初始化
 */
void am_zsl42x_pca_inst_deinit (am_zsl42x_pca_handle_t handle);

/**
 * \brief PCNT 实例初始化，获得PCNT 服务句柄
 *
 * \param 无
 *
 * \return PCNT 服务句柄，若为 NULL，表明初始化失败
 */
am_zsl42x_pcnt_handle_t am_zsl42x_pcnt_inst_init (void);

/**
 * \brief PCNT 实例解初始化
 *
 * \param[in] handle 通过 am_zsl42x_pcnt1_inst_init() 函数获得的 PCNT 服务句柄
 *
 * \return 无
 */
void am_zsl42x_pcnt_inst_deinit (am_zsl42x_pcnt_handle_t handle);

/**
 * \brief MICROPORT RTC 实例初始化，获得RTC 服务句柄
 *
 * \param 无
 *
 * \return RTC 服务句柄，若为 NULL，表明初始化失败
 */
am_rtc_handle_t am_microport_std_rtc_inst_init (void);

/**
 * \brief MICROPORT 1302 RTC 实例初始化，获得RTC 服务句柄
 *
 * \param 无
 *
 * \return RTC 服务句柄，若为 NULL，表明初始化失败
 */
am_rtc_handle_t am_microport_ds1302_rtc_inst_init (void);

/**
 * \brief MICROPORT RX8025 RTC 实例初始化，获得RTC 服务句柄
 *
 * \param 无
 *
 * \return RTC 服务句柄，若为 NULL，表明初始化失败
 */
am_rtc_handle_t am_microport_rx8025t_rtc_inst_init (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __am_zsl42x_inst_init_H */

/* end of file */
