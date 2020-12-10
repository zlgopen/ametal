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
 * \brief hc32f460 所有例程函数入口声明
 * \sa demo_amhc32f460_all_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 20-01-16  cds, first implementation
 * \endinternal
 */


/**
 * \brief ADC INT 例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_adc_int_entry (void);

/**
 * \brief ADC DMA 例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_adc_dma_entry (void);

/**
 * \brief ADC 多通道转换例程， 通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_multichan_adc_entry (void);

/**
 * \brief ADC 多通道连续扫描例程， 通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_multichan_seq_adc_entry (void);

/**
 * \brief ADC 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_adc_entry (void);

/**
 * \brief 多通道ADC 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_multichan_adc_entry (void);

/**
 * \brief ADC 序列A事件触发例程，通过HW接口实现
 */
void demo_hc32f460_core_hw_adc_seq_a_event_trigger_entry(void);

/**
 * \brief ADC 序列A、B双序列事件触发例程，通过HW接口实现
 */
void demo_hc32f460_core_hw_adc_seq_a_b_event_trigger_entry(void);

/**
 * \brief ADC 可编程增益放大器PGA例程 通过HW接口实现
 */
void demo_hc32f460_core_hw_adc_pga_entry(void);

/**
 * \brief ADC 模拟看门狗AWD例程 通过HW接口实现
 */
void demo_hc32f460_core_hw_adc_awd_entry(void);

/**
 * \brief 多ADC 协同工作例程 通过HW接口实现
 */
void demo_hc32f460_core_hw_adc_sync_entry(void);

/**
 * \brief 蜂鸣器例程，通过标准接口实现
 */
void demo_hc32f460_core_std_buzzer_entry (void);

/**
 * \brief CLK 例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_clk_entry (void);

/**
 * \brief CRC 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_crc_entry (void);

/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 */
void demo_hc32f460_core_drv_dma_m2m_entry (void);

/**
 * \brief DMA 内存到内存连锁传输例程，通过驱动层接口实现
 */
void demo_hc32f460_core_drv_dma_m2m_chain_entry(void);

/**
 * \brief FLASH 例程，通过驱动层接口实现
 */
void demo_hc32f460_core_drv_flash_entry (void);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_gpio_entry (void);

/**
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_gpio_trigger_entry (void);

/**
 * \brief GPIO 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_gpio_entry (void);

/**
 * \brief GPIO 引脚中断例程，通过标准接口实现
 */
void demo_hc32f460_core_std_gpio_trigger_entry (void);

/**
 * \brief LED 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_led_entry (void);

/**
 * \brief RTC 例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_rtc_sec_int_entry (void);

/**
 * \brief RTC 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_rtc_entry (void);

/**
 * \brief QSPI 例程，通过驱动层接口实现
 */
void demo_hc32f460_core_drv_qspi_entry (void);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_spi_master_entry (void);

/**
 * \brief SPI 主机 DMA 方式例程，通过标准接口实现
 */
void demo_hc32f460_core_std_spi_master_dma_entry (void);

/**
 * \brief SPI 主机中断方式例程，通过标准接口实现
 */
void demo_hc32f460_core_std_spi_master_int_entry (void);

/**
 * \brief SPI 主机轮询方式例程，通过标准接口实现
 */
void demo_hc32f460_core_std_spi_master_poll_entry (void);

/**
 * \brief SPI 从机DMA方式例程，通过标准接口实现
 */
void demo_hc32f460_core_std_spi_slaver_dma_entry (void);

/**
 * \brief UART 中断例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_uart_int_entry (void);

/**
 * \brief UART 轮询方式例程，通过标准接口实现
 */
void demo_hc32f460_core_hw_uart_polling_entry (void);

/**
 * \brief UART DMA 接收例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_uart_rx_dma_entry (void);

/**
 * \brief UART DMA 发送例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_uart_tx_dma_entry (void);

/**
 * \brief UART 轮询方式例程，通过标准接口实现
 */
void demo_hc32f460_core_std_uart_polling_entry (void);

/**
 * \brief UART 环形缓冲区例程，通过标准接口实现
 */
void demo_hc32f460_core_std_uart_ringbuf_entry (void);

/**
 * \brief WWDG 例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_wdt_entry (void);
/**
 * \brief WWDG 中断例程，通过 驱动 层接口实现
 */
void demo_hc32f460_core_drv_wdt_int_entry (void);

/**
 * \brief WWDG 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_wdt_entry (void);

/**
 * \brief gpio 例程， 通过标准接口实现
 */
void demo_std_gpio_entry (int input_pin, int output_pin);

