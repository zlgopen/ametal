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
 * \brief HC32 外设 实例初始化函数声明
 *
 * 所有外设设备（I2C、SPI、ADC...）的实例初始化函数均在本头文件中声明，使用实例
 * 初始化函数，可以轻松获得一个外设的句柄，然后使用相关的接口函数对外设进行操作。
 *
 * \sa am_hc32l19x_inst_init.h
 * \internal
 * \par Modification history
 * - 1.00 15-12-11  tee, first implementation
 * \endinternal
 */

#ifndef __am_hc32_inst_init_H
#define __am_hc32_inst_init_H

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

#include "am_hc32_pca.h"
#include "am_hc32_pcnt.h"
//#include "amhw_hc32_rtc.h"
//#include "am_hc32_pwr.h"
#include "am_hwconf_hc32l19x_led_gpio.h"
//#include "am_hwconf_led_gpio.h"
#include "am_hwconf_key_gpio.h"
#include "am_hwconf_buzzer_pwm.h"
#include "am_hwconf_debug_uart.h"
#include "am_hwconf_system_tick_softimer.h"
//#include "am_hwconf_lm75.h"
//#include "am_hwconf_microport.h"
#include "am_hwconf_miniport_zlg72128.h"
#include "am_hwconf_miniport_595.h"
#include "am_hwconf_miniport_view.h"
#include "am_hwconf_miniport_led.h"
#include "am_hwconf_miniport_key.h"
#include "am_hwconf_miniport_view_key.h"
#include "am_event_input_key.h"
#include "am_event_category_input.h"

/**
 * \addtogroup am_if_hc32_inst_init
 * \copydoc am_hc32l19x_inst_init.h
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
 * \brief  timer_cap 例程
 *
 * \param[in] cap_handle : cap标准句柄
 *
 * \param[in] cap_chan   : cap 通道
 */
void demo_std_timer_cap_entry (am_cap_handle_t cap_handle, int cap_chan);

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
 * \brief  key_digitron 例程
 *
 * \param[in] rtc_handle : rtc 标准句柄
 */
void demo_std_rtc_entry (am_rtc_handle_t rtc_handle);

/**
 * \brief  key_digitron 例程
 *
 * \param[in] id : key id号
 */
void demo_std_key_digitron_rngbuf_entry (int32_t id);

/**
 * \brief  hc595_led 例程
 *
 * \param[in] led_id_start : 起始led id号
 *
 * \param[in] num          : led个数
 */
void demo_std_led_water_light_entry (int led_id_start, int num);

/**
 * \brief  hc595_led 例程
 *
 * \param[in] 无
 */
 void demo_std_4key_entry (void);
 
/**
 * \brief  hc595_led 例程
 *
 * \param[in] hc595_handle  : 标准hc595句柄
 */
 void demo_std_hc595_led_entry (am_hc595_handle_t hc595_handle);
/**
 * \brief  digitron_temp 例程
 *
 * \param[in] temp_handle  : 标准temp_handle句柄
 *
 * \param[in] id        : id号
 */
void demo_std_digitron_temp_entry (int32_t id, am_temp_handle_t temp_handle);
/**
 * \brief  key_digitron 例程
 *
 * \param[in] id     : id号
 */
void demo_std_key_digitron_entry (int32_t id);
	
/**
 * \brief  digitron 例程
 *
 * \param[in] id     : id号
 */
void demo_std_digitron_60s_counting_entry (int32_t id);

/**
 * \brief  mtd 例程
 *
 * \param[in] rtc_handle     : rtc 句柄
 */
void demo_std_rtc_entry (am_rtc_handle_t rtc_handle);

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
 * \brief timer定时器
 *
 * \param[in] am_timer_handle_t  : timer句柄
 *
 * \param[in] timing_chan        : 通道
 */
void demo_std_timer_timing_entry (am_timer_handle_t handle,
                                  int               timing_chan);

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
int am_hc32_nvic_inst_init (void);

/**
 * \brief 中断实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_hc32_nvic_inst_deinit (void);

/**
 * \brief ILI9341 实例初始化，获得 ILI9341 标准服务句柄
 *
 * \param 无
 *
 * \return ILI9341 标准服务句柄，若为 NULL，表明初始化失败
 */
am_ili9341_handle_t am_hc32_ili9341_inst_init (void);

/**
 * \brief ILI9341 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_ili9341_inst_init() 函数获得的 ILI9341
 *                   标准服务句柄
 *
 * \return 无
 */
void am_hc32_ili9341_inst_deinit (am_ili9341_handle_t handle);

/**
 * \brief ADC 实例初始化，获得 ADC 标准服务句柄
 *
 * \param 无
 *
 * \return ADC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_adc_handle_t am_hc32_adc_inst_init (void);

/**
 * \brief ADC 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_adc1_inst_init() 函数获得的 ADC 标准服务句柄
 *
 * \return 无
 */
