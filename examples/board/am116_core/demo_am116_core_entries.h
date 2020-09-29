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
 * \brief ZLG116 所有例程函数入口声明
 * \sa demo_am116ble_all_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  lnk, first implementation
 * \endinternal
 */
 
#ifndef __DEMO_AM116_CORE_ENTRIES_H
#define __DEMO_AM116_CORE_ENTRIES_H
 
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief ADC INT 例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_adc_int_entry (void);

/**
 * \brief ADC 例程，通过标准接口实现
 */
void demo_am116_core_std_adc_entry (void);

/**
 * \brief ADC 利用板子的热敏电阻(NTC)，推算出温度例程，通过标准接口实现
 */
void demo_am116_core_std_adc_ntc_entry (void);

/**
 * \brief 蜂鸣器例程，通过标准接口实现
 */
void demo_am116_core_std_buzzer_entry (void);

/**
 * \brief CLK 例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_clk_entry (void);

/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 */
void demo_am116_core_drv_dma_m2m_entry (void);

/**
 * \brief FLASH 例程，通过驱动层接口实现
 */
void demo_am116_core_drv_flash_entry (void);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_gpio_entry (void);

/**
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_gpio_trigger_entry (void);

/**
 * \brief GPIO 例程，通过标准接口实现
 */
void demo_am116_core_std_gpio_entry (void);

/**
 * \brief GPIO 引脚中断例程，通过标准接口实现
 */
void demo_am116_core_std_gpio_trigger_entry (void);

/**
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_i2c_master_poll_entry (void);

/**
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 */
void demo_am116_core_hw_i2c_slave_poll_entry (void);

/**
 * \brief I2C 主机从 LM75 传感器读取温度值例程，通过标准接口实现
 */
void demo_am116_core_std_i2c_lm75_entry (void);

/**
 * \brief I2C 主机访问 EEPROM 例程，通过异步标准接口实现
 */
void demo_am116_core_std_i2c_master_async_entry (void);

/**
 * \brief I2C 主机访问 EEPROM 例程，通过同步标准接口实现
 */
void demo_am116_core_std_i2c_master_sync_entry (void);

/**
 * \brief I2C 从机例程，通过标准接口实现
 */
void demo_am116_core_std_i2c_slave_entry (void);

/**
 * \brief I2C 从机模拟 TMP100 例程，通过标准接口实现
 */
void demo_am116_core_std_i2c_slave_subaddr_entry (void);

/**
 * \brief 输入子系统例程，通过标准接口实现
 */
void demo_am116_core_std_key_entry (void);

/**
 * \brief LED 例程，通过标准接口实现
 */
void demo_am116_core_std_led_entry (void);

/**
 * \brief 软件定时器例程，通过标准接口实现
 */
void demo_am116_core_std_softimer_entry (void);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_spi_master_entry (void);

/**
 * \brief SPI 主机 DMA 方式与 FLASH 通信例程，通过标准接口实现
 */
void demo_am116_core_std_spi_flash_dma_entry (void);

/**
 * \brief SPI 主机中断方式与 FLASH 通信例程，通过标准接口实现
 */
void demo_am116_core_std_spi_flash_int_entry (void);

/**
 * \brief SPI 主机 DMA 方式例程，通过标准接口实现
 */
void demo_am116_core_std_spi_master_dma_entry (void);

/**
 * \brief SPI 主机中断方式例程，通过标准接口实现
 */
void demo_am116_core_std_spi_master_int_entry (void);

/**
 * \brief SPI 从机 DMA 方式例程，通过标准接口实现
 */
void demo_am116_core_std_spi_slave_dma_entry (void);

/**
 * \brief 定时器 CAP 捕获例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_tim_cap_entry (void);

/**
 * \brief 定时器通道比较匹配引脚翻转例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_tim_cmp_toggle_entry (void);

/**
 * \brief 定时器 PWM 输出例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_tim_pwm_entry (void);

/**
 * \brief 定时器带死区时间的互补 PWM 输出例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_tim_pwm_dead_entry (void);

/**
 * \brief 定时器 TIMING 例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_tim_timing_entry (void);

/**
 * \brief TIM14 定时器 CAP 例程，通过标准接口实现
 */
void demo_am116_core_std_tim14_cap_entry (void);

/**
 * \brief TIM14 定时器 PWM 例程，通过标准接口实现
 */
void demo_am116_core_std_tim14_pwm_entry (void);

/**
 * \brief TIM14 定时器 TIMING 例程，通过标准接口实现
 */
void demo_am116_core_std_tim14_timing_entry (void);

/**
 * \brief TIM16 定时器 CAP 例程，通过标准接口实现
 */
void demo_am116_core_std_tim16_cap_entry (void);

/**
 * \brief TIM16 定时器 PWM 例程，通过标准接口实现
 */
void demo_am116_core_std_tim16_pwm_entry (void);

/**
 * \brief TIM16 定时器 TIMING 例程，通过标准接口实现
 */
void demo_am116_core_std_tim16_timing_entry (void);

/**
 * \brief TIM17 定时器 CAP 例程，通过标准接口实现
 */
