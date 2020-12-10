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
 * \brief HC32F07X 例程工程
 *
 * - 实验现象:
 *   模板例程:LED0以1秒的频率闪烁; demo例程入口:详见demo入口函数文件介绍
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-19  zp, first implementation
 * \endinternal
 */

/**
 * \brief 例程入口
 */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_softimer.h"
#include "demo_amf07x_core_entries.h"
int am_main (void)
{
    AM_DBG_INFO("Start up successful!\r\n");

    /* demo例程入口 */
    demo_hc32f07x_core_std_led_entry ();
//    demo_hc32f07x_drv_lpmode_wake_up_entry ();
//    demo_hc32f07x_core_hw_clk_entry();
//    demo_hc32f07x_core_std_buzzer_entry();
//    demo_hc32f07x_core_std_adc_entry();
//    demo_hc32f07x_core_hw_adc_int_entry();
//    demo_hc32f07x_core_std_multichan_adc_entry();
//    demo_hc32f07x_core_hw_multichan_adc_entry();
//    demo_hc32f07x_core_hw_adc_dma_entry();
//    demo_hc32f07x_core_std_crc_entry();
//    demo_hc32f07x_core_drv_dma_m2m_entry();
//    demo_hc32f07x_core_drv_flash_entry();
//    demo_hc32f07x_core_drv_flash_write_protect_entry();
//    demo_hc32f07x_core_drv_pca_8bit_pwm_entry ();
//    demo_hc32f07x_core_drv_pca_16bit_pwm_entry ();
//    demo_hc32f07x_core_hw_gpio_entry();
//    demo_hc32f07x_core_hw_gpio_trigger_entry();
//    demo_hc32f07x_core_std_gpio_entry();
//    demo_hc32f07x_core_std_gpio_trigger_entry();
//    demo_hc32f07x_core_std_i2c_slave_int_entry();
//    demo_hc32f07x_core_std_i2c_master_async_entry();
//    demo_hc32f07x_core_std_i2c_master_sync_entry();
//    demo_hc32f07x_core_std_key_input_event_entry();
//    demo_hc32f07x_core_hw_spi_master_entry();
//    demo_hc32f07x_core_std_spi_master_dma_entry();
//    demo_hc32f07x_core_std_spi_master_int_entry();
//    demo_hc32f07x_core_hw_adtim_swsync_entry();
//    demo_hc32f07x_core_hw_adtim_port_sw_brake_entry();
//    demo_hc32f07x_core_hw_adtim_vc_brake_entry();
//    demo_hc32f07x_core_hw_adtim_trig_adc_sqr_entry();
//    demo_hc32f07x_core_hw_tim_port_brake_entry();
//    demo_hc32f07x_core_hw_tim_vc_brake_entry();
//    demo_hc32f07x_core_hw_tim_trig_adc_sqr_entry();
//    demo_hc32f07x_core_hw_tim_cap_entry();
//    demo_hc32f07x_core_hw_tim_cmp_toggle_entry();
//    demo_hc32f07x_core_hw_tim_pwm_entry();
//    demo_hc32f07x_core_hw_tim_pwm_dead_entry();
//    demo_hc32f07x_core_hw_tim_timing_entry();
//    demo_hc32f07x_core_std_tim0_cap_entry();
//    demo_hc32f07x_core_std_tim1_cap_entry();
//    demo_hc32f07x_core_std_tim2_cap_entry();
//    demo_hc32f07x_core_std_tim3_cap_entry();
//    demo_hc32f07x_core_std_tim4_cap_entry();
//    demo_hc32f07x_core_std_tim5_cap_entry();
//    demo_hc32f07x_core_std_tim6_cap_entry();
//    demo_hc32f07x_core_std_tim0_pwm_entry();
//    demo_hc32f07x_core_std_tim1_pwm_entry();
//    demo_hc32f07x_core_std_tim2_pwm_entry();
//    demo_hc32f07x_core_std_tim3_pwm_entry();
//    demo_hc32f07x_core_std_tim4_pwm_entry();
//    demo_hc32f07x_core_std_tim5_pwm_entry();
//    demo_hc32f07x_core_std_tim6_pwm_entry();
//    demo_hc32f07x_core_std_tim0_timing_entry();
//    demo_hc32f07x_core_std_tim1_timing_entry();
//    demo_hc32f07x_core_std_tim2_timing_entry();
//    demo_hc32f07x_core_std_tim3_timing_entry();
//    demo_hc32f07x_core_std_tim4_timing_entry();
//    demo_hc32f07x_core_std_tim5_timing_entry();
//    demo_hc32f07x_core_std_tim6_timing_entry();
//    demo_hc32f07x_core_hw_uart_int_entry();
//    demo_hc32f07x_core_hw_uart_polling_entry();
//    demo_hc32f07x_core_hw_uart_rx_dma_entry();
//    demo_hc32f07x_core_hw_uart_tx_dma_entry();
//    demo_hc32f07x_core_std_uart_polling_entry();
//    demo_hc32f07x_core_std_uart_ringbuf_entry();
//    demo_hc32f07x_core_hw_lpuart_int_entry();
//    demo_hc32f07x_core_hw_lpuart_polling_entry();
//    demo_hc32f07x_core_hw_lpuart_rx_dma_entry();
//    demo_hc32f07x_core_hw_lpuart_tx_dma_entry();
//    demo_hc32f07x_core_std_lpuart_polling_entry();
//    demo_hc32f07x_core_std_lpuart_ringbuf_entry();
//    demo_hc32f07x_core_hw_lvd_system_reset_entry ();
//    demo_hc32f07x_core_hw_lvd_int_entry ();
//    demo_hc32f07x_core_hw_opa_one_entry ();
//    demo_hc32f07x_core_hw_opa_dac_entry ();
//    demo_hc32f07x_core_hw_dac_dma_entry();
//    demo_hc32f07x_core_std_dac_entry ();
//    demo_hc32f07x_core_hw_vc_int_entry();
//    demo_hc32f07x_core_hw_aes_ency_decy_entry ();
//    demo_hc32f07x_core_hw_trng_generate_entry ();
//    demo_hc32f07x_core_hw_wdt_entry();
//    demo_hc32f07x_core_std_wdt_entry();
//    demo_hc32f07x_core_hw_lcd_entry ();
//    demo_hc32f072_core_std_can_entry ();
//    demo_hc32f07x_usbd_msc_entry ();
//    demo_hc32f07x_usbd_mouse_entry ();
//    demo_hc32f07x_usbd_keyboard_entry ();
//    demo_hc32f07x_usbd_printer_counter_entry ();
//    demo_hc32f07x_usbd_printer_entry ();
//    demo_hc32f07x_usbd_vcom_entry ();
//    demo_hc32f07x_hw_i2s_int_entry ();
//    demo_hc32f07x_core_hw_hdiv_entry ();
//    demo_hc32f07x_core_hw_ram_entry();
//    demo_hc32f07x_core_clktrim_entry ();
//    demo_hc32f07x_core_cts_entry ();
    while (1) {

    }
}

/* end of file */
