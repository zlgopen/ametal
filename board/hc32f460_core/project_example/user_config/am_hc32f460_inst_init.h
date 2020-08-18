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
 * \brief HC32F460 外设 实例初始化函数声明
 *
 * 所有外设设备（I2C、SPI、ADC...）的实例初始化函数均在本头文件中声明，使用实例
 * 初始化函数，可以轻松获得一个外设的句柄，然后使用相关的接口函数对外设进行操作。
 *
 * \sa am_hc32f460_inst_init.h
 * \internal
 * \par Modification history
 * - 1.00 15-12-11  tee, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_INST_INIT_H
#define __AM_HC32F460_INST_INIT_H

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
#include "am_can.h"
#include "am_sdio.h"
#include "am_sdcard.h"
#include "am_spi_slv.h"
#include "am_usbd_msc.h"
//#include "am_hwconf_led_gpio.h"
#include "am_hwconf_key_gpio.h"
//#include "am_hwconf_buzzer_pwm.h"
//#include "am_hwconf_debug_uart.h"
#include "am_hwconf_system_tick_softimer.h"
//#include "am_hwconf_lm75.h"
//#include "am_hwconf_microport.h"
//#include "am_hwconf_miniport_zlg72128.h"
//#include "am_hwconf_miniport_595.h"
//#include "am_hwconf_miniport_view.h"
//#include "am_hwconf_miniport_led.h"
//#include "am_hwconf_miniport_key.h"
//#include "am_hwconf_miniport_view_key.h"
//#include "am_event_input_key.h"
//#include "am_event_category_input.h"

#include "am_hc32f460_aes.h"
#include "am_hc32f460_hash.h"
#include "am_hc32f460_trng.h"
#include "am_hc32f460_qspi.h"

/**
 * \addtogroup am_if_hc32f460_inst_init
 * \copydoc am_hc32f460_inst_init.h
 * @{
 */