void am_hc32_adc_inst_deinit (am_adc_handle_t handle);

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_hc32_clk_inst_init (void);

/**
 * \brief CRC 实例初始化，获得 CRC 标准服务句柄
 *
 * \param 无
 *
 * \return CRC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_crc_handle_t am_hc32_crc_inst_init (void);

/**
 * \brief CRC 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_crc_inst_init() 函数获得的 CRC 标准服务句柄
 *
 * \return 无
 */
void am_hc32_crc_inst_deinit (am_crc_handle_t handle);

/**
 * \brief DAC 实例初始化，获得 DAC 标准服务句柄
 *
 * \param 无
 *
 * \return DAC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_dac_handle_t am_hc32_dac_inst_init (void);

/**
 * \brief DAC 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_dac_inst_init() 函数获得的 DAC 标准服务句柄
 *
 * \return 无
 */
void am_hc32_dac_inst_deinit (am_dac_handle_t handle);

/**
 * \brief DMA 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_hc32_dma_inst_init (void);

/**
 * \brief DMA 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_hc32_dma_inst_deinit (void);

/**
 * \brief GPIO 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_hc32_gpio_inst_init (void);

/**
 * \brief GPIO 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_hc32_gpio_inst_deinit (void);

/**
 * \brief I2C1 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_hc32_i2c1_inst_init (void);

/**
 * \brief I2C1 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_i2c1_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_hc32_i2c1_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C2 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_hc32_i2c0_inst_init (void);

/**
 * \brief I2C2 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_i2c2_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_hc32_i2c0_inst_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C1 从机实例初始化，获得 I2C 从机标准服务句柄
 *
 * \param 无
 *
 * \return I2C 从机标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_slv_handle_t am_hc32_i2c1_slv_inst_init (void);

/**
 * \brief I2C1 从机实例解初始化
 *
 * \param[in] handle 通过 am_hc32_i2c1_slv_inst_init() 函数
 *                   获得的 I2C 从机标准服务句柄
 *
 * \return 无
 */
void am_hc32_i2c1_slv_inst_deinit (am_i2c_slv_handle_t handle);

/**
 * \brief I2C2 从机实例初始化，获得 I2C 从机标准服务句柄
 *
 * \param 无
 *
 * \return I2C 从机标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_slv_handle_t am_hc32_i2c0_slv_inst_init (void);

/**
 * \brief I2C2 从机实例解初始化
 *
 * \param[in] handle 通过 am_hc32_i2c2_slv_inst_init() 函数
 *                   获得的 I2C 从机标准服务句柄
 *
 * \return 无
 */
void am_hc32_i2c0_slv_inst_deinit (am_i2c_slv_handle_t handle);

/**
 * \brief IWDG 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_hc32_iwdg_inst_init (void);

/**
 * \brief IWDG 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_iwdg_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_hc32_iwdg_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief PWR 实例初始化
 *
 * \param 无
 *
 * \return PWR 标准服务句柄，若为 NULL，表明初始化失败
 */
//am_hc32_pwr_handle_t am_hc32_pwr_inst_init (void);

/**
 * \brief PWR 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_hc32_pwr_inst_deinit (void);

/**
 * \brief RTC 实例初始化，获得 RTC 标准服务句柄
 *
 * \param 无
 *
 * \return RTC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_rtc_handle_t am_hc32_rtc_inst_init (void);

/**
 * \brief RTC 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_rtc_inst_init() 函数获得的 RTC 标准服务句柄
 *
 * \return 无
 */
void am_hc32_rtc_inst_deinit (am_rtc_handle_t handle);

