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
 * \brief HC32L19x 所有例程函数入口声明
 * \sa demo_am118ble_all_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \brief ADC INT 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_adc_int_entry (void);

/**
 * \brief ADC DMA 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_adc_dma_entry (void);

/**
 * \brief 例程入口
 */
void demo_hc32l19x_core_hw_multichan_adc_entry (void);

/**
 * \brief ADC 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_adc_entry (void);

/**
 * \brief ADC 内部温度传感器例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_adc_its_entry (void);

/**
 * \brief 多通道ADC 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_multichan_adc_entry (void);

/**
 * \brief 蜂鸣器例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_buzzer_entry (void);

/**
 * \brief CLK 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_clk_entry (void);

/**
 * \brief CRC 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_hw_crc_entry (void);

/**
 * \brief CRC 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_crc_entry (void);

/**
 * \brief DAC 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_dac_entry (void);

/**
 * \brief DAC 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_dac_entry (void);

/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 */
void demo_hc32l19x_core_drv_dma_m2m_entry (void);

/**
 * \brief FLASH 例程，通过驱动层接口实现
 */
void demo_hc32l19x_core_drv_flash_entry (void);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_gpio_entry (void);

/**
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_gpio_trigger_entry (void);

/**
 * \brief GPIO 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_gpio_entry (void);

/**
 * \brief GPIO 引脚中断例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_gpio_trigger_entry (void);

/**
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_i2c_slave_int_entry (void);

/**
 * \brief I2C 主机访问 EEPROM 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_i2c_master_poll_entry (void);

/**
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过标准接口实现
 */
void demo_hc32l19x_core_std_i2c_slave_int_entry (void);

/**
 * \brief I2C 主机从 LM75 传感器读取温度值例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_i2c_lm75_entry (void);

/**
 * \brief I2C 主机访问 EEPROM 例程，通过异步标准接口实现
 */
void demo_hc32l19x_core_std_i2c_master_async_entry (void);

/**
 * \brief I2C 主机访问 EEPROM 例程，通过同步标准接口实现
 */
void demo_hc32l19x_core_std_i2c_master_sync_entry (void);

/**
 * \brief 按键输入子系统例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_key_input_event_entry (void);

/**
 * \brief LED 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_led_entry (void);

/**
 * \brief RTC 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_rtc_entry (void);

/**
 * \brief RTC 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_rtc_entry (void);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_spi_master_entry (void);

/**
 * \brief SPI 主机 DMA 方式例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_spi_master_dma_entry (void);

/**
 * \brief SPI 主机中断方式例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_spi_master_int_entry (void);

/**
 * \brief 延时例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_delay_entry (void);

/**
 * \brief SYSTICK 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_systick_timer_entry (void);

/**
 * \brief ADTIM定时器软件同步例程，通过HW接口实现
 */
void demo_hc32l19x_core_hw_adtim_swsync_entry(void);

/**
 * \brief ADTIM定时器端口、软件刹车例程，通过HW接口实现
 */
void demo_hc32l19x_core_hw_adtim_port_sw_brake_entry (void);

/**
 * \brief ADTIM定时器VC刹车例程，通过HW接口实现
 */
void demo_hc32l19x_core_hw_adtim_vc_brake_entry (void);

/**
 * \brief 定时器 CAP 捕获例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_tim_cap_entry (void);

/**
 * \brief TIM0 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim0_cap_entry (void);

/**
 * \brief TIM1 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim1_cap_entry (void);

/**
 * \brief TIM2 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim2_cap_entry (void);

/**
 * \brief TIM3 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim3_cap_entry (void);

/**
 * \brief ADTIM4定时器cap例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim4_cap_entry(void);

/**
 * \brief ADTIM5定时器cap例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim5_cap_entry(void);

/**
 * \brief ADTIM6定时器cap例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim6_cap_entry(void);

/**
 * \brief TIM 实现比较匹配翻转，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_tim_cmp_toggle_entry (void);

/**
 * \brief 定时器 PWM 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_tim_pwm_entry (void);

/**
 * \brief 定时器 带死区的互补PWM 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_tim_pwm_dead_entry (void);


/**
 * \brief 通用定时器 端口刹车例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_tim_port_brake_entry (void);

/**
 * \brief 通用定时器 端口刹车例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_tim_vc_brake_entry (void);

/**
 * \brief TIM0 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim0_pwm_entry (void);

/**
 * \brief TIM1 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim1_pwm_entry (void);

/**
 * \brief TIM2 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim2_pwm_entry (void);

/**
 * \brief TIM3 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim3_pwm_entry (void);

/**
 * \brief ADTIM4定时器pwm例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim4_pwm_entry (void);

/**
 * \brief ADTIM5定时器pwm例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim5_pwm_entry (void);

/**
 * \brief ADTIM6定时器pwm例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim6_pwm_entry (void);

/**
 * \brief 定时器 TIMING 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_tim_timing_entry (void);

/**
 * \brief TIM触发ADC采集例程，通过HW接口实现
 */