///**
// * \brief  timer_pwm 例程
// *
// * \param[in] pwm_handle : cap标准句柄
// *
// * \param[in] pwm_chan   : cap 通道
// */
//void demo_std_timer_pwm_entry (am_pwm_handle_t pwm_handle, int pwm_chan);
//
///**
// * \brief  timer_cap 例程
// *
// * \param[in] cap_handle : cap标准句柄
// *
// * \param[in] cap_chan   : cap 通道
// */
//void demo_std_timer_cap_entry (am_cap_handle_t cap_handle, int cap_chan);
//
///**
// * \brief  uart_polling 例程
// *
// * \param[in] handle : uart标准句柄
// */
//void demo_std_uart_polling_entry (am_uart_handle_t handle);
//
///**
// * \brief  delay 例程
// *
// * \param[in] led_id : led id号
// */
//void demo_std_delay_entry (int led_id);
//
///**
// * \brief  key_digitron 例程
// *
// * \param[in] rtc_handle : rtc 标准句柄
// */
//void demo_std_rtc_entry (am_rtc_handle_t rtc_handle);
//
///**
// * \brief  key_digitron 例程
// *
// * \param[in] id : key id号
// */
//void demo_std_key_digitron_rngbuf_entry (int32_t id);
//
///**
// * \brief  hc595_led 例程
// *
// * \param[in] led_id_start : 起始led id号
// *
// * \param[in] num          : led个数
// */
//void demo_std_led_water_light_entry (int led_id_start, int num);
//
///**
// * \brief  hc595_led 例程
// *
// * \param[in] 无
// */
// void demo_std_4key_entry (void);
//
///**
// * \brief  hc595_led 例程
// *
// * \param[in] hc595_handle  : 标准hc595句柄
// */
// void demo_std_hc595_led_entry (am_hc595_handle_t hc595_handle);
///**
// * \brief  digitron_temp 例程
// *
// * \param[in] temp_handle  : 标准temp_handle句柄
// *
// * \param[in] id        : id号
// */
//void demo_std_digitron_temp_entry (int32_t id, am_temp_handle_t temp_handle);
///**
// * \brief  key_digitron 例程
// *
// * \param[in] id     : id号
// */
//void demo_std_key_digitron_entry (int32_t id);
//
///**
// * \brief  digitron 例程
// *
// * \param[in] id     : id号
// */
//void demo_std_digitron_60s_counting_entry (int32_t id);
//
///**
// * \brief  mtd 例程
// *
// * \param[in] rtc_handle     : rtc 句柄
// */
//void demo_std_rtc_entry (am_rtc_handle_t rtc_handle);
//
///**
// * \brief  mtd 例程
// *
// * \param[in] mtd_handle     : mtd 句柄
// *
// * \param[in] test_lenth     : 测试长度
// */
//void demo_mtd_entry (am_mtd_handle_t mtd_handle, int32_t test_lenth);
//
///**
// * \brief  ftl 例程
// *
// * \param[in] ftl_handle     : ftl 句柄
// *
// * \param[in] test_lenth     : 测试长度
// */
//void demo_ftl_entry (am_ftl_handle_t ftl_handle, int32_t test_lenth);
//
///**
// * \brief ep24cxx例程
// *
// * \param[in] mx25xx_handle    :    mx25xx句柄
// *
// * \param[in] test_lenth       :   测试长度
// */
//void demo_mx25xx_entry (am_mx25xx_handle_t mx25xx_handle,
//                        int32_t test_lenth);
///**
// * \brief ep24cxx例程
// *
// * \param[in] ep24cxx_handle    :   ep24cxx句柄
// *
// * \param[in] test_lenth        :   测试长度
// */
//void demo_ep24cxx_entry (am_ep24cxx_handle_t ep24cxx_handle,
//                         int32_t test_lenth);
//
///**
// * \brief timer定时器
// *
// * \param[in] am_timer_handle_t  : timer句柄
// *
// * \param[in] timing_chan        : 通道
// */
//void demo_std_timer_timing_entry (am_timer_handle_t handle,
//                                  int               timing_chan);
//
///**
// * \brief MiniPort-ZLG72128 实例初始化
// *
// * \param 无
// *
// * \return 返回 AW_OK 为初始化成功，其它为初始化失败
// */
//int am_miniport_zlg72128_inst_init (void);
//
///**
// * \brief 实例初始化函数
// *
// * \param 无
// *
// * \return 返回 AW_OK 为初始化成功，其它为初始化失败
// */
//int am_key_gpio_inst_init (void);
//
/**
 * \brief 中断实例初始化，初始化中断驱动
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_hc32f460_nvic_inst_init (void);

/**
 * \brief 中断实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_hc32f460_nvic_inst_deinit (void);

/**
 * \brief 调试串口实例初始化
 *
 * \param 无
 *
 * \return 无
 */
