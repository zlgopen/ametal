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
 * \brief STM32F103RBT6 外设 实例初始化函数声明
 *
 * 所有外设设备（I2C、SPI、ADC...）的实例初始化函数均在本头文件中声明，使用实例
 * 初始化函数，可以轻松获得一个外设的句柄，然后使用相关的接口函数对外设进行操作。
 *
 * \sa am_stm32f103rbt6_inst_init.h
 * \internal
 * \par Modification history
 * - 1.00 15-12-11  tee, first implementation
 * \endinternal
 */

#ifndef __AM_STM32F103RBT6_INST_INIT_H
#define __AM_STM32F103RBT6_INST_INIT_H

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
#include "am_can.h"
#include "am_pwm.h"
#include "am_wdt.h"
#include "am_spi.h"
#include "am_spi_slv.h"
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
#include "am_stm32f103rbt6_tim_cap.h"
#include "amhw_stm32f103rbt6_rtc.h"
#include "am_stm32f103rbt6_pwr.h"
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
#include "am_event_input_key.h"
#include "am_event_category_input.h"

/**
 * \addtogroup am_if_stm32f103rbt6_inst_init
 * \copydoc am_stm32f103rbt6_inst_init.h
 * @{
 */


/**
 * \brief  timer_pwm 例程
 *
 * \param[in] pwm_handle : cap标准句柄
 *
 * \param[in] pwm_chan   : cap 通道
 */
void demo_std_timer_pwm_entry (am_pwm_handle_t pwm_handle, int pwm_chan);

/**
 * \brief  uart_polling 例程
 *
 * \param[in] handle : uart标准句柄
 */
void demo_std_uart_polling_entry (am_uart_handle_t handle);

/**
 * \brief  delay 例程
 *
 * \param[in] led_id : led id号
 */
void demo_std_delay_entry (int led_id);

/**
 * \brief  digitron 例程
 *
 * \param[in] id     : id号
 */
void demo_std_digitron_60s_counting_entry (int32_t id);

/**
 * \brief  mtd 例程
 *
 * \param[in] mtd_handle     : mtd 句柄
 *
 * \param[in] test_lenth     : 测试长度
 */
void demo_mtd_entry (am_mtd_handle_t mtd_handle, int32_t test_lenth);

/**
 * \brief  ftl 例程
 *
 * \param[in] ftl_handle     : ftl 句柄
 *
 * \param[in] test_lenth     : 测试长度
 */
void demo_ftl_entry (am_ftl_handle_t ftl_handle, int32_t test_lenth);

/**
 * \brief ep24cxx例程
 *
 * \param[in] mx25xx_handle    :    mx25xx句柄
 *
 * \param[in] test_lenth       :   测试长度
 */
void demo_mx25xx_entry (am_mx25xx_handle_t mx25xx_handle,
                        int32_t test_lenth);
/**
 * \brief ep24cxx例程
 *
 * \param[in] ep24cxx_handle    :   ep24cxx句柄
 *
 * \param[in] test_lenth        :   测试长度
 */
void demo_ep24cxx_entry (am_ep24cxx_handle_t ep24cxx_handle, 
                         int32_t test_lenth);

/**
 * \brief 中断实例初始化，初始化中断驱动
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_stm32f103rbt6_nvic_inst_init (void);

/**
 * \brief 中断实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_stm32f103rbt6_nvic_inst_deinit (void);

/**
 * \brief ILI9341 实例初始化，获得 ILI9341 标准服务句柄
 *
 * \param 无
 *
 * \return ILI9341 标准服务句柄，若为 NULL，表明初始化失败
 */
am_ili9341_handle_t am_stm32f103rbt6_ili9341_inst_init (void);

