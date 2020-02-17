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
 * \brief ZLG116 例程工程
 *
 * - 操作步骤：
 *   1. 取消屏蔽需要使用的例程。
 *
 * \note
 *    同一时刻只能使用一个例程。
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-07  nwt, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "demo_am116_core_entries.h"
/**
 * \brief AMetal 应用程序入口
 */
int am_main (void)
{
    AM_DBG_INFO("Start up successful!\r\n");
    /*
     * 以下为所有demo的入口函数，需要运行哪个 demo， 就取消对应函数调用行的注释
     *
     * 默认运行的是LED闪烁 demo。
     *
     * 注意：同一时刻只能运行一个 demo，即只能使某一行处于取消注释状态。
     */
    demo_am116_core_std_led_entry();
//    demo_am116_core_std_delay_entry();
//    demo_am116_core_hw_adc_int_entry();
//    demo_am116_core_std_adc_entry();
//    demo_am116_core_std_adc_ntc_entry();
//    demo_am116_core_std_buzzer_entry();
//    demo_am116_core_hw_clk_entry();
//    demo_am116_core_drv_dma_m2m_entry();
//    demo_am116_core_drv_flash_entry();
//    demo_am116_core_hw_gpio_entry();
//    demo_am116_core_hw_gpio_trigger_entry();
//    demo_am116_core_std_gpio_entry();
//    demo_am116_core_std_gpio_trigger_entry();
//    demo_am116_core_hw_i2c_master_poll_entry();
//    demo_am116_core_hw_i2c_slave_poll_entry();
//    demo_am116_core_std_i2c_lm75_entry();
//    demo_am116_core_std_i2c_master_async_entry();
//    demo_am116_core_std_i2c_master_sync_entry();
//    demo_am116_core_std_i2c_slave_entry();
//    demo_am116_core_std_i2c_slave_subaddr_entry();
//    demo_am116_core_std_key_entry();
//    demo_am116_core_std_softimer_entry();
//    demo_am116_core_hw_spi_master_entry();
//    demo_am116_core_std_spi_flash_dma_entry();
//    demo_am116_core_std_spi_flash_int_entry();
//    demo_am116_core_std_spi_master_dma_entry();
//    demo_am116_core_std_spi_master_int_entry();
//    demo_am116_core_std_spi_slave_dma_entry();
//    demo_am116_core_hw_tim_cap_entry();
//    demo_am116_core_hw_tim_cmp_toggle_entry();
//    demo_am116_core_hw_tim_pwm_entry();
//    demo_am116_core_hw_tim_pwm_dead_entry();
//    demo_am116_core_hw_tim_timing_entry();
//    demo_am116_core_std_tim14_cap_entry();
//    demo_am116_core_std_tim14_pwm_entry();
//    demo_am116_core_std_tim14_timing_entry();
//    demo_am116_core_std_tim16_cap_entry();
//    demo_am116_core_std_tim16_pwm_entry();
//    demo_am116_core_std_tim16_timing_entry();
//    demo_am116_core_std_tim17_cap_entry();
//    demo_am116_core_std_tim17_pwm_entry();
//    demo_am116_core_std_tim17_timing_entry();
//    demo_am116_core_std_tim1_cap_entry();
//    demo_am116_core_std_tim1_pwm_entry();
//    demo_am116_core_std_tim1_timing_entry();
//    demo_am116_core_std_tim2_cap_entry();
//    demo_am116_core_std_tim2_pwm_entry();
//    demo_am116_core_std_tim2_timing_entry();
//    demo_am116_core_std_tim3_cap_entry();
//    demo_am116_core_std_tim3_pwm_entry();
//    demo_am116_core_std_tim3_timing_entry();
//    demo_am116_core_hw_uart_int_entry();
//    demo_am116_core_hw_uart_polling_entry();
//    demo_am116_core_hw_uart_rx_dma_entry();
//    demo_am116_core_hw_uart_tx_dma_entry();
//    demo_am116_core_std_uart_polling_entry();
//    demo_am116_core_std_uart_ringbuf_entry();
//    demo_am116_core_drv_sleepmode_timer_wake_up_entry();
//    demo_am116_core_drv_sleepmode_wake_up_entry();
//    demo_am116_core_drv_standbymode_wake_up_entry();
//    demo_am116_core_drv_stopmode_wake_up_entry();
//    demo_am116_core_hw_iwdg_entry();
//    demo_am116_core_hw_wwdg_entry();
//    demo_am116_core_std_iwdg_entry();
//    demo_am116_core_std_wwdg_entry();
//    demo_am116_core_microport_ds1302_entry();
//    demo_am116_core_microport_eeprom_entry();
//    demo_am116_core_microport_eeprom_nvram_entry();
//    demo_am116_core_microport_flash_entry();
//    demo_am116_core_microport_flash_ftl_entry();
//    demo_am116_core_microport_flash_mtd_entry();
//    demo_am116_core_microport_rs485_entry();
//    demo_am116_core_microport_rtc_entry();
//    demo_am116_core_microport_hmi_int_entry();
//    demo_am116_core_microport_rx8025t_entry();
//    demo_am116_core_microport_usb_entry();
//    demo_am116_core_miniport_digitron_entry();
//    demo_am116_core_miniport_hc595_digitron_entry();
//    demo_am116_core_miniport_hc595_digitron_key_entry();
//    demo_am116_core_miniport_hc595_digitron_temp_entry();
//    demo_am116_core_miniport_hc595_led_entry();
//    demo_am116_core_miniport_key_entry();
//    demo_am116_core_miniport_key_digitron_entry();
//    demo_am116_core_miniport_led_entry();
//    demo_am116_core_miniport_zlg72128_entry();
//    demo_am116_core_dr_fm175xx_picca_read_id();
//    demo_am116_core_dr_fm175xx_picca_halt();
//    demo_am116_core_dr_fm175xx_picca_read_block ();
//    demo_am116_core_dr_fm175xx_picca_val_operate();
//    demo_am116_core_dr_fm175xx_picca_write_block();
//    demo_am116_core_dr_fm175xx_piccb_read_id();
//    demo_am116_core_dr_fm175xx_picca_lpcd_read_id();
//    demo_am116_zsn603_led_test_entry();
//    demo_am116_zsn603_picca_test_entry();
//    demo_am116_zsn603_piccb_test_entry();
//    demo_am116_zsn603_auto_detect_test_entry();
//    demo_am116_core_sensor_fxos8700cq_entry();
//    demo_am116_core_sensor_fxos8700cq_int_entry();
//    demo_am116_core_sensor_mpl3115a2_entry();
//    demo_am116_core_sensor_mpl3115a2_int_entry();
//    demo_am116_core_sensor_mag3110_entry();
//    demo_am116_core_sensor_mag3110_int_entry();
//    demo_am116_core_mk100_test_entry();
//    demo_am116_core_gs200_set_baudrate_entry();
//    demo_am116_core_rs200_test_entry();
    while (1) {
    }
}
