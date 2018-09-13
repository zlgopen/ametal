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
 * \brief ZLG116 �������̺����������
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
 * \brief ADC INT ���̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_adc_int_entry (void);

/**
 * \brief ADC ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_adc_entry (void);

/**
 * \brief ADC ���ð��ӵ���������(NTC)��������¶����̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_adc_ntc_entry (void);

/**
 * \brief ���������̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_buzzer_entry (void);

/**
 * \brief CLK ���̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_clk_entry (void);

/**
 * \brief DMA �ڴ浽�ڴ����̣�ͨ��������ӿ�ʵ��
 */
void demo_am116_core_drv_dma_m2m_entry (void);

/**
 * \brief FLASH ���̣�ͨ��������ӿ�ʵ��
 */
void demo_am116_core_drv_flash_entry (void);

/**
 * \brief GPIO ���̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_gpio_entry (void);

/**
 * \brief GPIO �����ж����̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_gpio_trigger_entry (void);

/**
 * \brief GPIO ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_gpio_entry (void);

/**
 * \brief GPIO �����ж����̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_gpio_trigger_entry (void);

/**
 * \brief I2C ��ѯģʽ�²��� EEPROM ���̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_i2c_master_poll_entry (void);

/**
 * \brief I2C �ӻ�����(�����̿�������ģ�� EEPROM)��ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_i2c_slave_poll_entry (void);

/**
 * \brief I2C ������ LM75 ��������ȡ�¶�ֵ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_i2c_lm75_entry (void);

/**
 * \brief I2C �������� EEPROM ���̣�ͨ���첽��׼�ӿ�ʵ��
 */
void demo_am116_core_std_i2c_master_async_entry (void);

/**
 * \brief I2C �������� EEPROM ���̣�ͨ��ͬ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_i2c_master_sync_entry (void);

/**
 * \brief I2C �ӻ����̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_i2c_slave_entry (void);

/**
 * \brief I2C �ӻ�ģ�� TMP100 ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_i2c_slave_subaddr_entry (void);

/**
 * \brief ������ϵͳ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_key_entry (void);

/**
 * \brief LED ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_led_entry (void);

/**
 * \brief �����ʱ�����̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_softimer_entry (void);

/**
 * \brief SPI �������̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_spi_master_entry (void);

/**
 * \brief SPI ���� DMA ��ʽ�� FLASH ͨ�����̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_spi_flash_dma_entry (void);

/**
 * \brief SPI �����жϷ�ʽ�� FLASH ͨ�����̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_spi_flash_int_entry (void);

/**
 * \brief SPI ���� DMA ��ʽ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_spi_master_dma_entry (void);

/**
 * \brief SPI �����жϷ�ʽ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_spi_master_int_entry (void);

/**
 * \brief SPI �ӻ� DMA ��ʽ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_spi_slave_dma_entry (void);

/**
 * \brief ��ʱ�� CAP �������̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_tim_cap_entry (void);

/**
 * \brief ��ʱ��ͨ���Ƚ�ƥ�����ŷ�ת���̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_tim_cmp_toggle_entry (void);

/**
 * \brief ��ʱ�� PWM ������̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_tim_pwm_entry (void);

/**
 * \brief ��ʱ��������ʱ��Ļ��� PWM ������̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_tim_pwm_dead_entry (void);

/**
 * \brief ��ʱ�� TIMING ���̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_tim_timing_entry (void);

/**
 * \brief TIM14 ��ʱ�� CAP ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim14_cap_entry (void);

/**
 * \brief TIM14 ��ʱ�� PWM ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim14_pwm_entry (void);

/**
 * \brief TIM14 ��ʱ�� TIMING ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim14_timing_entry (void);

/**
 * \brief TIM16 ��ʱ�� CAP ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim16_cap_entry (void);

/**
 * \brief TIM16 ��ʱ�� PWM ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim16_pwm_entry (void);

/**
 * \brief TIM16 ��ʱ�� TIMING ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim16_timing_entry (void);

/**
 * \brief TIM17 ��ʱ�� CAP ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim17_cap_entry (void);

/**
 * \brief TIM17 ��ʱ�� PWM ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim17_pwm_entry (void);

/**
 * \brief TIM17 ��ʱ�� TIMING ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim17_timing_entry (void);

/**
 * \brief TIM1 ��ʱ�� CAP ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim1_cap_entry (void);

/**
 * \brief TIM1 ��ʱ�� PWM ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim1_pwm_entry (void);

/**
 * \brief TIM1 ��ʱ�� TIMING ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim1_timing_entry (void);

/**
 * \brief TIM2 ��ʱ�� CAP ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim2_cap_entry (void);

/**
 * \brief TIM2 ��ʱ�� PWM ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim2_pwm_entry (void);

/**
 * \brief TIM2 ��ʱ�� TIMING ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim2_timing_entry (void);

/**
 * \brief TIM3 ��ʱ�� CAP ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim3_cap_entry (void);

/**
 * \brief TIM3 ��ʱ�� PWM ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim3_pwm_entry (void);

/**
 * \brief TIM3 ��ʱ�� TIMING ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_tim3_timing_entry (void);

/**
 * \brief UART �жϷ������̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_uart_int_entry (void);

/**
 * \brief UART ��ѯ��ʽ���̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_uart_polling_entry (void);

/**
 * \brief UART DMA �������̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_uart_rx_dma_entry (void);

/**
 * \brief UART DMA �������̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_uart_tx_dma_entry (void);

/**
 * \brief UART ��ѯ��ʽ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_uart_polling_entry (void);

/**
 * \brief UART ���λ��������̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_uart_ringbuf_entry (void);

/**
 * \brief ˯��ģʽ���̣�ʹ�ö�ʱ�����ڻ��ѣ�ͨ��������ӿ�ʵ��
 */
