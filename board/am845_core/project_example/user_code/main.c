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
 * \brief LPC84X 模板工程
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-07  nwt, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_led.h"
#include "am_fm175xx.h"
#include "demo_am845_core_entries.h"

/**
 * \brief AMetal 应用程序入口
 */
void am_main (void) 
{
    AM_DBG_INFO("Start up successful!\r\n");
    /*
     * 以下为所有demo的入口函数，需要运行哪个 demo， 就取消对应函数调用行的注释
     *
     * 默认运行的是LED闪烁 demo。
     *
     * 注意：同一时刻只能运行一个 demo，即只能使某一行处于取消注释状态。
     */

   demo_am845_core_std_led_entry();
   //demo_am845_core_std_wwdt_entry();
   //demo_am845_core_hw_wwdt_entry();
   //demo_am845_core_hw_acmp_int_entry();
   //demo_am845_core_hw_acmp_lad_entry();
   //demo_am845_core_hw_acmp_poll_entry();
   //demo_am845_core_hw_adc_dma_entry();
   //demo_am845_core_hw_adc_thcmp_entry();
   //demo_am845_core_std_adc_dma_int_entry();
   //demo_am845_core_std_adc_dma_poll_entry();
   //demo_am845_core_std_adc_int_entry();
   //demo_am845_core_std_adc_ntc_entry();
   //demo_am845_core_std_adc_poll_entry();
   //demo_am845_core_hw_crc_entry();
   //demo_am845_core_std_crc_entry();
   //demo_am845_core_drv_dma_hwtrigger_burst_entry();
   //demo_am845_core_drv_dma_m2m_entry();
   //demo_am845_core_drv_dma_ping_pong_entry();
   //demo_am845_core_hw_flash_ctrl_entry();
   //demo_am845_core_hw_gpio_entry();
   //demo_am845_core_hw_gpio_int_entry();
   //demo_am845_core_std_gpio_entry();
   //demo_am845_core_std_gpio_int_entry();
   //demo_am845_core_std_gpio_key_entry();
   //demo_am845_core_hw_i2c_master_dma_entry();
   //demo_am845_core_hw_i2c_master_poll_entry();
   //demo_am845_core_hw_i2c_slave_dma_entry();
   //demo_am845_core_hw_i2c_slave_int_entry();
   //demo_am845_core_hw_i2c_slave_poll_entry();
   //demo_am845_core_std_i2c_lm75_entry();
   //demo_am845_core_std_i2c_master_sync_entry();
   //demo_am845_core_std_i2c_master_async_entry();
   //demo_am845_core_std_key_input_event_entry();
   //demo_am845_core_hw_mrt_entry();
   //demo_am845_core_std_mrt_timer_entry();
   //demo_am845_core_hw_pmu_deeppowerdown_entry();
   //demo_am845_core_hw_pmu_deepsleep_entry();
   //demo_am845_core_hw_pmu_powerdown_entry();
   //demo_am845_core_hw_pmu_sleep_entry();
   //demo_am845_core_drv_sct_timing_pwm_entry();
   //demo_am845_core_hw_sct_1_32bit_pwm_entry();
   //demo_am845_core_hw_sct_1_32bit_timing_entry();
   //demo_am845_core_hw_sct_2_16bit_pwm_entry();
   //demo_am845_core_hw_sct_2_16bit_timing_entry();
   //demo_am845_core_hw_sct_multi_states_entry();
   //demo_am845_core_std_sct_pwm_entry();
   //demo_am845_core_std_sct_timer_entry();
   //demo_am845_core_hw_spi_master_entry();
   //demo_am845_core_hw_spi_slave_entry();
   //demo_am845_core_hw_spi_slave_dma_entry();
   //demo_am845_core_std_spi_master_dma_entry();
   //demo_am845_core_std_spi_master_int_entry();
   //demo_am845_core_hw_clkout_entry();
   //demo_am845_core_std_delay_entry();
   //demo_am845_core_std_systick_timer_entry();
   //demo_am845_core_hw_usart_poll_entry();
   //demo_am845_core_hw_usart_rx_dma_entry();
   //demo_am845_core_hw_usart_tx_dma_entry();
   //demo_am845_core_std_usart_flowctrl_entry();
   //demo_am845_core_std_usart_poll_entry();
   //demo_am845_core_std_usart_rngbuf_entry();
   //demo_am845_core_hw_wkt_deeppowerdown_wakeup_entry();
   //demo_am845_core_hw_wkt_timer_entry();
   //demo_am845_core_std_wkt_timer_entry();
   //demo_am845_core_hw_dac_entry ();
   //demo_am845_core_std_dac_entry();
   //demo_am845_core_hw_flash_ctrl_entry ();
   //demo_am845_core_hw_iap_entry ();
   //demo_am845_corefid_dr_fm175xx_picca_halt ();
   //demo_am845_corefid_dr_fm175xx_picca_val_operate ();
   //demo_am845_corefid_dr_fm175xx_picca_read_block ();
   //demo_am845_corefid_dr_fm175xx_picca_read_id ();
   //demo_am845_corefid_dr_fm175xx_picca_write_block ();
   //demo_am845_core_zlg52810_auto_baudrate_entry();
   //demo_am845_core_eeprom_entry();

   while(1) {
        ;
    }
}

/* end of file */
