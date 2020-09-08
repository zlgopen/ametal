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
 * \brief stm32f103rbt6 模板工程
 *
 * - 实验现象:
 *   模板例程:LED0以1秒的频率闪烁; demo例程入口:详见demo入口函数文件介绍
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-027  yrh, first implementation
 * \endinternal
 */

/**
 * \brief 例程入口
 */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "demo_stm32f103rbt6_core_entries.h"
#include "stm32f103rbt6_pin.h"

int am_main (void)
{
    AM_DBG_INFO("Start up successful!\r\n");
    /*
     * 以下为所有demo的入口函数
     *
     * 需要运行哪个demo，就取消对应函数调用行的注释，
     * 另外    \examples\board\stm32f103rbt6_core
     *     \examples\soc\stm
     *     \examples\std
     * 三个路径下的相关文件需要取消屏蔽。
     *
     * 默认运行的是LED闪烁 demo
     *
     * 注意：同一时刻只能运行一个 demo，即只能使某一行处于取消注释状态
     */

    demo_stm32f103rbt6_core_std_led_entry();
//    demo_stm32f103rbt6_core_std_delay_entry();
//    demo_stm32f103rbt6_core_std_adc_entry();
//    demo_stm32f103rbt6_core_hw_adc_int_entry();
//    demo_stm32f103rbt6_core_std_multichan_adc_entry();
//    demo_stm32f103rbt6_core_hw_adc_dma_entry();
//    demo_stm32f103rbt6_core_hw_adc_dma_double_entry();
//    demo_stm32f103rbt6_core_std_buzzer_entry();
//    demo_stm32f103rbt6_core_std_crc_entry();
//    demo_stm32f103rbt6_core_drv_dma_m2m_entry();
//    demo_stm32f103rbt6_core_drv_flash_entry();
//    demo_stm32f103rbt6_core_hw_gpio_entry();
//    demo_stm32f103rbt6_core_hw_gpio_trigger_entry();
//    demo_stm32f103rbt6_core_std_gpio_entry();
//    demo_stm32f103rbt6_core_std_gpio_trigger_entry();
//    demo_stm32f103rbt6_core_hw_i2c_master_poll_entry();
//    demo_stm32f103rbt6_core_hw_i2c_slave_poll_entry();
//    demo_stm32f103rbt6_core_std_i2c_lm75_entry();
//    demo_stm32f103rbt6_core_std_i2c_master_async_entry();
//    demo_stm32f103rbt6_core_std_i2c_master_sync_entry();
//    demo_stm32f103rbt6_core_std_key_input_event_entry();
//    demo_stm32f103rbt6_core_hw_iwdg_entry();
//    demo_stm32f103rbt6_core_std_iwdg_entry();
//    demo_stm32f103rbt6_core_microport_ds1302_entry();
//    demo_stm32f103rbt6_core_microport_eeprom_entry();
//    demo_stm32f103rbt6_core_microport_eeprom_nvram_entry();
//    demo_stm32f103rbt6_core_microport_flash_entry();
//    demo_stm32f103rbt6_core_microport_flash_ftl_entry();
//    demo_stm32f103rbt6_core_microport_flash_mtd_entry();
//    demo_stm32f103rbt6_core_microport_rs485_entry();
//    demo_stm32f103rbt6_core_microport_rtc_entry();
//    demo_stm32f103rbt6_core_microport_rx8025t_entry();
//    demo_stm32f103rbt6_core_miniport_digitron_entry();
//    demo_stm32f103rbt6_core_miniport_hc595_digitron_entry();
//    demo_stm32f103rbt6_core_miniport_hc595_digitron_key_entry();
//    demo_stm32f103rbt6_core_miniport_hc595_digitron_temp_entry();
//    demo_stm32f103rbt6_core_miniport_hc595_led_entry();
//    demo_stm32f103rbt6_core_miniport_key_entry();
//    demo_stm32f103rbt6_core_miniport_key_digitron_entry();
//    demo_stm32f103rbt6_core_miniport_led_entry();
//    demo_stm32f103rbt6_core_miniport_zlg72128_entry();
//    demo_stm32f103rbt6_core_std_rtc_entry();
//    demo_stm32f103rbt6_core_hw_spi_master_entry ();
//    demo_stm32f103rbt6_core_std_spi_master_dma_entry ();
//    demo_stm32f103rbt6_core_std_spi_master_int_entry ();
//    demo_stm32f103rbt6_core_std_spi_master_poll_entry ();
//    demo_stm32f103rbt6_core_std_spi_slaver_dma_entry ();
//    demo_stm32f103rbt6_core_hw_tim_cap_entry();
//    demo_stm32f103rbt6_core_std_tim1_cap_entry();
//    demo_stm32f103rbt6_core_std_tim2_cap_entry();
//    demo_stm32f103rbt6_core_std_tim3_cap_entry();
//    demo_stm32f103rbt6_core_std_tim4_cap_entry();
//    demo_stm32f103rbt6_core_hw_tim_cmp_toggle_entry();
//    demo_stm32f103rbt6_core_hw_tim_pwm_entry();
//    demo_stm32f103rbt6_core_std_tim1_pwm_entry();
//    demo_stm32f103rbt6_core_std_tim2_pwm_entry();
//    demo_stm32f103rbt6_core_std_tim3_pwm_entry();
//    demo_stm32f103rbt6_core_std_tim4_pwm_entry();
//    demo_stm32f103rbt6_core_hw_tim_timing_entry();
//    demo_stm32f103rbt6_core_std_tim1_timing_entry();
//    demo_stm32f103rbt6_core_std_tim2_timing_entry();
//    demo_stm32f103rbt6_core_std_tim3_timing_entry();
//    demo_stm32f103rbt6_core_std_tim4_timing_entry();
//    demo_stm32f103rbt6_core_hw_usart_int_entry();
//    demo_stm32f103rbt6_core_hw_usart_polling_entry();
//    demo_stm32f103rbt6_core_hw_usart_rx_dma_entry();
//    demo_stm32f103rbt6_core_hw_usart_tx_dma_entry();
//    demo_stm32f103rbt6_core_std_usart_polling_entry();
//    demo_stm32f103rbt6_core_std_usart_ringbuf_entry();
//    demo_stm32f103rbt6_core_hw_wwdg_entry();
//    demo_stm32f103rbt6_core_std_wwdg_entry();
//    demo_stm32f103rbt6_core_dr_fm175xx_cpu_card();
//    demo_stm32f103rbt6_core_dr_fm175xx_picca_read_id();
//    demo_stm32f103rbt6_core_dr_fm175xx_picca_halt();
//    demo_stm32f103rbt6_core_dr_fm175xx_picca_read_block ();
//    demo_stm32f103rbt6_core_dr_fm175xx_picca_val_operate();
//    demo_stm32f103rbt6_core_dr_fm175xx_picca_write_block();
//    demo_stm32f103rbt6_core_dr_fm175xx_piccb_read_id();
//    demo_stm32f103rbt6_core_dr_fm175xx_picca_lpcd_read_id();
//    demo_stm32f103rbt6_drv_sleepmode_timer_wake_up_entry();
//    demo_stm32f103rbt6_drv_sleepmode_wake_up_entry();
//    demo_stm32f103rbt6_drv_standbymode_wake_up_entry();
//    demo_stm32f103rbt6_drv_stopmode_wake_up_entry();
//    demo_stm32f103rbt6_core_can_int_entry();
//    demo_stm32f103rbt6_core_can_entry();
//    demo_stm32f103rbt6_core_std_can_entry();
//    demo_stm32f103rbt6_core_sensor_tmd3702vc_entry();
//    demo_stm32f103rbt6_core_sensor_tmd3702vc_int_entry();
//    demo_stm32f103rbt6_core_sensor_tcs3430_entry();
//    demo_stm32f103rbt6_core_sensor_tcs3430_int_entry();
//    demo_stm32f103rbt6_core_sensor_as621x_comparator_mode_entry();
//    demo_stm32f103rbt6_core_sensor_as621x_interrupt_mode_entry();
//    demo_stm32f103rbt6_core_sensor_as7341_entry();
//    demo_stm32f103rbt6_core_sensor_as7262_i2c_entry();
//    demo_stm32f103rbt6_core_sensor_as7262_i2c_int_entry();
//    demo_stm32f103rbt6_core_sensor_as7262_uart_entry();
//    demo_stm32f103rbt6_core_sensor_as5600_entry();
//    demo_stm32f103rbt6_core_sensor_tmf8801_entry();
//    demo_stm32f103rbt6_core_sensor_tmf8801_int_entry();

    while (1) {

    }
}

/* end of file */
