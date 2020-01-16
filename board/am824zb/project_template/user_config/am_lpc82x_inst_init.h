/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82x 外设 实例初始化函数声明
 *
 * 所有外设设备（I2C、SPI、ADC...）的实例初始化函数均在本头文件中声明，使用实例
 * 初始化函数，可以轻松获得一个外设的句柄，然后使用相关的接口函数对外设进行操作。
 *
 * \sa am_lpc82x_inst_init.h
 * \internal
 * \par Modification history
 * - 1.00 15-12-11  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC82X_INST_INIT_H
#define __AM_LPC82X_INST_INIT_H

#include "am_lpc_sct.h"
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
#include "am_zm516x.h"
#include "am_hwconf_miniport_zlg72128.h"
#include "am_hwconf_miniport_595.h"
#include "am_hwconf_led_gpio.h"
#include "am_hwconf_key_gpio.h"
#include "am_hwconf_miniport_view.h"
#include "am_hwconf_miniport_led.h"
#include "am_hwconf_miniport_key.h"
#include "am_hwconf_miniport_view_key.h"
#include "am_hwconf_lm75.h"
#include "am_hwconf_debug_uart.h"

/**
 * \addtogroup am_if_lpc82x_inst_init
 * \copydoc am_lpc82x_inst_init.h
 * @{
 */

/**
 * \brief  CLK 实例初始化，初始化系统时钟
 * \retval AM_OK : 时钟成功初始化
 * \note 时钟较为特殊，无解初始化函数
 */
int am_lpc82x_clk_inst_init (void);


/**
 * \brief  ADC0 中断服务实例初始化，获得ADC标准服务句柄
 * \return ADC标准服务句柄，若为NULL，表明初始化失败
 */
am_adc_handle_t am_lpc82x_adc0_int_inst_init (void);

/**
 * \brief ADC0 中断实例解初始化
 * \param[in] handle : 通过 am_lpc82x_adc0_inst_init() 函数获得的ADC句柄
 * \return 无
 */
void am_lpc82x_adc0_int_inst_deinit (am_adc_handle_t handle);


/**
 * \brief  ADC0 dma服务实例初始化，获得ADC标准服务句柄
 * \return ADC标准服务句柄，若为NULL，表明初始化失败
 */
am_adc_handle_t am_lpc82x_adc0_dma_inst_init (void);

/**
 * \brief ADC0 dma实例解初始化
 * \param[in] handle : 通过 am_lpc82x_adc0_inst_init() 函数获得的ADC句柄
 * \return 无
 */
void am_lpc82x_adc0_dma_inst_deinit (am_adc_handle_t handle);


/**
 * \brief  CRC 实例初始化，获得CRC标准服务句柄
 * \return CRC标准服务句柄，若为NULL，表明初始化失败
 */
am_crc_handle_t am_lpc82x_crc_inst_init (void);

/**
 * \brief CRC 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_crc_inst_init() 函数获得的CRC句柄
 * \return 无
 */
void am_lpc82x_crc_inst_deinit (am_crc_handle_t handle);

/**
 * \brief  DMA 实例初始化
 * \retval AM_OK    : 初始化成功
 */
int am_lpc82x_dma_inst_init (void);

/**
 * \brief DMA 实例解初始化
 * \return 无
 */
void am_lpc82x_dma_inst_deinit (void);

/**
 * \brief  GPIO 实例初始化
 * \retval AM_OK    : 初始化成功
 */
int am_lpc82x_gpio_inst_init (void);

/**
 * \brief GPIO 实例解初始化
 * \return 无
 */
void am_lpc82x_gpio_inst_deinit (void);


/**
 * \brief  I2C0 实例初始化，获得I2C标准服务句柄
 * \return I2C标准服务句柄，若为NULL，表明初始化失败
 */
am_i2c_handle_t am_lpc82x_i2c0_inst_init (void);

/**
 * \brief I2C0 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_i2c0_inst_init() 函数获得的I2C句柄
 * \return 无
 */
void am_lpc82x_i2c0_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief  I2C1 实例初始化，获得I2C标准服务句柄
 * \return I2C标准服务句柄，若为NULL，表明初始化失败
 */
am_i2c_handle_t am_lpc82x_i2c1_inst_init (void);