am_uart_handle_t am_debug_uart_inst_init (void);
/**
 * \brief ADC1 实例初始化，获得 ADC 标准服务句柄
 *
 * \param 无
 *
 * \return ADC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_adc_handle_t am_hc32f460_adc1_inst_init (void);

/**
 * \brief ADC1 实例解初始化
 *
 * \param[in] handle 通过 am_zlg118_adc1_inst_init() 函数获得的 ADC 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_adc1_inst_deinit (am_adc_handle_t handle);

/**
 * \brief ADC1 实例初始化，获得 ADC 标准服务句柄
 *
 * \param 无
 *
 * \return ADC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_adc_handle_t am_hc32f460_adc2_inst_init (void);

/**
 * \brief ADC1 实例解初始化
 *
 * \param[in] handle 通过 am_zlg118_adc1_inst_init() 函数获得的 ADC 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_adc2_inst_deinit (am_adc_handle_t handle);

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_hc32f460_clk_inst_init (void);

/**
 * \brief adtim61 cap实例初始化
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32f460_adtim61_cap_inst_init (void);

/**
 * \brief adtim61 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_adtim61_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_adtim61_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief adtim62 cap实例初始化
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32f460_adtim62_cap_inst_init (void);

/**
 * \brief adtim62 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_adtim62_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_adtim62_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief adtim63 cap实例初始化
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32f460_adtim63_cap_inst_init (void);

/**
 * \brief adtim61 CAP 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_adtim63_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_adtim63_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief adtim6 pwm 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_adtim61_pwm_inst_init (void);

/**
 * \brief adtim6 pwm 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_adtim61_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_adtim61_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief adtim6 pwm 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_adtim62_pwm_inst_init (void);

/**
 * \brief adtim6 pwm 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_adtim62_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_adtim62_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief adtim6 pwm 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_adtim63_pwm_inst_init (void);

/**
 * \brief adtim6 pwm 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_adtim63_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_adtim63_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief 定时器ADTIM6 Timing实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_tim61_timing_inst_init (void);

/**
 * \brief ADTIM6 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim61_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim61_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器ADTIM6 Timing实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_tim62_timing_inst_init (void);

/**
 * \brief ADTIM6 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim62_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim62_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器ADTIM6 Timing实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_tim63_timing_inst_init (void);

/**
 * \brief ADTIM6 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim63_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim63_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief AES 实例初始化，获得 aes 标准服务句柄
 *
 * \param 无
 *
 * \return aes 标准服务句柄，若为 NULL，表明初始化失败
 */
am_aes_handle_t am_hc32f460_aes_inst_init (void);

/**
 * \brief AES 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_aes_inst_init() 函数
 *                   获得的 aes 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_aes_inst_deinit (am_aes_handle_t handle);

/**
 * \brief CAN 实例初始化，获得 can 标准服务句柄
 *
 * \param 无
 *
 * \return can 标准服务句柄，若为 NULL，表明初始化失败
 */
am_can_handle_t am_hc32f460_can_inst_init (void);

/**
 * \brief CAN 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_can_inst_init() 函数
 *                   获得的can 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_can_inst_deinit (am_can_handle_t handle);

/**
 * \brief DMA 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_hc32f460_dma1_inst_init (void);

/**
 * \brief DMA 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_hc32f460_dma1_inst_deinit (void);

/**
 * \brief DMA 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_hc32f460_dma2_inst_init (void);

/**
 * \brief DMA 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_hc32f460_dma2_inst_deinit (void);

/**
 * \brief GPIO 实例初始化
 *
 * \param 无
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_hc32f460_gpio_inst_init (void);

/**
 * \brief GPIO 实例解初始化
 *
 * \param 无
 *
 * \return 无
 */
void am_hc32f460_gpio_inst_deinit (void);

/**
 * \brief HASH 实例初始化
 *
 * \param 无
 *
 * \return HASH 标准服务句柄，若为 NULL，表明初始化失败
 */
am_hash_handle_t am_hc32f460_hash_inst_init (void);

/**
 * \brief HASH 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_hash_inst_init() 函数获得的 HASH 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_hash_inst_deinit (am_hash_handle_t handle);


/**
 * \brief I2C1 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_hc32f460_i2c1_inst_init (void);

/**
 * \brief I2C1 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_i2c1_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_i2c_inst1_deinit (am_i2c_handle_t handle);


/**
 * \brief I2C2 实例初始化，获得 I2C 标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_handle_t am_hc32f460_i2c2_inst_init (void);

/**
 * \brief I2C2 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_i2c2_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_i2c_inst2_deinit (am_i2c_handle_t handle);

/**
 * \brief I2C1从机 实例初始化，获得I2C标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
am_i2c_slv_handle_t  am_hc32f460_i2c1_slv_inst_init (void);

/**
 * \brief I2C1从机 实例初始化，获得I2C标准服务句柄
 *
 * \param 无
 *
 * \return I2C 标准服务句柄，若为 NULL，表明初始化失败
 */
void am_hc32f460_i2c1_slv_inst_deinit (am_i2c_slv_handle_t handle);

