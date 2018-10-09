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
 * \brief KL26 所有例程函数入口声明
 * \sa    demo_all_entrys.h
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  lnk, first implementation.
 * \endinternal
 */

/**
 * \brief TPM实现标准定时器例程，通过标准接口实现
 * \return 无
 */
void demo_amks16z_core_std_tpm_timing_entry (void);

/**
 * \brief TPM实现PWM标准接口例程，通过标准接口实现
 * \return 无
 */
void demo_amks16z_core_std_tpm_pwm_entry (void);

/**
 * \brief TPM实现捕获功能，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_std_tpm_cap_entry (void);

/**
 * \brief TPM实现标准定时器例程，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_hw_tpm_timing_entry (void);

/**
 * \brief TPM实现PWM功能，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_hw_tpm_pwm_entry (void);

/**
 * \brief TPM实现捕获功能，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_hw_tpm_cap_entry (void);

/**
 * \brief RTC例程，通过标准接口实现
 * \return 无
 */
void demo_amks16z_core_std_rtc_entry (void);

/**
 * \brief PIT 定时器实现(32位)定时功能的例程，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_hw_pit_1_32bit_timing_entry (void);

/**
 * \brief PIT实现标准定时器(32位)例程，通过标准接口实现
 * \return 无
 */
void demo_amks16z_core_std_pit_1_32bit_timing_entry (void);

/**
 * \brief PIT实现标准定时器(64位)例程，通过标准接口实现
 * \return 无
 */
void demo_amks16z_core_std_pit_2_64bit_timing_entry (void);

/**
 * \brief LPTMR实现标准定时器例程，通过标准接口实现
 * \return 无
 */
void demo_amks16z_core_std_lptmr_timing_entry (void);

/**
 * \brief LPTMR实现定时器例程，通过HW层接口实现
 * \return 无
 */
void demo_amks16z_core_hw_lptmr_timing_entry (void);

/**
 * \brief ADC查询获取采样值，使用HW层接口函数
 * \return 无
 */
void demo_amks16z_core_hw_adc_poll_entry (void);

/**
 * \brief 中断方式获取软件触发的ADC转换结果，通过HW层接口实现
 * \return 无
 */
void demo_amks16z_core_hw_adc_int_entry (void);

/**
 * \brief ADC硬件触发转换，DMA传输转换结果，通过标准接口实现
 * \return 无
 */
void demo_amks16z_core_std_adc_ht_entry (void);

/**
 * \brief adc软件触发，DMA传输结果
 * \return 无
 */
void demo_amks16z_core_std_adc_st_entry (void);

/**
 * \brief ADC硬件触发转换，硬件平均转换结果，DMA传输转换结果
 * \return 无
 */
void demo_amks16z_core_std_adc_ht_avg_entry (void);

/**
 * \brief ADC硬件触发比较，DMA传输
 * \return 无
 */
void demo_amks16z_core_std_adc_ht_cmp_entry (void);

/**
 * \brief ADC硬件触发转换，硬件平均比较输出，DMA传输结果
 * \return 无
 */
int demo_amks16z_core_std_adc_ht_avg_cmp_entry (void);

/**
 * \brief ADC差分输入，DMA传输，软件触发
 * \return 无
 */
void demo_amks16z_core_std_adc_dif_st_entry (void);

/**
 * \brief adc软件触发，DMA传输结果，一次采样两个ADC描述符
 * \return 无
 */
void demo_amks16z_core_std_adc_descs_st_entry(void);

/**
 * \brief DMA内存到内存
 * \return 无
 */
void demo_amks16z_core_dr_dma_m2m_entry (void);

/**
 * \brief CMP比较器HW层中断例程
 * \return 无
 */
void demo_amks16z_core_hw_cmp_int_entry (void);

/**
 * \brief FTFA例程,通过驱动层的接口实现
 * \return 无
 */
void demo_amks16z_core_dr_ftfa_entry (void);

/**
 * \brief  GPIO硬件HW层例程
 * \return 无
 */
void demo_amks16z_core_hw_gpio_entry (void);