void demo_hc32l19x_core_hw_tim_trig_adc_sqr_entry (void);

/**
 * \brief TIM0 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim0_timing_entry (void);

/**
 * \brief TIM1 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim1_timing_entry (void);

/**
 * \brief TIM2 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim2_timing_entry (void);

/**
 * \brief TIM3 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim3_timing_entry (void);

/**
 * \brief ADTIM4定时器timing例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim4_timing_entry (void);

/**
 * \brief ADTIM5定时器timing例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim5_timing_entry (void);

/**
 * \brief ADTIM6定时器timing例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_tim6_timing_entry (void);

/**
 * \brief LPTIM0定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_lptim0_timing_entry (void);

/**
 * \brief LPTIM1定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_lptim1_timing_entry (void);

/**
 * \brief LPTIM1定时器和LPTIM2定时器TIMING例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_lptim_timing_gate_entry (void);

/**
 * \brief ADTIM触发ADC采集例程，通过HW接口实现
 */
void demo_hc32l19x_core_hw_adtim_trig_adc_sqr_entry (void);

/**
 * \brief U_ID 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_uid_entry (void);

/**
 * \brief UART 中断例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_uart_int_entry (void);

/**
 * \brief UART 轮询方式例程，通过标准接口实现
 */
void demo_hc32l19x_core_hw_uart_polling_entry (void);

/**
 * \brief UART DMA 接收例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_uart_rx_dma_entry (void);

/**
 * \brief UART DMA 发送例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_uart_tx_dma_entry (void);

/**
 * \brief UART 轮询方式例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_uart_polling_entry (void);

/**
 * \brief UART 环形缓冲区例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_uart_ringbuf_entry (void);

/**
 * \brief LPUART 中断例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_lpuart_int_entry (void);

/**
 * \brief LPUART 轮询方式例程，通过标准接口实现
 */
void demo_hc32l19x_core_hw_lpuart_polling_entry (void);

/**
 * \brief LPUART DMA 接收例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_lpuart_rx_dma_entry (void);

/**
 * \brief LPUART DMA 发送例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_lpuart_tx_dma_entry (void);

/**
 * \brief LPUART 轮询方式例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_lpuart_polling_entry (void);

/**
 * \brief LPUART 环形缓冲区例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_lpuart_ringbuf_entry (void);

/**
 * \brief 低功耗模式（睡眠、深度睡眠）例程，通过驱动层接口实现
 */
void demo_hc32l19x_drv_lpmode_wake_up_entry (void);

/**
 * \brief 低功耗模式（深度睡眠）低功耗串口唤醒例程
 */
void demo_hc32l19x_lpuart_wake_up_entry (void);

/**
 * \brief WWDG 例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_wdt_entry (void);

/**
 * \brief WWDG 例程，通过标准接口实现
 */
void demo_hc32l19x_core_std_wdt_entry (void);

/**
 * \brief MicroPort DS1302 例程，通过标准接口实现
 */
void demo_hc32l19x_core_microport_ds1302_entry (void);

/**
 * \brief MicroPort EEPROM 例程，通过标准接口实现
 */
void demo_hc32l19x_core_microport_eeprom_entry (void);

/**
 * \brief MicroPort EEPROM 例程，通过 NVRAM 接口实现
 */
void demo_hc32l19x_core_microport_eeprom_nvram_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过标准接口实现
 */
void demo_hc32l19x_core_microport_flash_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过 MTD 接口实现
 */
void demo_hc32l19x_core_microport_flash_mtd_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过 FTL 接口实现
 */
void demo_hc32l19x_core_microport_flash_ftl_entry (void);

/**
 * \brief MicroPort RS485 例程，通过标准接口实现
 */
void demo_hc32l19x_core_microport_rs485_entry (void);

/**
 * \brief MicroPort RTC 例程，通过标准接口实现
 */
void demo_hc32l19x_core_microport_rtc_entry (void);

/**
 * \brief MicroPort RX8025T 例程，通过标准接口实现
 */
void demo_hc32l19x_core_microport_rx8025t_entry (void);

/**
 * \brief MicroPort USB 例程，通过标准接口实现
 */
void demo_hc32l19x_core_microport_usb_entry (void);

/**
 * \brief MiniPort-VIEW 例程
 */
void demo_hc32l19x_core_miniport_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW 例程
 */
void demo_hc32l19x_core_miniport_hc595_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-KEY 例程
 */
void demo_hc32l19x_core_miniport_hc595_digitron_key_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW + LM75 例程
 */