/**
 * \brief led gpio 实例初始化
 *
 * \param[in] handle 通过 am_hc32f460_i2c2_inst_init() 函数获得的 I2C 标准服务句柄
 *
 * \return 返回 AW_OK 为初始化成功，其它为初始化失败
 */
int am_hc32f460_led_gpio_inst_init (void);

/**
 * \brief QSPI 实例初始化，获得 qspi 标准服务句柄
 *
 * \param 无
 *
 * \return QSPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_qspi_handle_t am_hc32f460_qspi_inst_init(void);

/**
 * \brief QSPI 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_qspi_inst_init() 函数获得的 QSPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_qspi_inst_deinit(am_qspi_handle_t handle);

/**
 * \brief RTC 实例初始化，获得 RTC 标准服务句柄
 *
 * \param 无
 *
 * \return RTC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_rtc_handle_t am_hc32f460_rtc_inst_init (void);

/**
 * \brief RTC 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_rtc_inst_init() 函数获得的 RTC 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_rtc_inst_deinit (am_rtc_handle_t handle);

/**
 * \brief SDCARD1 实例初始化，获得 SDCARD 标准服务句柄
 *
 * \param 无
 *
 * \return SDCARD 标准服务句柄，若为 NULL，表明初始化失败
 */
am_sdcard_handle_t am_sdcard1_inst_init (void);

/**
 * \brief SDIOC 实例初始化，获得 SDIOC 标准服务句柄
 *
 * \param 无
 *
 * \return SDIOC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_sdio_handle_t am_hc32f460_sdioc1_inst_init (void);

/**
 * \brief SDIOC 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_sdioc1_inst_init() 函数获得的 SDIOC 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_sdioc1_inst_deinit (am_sdio_handle_t handle);

/**
 * \brief SDIOC 实例初始化，获得 SDIOC 标准服务句柄
 *
 * \param 无
 *
 * \return SDIOC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_sdio_handle_t am_hc32f460_sdioc2_inst_init (void);

/**
 * \brief SDIOC 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_sdioc2_inst_init() 函数获得的 SDIOC 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_sdioc2_inst_deinit (am_sdio_handle_t handle);

/**
 * \brief SPI1 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi1_dma_inst_init (void);

/**
 * \brief SPI1 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi1_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi1_dma_inst_deinit (am_spi_handle_t handle);


/**
 * \brief SPI2 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi2_dma_inst_init (void);

/**
 * \brief SPI2 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi2_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi2_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI3 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi3_dma_inst_init (void);

/**
 * \brief SPI3 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi3_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi3_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI4 DMA 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi4_dma_inst_init (void);

/**
 * \brief SPI4 DMA 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi4_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi4_dma_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi1_int_inst_init (void);

/**
 * \brief SPI1 INT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi1_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hcc32f460_spi1_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI2 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi2_int_inst_init (void);

/**
 * \brief SPI2 INT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi2_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hcc32f460_spi2_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI3 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi3_int_inst_init (void);

/**
 * \brief SPI3 INT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi3_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hcc32f460_spi3_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI4 INT 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi4_int_inst_init (void);

/**
 * \brief SPI4 INT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi4_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hcc32f460_spi4_int_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 POLL 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi1_poll_inst_init (void);

/**
 * \brief SPI1 POLL 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi1_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi1_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI2 POLL 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi2_poll_inst_init (void);

/**
 * \brief SPI2 POLL 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi2_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi2_poll_inst_deinit (am_spi_handle_t handle);


/**
 * \brief SPI3 POLL 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi3_poll_inst_init (void);

/**
 * \brief SPI3 POLL 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi3_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi3_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI4 POLL 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_handle_t am_hc32f460_spi4_poll_inst_init (void);

/**
 * \brief SPI4 POLL 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi4_int_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi4_poll_inst_deinit (am_spi_handle_t handle);

/**
 * \brief SPI1 SLV 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_slv_handle_t am_hc32f460_spi1_slv_dma_inst_init (void);

/**
 * \brief SPI1 SLV 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi1_slv_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi1_slv_dma_inst_deinit (am_spi_slv_handle_t handle);


/**
 * \brief SPI2 SLV 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_slv_handle_t am_hc32f460_spi2_slv_dma_inst_init (void);

/**
 * \brief SPI2 SLV 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi2_slv_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi2_slv_dma_inst_deinit (am_spi_slv_handle_t handle);

/**
 * \brief SPI3 SLV 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_slv_handle_t am_hc32f460_spi3_slv_dma_inst_init (void);

/**
 * \brief SPI3 SLV 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi3_slv_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi3_slv_dma_inst_deinit (am_spi_slv_handle_t handle);

/**
 * \brief SPI4 SLV 实例初始化，获得 SPI 标准服务句柄
 *
 * \param 无
 *
 * \return SPI 标准服务句柄，若为 NULL，表明初始化失败
 */