/**
 * \brief  FGPIO硬件HW层例程
 * \return 无
 */
void demo_amks16z_core_hw_fgpio_entry (void);

/**
 * \brief  GPIO硬件HW层中断例程
 * \return 无
 */
void demo_amks16z_core_hw_gpio_int_entry (void);

/**
 * \brief  GPIO标准例程
 * \return 无
 */
void demo_amks16z_core_std_gpio_entry (void);

/**
 * \brief  GPIO标准中断触发例程
 * \return 无
 */
void demo_amks16z_core_std_gpio_trigger_entry (void);

/**
 * \brief  串口硬件HW层查询收发例程
 * \return 无
 */
void demo_amks16z_core_hw_uart_polling_entry (void);

/**
 * \brief  串口硬件HW层中断收发例程
 * \retval 无
 */
void demo_amks16z_core_hw_uart_int_entry (void);

/**
 * \brief  串口标准接口查询收发例程
 * \return 无
 */
void demo_amks16z_core_std_uart_polling_entry (void);

/**
 * \brief  串口环形缓冲区例程
 * \return 无
 */
void demo_amks16z_core_std_uart_ringbuf_entry (void);

/**
 * \brief  串口接收使用DMA例程
 * \return 无
 */
void demo_amks16z_core_hw_uart_rx_dma_entry (void);

/**
 * \brief  LED例程
 * \return 无
 */
void demo_amks16z_core_std_led_entry (void);

/**
 * \brief i2c读写AM75示例，通过标准接口实现
 * \return 无
 */
void demo_amks16z_core_std_i2c_lm75_entry (void);
/**
 * \brief i2c读写AM75示例，通过hw层接口实现
 * \return 无
 */
void demo_amks16z_core_hw_i2c_master_poll_entry (void);

/**
 * \brief flash hw层例程
 * \return 无
 */
void demo_amks16z_core_hw_ftfa_entry (void);

/**
 * \brief flash 驱动层例程
 * \return 无
 */
void demo_amks16z_core_dr_ftfa_entry (void);

/**
 * \brief DAC输出电压值，调用HW层实现
 * \return 无
 */
void demo_amks16z_core_hw_dac_entry (void);

/**
 * \brief DAC 使用缓冲区中断输出正弦波信号，HW层实现
 * \return 无
 */
void demo_amks16z_core_hw_dac_buf_int_entry (void);

/**
 * \brief DAC输出电压，标准接口层实现
 * \return 无
 */
void demo_amks16z_core_std_dac_entry (void);

/**
 * \brief DAC输出电压，DMA传输数据，HW层接口实现
 * \return 无
 */
void demo_amks16z_core_hw_dac_dma_st_entry (void);

/**
 * \brief  TSI驱动层中断例程
 * \return 无
 */
void demo_amks16z_core_dr_tsi_int_entry (void);

/**
 * \brief  TSI HW层中断例程
 * \return 无
 */
void demo_amks16z_core_hw_tsi_int_entry (void);

/**
 * \brief  TSI HW层例程
 * \return 无
 */
void demo_amks16z_core_hw_tsi_entry (void);


/**
 * \brief  BME HW层封装式存储指令(gpio)例程
 * \return 无
 */
void demo_amks16z_core_hw_bme_deco_stores_gpio_entry (void);

/**
 * \brief  BME HW层封装式存储指令例程
 * \return 无
 */
void demo_amks16z_core_hw_bme_deco_stores_entry (void);

/**
 * \brief  BME HW层封装式载入指令例程
 * \return 无
 */
void demo_amks16z_core_hw_bme_deco_loads_entry (void);


/**
 * \brief i2s数据发送，调用HW层接口函数实现
 * \return 无
 */
void demo_amks16z_core_hw_i2s_polling_entry (void);

/**
 * \brief i2s中断数据发送，调用HW层接口函数实现
 * \return 无
 */
void demo_amks16z_core_hw_i2s_int_entry (void);

/**
 * \brief i2s dma方式数据发送，调用HW层接口函数实现
 * \return 无
 */
void demo_amks16z_core_hw_i2s_dma_entry (void);

