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
 * \brief am824 所有例程函数入口声明
 * \sa demo_am824_core_all_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  lnk, first implementation
 * \endinternal
 */

#ifndef __DEMO_AM824_CORE_ALL_ENTRIES_H
#define __DEMO_AM824_CORE_ALL_ENTRIES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 模拟比较器例程（中断模式），通过 HW 层接口实现
 */
void demo_am824_core_hw_acmp_int_entry (void);

/**
 * \brief 模拟比较器例程（电压阶梯），通过 HW 层接口实现
 */
void demo_am824_core_hw_acmp_lad_entry (void);

/**
 * \brief 模拟比较器例程（查询模式），通过 HW 层接口实现
 */
void demo_am824_core_hw_acmp_poll_entry (void);

/**
 * \brief ADC 例程，转换结果数据通过 DMA 传输，通过 HW 层接口实现
 */
void demo_am824_core_hw_adc_dma_entry (void);

/**
 * \brief ADC 阈值比较例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_adc_thcmp_entry (void);

/**
 * \brief ADC DMA 例程（中断方式），通过标准接口实现
 */
void demo_am824_core_std_adc_dma_int_entry (void);

/**
 * \brief ADC DMA 例程（轮询方式），通过标准接口实现
 */
void demo_am824_core_std_adc_dma_poll_entry (void);

/**
 * \brief ADC 例程（中断方式），通过标准接口实现
 */
void demo_am824_core_std_adc_int_entry (void);

/**
 * \brief ADC 利用板载热敏电阻 (NTC) 进行温度测量例程，通过标准接口实现
 */
void demo_am824_core_std_adc_ntc_entry (void);

/**
 * \brief ADC 例程（轮询方式），通过标准接口实现
 */
void demo_am824_core_std_adc_poll_entry (void);

/**
 * \brief 蜂鸣器例程，通过标准接口实现
 */
void demo_am824_core_std_buzzer_entry (void);

/**
 * \brief CRC 例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_crc_entry (void);

/**
 * \brief CRC例程，通过标准接口实现
 */
void demo_am824_core_std_crc_entry (void);

/**
 * \brief DMA 硬件触发例程，通过驱动层接口实现
 */
void demo_am824_core_drv_dma_hwtrigger_burst_entry (void);

/**
 * \brief 内存到内存的 DMA 传输例程，通过驱动层接口实现
 */
void demo_am824_core_drv_dma_m2m_entry (void);

/**
 * \brief DMA 乒乓传输例程，通过驱动层接口实现
 */
void demo_am824_core_drv_dma_ping_pong_entry (void);

/**
 * \brief 闪存控制器例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_flash_ctrl_entry (void);

/**
 * \brief GPIO 引脚中断例程，通过标准接口实现
 */
void demo_am824_core_hw_gpio_int_entry (void);

/**
 * \brief GPIO 模式匹配引擎演示例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_gpio_pmatch_entry (void);

/**
 * \brief GPIO 例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_gpio_entry (void);

/**
 * \brief GPIO 引脚中断例程，通过标准接口实现
 */
void demo_am824_core_std_gpio_int_entry (void);

/**
 * \brief GPIO 按键例程，通过标准接口实现
 */
void demo_am824_core_std_gpio_key_entry (void);

/**
 * \brief GPIO 例程，通过标准接口实现
 */
void demo_am824_core_std_gpio_entry (void);

/**
 * \brief I2C 读写 EEPROM 例程（DMA 方式），通过 HW 层接口实现
 */
void demo_am824_core_hw_i2c_master_dma_entry (void);

/**
 * \brief I2C 读写 EEPROM 例程（轮询方式），通过 HW 层接口实现
 */
void demo_am824_core_hw_i2c_master_poll_entry (void);

/**
 * \brief I2C 从机例程（DMA 方式），通过 HW 层接口实现
 */
void demo_am824_core_hw_i2c_slave_dma_entry (void);

/**
 * \brief I2C 从机例程（中断方式），通过 HW 层接口实现
 */
void demo_am824_core_hw_i2c_slave_int_entry (void);

/**
 * \brief I2C 从机例程（轮询方式），通过 HW 层接口实现
 */
void demo_am824_core_hw_i2c_slave_poll_entry (void);