void demo_am116_core_drv_sleepmode_timer_wake_up_entry (void);

/**
 * \brief ˯��ģʽ���̣�ͨ��������ӿ�ʵ��
 */
void demo_am116_core_drv_sleepmode_wake_up_entry (void);

/**
 * \brief ����ģʽ���̣�ͨ��������ӿ�ʵ��
 */
void demo_am116_core_drv_standbymode_wake_up_entry (void);

/**
 * \brief ֹͣģʽ���̣�ͨ��������ӿ�ʵ��
 */
void demo_am116_core_drv_stopmode_wake_up_entry (void);

/**
 * \brief IWDG ���̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_iwdg_entry (void);

/**
 * \brief WWDG ���̣�ͨ�� HW ��ӿ�ʵ��
 */
void demo_am116_core_hw_wwdg_entry (void);

/**
 * \brief IWDG ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_iwdg_entry (void);

/**
 * \brief WWDG ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_std_wwdg_entry (void);

/**
 * \brief MicroPort DS1302 ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_microport_ds1302_entry (void);

/**
 * \brief MicroPort EEPROM ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_microport_eeprom_entry (void);

/**
 * \brief MicroPort EEPROM ���̣�ͨ�� NVRAM �ӿ�ʵ��
 */
void demo_am116_core_microport_eeprom_nvram_entry (void);

/**
 * \brief MicroPort FLASH ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_microport_flash_entry (void);

/**
 * \brief MicroPort FLASH ���̣�ͨ�� FTL �ӿ�ʵ��
 */
void demo_am116_core_microport_flash_ftl_entry (void);

/**
 * \brief MicroPort FLASH ���̣�ͨ�� MTD �ӿ�ʵ��
 */
void demo_am116_core_microport_flash_mtd_entry (void);

/**
 * \brief MicroPort RS485 ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_microport_rs485_entry (void);

/**
 * \brief MicroPort RTC ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_microport_rtc_entry (void);

/**
 * \brief MicroPort RX8025T ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_microport_rx8025t_entry (void);

/**
 * \brief MicroPort USB ���̣�ͨ����׼�ӿ�ʵ��
 */
void demo_am116_core_microport_usb_entry (void);

/**
 * \brief MiniPort-VIEW ����
 */
void demo_am116_core_miniport_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW ����
 */
void demo_am116_core_miniport_hc595_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW + MiniPort-KEY ����
 */
void demo_am116_core_miniport_hc595_digitron_key_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW + LM75 �¶���ʾ����
 */
void demo_am116_core_miniport_hc595_digitron_temp_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-LED ����
 */
void demo_am116_core_miniport_hc595_led_entry (void);

/**
 * \brief MiniPort-KEY ����
 */
void demo_am116_core_miniport_key_entry (void);

/**
 * \brief MiniPort-KEY + MiniPort-DIGITORN��ʾ����
 */
void demo_am116_core_miniport_key_digitron_entry (void);

/**
 * \brief MiniPort-LED ����
 */
void demo_am116_core_miniport_led_entry (void);

/**
 * \brief MiniPort-ZLG72128 ��ʾ����
 */
void demo_am116_core_miniport_zlg72128_entry (void);

/**
 * \brief ��ʱ��������
 */
void demo_am116_core_std_delay_entry(void);

/**
 * \brief A�࿨�������ͺͿ�������
 */
void demo_am116_core_dr_fm175xx_picca_read_id(void);

/**
 * \brief FM175XX LCPDģʽ A�࿨�������ͺͿ�������
 */
void demo_am116_core_dr_fm175xx_picca_lpcd_read_id(void);

/**
 * \brief BMA253������ٶ�����
 */
void demo_bma253_entry (void);

/**
 * \brief LTR_553ALS���մ���������
 */
void demo_ltr_553als_entry (void);

/**
 * \brief BMP280ѹ������������
 */
void demo_bmp280_entry (void);

/**
 * \brief BME280ѹ����ʪ�ȴ���������
 */
void demo_bme280_entry (void);

/**
 * \brief LPS22HBѹ������������
 */
void demo_lps22hb_entry (void);

/**
 * \brief  HTS221��ʪ�ȴ���������
 */
void demo_hts221_entry (void);

/**
 * \brief BH1730FVC�⴫��������
 */
void demo_bh1730fvc_entry (void);

/**
 * \brief LIS3MDL����Ŵ���������
 */
void demo_lis3mdl_entry (void);

/**
 * \brief SHTC1��ʪ�ȴ���������
 */
void demo_shtc1_entry (void);

/**
 * \brief BMG160���������Ǵ���������
 */
void demo_bmg160_entry (void);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_STD_ENTRIES_H */

/* end of file */