am_spi_slv_handle_t am_hc32f460_spi4_slv_dma_inst_init (void);

/**
 * \brief SPI4 SLV 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_spi4_slv_dma_inst_init() 函数
 *                   获得的 SPI 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_spi4_slv_dma_inst_deinit (am_spi_slv_handle_t handle);

/**
 * \brief SWDT 实例初始化，获得 SWDT 标准服务句柄
 *
 * \param 无
 *
 * \return SWDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_hc32f460_swdt_inst_init (void);

/**
 * \brief SWDT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_swdt_inst_init() 函数
 *                   获得的 SWDT 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_swdt_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief SYSTICK 实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_systick_inst_init (void);

/**
 * \brief SYSTICK 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_systick_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_systick_inst_deinit (am_timer_handle_t handle);


/**
 * \brief 定时器TIM0 Timing实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_tim01_timing_inst_init (void);

/**
 * \brief 定时器TIM0 Timing 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim01_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim01_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器TIM0 Timing实例初始化，获得 TIMER 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_tim02_timing_inst_init (void);

/**
 * \brief 定时器TIM0 Timing 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim02_timing_inst_init() 函数
 *                   获得的 TIMER 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim02_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief tim41 pwm 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_tim41_pwm_inst_init (void);

/**
 * \brief tim41 pwm 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim41_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim41_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief tim42 pwm 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_tim42_pwm_inst_init (void);

/**
 * \brief tim41 pwm 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim42_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim42_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief tim43 pwm 实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_tim43_pwm_inst_init (void);

/**
 * \brief tim43 pwm 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim43_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim43_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief 定时器TIM4 Timing 实例初始化，获得 Timer 标准服务句柄
 *
 * \param 无
 *
 * \return Timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_tim41_timing_inst_init (void);

/**
 * \brief TIM4 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim41_timing_inst_init() 函数
 *                   获得的 Timer 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim41_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器TIM4 Timing 实例初始化，获得 Timer 标准服务句柄
 *
 * \param 无
 *
 * \return Timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_tim42_timing_inst_init (void);

/**
 * \brief TIM4 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim42_timing_inst_init() 函数
 *                   获得的 Timer 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim42_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器TIM4 Timing 实例初始化，获得 Timer 标准服务句柄
 *
 * \param 无
 *
 * \return Timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_tim43_timing_inst_init (void);

/**
 * \brief TIM4 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_tim43_timing_inst_init() 函数
 *                   获得的 Timer 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim43_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief timea1 cap实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32f460_timea1_cap_inst_init (void);

/**
 * \brief timea1 cap实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea1_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea1_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief timea2 cap实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32f460_timea2_cap_inst_init (void);

/**
 * \brief timea2 cap实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea2_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea2_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief timea3 cap实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32f460_timea3_cap_inst_init (void);

/**
 * \brief timea3 cap实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea3_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea3_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief timea4 cap实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32f460_timea4_cap_inst_init (void);

/**
 * \brief timea4 cap实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea4_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea4_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief timea5 cap实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32f460_timea5_cap_inst_init (void);

/**
 * \brief timea5 cap实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea5_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea5_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief timea6 cap实例初始化，获得 CAP 标准服务句柄
 *
 * \param 无
 *
 * \return CAP 标准服务句柄，若为 NULL，表明初始化失败
 */