/**
 * \brief I2C1 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_i2c1_inst_init() 函数获得的I2C句柄
 * \return 无
 */
void am_lpc82x_i2c1_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief  I2C2 实例初始化，获得I2C标准服务句柄
 * \return I2C标准服务句柄，若为NULL，表明初始化失败
 */
am_i2c_handle_t am_lpc82x_i2c2_inst_init (void);

/**
 * \brief I2C2 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_i2c2_inst_init() 函数获得的I2C句柄
 * \return 无
 */
void am_lpc82x_i2c2_inst_deinit (am_i2c_handle_t handle);


/**
 * \brief  I2C3 实例初始化，获得I2C标准服务句柄
 * \return I2C标准服务句柄，若为NULL，表明初始化失败
 */
am_i2c_handle_t am_lpc82x_i2c3_inst_init (void);

/**
 * \brief I2C3 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_i2c3_inst_init() 函数获得的I2C句柄
 * \return 无
 */
void am_lpc82x_i2c3_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief  中断实例初始化，初始化中断驱动
 * \retval AM_OK为实例化成功，其他失败
 */
int am_lpc82x_nvic_inst_init (void);

/**
 * \brief  中断实例解初始化
 */
void am_lpc82x_nvic_inst_deinit (void);

/**
 * \brief  系统Systick初始化
 */
am_timer_handle_t am_arm_systick_inst_init (void);

/**
 * \brief  MRT 实例初始化，获得MRT标准服务句柄
 * \return Timer标准服务句柄，若为NULL，表明初始化失败
 */
am_timer_handle_t am_lpc82x_mrt_inst_init (void);

/**
 * \brief MRT 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_mrt_inst_init() 函数获得的Timer句柄
 * \return 无
 */
void am_lpc82x_mrt_inst_deinit (am_timer_handle_t handle);

/**
 * \brief  SCT0 实例初始化，获得SCT标准服务句柄
 * \return SCT标准服务句柄，若为NULL，表明初始化失败
 */
am_lpc_sct_handle_t am_lpc82x_sct0_inst_init (void);

/**
 * \brief SCT0 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_sct0_inst_init() 函数获得的SCT句柄
 * \return 无
 */
void am_lpc82x_sct0_inst_deinit (am_lpc_sct_handle_t handle);

/**
 * \brief  SCT0 CAP 实例初始化，获得CAP标准服务句柄
 * \return CAP标准服务句柄，若为NULL，表明初始化失败
 */
am_cap_handle_t am_lpc82x_sct0_cap_inst_init (void);

/**
 * \brief SCT0 CAP 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_sct0_cap_inst_init() 函数获得的CAP句柄
 * \return 无
 */
void am_lpc82x_sct0_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief  SCT0 PWM 实例初始化，获得PWM标准服务句柄
 * \return PWM标准服务句柄，若为NULL，表明初始化失败
 */
am_pwm_handle_t am_lpc82x_sct0_pwm_inst_init (void);

/**
 * \brief SCT0 PWM 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_sct0_pwm_inst_init() 函数获得的PWM句柄
 * \return 无
 */
void am_lpc82x_sct0_pwm_inst_deinit (am_pwm_handle_t handle);


/**
 * \brief  SCT0 Timer 实例初始化，获得Timer标准服务句柄
 * \return Timer标准服务句柄，若为NULL，表明初始化失败
 */
am_timer_handle_t am_lpc82x_sct0_timing_inst_init (void);

/**
 * \brief SCT0 Timer 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_sct0_timing_inst_init() 函数获得的Timer句柄
 * \return 无
 */
void am_lpc82x_sct0_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief  SPI0 中断方式实例初始化，获得SPI标准服务句柄
 * \return SPI标准服务句柄，若为NULL，表明初始化失败
 */
am_spi_handle_t am_lpc82x_spi0_int_inst_init (void);

/**
 * \brief SPI0 中断方式实例解初始化
 * \param[in] handle : 通过 am_lpc82x_spi0_int_inst_init() 函数获得的SPI句柄
 * \return 无
 */
void am_lpc82x_spi0_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief  SPI1 中断方式实例初始化，获得SPI标准服务句柄
 * \return SPI标准服务句柄，若为NULL，表明初始化失败
 */