void demo_am116_core_std_tim17_cap_entry (void);

/**
 * \brief TIM17 定时器 PWM 例程，通过标准接口实现
 */
void demo_am116_core_std_tim17_pwm_entry (void);

/**
 * \brief TIM17 定时器 TIMING 例程，通过标准接口实现
 */
void demo_am116_core_std_tim17_timing_entry (void);

/**
 * \brief TIM1 定时器 CAP 例程，通过标准接口实现
 */
void demo_am116_core_std_tim1_cap_entry (void);

/**
 * \brief TIM1 定时器 PWM 例程，通过标准接口实现
 */
void demo_am116_core_std_tim1_pwm_entry (void);

/**
 * \brief TIM1 定时器 TIMING 例程，通过标准接口实现
 */
void demo_am116_core_std_tim1_timing_entry (void);

/**
 * \brief TIM2 定时器 CAP 例程，通过标准接口实现
 */
void demo_am116_core_std_tim2_cap_entry (void);

/**
 * \brief TIM2 定时器 PWM 例程，通过标准接口实现
 */
void demo_am116_core_std_tim2_pwm_entry (void);

/**
 * \brief TIM2 定时器 TIMING 例程，通过标准接口实现
 */
void demo_am116_core_std_tim2_timing_entry (void);

/**
 * \brief TIM3 定时器 CAP 例程，通过标准接口实现
 */
void demo_am116_core_std_tim3_cap_entry (void);

/**
 * \brief TIM3 定时器 PWM 例程，通过标准接口实现
 */
void demo_am116_core_std_tim3_pwm_entry (void);

/**
 * \brief TIM3 定时器 TIMING 例程，通过标准接口实现
 */
void demo_am116_core_std_tim3_timing_entry (void);

/**
 * \brief UART 中断发送例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_uart_int_entry (void);

/**
 * \brief UART 轮询方式例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_uart_polling_entry (void);

/**
 * \brief UART DMA 接收例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_uart_rx_dma_entry (void);

/**
 * \brief UART DMA 发送例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_uart_tx_dma_entry (void);

/**
 * \brief UART 轮询方式例程，通过标准接口实现
 */
void demo_am116_core_std_uart_polling_entry (void);

/**
 * \brief UART 环形缓冲区例程，通过标准接口实现
 */
void demo_am116_core_std_uart_ringbuf_entry (void);

/**
 * \brief 睡眠模式例程，使用定时器周期唤醒，通过驱动层接口实现
 */
void demo_am116_core_drv_sleepmode_timer_wake_up_entry (void);

/**
 * \brief 睡眠模式例程，通过驱动层接口实现
 */
void demo_am116_core_drv_sleepmode_wake_up_entry (void);

/**
 * \brief 待机模式例程，通过驱动层接口实现
 */
void demo_am116_core_drv_standbymode_wake_up_entry (void);

/**
 * \brief 停止模式例程，通过驱动层接口实现
 */
void demo_am116_core_drv_stopmode_wake_up_entry (void);

/**
 * \brief IWDG 例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_iwdg_entry (void);

/**
 * \brief WWDG 例程，通过 HW 层接口实现
 */
void demo_am116_core_hw_wwdg_entry (void);

/**
 * \brief IWDG 例程，通过标准接口实现
 */
void demo_am116_core_std_iwdg_entry (void);

/**
 * \brief WWDG 例程，通过标准接口实现
 */
void demo_am116_core_std_wwdg_entry (void);

/**
 * \brief MicroPort DS1302 例程，通过标准接口实现
 */
void demo_am116_core_microport_ds1302_entry (void);

/**
 * \brief MicroPort EEPROM 例程，通过标准接口实现
 */
void demo_am116_core_microport_eeprom_entry (void);

/**
 * \brief MicroPort EEPROM 例程，通过 NVRAM 接口实现
 */
void demo_am116_core_microport_eeprom_nvram_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过标准接口实现
 */
void demo_am116_core_microport_flash_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过 FTL 接口实现
 */
void demo_am116_core_microport_flash_ftl_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过 MTD 接口实现
 */
void demo_am116_core_microport_flash_mtd_entry (void);

/**
 * \brief MicroPort RS485 例程，通过标准接口实现
 */
void demo_am116_core_microport_rs485_entry (void);

/**
 * \brief MicroPort RTC 例程，通过标准接口实现
 */
void demo_am116_core_microport_rtc_entry (void);

/**
 * \brief MicroPort RTC_HMI中断例程，通过标准接口实现
 */
void demo_am116_core_microport_hmi_int_entry (void);

/**
 * \brief MicroPort RX8025T 例程，通过标准接口实现
 */
void demo_am116_core_microport_rx8025t_entry (void);

/**
 * \brief MicroPort USB 例程，通过标准接口实现
 */
void demo_am116_core_microport_usb_entry (void);

/**
 * \brief MiniPort-VIEW 例程
 */
void demo_am116_core_miniport_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW 例程
 */
void demo_am116_core_miniport_hc595_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW + MiniPort-KEY 例程
 */