am_cap_handle_t am_hc32f460_timea6_cap_inst_init (void);

/**
 * \brief timea6 cap实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea6_cap_inst_init() 函数
 *                   获得的 CAP 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea6_cap_inst_deinit (am_cap_handle_t handle);

/**
 * \brief tima1 pwm实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_timea1_pwm_inst_init (void);

/**
 * \brief tima1 pwm实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea1_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea1_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief tima2 pwm实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_timea2_pwm_inst_init (void);

/**
 * \brief tima2 pwm实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea2_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea2_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief tima3 pwm实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_timea3_pwm_inst_init (void);

/**
 * \brief tima3 pwm实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea3_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea3_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief tima4 pwm实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_timea4_pwm_inst_init (void);

/**
 * \brief tima4 pwm实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea4_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea4_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief tima5 pwm实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_timea5_pwm_inst_init (void);

/**
 * \brief tima5 pwm实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea5_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea5_pwm_inst_deinit (am_pwm_handle_t handle);

/**
 * \brief tima6 pwm实例初始化，获得 PWM 标准服务句柄
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_hc32f460_timea6_pwm_inst_init (void);

/**
 * \brief tima6 pwm实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea6_pwm_inst_init() 函数
 *                   获得的 PWM 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea6_pwm_inst_deinit (am_pwm_handle_t handle);


/**
 * \brief 定时器tima1 Timing实例初始化，获得 Timer 标准服务句柄
 *
 * \param 无
 *
 * \return Timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_timea1_timing_inst_init (void);

/**
 * \brief tima1 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea1_timing_inst_init() 函数
 *                   获得的 Timer 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea1_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器tima2 Timing实例初始化，获得 Timer 标准服务句柄
 *
 * \param 无
 *
 * \return Timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_timea2_timing_inst_init (void);

/**
 * \brief tima2 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea2_timing_inst_init() 函数
 *                   获得的 Timer 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea2_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器tima3 Timing实例初始化，获得 Timer 标准服务句柄
 *
 * \param 无
 *
 * \return Timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_timea3_timing_inst_init (void);

/**
 * \brief tima3 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea3_timing_inst_init() 函数
 *                   获得的 Timer 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea3_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器tima4 Timing实例初始化，获得 Timer 标准服务句柄
 *
 * \param 无
 *
 * \return Timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_timea4_timing_inst_init (void);

/**
 * \brief tima4 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea4_timing_inst_init() 函数
 *                   获得的 Timer 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea4_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器tima5 Timing实例初始化，获得 Timer 标准服务句柄
 *
 * \param 无
 *
 * \return Timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_timea5_timing_inst_init (void);

/**
 * \brief tima5 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea5_timing_inst_init() 函数
 *                   获得的 Timer 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea5_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief 定时器tima6 Timing实例初始化，获得 Timer 标准服务句柄
 *
 * \param 无
 *
 * \return Timer 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_hc32f460_timea6_timing_inst_init (void);

/**
 * \brief tima6 Timing实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_timea6_timing_inst_init() 函数
 *                   获得的 Timer 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea6_timing_inst_deinit (am_timer_handle_t handle);

/**
 * \brief TRNG 实例初始化，获得 TRNG 标准服务句柄
 *
 * \param 无
 *
 * \return TRNG 标准服务句柄，若为 NULL，表明初始化失败
 */
am_trng_handle_t am_hc32f460_trng_inst_init (void);

