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
 * \brief LPC82X 外设实例初始化函数声明
 *
 * 所有外设设备（I2C、SPI、ADC...）的实例初始化函数均在本头文件中声明，使用实例
 * 初始化函数，可以轻松获得一个外设的句柄，然后使用相关的接口函数对外设进行操作
 *
 * \sa am_lpc84x_inst_init.h
 * \internal
 * \par Modification history
 * - 1.00 15-12-11  tee, first implementation
 * \endinternal
 */

#ifndef __AM_LPC82X_INST_INIT_H
#define __AM_LPC82X_INST_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_adc.h"
#include "am_crc.h"
#include "am_i2c.h"
#include "am_cap.h"
#include "am_pwm.h"
#include "am_wdt.h"
#include "am_spi.h"
#include "am_uart.h"
#include "am_timer.h"
#include "am_dac.h"
#include "am_ep24cxx.h"
#include "am_lpc_sct.h"
#include "am_hwconf_lm75.h"
#include "am_hwconf_led_gpio.h"
#include "am_hwconf_key_gpio.h"
#include "am_hwconf_buzzer_pwm.h"
#include "am_hwconf_debug_uart.h"
#include "am_hwconf_system_tick_softimer.h"

/**
 * \addtogroup am_if_lpc84x_inst_init
 * \copydoc am_lpc84x_inst_init.h
 * @{
 */

/**
 * \brief ADC0 (DMA 方式) 实例初始化，获得 ADC 标准服务句柄
 *
 * \param 无
 *
 * \return ADC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_adc_handle_t am_lpc84x_adc0_dma_inst_init (void);

/**
 * \brief ADC0 (DMA 方式) 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_adc0_dma_inst_init() 函数获得的 ADC 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_adc0_dma_inst_deinit (am_adc_handle_t handle);

/**
 * \brief ADC0 (中断方式) 实例初始化，获得 ADC 标准服务句柄
 *
 * \param 无
 *
 * \return ADC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_adc_handle_t am_lpc84x_adc0_int_inst_init (void);

/**
 * \brief ADC0 (中断方式) 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_adc0_int_inst_init() 函数获得的 ADC 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_adc0_int_inst_deinit (am_adc_handle_t handle);

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_lpc84x_clk_inst_init (void);

/**
 * \brief CRC 实例初始化，获得 CRC 标准服务句柄
 *
 * \param 无
 *
 * \return CRC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_crc_handle_t am_lpc84x_crc_inst_init (void);

/**
 * \brief CRC 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_lpc84x_crc_inst_deinit (void);

/**
 * \brief DMA 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_lpc84x_dma_inst_init (void);

/**
 * \brief DMA 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_lpc84x_dma_inst_deinit (void);

/**
 * \brief GPIO 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_lpc84x_gpio_inst_init (void);

/**
 * \brief GPIO 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_lpc84x_gpio_inst_deinit (void);

/**
 * \brief I2C0 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_lpc84x_i2c0_inst_init (void);

/**
 * \brief I2C0 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_i2c0_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_lpc84x_i2c0_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C1 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_lpc84x_i2c1_inst_init (void);

/**
 * \brief I2C1 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_i2c1_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_lpc84x_i2c1_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C2 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_lpc84x_i2c2_inst_init (void);

/**
 * \brief I2C2 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_i2c2_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_lpc84x_i2c2_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C3 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_lpc84x_i2c3_inst_init (void);

/**
 * \brief I2C3 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_i2c3_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_lpc84x_i2c3_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief MRT 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_lpc84x_mrt_inst_init (void);

/**
 * \brief MRT 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_mrt_inst_init() 函数获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_lpc84x_mrt_inst_deinit (am_timer_handle_t handle);

/**
 * \brief SCT0 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_lpc84x_sct0_pwm_inst_init (void);

/**
 * \brief SCT0 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_sct0_pwm_inst_init() 函数获得的 PWM 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_sct0_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief SCT0 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_lpc84x_sct0_timing_inst_init (void);

/**
 * \brief SCT0 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_sct0_timing_inst_init() 函数获得的 TIMER 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_sct0_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief SCT0 实例初始化，获得 SCT 服务句柄
 *
 * \param 无
 *
 * \return SCT 服务句柄，若为 NULL，表明初始化失败
 */