/**
 * \brief I2C 主机从 LM75 传感器读取温度值例程，通过标准接口实现
 */
void demo_am824_core_std_i2c_lm75_entry (void);

/**
 * \brief I2C 主机读写 EEPROM 例程，通过异步标准接口实现
 */
void demo_am824_core_std_i2c_master_async_entry (void);

/**
 * \brief I2C 主机读写 EEPROM 例程，通过同步标准接口实现
 */
void demo_am824_core_std_i2c_master_sync_entry (void);

/**
 * \brief IAP 例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_iap_entry (void);

/**
 * \brief 按键输入例程，通过标准接口实现
 */
void demo_am824_core_std_key_input_event_entry (void);

/**
 * \brief LED 例程，通过标准接口实现
 */
void demo_am824_core_std_led_entry (void);

/**
 * \brief 多频率定时器（MRT）例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_mrt_entry (void);

/**
 * \brief 多频率定时器（MRT）例程，通过标准接口实现
 */
void demo_am824_core_std_mrt_timer_entry (void);

/**
 * \brief PLL 例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_pll_entry (void);

/**
 * \brief PMU 深度掉电模式例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_pmu_deeppowerdown_entry (void);

/**
 * \brief PMU 深度睡眠模式例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_pmu_deepsleep_entry (void);

/**
 * \brief PMU 掉电模式例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_pmu_powerdown_entry (void);

/**
 * \brief PMU 睡眠模式例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_pmu_sleep_entry (void);

/**
 * \brief SCT 32 位 PWM 例程，通过驱动层接口实现
 */
void demo_am824_core_drv_sct_timing_pwm_entry (void);

/**
 * \brief SCT 32 位 PWM 例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_sct_1_32bit_pwm_entry (void);

/**
 * \brief SCT 32 位定时例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_sct_1_32bit_timing_entry (void);

/**
 * \brief SCT 作为 2 个 16 位定时器输出 PWM 例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_sct_2_16bit_pwm_entry (void);

/**
 * \brief SCT 作为 2 个 16 位定时器定时例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_sct_2_16bit_timing_entry (void);

/**
 * \brief SCT 32 位捕获例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_sct_cap_entry (void);

/**
 * \brief SCT 操作多状态例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_sct_multi_states_entry (void);

/**
 * \brief SCT 捕获例程，通过标准接口实现
 */
void demo_am824_core_std_sct_cap_entry (void);

/**
 * \brief SCT PWM 例程，通过标准接口实现
 */
void demo_am824_core_std_sct_pwm_entry (void);

/**
 * \brief SCT 定时例程，通过标准接口实现
 */
void demo_am824_core_std_sct_timer_entry (void);

/**
 * \brief SPI 主机发送数据例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_spi_master_entry (void);

/**
 * \brief SPI 从机接收数据例程（DMA 方式），通过 HW 层接口实现
 */
void demo_am824_core_hw_spi_slave_dma_entry (void);

/**
 * \brief SPI 从机接收数据例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_spi_slave_entry (void);

/**
 * \brief SPI 读写 FLASH(MX25L3206E) 例程（DMA 方式），通过标准接口实现
 */
void demo_am824_core_std_spi_flash_dma_entry (void);

/**
 * \brief SPI 读写 FLASH(MX25L3206E) 例程（中断方式），通过标准接口实现
 */
void demo_am824_core_std_spi_flash_int_entry (void);

/**
 * \brief SPI 回环例程（DMA 方式），通过标准接口实现
 */
void demo_am824_core_std_spi_master_dma_entry (void);

/**
 * \brief SPI 回环例程（中断方式），通过标准接口实现
 */
void demo_am824_core_std_spi_master_int_entry (void);

/**
 * \brief 掉电检测例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_bod_entry (void);

/**
 * \brief 时钟频率输出例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_clkout_entry (void);

/**
 * \brief Systick 延时例程，通过标准接口实现
 */
void demo_am824_core_std_delay_entry (void);

/**
 * \brief Systick 例程，通过标准接口实现
 */
void demo_am824_core_std_systick_timer_entry (void);

/**
 * \brief USART 自动匹配波特率例程，通过硬件层接口实现
 */
void demo_am824_core_hw_usart_autobaud_entry (void);