/**
* \brief buzzer示例，通过标准接口实现
* \return 无
*/
void demo_amks16z_core_std_buzzer_entry (void);

/**
* \brief SPI示例，通过标准接口实现(INT方式)
* \return 无
*/
void demo_amks16z_core_std_spi_master_int_entry (void);

/**
* \brief SPI示例，通过标准接口实现(DMA方式)
* \return 无
*/
void demo_amks16z_core_std_spi_master_dma_entry (void);

/**
* \brief SPI操作FLASH示例，通过标准接口实现(INT方式)
* \return 无
*/
void demo_amks16z_core_std_spi_flash_int_entry (void);

/**
* \brief SPI操作FLASH示例，通过标准接口实现(DMA方式)
* \return 无
*/
void demo_amks16z_core_std_spi_flash_dma_entry (void);

/**
 * \brief WDT入口函数，HW层实现
 * \return 无
 */
void demo_amks16z_core_hw_wdt_entry (void);

/**
 * \brief 看门狗入口函数，标准接口层实现
 *
 * \note 测试看门狗例程时请将am_prj_config.h文件中的宏AM_CFG_SIM_COP置为1
 *
 * \return 无
 */
void demo_amks16z_core_std_wdt_entry (void);

/**
 * \brief PMU示例，HW层实现
 *
 * \note 测试看门狗例程时请将am_prj_config.h文件中的宏AM_CFG_SIM_COP置为1
 *
 * \return 无
 */
void demo_amks16z_core_std_pmu_lowpower_entry (void);

/**
 * \brief SPI从机演示例程，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_hw_spi_slave_entry (void);

/**
 * \brief SPI从机匹配中断演示例程，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_hw_spi_slave_match_entry (void);

/**
 * \brief SPI从机通过DMA控制器接收数据，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_hw_spi_slave_dma_entry (void);

/**
 * \brief SPI主机演示例程，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_hw_spi_master_entry (void);

/**
 * \brief SPI从机演示例程，通过HW层的接口实现
 * \return 无
 */
void demo_amks16z_core_hw_spi_slave_irq_entry (void);

/**
 * \brief  串口发送使用DMA例程
 * \return 无
 */
void demo_amks16z_core_hw_uart_tx_dma_entry (void);

/**
 * \brief UID例程入口
 */
void demo_amks16z_core_hw_sim_uid_entry (void);

/**
 * \brief 后面安全密钥例程入口
 */
void demo_amks16z_core_hw_backdoor_sec_entry (void);

/**
 * \brief AMetal 应用程序入口
 */
void demo_amks16z_core_hw_rtc_sec_int_entry (void);

/**
 * \brief MiniPort-View + MiniPort-595 温度显示例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_hc595_digitron_temp_interface_entry (void);

/**
 * \brief MiniPort-LED 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_led_entry (void);

/**
 * \brief MiniPort-VIEW 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-LED 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_hc595_led_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_hc595_digitron_entry (void);

/**
 * \brief MiniPort-KEY
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_key_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-KEY 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_hc595_digitron_key_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW + LM75 温度显示例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_hc595_digitron_temp_entry (void);

/**
 * \brief MiniPort-ZLG72128显示例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_zlg72128_entry (void);

/**
 * \brief MiniPort-KEY + MiniPort-DIGITORN显示例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_miniport_key_digitron_entry (void);

/**
 * \brief MicroPort DS1302 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_ds1302_entry (void);

/**
 * \brief MicroPort EEPROM 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_eeprom_entry (void);

/**
 * \brief MicroPort EEPROM 例程，通过 NVRAM 接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_eeprom_nvram_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_flash_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过 MTD 接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_flash_mtd_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过 FTL 接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_flash_ftl_entry (void);

/**
 * \brief MicroPort RS485 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_rs485_entry (void);

/**
 * \brief MicroPort RTC 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_rtc_entry (void);

/**
 * \brief MicroPort RX8025T 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_rx8025t_entry (void);

/**
 * \brief MicroPort USB 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_usb_entry (void);


/* end of file */