am_lpc_sct_handle_t am_lpc84x_sct0_inst_init (void);

/**
 * \brief SCT0 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_sct0_inst_init() 函数获得的 SCT 服务句柄
 *
 * \return 无
 */
void am_lpc84x_sct0_inst_deinit (am_lpc_sct_handle_t handle);

/**
 * \brief SPI0 (DMA 方式) 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_lpc84x_spi0_dma_inst_init (void);

/**
 * \brief SPI0 (DMA 方式) 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_spi0_dma_inst_init() 函数获得的 SPI 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_spi0_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI0 (中断方式) 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_lpc84x_spi0_int_inst_init (void);

/**
 * \brief SPI0 (中断方式) 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_spi0_int_inst_init() 函数获得的 SPI 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_spi0_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 (DMA 方式) 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_lpc84x_spi1_dma_inst_init (void);

/**
 * \brief SPI1 (DMA 方式) 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_spi1_dma_inst_init() 函数获得的 SPI 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_spi1_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 (中断方式) 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_lpc84x_spi1_int_inst_init (void);

/**
 * \brief SPI1 (中断方式) 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_spi1_int_inst_init() 函数获得的 SPI 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_spi1_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief USART0 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_lpc84x_usart0_inst_init (void);

/**
 * \brief USART0 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_usart0_inst_init() 函数获得的 UART 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_usart0_inst_deinit (am_uart_handle_t handle);

/**
 * \brief USART1 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_lpc84x_usart1_inst_init (void);

/**
 * \brief USART1 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_usart1_inst_init() 函数获得的 UART 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_usart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief USART2 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_lpc84x_usart2_inst_init (void);

/**
 * \brief USART2 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_usart2_inst_init() 函数获得的 UART 标准
 *                   服务句柄
 *
 * \return 无
 */
void am_lpc84x_usart2_inst_deinit (am_uart_handle_t handle);

/**
 * \brief WKT 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_lpc84x_wkt_inst_init (void);

/**
 * \brief WKT 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_wkt_inst_init() 函数获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_lpc84x_wkt_inst_deinit (am_timer_handle_t handle);

/**
 * \brief WWDT 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_lpc84x_wwdt_inst_init (void);

/**
 * \brief WWDT 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_wwdt_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_lpc84x_wwdt_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief 中断实例初始化，初始化中断驱动
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_arm_nvic_inst_init (void);

/**
 * \brief 中断实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_arm_nvic_inst_deinit (void);

/**
 * \brief SYSTICK 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_arm_systick_inst_init (void);

/**
 * \brief SYSTICK 实例解初始化
 *
 * \param[in] handle 通过 am_arm_systick_inst_init() 函数获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_arm_systick_inst_deinit (am_timer_handle_t handle);

/**
 * \brief DAC0 实例初始化
 *
 * \return 无
 */
am_dac_handle_t am_lpc84x_dac0_inst_init (void);

/**
 * \brief DAC0 实例解初始化
 *
 * \param[in] handle 通过 am_lpc84x_dac0_inst_init() 函数获得的 dac 标准服务句柄
 *
 * \return 无
 */
void am_lpc84x_dac0_inst_deinit (am_dac_handle_t handle);

/**
 * \brief SP0I (轮询方式) 实例初始化
 */
am_spi_handle_t am_lpc84x_spi0_poll_inst_init (void);

/**
 * \brief SPI0 (轮询方式) 实例解初始化
 */
void am_lpc84x_spi0_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief CAT24C512 EEPROM 实例初始化
 */
am_ep24cxx_handle_t am_eeprom_cat24c512_inst_init (void);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AM_LPC82X_INST_INIT_H */

/* end of file */