void demo_hc32l19x_core_miniport_hc595_digitron_temp_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-LED 例程
 */
void demo_hc32l19x_core_miniport_hc595_led_entry (void);

/**
 * \brief MiniPort-KEY
 */
void demo_hc32l19x_core_miniport_key_entry (void);

/**
 * \brief MiniPort-KEY + MiniPort-DIGITORN显示例程
 */
void demo_hc32l19x_core_miniport_key_digitron_entry (void);

/**
 * \brief MiniPort-LED 例程
 */
void demo_hc32l19x_core_miniport_led_entry (void);

/**
 * \brief MiniPort-ZLG72128显示例程
 */
void demo_hc32l19x_core_miniport_zlg72128_entry (void);

/**
 * \brief gpio 例程
 */
void demo_std_gpio_entry (int input_pin, int output_pin);

/**
 * \brief 读CPU卡例程
 */
void demo_hc32l19x_core_dr_fm175xx_cpu_card (void);
/**
 * \brief A类卡读卡类型和卡号例程
 */
void demo_hc32l19x_core_dr_fm175xx_picca_read_id(void);

/**
 * \brief A类卡读休眠卡例程
 */
void demo_hc32l19x_core_dr_fm175xx_picca_halt (void);

/**
 * \brief A类卡读块例程
 */
void demo_hc32l19x_core_dr_fm175xx_picca_read_block (void);

/**
 * \brief A类卡值操作例程
 */
void demo_hc32l19x_core_dr_fm175xx_picca_val_operate(void);

/**
 * \brief A类卡写块例程
 */
void demo_hc32l19x_core_dr_fm175xx_picca_write_block(void);

/**
 * \brief B类卡读卡号例程
 */
void demo_hc32l19x_core_dr_fm175xx_piccb_read_id(void);
/**
 * \brief FM175XX LCPD模式 A类卡读卡类型和卡号例程
 */
void demo_hc32l19x_core_dr_fm175xx_picca_lpcd_read_id(void);

/**
 * \brief 基于KinetisFlashTool上位机的bootloader 例程
 */
void demo_hc32l19x_core_kft_bootloader_entry (void);

/**
 * \brief 基于KinetisFlashTool上位机的bootloader的测试应用例程
 */
void demo_hc32l19x_core_kft_application_entry (void);

/**
 * \brief uart 双区bootloader的测试应用程序例程
 */

void demo_hc32l19x_core_double_application_entry (void);

/**
 * \brief 单区bootloader的测试应用程序例程
 */

void demo_hc32l19x_core_single_application_entry (void);

/**
 * \brief 基于串口助手的单区bootloader例程
 */
void demo_hc32l19x_core_single_bootloader_uart_entry (void);

/**
 * \brief 基于串口助手的双区bootloader例程
 */
void demo_hc32l19x_core_double_bootloader_uart_entry(void);

/**
 * \brief VC中断触发例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_vc_int_entry (void);

/**
 * \brief LVD系统复位例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_lvd_system_reset_entry (void);

/**
 * \brief LVD 中断例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_lvd_int_entry (void);

/**
 * \brief OPA电压跟随例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_opa_one_entry (void);

/**
 * \brief OPA DAC电压跟随例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_opa_dac_entry (void);

/**
 * \brief AES 加密解密例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_aes_ency_decy_entry (void);

/**
 * \brief TRNG 随机数产生例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_trng_generate_entry (void);

/**
 * \brief LCD显示例程，通过 HW 层接口实现
 */
void demo_hc32l19x_core_hw_lcd_entry (void);

/**
 * \brief PCA 8bit PWM例程,通过驱动层实现
 */
void demo_hc32l19x_core_drv_pca_8bit_pwm_entry (void);

/**
 * \brief  PCA 16bit PWM例程,通过驱动层实现
 */
void demo_hc32l19x_core_drv_pca_16bit_pwm_entry (void);

/**
 * \brief PCNT单通道计数例程,通过驱动层实现
 */
void demo_hc32l19x_core_drv_pcnt_single_entry (void);

/**
 * \brief PCNT 双通道正交脉冲计数例程,通过驱动层实现
 */
void demo_hc32l19x_core_drv_pcnt_double_entry (void);

/**
 * \brief PCNT 双通道非交脉冲计数例程,通过驱动层实现
 */
void demo_hc32l19x_core_drv_pcnt_special_entry (void);

/**
 * \brief modbus 主机通信例程
 */
void demo_hc32l19x_core_mb_master_demo_entry (void);
/**
 * \brief modbus 从站通信例程
 */
void demo_hc32l19x_core_mb_slave_test_entry (void);

/**
 * \brief FLASH 写保护例程
 */
void demo_hc32l19x_core_drv_flash_write_protect_entry (void);