/**
 * \brief TRNG 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_trng_inst_init() 函数获得的 TRNG 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_trng_inst_deinit (am_trng_handle_t handle);

/**
 * \brief WDT 实例初始化，获得 WDT 标准服务句柄
 *
 * \param 无
 *
 * \return WDT 标准服务句柄，若为 NULL，表明初始化失败
 */
am_wdt_handle_t am_hc32f460_wdt_inst_init (void);

/**
 * \brief WDT 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_wdt_inst_init() 函数获得的 WDT 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_wdt_inst_deinit (am_wdt_handle_t handle);

/**
 * \brief 系统滴答实例初始化(不使用软件定时器)
 *
 * \param[in] 无
 *
 * \return 定时器标准服务操作句柄
 */
am_timer_handle_t am_system_tick_inst_init (void);

/**
 * \brief 系统滴答实例初始化(使用软件定时器)
 *
 * \param[in] 无
 *
 * \return 定时器标准服务操作句柄
 */
am_timer_handle_t am_system_tick_softimer_inst_init (void);


/**
 * \brief CRC 实例初始化，获得 CRC 标准服务句柄
 *
 * \param 无
 *
 * \return CRC 标准服务句柄，若为 NULL，表明初始化失败
 */
am_crc_handle_t am_hc32f460_crc_inst_init (void);

/**
 * \brief CRC 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_crc_inst_init() 函数获得的 CRC 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_crc_inst_deinit (am_crc_handle_t handle);
//

/**
 * \brief UART1 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32f460_uart1_inst_init (void);

/**
 * \brief UART1 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_uart1_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_uart1_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART2 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32f460_uart2_inst_init (void);

/**
 * \brief UART2 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_uart2_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_uart2_inst_deinit (am_uart_handle_t handle);

/**
 * \brief UART3 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32f460_uart3_inst_init (void);

/**
 * \brief UART3 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_uart3_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_uart3_inst_deinit (am_uart_handle_t handle);
/**
 * \brief UART4 实例初始化，获得 UART 标准服务句柄
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_hc32f460_uart4_inst_init (void);

/**
 * \brief UART4 实例解初始化
 *
 * \param[in] handle 通过 am_hc32f460_uart4_inst_init() 函数获得的 UART 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_uart4_inst_deinit (am_uart_handle_t handle);

/**
 * \brief usb_msc实例初始化，获得usb_msc标准服务句柄
 *
 * \param 无
 *
 * \return usb_msc 标准服务句柄，若为 NULL，表明初始化失败
 */
am_usbd_msc_handle am_hc32f460_usb_msc_inst_init (void);

/**
 * \brief usb_msc解初始化
 *
 * \param[in] handle 通过 am_hc32f460_usb_msc_inst_init() 函数获得的 usb_msc 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_usb_msc_inst_deinit (void);

/**
 * \brief usb_mouse实例初始化，获得usb_mouse标准服务句柄
 *
 * \param 无
 *
 * \return usb_mouse 标准服务句柄，若为 NULL，表明初始化失败
 */
am_usbd_dev_t am_hc32f460_usb_mouse_inst_init (void);

/**
 * \brief usb_mouse解初始化
 *
 * \param[in] handle 通过 am_hc32f460_usb_mouse_inst_init() 函数获得的 usb_msc 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_usb_mouse_inst_deinit (void);


/**
 * \brief usb hid custom实例初始化，获得usb hid custom标准服务句柄
 *
 * \param 无
 *
 * \return usb hid custom 标准服务句柄，若为 NULL，表明初始化失败
 */
am_usbd_dev_t am_hc32f460_usb_hid_custom_inst_init (void);

/**
 * \brief usb hid custom解初始化
 *
 * \param[in] handle 通过 am_hc32f460_usb__hid_custom_inst_init() 函数获得的 usb_msc 标准服务句柄
 *
 * \return 无
 */
void am_hc32f460_usb_hid_custom_inst_deinit (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AM_HC32F460_INST_INIT_H */

/* end of file */