/**
 * \brief TIM01 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim01_timing_entry(void);

/**
 * \brief TIM02 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim02_timing_entry(void);

/**
 * \brief TIMA1 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea1_timing_entry (void);

/**
 * \brief TIMA2 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea2_timing_entry (void);

/**
 * \brief TIMA3 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea3_timing_entry (void);

/**
 * \brief TIMA4 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea4_timing_entry (void);

/**
 * \brief TIMA5 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea5_timing_entry (void);

/**
 * \brief TIMA6 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea6_timing_entry (void);

/**
 * \brief TIM41 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim41_timing_entry (void);

/**
 * \brief TIM42 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim42_timing_entry (void);

/**
 * \brief TIM43 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim43_timing_entry (void);

/**
 * \brief TIM61 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim61_timing_entry (void);

/**
 * \brief TIM62 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim62_timing_entry (void);

/**
 * \brief TIM63 定时器 TIMING 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim63_timing_entry (void);

/**
 * \brief TIMERA1 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea1_pwm_entry(void);

/**
 * \brief TIMERA2 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea2_pwm_entry(void);

/**
 * \brief TIMERA3 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea3_pwm_entry(void);

/**
 * \brief TIMERA4 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea4_pwm_entry(void);

/**
 * \brief TIMERA5 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea5_pwm_entry(void);

/**
 * \brief TIMERA6 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea6_pwm_entry(void);

/**
 * \brief TIM41 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim41_pwm_entry(void);

/**
 * \brief TIM42 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim42_pwm_entry(void);

/**
 * \brief TIM43 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_tim43_pwm_entry(void);

/**
 * \brief ADTIMER61 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_adtim61_pwm_entry(void);

/**
 * \brief ADTIMER62 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_adtim62_pwm_entry(void);

/**
 * \brief ADTIMER63 定时器 PWM 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_adtim63_pwm_entry(void);

/**
 * \brief TIMA1 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea1_cap_entry(void);

/**
 * \brief TIMA2 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea2_cap_entry(void);

/**
 * \brief TIMA3 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea3_cap_entry(void);

/**
 * \brief TIMA4 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea4_cap_entry(void);

/**
 * \brief TIMA5 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea5_cap_entry(void);

/**
 * \brief TIMA6 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_timea6_cap_entry(void);

/**
 * \brief ADTIMER61 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_adtim61_cap_entry(void);

/**
 * \brief ADTIMER62 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_adtim62_cap_entry(void);

/**
 * \brief ADTIMER63 定时器 CAP 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_adtim63_cap_entry(void);

/**
 * \brief I2C 主机访问 EEPROM 例程，通过同步标准接口实现
 */
void demo_hc32f460_core_std_i2c_master_sync_entry (void);

/**
 * \brief I2C 主机访问 EEPROM 例程，通过异步标准接口实现
 */
void demo_hc32f460_core_std_i2c_master_async_entry (void);

/**
 * \brief I2C 主机轮询例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_i2c_master_poll_entry (void);

/**
 * \brief I2C 从机轮询例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_i2c_slave_poll_entry (void);

/**
 * \brief RTC 例程，通过标准接口和中间驱动层函数实现
 */
void demo_hc32f460_core_std_rtc_entry (void);

/**
 * \brief CAN 例程，通过标准接口实现
 */
void demo_hc32f460_core_std_can_entry (void);
/**
 * \brief USB msc例程，通过标准接口实现
 */
void demo_hc32f460_usbd_msc_entry (void);
/**
 * \brief USB mouse例程，通过标准接口实现
 */
void demo_hc32f460_usbd_mouse_entry (void);

/**
 * \brief USB hid custom例程，通过标准接口实现
 */
void demo_hc32f460_usbd_hid_custom_entry (void);

/**
 * \brief SD卡多块读写例程，通过标准层接口实现
 */
void demo_hc32f460_core_std_sdcard_entry (void);

/**
 * \brief 专用看门狗 中断例程，通过 驱动 层接口实现
 */
void demo_hc32f460_core_drv_swdt_int_entry (void);

/**
 * \brief MPU 内存保护例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_mpu_entry (void);

/**
 * \brief HASH 消息摘要计算例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_hash_entry (void);

/**
 * \brief HASH 消息摘要计算例程，通过 DRV 层接口实现
 */
void demo_hc32f460_core_drv_hash_entry(void);

/**
 * \brief TRNG 随机数产生例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_trng_generate_entry (void);

/**
 * \brief TRNG 随机数产生例程，通过 DRV 层接口实现
 */
void demo_hc32f460_core_drv_trng_entry(void);

/**
 * \brief AES 加密解密例程，通过 HW 层接口实现
 */
void demo_hc32f460_core_hw_aes_ency_decy_entry (void);

/**
 * \brief AES 加密解密例程，通过 DRV 层接口实现
 */
void demo_hc32f460_core_drv_aes_entry (void);

/**
 * \brief DCU 数据计算例程，通过HW接口实现
 */
void demo_hc32f460_core_hw_dcu_entry (void);

/**
 * \brief KEY例程，通过标准接口实现
 */
void demo_hc32f460_core_std_key_input_event_entry (void);

/**
 * \brief 单区bootloader例程入口
 */
void demo_hc32f460_core_single_bootloader_uart_entry(void);

/**
 * \brief 双区bootloader例程入口
 */
void demo_hc32f460_core_double_bootloader_uart_entry(void);

/**
 * \brief 单区APP例程入口
 */
void demo_hc32f460_core_single_application_entry (void);

/**
 * \brief 双区APP例程入口
 */
void demo_hc32f460_core_double_application_entry (void);

/**
 * \brief flash写保护
 */
void demo_hc32f460_core_drv_flash_write_protect_entry (void);