/**
 * \brief ILI9341 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_ili9341_inst_init() 函数获得的 ILI9341
 *                   标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_ili9341_inst_deinit (am_ili9341_handle_t handle);

/**
 * \brief ADC1 实例初始化，获得 ADC 标准服务句柄
 *
 * \param 无
 *
 * \return ADC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_adc_handle_t am_stm32f103rbt6_adc1_inst_init (void);

/**
 * \brief ADC1 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_adc1_inst_init() 函数获得的 ADC 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_adc1_inst_deinit (am_adc_handle_t handle);

/**
 * \brief ADC2 实例初始化，获得 ADC 标准服务句柄
 *
 * \param 无
 *
 * \return ADC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_adc_handle_t am_stm32f103rbt6_adc2_inst_init (void);

/**
 * \brief ADC2 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_adc2_inst_init() 函数获得的 ADC 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_adc2_inst_deinit (am_adc_handle_t handle);

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_stm32f103rbt6_clk_inst_init (void);

/**
 * \brief CRC 实例初始化，获得 CRC 标准服务句柄
 *
 * \param 无
 *
 * \return CRC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_crc_handle_t am_stm32f103rbt6_crc_inst_init (void);

/**
 * \brief CRC 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_crc_inst_init() 函数获得的 CRC 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_crc_inst_deinit (am_crc_handle_t handle);

/**
 * \brief DAC1 实例初始化，获得 DAC 标准服务句柄
 *
 * \param 无
 *
 * \return DAC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_dac_handle_t am_stm32f103rbt6_dac1_inst_init (void);

/**
 * \brief DAC1 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_dac1_inst_init() 函数获得的 DAC 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_dac1_inst_deinit (am_dac_handle_t handle);

/**
 * \brief DAC2 实例初始化，获得 DAC 标准服务句柄
 *
 * \param 无
 *
 * \return DAC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_dac_handle_t am_stm32f103rbt6_dac2_inst_init (void);

/**
 * \brief DAC2 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_dac2_inst_init() 函数获得的 DAC 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_dac2_inst_deinit (am_dac_handle_t handle);

/**
 * \brief DMA 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_stm32f103rbt6_dma_inst_init (void);

/**
 * \brief DMA 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_stm32f103rbt6_dma_inst_deinit (void);

/**
 * \brief GPIO 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_stm32f103rbt6_gpio_inst_init (void);

/**
 * \brief GPIO 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_stm32f103rbt6_gpio_inst_deinit (void);

/**
 * \brief I2C1 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_stm32f103rbt6_i2c1_inst_init (void);

/**
 * \brief I2C1 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_i2c1_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_i2c1_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C2 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_stm32f103rbt6_i2c2_inst_init (void);

/**
 * \brief I2C2 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_i2c2_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_i2c2_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C1 从机实例初始化，获得 I2C 从机标准服务句柄
 *
 * \param 无
 *
 * \return I2C 从机标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_slv_handle_t am_stm32f103rbt6_i2c1_slv_inst_init (void);

/**
 * \brief I2C1 从机实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_i2c1_slv_inst_init() 函数
 *                   获得的 I2C 从机标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_i2c1_slv_inst_deinit (am_i2c_slv_handle_t handle);

/**
 * \brief I2C2 从机实例初始化，获得 I2C 从机标准服务句柄
 *
 * \param 无
 *
 * \return I2C 从机标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_slv_handle_t am_stm32f103rbt6_i2c2_slv_inst_init (void);

/**
 * \brief I2C2 从机实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_i2c2_slv_inst_init() 函数
 *                   获得的 I2C 从机标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_i2c2_slv_inst_deinit (am_i2c_slv_handle_t handle);

/**
 * \brief IWDG 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_stm32f103rbt6_iwdg_inst_init (void);

/**
 * \brief IWDG 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_iwdg_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_iwdg_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief PWR 实例初始化
 *
 * \param 无
 *
 * \return PWR 标准服务句柄，若为 NULL，表明初始化失败
 */
am_stm32f103rbt6_pwr_handle_t am_stm32f103rbt6_pwr_inst_init (void);