void demo_am116_core_miniport_hc595_digitron_key_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW + LM75 温度显示例程
 */
void demo_am116_core_miniport_hc595_digitron_temp_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-LED 例程
 */
void demo_am116_core_miniport_hc595_led_entry (void);

/**
 * \brief MiniPort-KEY 例程
 */
void demo_am116_core_miniport_key_entry (void);

/**
 * \brief MiniPort-KEY + MiniPort-DIGITORN显示例程
 */
void demo_am116_core_miniport_key_digitron_entry (void);

/**
 * \brief MiniPort-LED 例程
 */
void demo_am116_core_miniport_led_entry (void);

/**
 * \brief MiniPort-ZLG72128 显示例程
 */
void demo_am116_core_miniport_zlg72128_entry (void);

/**
 * \brief 延时函数例程
 */
void demo_am116_core_std_delay_entry(void);

/**
 * \brief 读CPU卡例程
 */
void demo_am116_core_dr_fm175xx_cpu_card (void);
/**
 * \brief A类卡读卡类型和卡号例程
 */
void demo_am116_core_dr_fm175xx_picca_read_id(void);

/**
 * \brief A类卡读休眠卡例程
 */
void demo_am116_core_dr_fm175xx_picca_halt (void);

/**
 * \brief A类卡读块例程
 */
void demo_am116_core_dr_fm175xx_picca_read_block (void);

/**
 * \brief A类卡值操作例程
 */
void demo_am116_core_dr_fm175xx_picca_val_operate (void);

/**
 * \brief A类卡写块例程
 */
void demo_am116_core_dr_fm175xx_picca_write_block (void);

/**
 * \brief B类卡读卡号例程
 */
void demo_am116_core_dr_fm175xx_piccb_read_id (void);
/**
 * \brief FM175XX LCPD模式 A类卡读卡类型和卡号例程
 */
void demo_am116_core_dr_fm175xx_picca_lpcd_read_id(void);

/**
 * \brief 传感器查询模式例程入口
 */
void demo_am116_core_sensor_hts221_entry (void);

/**
 * \brief 传感器触发模式例程入口
 */
void demo_am116_core_sensor_hts221_int_entry (void);

/**
 * \brief 三轴陀螺仪传感器查询模式例程入口
 */
void demo_am116_core_sensor_bmg160_entry (void);

/**
 * \brief 三轴磁传感器查询模式例程入口
 */
void demo_am116_core_sensor_mmc5883ma_entry (void);

/**
 * \brief 六轴传感器查询模式例程入口
 */
void demo_am116_core_sensor_fxos8700cq_entry (void);

/**
 * \brief 六轴传感器触发模式例程入口
 */
void demo_am116_core_sensor_fxos8700cq_int_entry (void);

/**
 * \brief 气压温度传感器查询模式例程入口
 */
void demo_am116_core_sensor_mpl3115a2_entry (void);

/**
 * \brief 气压温度传感器触发模式例程入口
 */
void demo_am116_core_sensor_mpl3115a2_int_entry (void);

/**
 * \brief 磁力传感器查询模式例程入口
 */
void demo_am116_core_sensor_mag3110_entry (void);

/**
 * \brief 磁力传感器触发模式例程入口
 */
void demo_am116_core_sensor_mag3110_int_entry (void);

/**
 * \brief bootloader demo例程入口
 */
void demo_am116_core_bootloader_entry(void);

/**
 * \brief bootloader 应用程序demo例程入口
 */
void demo_am116_core_application_entry(void);

/**
 * \brief ZSN603 LED测试程序
 */
void demo_am116_zsn603_led_test_entry(void);

/**
 * \brief ZSN603 A类卡测试程序
 */
void demo_am116_zsn603_picca_test_entry(void);

/**
 * \brief ZSN603 B类卡测试程序
 */
void demo_am116_zsn603_piccb_test_entry(void);

/**
 * \brief ZSN603 自动检测测试程序
 */
void demo_am116_zsn603_auto_detect_test_entry(void);

/**
 * \brief MK100 磁旋钮 例程
 */
void demo_am116_core_mk100_test_entry (void);

/**
 * \brief GS200 例程
 */
void demo_am116_core_gs200_set_baudrate_entry (void);

/**
 * \brief RS200 例程
 */
void demo_am116_core_rs200_test_entry (void);

/**
 * \brief USBD键盘例程
 */
void demo_zlg126_usbd_keyboard_entry (void);

/**
 * \brief USBD打印机 例程
 */
void demo_zlg126_usbd_printer_entry (void);

/**
 * \brief USBD U盘 例程
 */
void demo_zlg126_usbd_msc_entry (void);

/**
 * \brief USBD 虚拟串口  例程
 */
void demo_zlg126_usbd_vcom_entry (void);

/**
 * \brief 单区bootloader的测试应用程序例程
 */
void demo_am116_core_single_application_entry (void);

/**
 * \brief 基于串口助手的单区bootloader例程
 */
void demo_am116_core_single_bootloader_uart_entry (void);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_STD_ENTRIES_H */

/* end of file */