/**
 * \brief USART 例程（轮询方式），通过硬件层接口实现
 */
void demo_am824_core_hw_usart_poll_entry (void);

/**
 * \brief USART 接收数据例程（DMA 方式），通过 HW 层接口实现
 */
void demo_am824_core_hw_usart_rx_dma_entry (void);

/**
 * \brief USART 发送数据例程（DMA 方式），通过 HW 层接口实现
 */
void demo_am824_core_hw_usart_tx_dma_entry (void);

/**
 * \brief USART 流控制例程，通过标准接口实现
 */
void demo_am824_core_std_usart_flowctrl_entry (void);

/**
 * \brief USART 例程（轮询方式），通过标准接口实现
 */
void demo_am824_core_std_usart_poll_entry (void);

/**
 * \brief USART 环形缓冲区例程，通过标准接口实现
 */
void demo_am824_core_std_usart_rngbuf_entry (void);

/**
 * \brief WKT 深度掉电唤醒例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_wkt_deeppowerdown_wakeup_entry (void);

/**
 * \brief WKT 定时器例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_wkt_timer_entry (void);

/**
 * \brief WKT 定时器例程，通过标准接口实现
 */
void demo_am824_core_std_wkt_timer_entry (void);

/**
 * \brief WWDT 例程，通过 HW 层接口实现
 */
void demo_am824_core_hw_wwdt_entry (void);

/**
 * \brief WWDT 例程，通过标准接口实现
 */
void demo_am824_core_std_wwdt_entry (void);

/**
 * \brief MicroPort-DS1302 例程，通过标准接口实现
 */
void demo_am824_core_microport_ds1302_entry (void);

/**
 * \brief MicroPort-EEPROM 例程，通过 NVRAM 标准接口实现
 */
void demo_am824_core_microport_eeprom_nvram_entry (void);

/**
 * \brief MicroPort-EEPROM 例程，通过标准接口实现
 */
void demo_am824_core_microport_eeprom_entry (void);

/**
 * \brief MicroPort-FLASH 例程，通过 FTL 标准接口实现
 */
void demo_am824_core_microport_flash_ftl_entry (void);

/**
 * \brief MicroPort-FLASH 例程，通过 MTD 标准接口实现
 */
void demo_am824_core_microport_flash_mtd_entry (void);

/**
 * \brief MicroPort-FLASH 例程，通过标准接口实现
 */
void demo_am824_core_microport_flash_entry (void);

/**
 * \brief MicroPort-RS485 例程，通过标准接口实现
 */
void demo_am824_core_microport_rs485_entry (void);

/**
 * \brief MicroPort-RTC(PCF85063) 例程，通过标准接口实现
 */
void demo_am824_core_microport_rtc_entry (void);

/**
 * \brief MicroPort-RX8025T 例程，通过标准接口实现
 */
void demo_am824_core_microport_rx8025t_entry (void);

/**
 * \brief MicroPort USB(XR21V141x) 例程，通过标准接口实现
 */
void demo_am824_core_microport_usb_entry (void);

/**
 * \brief MiniPort-View 例程，通过标准接口实现
 */
void demo_am824_core_miniport_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-View + MiniPort-KEY 例程，通过标准接口实现
 */
void demo_am824_core_miniport_hc595_digitron_key_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-View + 板载 LM75 例程，通过标准接口实现
 */
void demo_am824_core_miniport_hc595_digitron_temp_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-View 例程，通过标准接口实现
 */
void demo_am824_core_miniport_hc595_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-LED 例程，通过标准接口实现
 */
void demo_am824_core_miniport_hc595_led_entry (void);

/**
 * \brief MiniPort-KEY + MiniPort-View 例程，通过标准接口实现
 */
void demo_am824_core_miniport_key_digitron_entry (void);

/**
 * \brief MiniPort-KEY 例程，通过标准接口实现
 */
void demo_am824_core_miniport_key_entry (void);

/**
 * \brief MiniPort-LED 例程，通过标准接口实现
 */
void demo_am824_core_miniport_led_entry (void);

/**
 * \brief MiniPort-ZLG72128 例程，通过标准接口实现
 */
void demo_am824_core_miniport_zlg72128_entry (void);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_AM824_CORE_ALL_ENTRIES_H */

/* end of file */