/**
 * \brief PWR 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_stm32f103rbt6_pwr_inst_deinit (void);

/**
 * \brief RTC 实例初始化，获得 RTC 标准服务句柄
 *
 * \param 无
 *
 * \return RTC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_rtc_handle_t am_stm32f103rbt6_rtc_inst_init (void);

/**
 * \brief RTC 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_rtc_inst_init() 函数获得的 RTC 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_rtc_inst_deinit (am_rtc_handle_t handle);

/**
 * \brief SPI1 POLL 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_stm32f103rbt6_spi1_poll_inst_init (void);

/**
 * \brief SPI1 POLL 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_spi1_poll_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_spi1_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI2 POLL 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_stm32f103rbt6_spi2_poll_inst_init (void);

/**
 * \brief SPI2 POLL 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_spi2_poll_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_spi2_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_stm32f103rbt6_spi1_dma_inst_init (void);

/**
 * \brief SPI1 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_spi1_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_spi1_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI2 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_stm32f103rbt6_spi2_dma_inst_init (void);

/**
 * \brief SPI2 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_spi2_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_spi2_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_stm32f103rbt6_spi1_int_inst_init (void);

/**
 * \brief SPI1 INT 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_spi1_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_spi1_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI2 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_stm32f103rbt6_spi2_int_inst_init (void);

/**
 * \brief SPI2 INT 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_spi2_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_spi2_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 Slaver DMA 实例初始化，获得 SPI SLV标准服务句柄
 *
 * \param 无
 *
 * \return SPI SLV标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_slv_handle_t am_stm32f103rbt6_spi1_slv_dma_inst_init (void);

/**
 * \brief SPI1 Slaver DMA 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_spi1_slv_dma_inst_init() 函数
 *                   获得的 SPI SLV标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_spi1_slv_dma_inst_deinit (am_spi_slv_handle_t handle);

/**
 * \brief SPI2 Slaver DMA 实例初始化，获得 SPI SLV标准服务句柄
 *
 * \param 无
 *
 * \return SPI SLV标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_slv_handle_t am_stm32f103rbt6_spi2_slv_dma_inst_init (void);

/**
 * \brief SPI2 Slaver DMA 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_spi2_slv_dma_inst_init() 函数
 *                   获得的 SPI SLV标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_spi2_slv_dma_inst_deinit (am_spi_slv_handle_t handle);

/**
 * \brief SYSTICK 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_stm32f103rbt6_systick_inst_init (void);

/**
 * \brief SYSTICK 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_systick_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_systick_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM1 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_stm32f103rbt6_tim1_cap_inst_init (void);

/**
 * \brief TIM1 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim1_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim1_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM2 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_stm32f103rbt6_tim2_cap_inst_init (void);

/**
 * \brief TIM2 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim2_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim2_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM3 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_stm32f103rbt6_tim3_cap_inst_init (void);

/**
 * \brief TIM3 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim3_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim3_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM4 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_stm32f103rbt6_tim4_cap_inst_init (void);

/**
 * \brief TIM4 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim4_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim4_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM1 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_stm32f103rbt6_tim1_pwm_inst_init (void);

/**
 * \brief TIM1 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim1_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim1_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM2 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_stm32f103rbt6_tim2_pwm_inst_init (void);

/**
 * \brief TIM2 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim2_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim2_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM3 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_stm32f103rbt6_tim3_pwm_inst_init (void);

/**
 * \brief TIM3 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim3_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim3_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM4 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_stm32f103rbt6_tim4_pwm_inst_init (void);

/**
 * \brief TIM4 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim4_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim4_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM1 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_stm32f103rbt6_tim1_timing_inst_init (void);

/**
 * \brief TIM1 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim1_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim1_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM2 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_stm32f103rbt6_tim2_timing_inst_init (void);

/**
 * \brief TIM2 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim2_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim2_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM3 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_stm32f103rbt6_tim3_timing_inst_init (void);

/**
 * \brief TIM3 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim3_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim3_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM4 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_stm32f103rbt6_tim4_timing_inst_init (void);

/**
 * \brief TIM4 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_tim4_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_tim4_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief UART1 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_stm32f103rbt6_usart1_inst_init (void);

/**
 * \brief UART1 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_uart1_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_usart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART2 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_stm32f103rbt6_usart2_inst_init (void);

/**
 * \brief UART2 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_uart2_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_usart2_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART3 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_stm32f103rbt6_usart3_inst_init (void);

/**
 * \brief UART3 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_uart3_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_usart3_inst_deinit (am_uart_handle_t handle);

/**
 * \brief WWDG 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_stm32f103rbt6_wwdg_inst_init (void);

/**
 * \brief WWDG 实例解初始化
 *
 * \param[in] handle 通过 am_stm32f103rbt6_wwdg_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_stm32f103rbt6_wwdg_inst_deinit (am_wdt_handle_t handle);


/** \brief CAN实例初始化，获得CAN标准服务句柄 */
am_can_handle_t am_stm32f103rbt6_can_inst_init (void);

/** \brief CAN实例解初始化 */
void am_stm32f103rbt6_can_inst_deinit (am_can_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AM_STM32F103RBT6_INST_INIT_H */

/* end of file */