am_spi_handle_t am_lpc82x_spi1_int_inst_init (void);

/**
 * \brief SPI1 中断方式实例解初始化
 * \param[in] handle : 通过 am_lpc82x_spi1_int_inst_init() 函数获得的SPI句柄
 * \return 无
 */
void am_lpc82x_spi1_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief  SPI0 DMA方式实例初始化，获得SPI标准服务句柄
 * \return SPI标准服务句柄，若为NULL，表明初始化失败
 */
am_spi_handle_t am_lpc82x_spi0_dma_inst_init (void);

/**
 * \brief SPI0 DMA方式实例解初始化
 * \param[in] handle : 通过 am_lpc82x_spi0_dma_inst_init() 函数获得的SPI句柄
 * \return 无
 */
void am_lpc82x_spi0_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief  SPI1 DMA方式实例初始化，获得SPI标准服务句柄
 * \return SPI标准服务句柄，若为NULL，表明初始化失败
 */
am_spi_handle_t am_lpc82x_spi1_dma_inst_init (void);

/**
 * \brief SPI1 DMA方式实例解初始化
 * \param[in] handle : 通过 am_lpc82x_spi1_dma_inst_init() 函数获得的SPI句柄
 * \return 无
 */
void am_lpc82x_spi1_dma_inst_deinit (am_spi_handle_t handle);


/** \brief SYSTICK 实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_lpc82x_systick_inst_init (void);

/** \brief SYSTICK 实例解初始化 */
void am_lpc82x_systick_inst_deinit (am_timer_handle_t handle);

/**
 * \brief  USART0 实例初始化，获得UART标准服务句柄
 * \return UART标准服务句柄，若为NULL，表明初始化失败
 */
am_uart_handle_t am_lpc82x_usart0_inst_init (void);

/**
 * \brief USART0 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_usart0_inst_init() 函数获得的UART句柄
 * \return 无
 */
void am_lpc82x_usart0_inst_deinit (am_uart_handle_t handle);


/**
 * \brief  USART1 实例初始化，获得UART标准服务句柄
 * \return UART标准服务句柄，若为NULL，表明初始化失败
 */
am_uart_handle_t am_lpc82x_usart1_inst_init (void);

/**
 * \brief     USART1 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_usart1_inst_init() 函数获得的UART句柄
 * \return 无
 */
void am_lpc82x_usart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief  USART2 实例初始化，获得UART标准服务句柄
 * \return UART标准服务句柄，若为NULL，表明初始化失败
 */
am_uart_handle_t am_lpc82x_usart2_inst_init (void);

/**
 * \brief USART2 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_usart2_inst_init() 函数获得的UART句柄
 * \return 无
 */
void am_lpc82x_usart2_inst_deinit (am_uart_handle_t handle);

/**
 * \brief  WKT 实例初始化，获得Timer标准服务句柄
 * \return Timer标准服务句柄，若为NULL，表明初始化失败
 */
am_timer_handle_t am_lpc82x_wkt_inst_init (void);

/**
 * \brief WKT 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_wkt_inst_init() 函数获得的Timer句柄
 * \return 无
 */
void am_lpc82x_wkt_inst_deinit (am_timer_handle_t handle);


/**
 * \brief  WWDT 实例初始化，获得WDT标准服务句柄
 * \return WDT标准服务句柄，若为NULL，表明初始化失败
 */
am_wdt_handle_t am_lpc82x_wwdt_inst_init (void);

/**
 * \brief WWDT 实例解初始化
 * \param[in] handle : 通过 am_lpc82x_wwdt_inst_init() 函数获得的WDT句柄
 * \return 无
 */
void am_lpc82x_wwdt_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief ZM516X实例初始化，获得ZM516X模块标准服务操作句柄
 *
 * \return ZM516X模块标准服务操作句柄
 */
am_zm516x_handle_t am_zm516x_inst_init (void);

/**
 * \brief  WDT 实例初始化，获得WDT标准服务句柄
 */
am_wdt_handle_t am_cat823_inst_init (void);

/**
 * \brief WDT 实例解初始化
 */
void am_cat823_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief 调试串口实例初始化
 */
am_uart_handle_t am_debug_uart_inst_init (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AM_LPC82X_INST_INIT_H */

/* end of file */