/**
 * \brief SPI0 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32_spi0_dma_inst_init (void);

/**
 * \brief SPI2 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_spi0_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32_spi0_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32_spi1_dma_inst_init (void);

/**
 * \brief SPI1 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_spi1_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32_spi1_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI0 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32_spi0_int_inst_init (void);

/**
 * \brief SPI0 INT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_spi0_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32_spi0_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32_spi1_int_inst_init (void);

/**
 * \brief SPI1 INT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_spi1_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32_spi1_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI0 poll 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32_spi0_poll_inst_init (void);

/**
 * \brief SPI2 poll 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_spi0_poll_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32_spi0_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 poll 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32_spi1_poll_inst_init (void);

/**
 * \brief SPI1 poll 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_spi1_poll_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32_spi1_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SYSTICK 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_systick_inst_init (void);

/**
 * \brief SYSTICK 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_systick_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32_systick_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM0 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32_tim0_cap_inst_init (void);

/**
 * \brief TIM0 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim3_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim0_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM1 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32_tim1_cap_inst_init (void);

/**
 * \brief TIM1 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim1_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim1_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM2 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32_tim2_cap_inst_init (void);

/**
 * \brief TIM2 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim2_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim2_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM3 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32_tim3_cap_inst_init (void);

/**
 * \brief TIM3 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim3_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim3_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief TIM0 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32_tim0_pwm_inst_init (void);

/**
 * \brief TIM0 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim1_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim0_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM1 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32_tim1_pwm_inst_init (void);

/**
 * \brief TIM1 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim1_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim1_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM2 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32_tim2_pwm_inst_init (void);

/**
 * \brief TIM2 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim2_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim2_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief TIM3 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32_tim3_pwm_inst_init (void);

/**
 * \brief TIM3 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim3_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim3_pwm_inst_deinit (am_pwm_handle_t handle);


/**
 * \brief TIM0 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_tim0_timing_inst_init (void);

/**
 * \brief TIM0 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim0_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim0_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM1 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_tim1_timing_inst_init (void);

/**
 * \brief TIM1 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim1_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim1_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM2 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_tim2_timing_inst_init (void);

/**
 * \brief TIM2 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim2_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim2_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TIM3 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_tim3_timing_inst_init (void);

/**
 * \brief TIM3 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim3_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim3_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief LPTIM0 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_lptim0_timing_inst_init (void);

/**
 * \brief LPTIM0 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_lptim0_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32_lptim0_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief LPTIM1 TIMING 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_lptim1_timing_inst_init (void);

/**
 * \brief LPTIM1 TIMING 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_lptim1_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32_lptim1_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief ADTIM4 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32_tim4_cap_inst_init (void);

/**
 * \brief ADTIM4 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim4_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim4_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief ADTIM5 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32_tim5_cap_inst_init (void);

/**
 * \brief ADTIM5 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim5_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim5_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief ADTIM6 CAP 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32_tim6_cap_inst_init (void);

/**
 * \brief ADTIM6 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim6_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim6_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief ADTIM4 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32_tim4_pwm_inst_init (void);

/**
 * \brief ADTIM4 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim4_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim4_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief ADTIM5 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32_tim5_pwm_inst_init (void);

/**
 * \brief ADTIM5 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim5_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim5_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief ADTIM6 PWM 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32_tim6_pwm_inst_init (void);

/**
 * \brief ADTIM6 PWM 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim6_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim6_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief ADTIM4 timing 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_tim4_timing_inst_init (void);

/**
 * \brief ADTIM4 timing 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim4_timing_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim4_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief ADTIM5 timing 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_tim5_timing_inst_init (void);

/**
 * \brief ADTIM5 timing 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim5_timing_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim5_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief ADTIM6 timing 实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32_tim6_timing_inst_init (void);

/**
 * \brief ADTIM6 timing 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_tim6_timing_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32_tim6_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief UART0 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32_uart0_inst_init (void);

/**
 * \brief UART0 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_uart1_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32_uart0_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART1 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32_uart1_inst_init (void);

/**
 * \brief UART1 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_uart1_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32_uart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART2 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32_uart2_inst_init (void);

/**
 * \brief UART2 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_uart2_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32_uart2_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART3 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32_uart3_inst_init (void);

/**
 * \brief UART3 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_uart3_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32_uart3_inst_deinit (am_uart_handle_t handle);

/**
 * \brief LPUART0 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32_lpuart0_inst_init (void);

/**
 * \brief LPUART0 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_lpuart0_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32_lpuart0_inst_deinit (am_uart_handle_t handle);

/**
 * \brief LPUART1 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32_lpuart1_inst_init (void);

/**
 * \brief LPUART1 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_lpuart1_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32_lpuart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief WDT 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_hc32_wdt_inst_init (void);

/**
 * \brief WDT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_wdt_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_hc32_wdt_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief PCA 实例初始化
 */
am_hc32_pca_handle_t am_hc32_pca1_inst_init (void);

/**
 * \brief PCA 实例解初始化
 */
void am_hc32_pca_inst_deinit (am_hc32_pca_handle_t handle);

/**
 * \brief PCNT 实例初始化，获得PCNT 服务句柄
 *
 * \param 无
 *
 * \return PCNT 服务句柄，若为 NULL，表明初始化失败
 */
am_hc32_pcnt_handle_t am_hc32_pcnt_inst_init (void);

/**
 * \brief PCNT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32_pcnt1_inst_init() 函数获得的 PCNT 服务句柄
 *
 * \return 无
 */
void am_hc32_pcnt_inst_deinit (am_hc32_pcnt_handle_t handle);

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

#endif  /* __am_hc32_inst_init_H */

/* end of file */
