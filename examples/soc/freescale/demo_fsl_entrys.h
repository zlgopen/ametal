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
 * \brief fsl系列外设驱动的所有例程函数入口声明
 * \sa    demo_fsl_entrys.h
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  lnk, first implementation.
 * \endinternal
 */

#include "hw/amhw_fsl_adc.h"
#include "hw/amhw_fsl_bme.h"
#include "hw/amhw_fsl_cmp.h"
#include "hw/amhw_fsl_dac.h"
#include "hw/amhw_kl26_dma.h"
#include "hw/amhw_fsl_ftfa.h"
#include "hw/amhw_fsl_i2c.h"
#include "hw/amhw_fsl_i2s.h"
#include "hw/amhw_fsl_llwu.h"
#include "hw/amhw_fsl_lptmr.h"
#include "hw/amhw_kl26_mcm.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "hw/amhw_fsl_pit.h"
#include "hw/amhw_fsl_pmc.h"
#include "hw/amhw_fsl_rcm.h"
#include "hw/amhw_fsl_rtc.h"
#include "hw/amhw_fsl_smc.h"
#include "hw/amhw_fsl_spi.h"
#include "hw/amhw_fsl_tpm.h"
#include "hw/amhw_fsl_tsi.h"
#include "hw/amhw_fsl_uart.h"

#include "am_dac.h"
#include "am_adc.h"
#include "am_i2c.h"
#include "am_spi.h"
#include "am_pwm.h"

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num   : 中断号
 * \param[in] chan      : 引脚对应的 ADC 通道号
 * \param[in] clk_bus   : 总线时钟
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_fsl_hw_adc_int_entry (amhw_fsl_adc_t     *p_hw_adc,
                                int                 int_num,
                                amhw_fsl_adc_chan_t chan,
                                uint32_t            clk_bus,
                                uint32_t            vref_mv);

/**
 * \brief ADC查询获取采样值，使用HW层接口函数
 *
 * \param[in] p_hw_adc  : 指向 ADC 外设寄存器块的指针
 * \param[in] int_num   : 中断号
 * \param[in] chan      : 引脚对应的 ADC 通道号
 * \param[in] clk_bus   : 总线时钟
 * \param[in] vref_mv   : ADC参考电压，用以将ADC采样结果转换为电压值显示
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_fsl_hw_adc_poll_entry (amhw_fsl_adc_t     *p_hw_adc,
                                 amhw_fsl_adc_chan_t chan,
                                 uint32_t            clk_bus,
                                 uint32_t            vref_mv);

/**
 * \brief adc软件触发，DMA传输结果，一次采样两个ADC描述符
 *
 * \param[in] adc0_handle  : ADC句柄
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_fsl_std_adc_descs_st_entry(am_adc_handle_t adc0_handle);

/**
 * \brief ADC差分输入，DMA传输，软件触发
 *
 * \param[in] adc0_handle  : ADC句柄
 *
 * \note 一般情况下，在使用本 demo 前，还需打开 ADC 模块相应的时钟，
 * 配置 ADC 通道对应的引脚，这些配置都与具体芯片相关。
 *
 * \return 无
 */
void demo_fsl_std_adc_dif_st_entry (am_adc_handle_t adc0_handle);

/**
 * \brief  BME HW层封装式载入指令例程
 * \return 无
 */
void demo_kl26_hw_bme_deco_loads_entry (void);

/**
 * \brief  BME HW层封装式存储指令(gpio)例程
 * \return 无
 */
void demo_kl26_hw_bme_deco_stores_gpio_entry (void);

/**
 * \brief  BME HW层封装式存储指令例程
 * \return 无
 */
void demo_kl26_hw_bme_deco_stores_entry (void);

/**
 * \brief CMP比较器HW层中断例程
 *
 * \param[in] p_hw_cmp  : 指向 cmp 寄存器块的指针
 * \param[in] inum      : 中断号
 *
 * \return 无
 */
void demo_fsl_hw_cmp_int_entry (amhw_fsl_cmp_t *p_hw_cmp,
                                int             inum);

/**
 * \brief DAC 使用缓冲区中断输出正弦波信号，HW层实现
 *
 * \param[in] p_hw_dac  : 指向 DAC 寄存器块的指针
 * \param[in] inum      : 中断号
 *
 * \return 无
 */
void demo_fsl_hw_dac_buf_int_entry (amhw_fsl_dac_t *p_hw_dac,
                                    int             inum);

/**
 * \brief DAC输出电压，DMA传输数据，HW层接口实现
 * \return 无
 */
void demo_kl26_hw_dac_dma_st_entry (void);

/**
 * \brief DAC输出电压值，调用HW层实现
 *
 * \param[in] p_hw_dac  : 指向 DAC 寄存器块的指针
 * \param[in] voltage_mv: 使DAC输出的电压值(单位:mv)
 *
 * \return 无
 */
void demo_fsl_hw_dac_entry (amhw_fsl_dac_t *p_hw_dac,
                            uint16_t        mv_out);

/**
 * \brief DAC输出电压，标准接口层实现
 *
 * \param[in] p_hw_dac  : 指向 DAC 寄存器块的指针
 * \param[in] voltage_mv: 使DAC输出的电压值(单位:mv)
 *
 * \return 无
 */
void demo_fsl_std_dac_entry (am_dac_handle_t dac0_handle,
                             uint32_t        mv_out);

/**
 * \brief DMA内存到内存
 * \return 无
 */
void demo_kl26_dr_dma_m2m_entry (void);

/**
 * \brief FTFA例程,通过驱动层的接口实现
 *
 * \param[in] p_hw_ftfa  : 指向 FTFA 寄存器块的指针
 *
 * \return 无
 */
void demo_fsl_dr_ftfa_entry (amhw_fsl_ftfa_t *p_hw_ftfa);

/**
 * \brief FTFA例程,通过HW层的接口实现
 *
 * \param[in] p_hw_ftfa  : 指向 FTFA 寄存器块的指针
 *
 * \return 无
 */
void deme_fsl_hw_ftfa_entry (amhw_fsl_ftfa_t *p_hw_ftfa);

/**
 * \brief  FGPIO硬件HW层例程
 * \return 无
 */
void demo_kl26_hw_fgpio_entry (void);

/**
 * \brief  GPIO硬件HW层中断例程
 * \return 无
 */
void demo_kl26_hw_gpio_int_entry (void);

/**
 * \brief  GPIO硬件HW层例程
 * \return 无
 */
void demo_kl26_hw_gpio_entry (void);

/**
 * \brief i2c读写AM75示例，通过hw层接口实现
 *
 * \param[in] p_hw_i2c  : 指向 I2C 寄存器块的指针
 * \param[in] i2c_clk   : I2C时钟频率
 *
 * \return 无
 */
void demo_fsl_hw_i2c_master_poll_entry (amhw_fsl_i2c_t *p_hw_i2c,
                                        uint32_t        i2c_clk);

/**
 * \brief i2s dma方式数据发送，调用HW层接口函数实现
 * \return 无
 */
void demo_kl26_hw_i2s_dma_entry (void);

/**
 * \brief i2s中断数据发送，调用HW层接口函数实现
 *
 * \param[in] p_hw_i2s    : 指向 I2S 寄存器块的指针
 * \param[in] inum_i2s    : 中断号
 * \param[in] p_i2s0_clk  : I2S外设时钟
 *
 * \return 无
 */
void demo_fsl_hw_i2s_int_entry (amhw_fsl_i2s_t           *p_hw_i2s,
                                int                       inum_i2s,
                                amhw_fsl_i2s_clock_cfg_t *p_i2s0_clk);

/**
 * \brief i2s数据发送，调用HW层接口函数实现
 *
 * \param[in] p_hw_i2s    : 指向 I2S 寄存器块的指针
 * \param[in] p_i2s0_clk  : I2S外设时钟
 *
 * \return 无
 */
void demo_fsl_hw_i2s_polling_entry (amhw_fsl_i2s_t           *p_hw_i2s,
                                    amhw_fsl_i2s_clock_cfg_t *p_i2s0_clk);

/**
 * \brief LPTMR实现定时器例程，通过HW层接口实现
 *
 * \param[in] p_hw_lptmr    : 指向 lptmr 寄存器块的指针
 * \param[in] inum        : 中断号
 * \param[in] lptmr_clock : lptmr时钟频率
 *
 * \return 无
 */
void demo_fsl_hw_lptmr_timing_entry (amhw_fsl_lptmr_t *p_hw_lptmr,
                                     int               inum,
                                     uint32_t          lptmr_clock);

/**
 * \brief PIT 定时器实现(32位)定时功能的例程，通过HW层的接口实现
 *
 * \param[in] p_hw_pit    : 指向 pit 寄存器块的指针
 * \param[in] inum        : 中断号
 * \param[in] pit_clk     : pit时钟频率
 *
 * \return 无
 */
void demo_fsl_hw_pit_1_32bit_timing_entry (amhw_fsl_pit_t *p_hw_pit,
                                           int             inum,
                                           uint32_t        pit_clk);

/**
 * \brief PMU示例，HW层实现
 *
 * \note 测试看门狗例程时请将am_prj_config.h文件中的宏AM_CFG_SIM_COP置为1
 *
 * \return 无
 */
void demo_kl26_std_pmu_lowpower_entry (void);

/**
 * \brief RTC示例， HW层实现
 *
 * \param[in] p_hw_rtc    : 指向 rtc 寄存器块的指针
 *
 * \return 无
 */
void demo_fsl_hw_rtc_sec_int_entry (amhw_fsl_rtc_t *p_hw_rtc);

/**
 * \brief 后面安全密钥例程入口
 *
 * \param[in] p_hw_ftfa    : 指向 ftfa 寄存器块的指针
 * \param[in] uart_handle  : uart 串口句柄
 *
 * \return 无
 */
void demo_fsl_hw_backdoor_sec_entry (amhw_fsl_ftfa_t *p_hw_ftfa,
                                     am_uart_handle_t uart_handle);

/**
 * \brief UID例程入口
 */
void demo_kl26_hw_sim_uid_entry (void);

/**
 * \brief SPI主机演示例程，通过HW层的接口实现
 *
 * \param[in] p_hw_spi    : 指向 spi 寄存器块的指针
 * \param[in] pin_cs      : spi片选引脚
 * \param[in] spi_freq    : spi 外设时钟频率
 *
 * \return 无
 */
void demo_fsl_hw_spi_master_entry (amhw_fsl_spi_t *p_hw_spi,
                                   int             pin_cs,
                                   uint32_t        spi_freq);
/**
 * \brief SPI从机演示例程，通过HW层的接口实现
 *
 * \param[in] p_hw_spi0    : 指向从机 spi0 寄存器块的指针
 * \param[in] p_hw_spi1    : 指向主机 spi1 寄存器块的指针
 * \param[in] slave_inum   : 从机中断号
 * \param[in] spi_freq     : spi 主机外设时钟频率
 *
 * \return 无
 */
void demo_fsl_hw_spi_slave_irq_entry (amhw_fsl_spi_t *p_hw_spi0,
                                      amhw_fsl_spi_t *p_hw_spi1,
                                      int             slave_inum,
                                      uint32_t        spi_freq);

/**
 * \brief SPI从机匹配中断演示例程，通过HW层的接口实现
 *
 * \param[in] p_hw_spi0    : 指向从机 spi0 寄存器块的指针
 * \param[in] p_hw_spi1    : 指向主机 spi1 寄存器块的指针
 * \param[in] slave_inum   : 从机中断号
 * \param[in] spi_freq     : spi 主机外设时钟频率
 *
 * \return 无
 */
void demo_fsl_hw_spi_slave_match_entry (amhw_fsl_spi_t *p_hw_spi0,
                                        amhw_fsl_spi_t *p_hw_spi1,
                                        int             slave_inum,
                                        uint32_t        spi_freq);

/**
 * \brief SPI从机演示例程，通过HW层的接口实现
 *
 * \param[in] p_hw_spi    : 指向从机 spi 寄存器块的指针
 *
 * \return 无
 */
void demo_fsl_hw_spi_slave_entry (amhw_fsl_spi_t *p_hw_spi);

/**
 * \brief TPM实现捕获功能，通过HW层的接口实现
 *
 * \param[in] tpm2_pwm_handle    : pwm标准句柄
 * \param[in] p_hw_tpm           : 指向tpm 寄存器块的指针
 * \param[in] inum               : 中断号
 * \param[in] tpm_clock          : tpm 时钟频率
 *
 * \return 无
 */
void demo_fsl_hw_tpm_cap_entry (am_pwm_handle_t  tpm2_pwm_handle,
                                amhw_fsl_tpm_t  *p_hw_tpm,
                                int              inum,
                                uint32_t         tpm_clock);

/**
 * \brief TPM实现PWM功能，通过HW层的接口实现
 *
 * \param[in] p_hw_tpm           : 指向tpm 寄存器块的指针
 * \param[in] clkfreq            : tpm 时钟频率
 *
 * \return 无
 */
void demo_fsl_hw_tpm_pwm_entry (amhw_fsl_tpm_t *p_hw_tpm,
                                uint32_t        clkfreq);

/**
 * \brief TPM实现标准定时器例程，通过HW层的接口实现
 *
 * \param[in] p_hw_tpm       : 指向tpm 寄存器块的指针
 * \param[in] freq           : tpm定时器中断频率
 * \param[in] inum           : 中断号
 * \param[in] tpm_clock      : tpm 时钟频率
 *
 * \return 无
 */
void demo_fsl_hw_tpm_timing_entry (amhw_fsl_tpm_t *p_hw_tpm,
                                   uint32_t        freq,
                                   int             inum,
                                   uint32_t        tpm_clk);
/**
 * \brief  TSI驱动层中断例程
 * \return 无
 */
void demo_fsl_dr_tsi_int_entry (void);

/**
 * \brief  TSI HW层中断例程
 *
 * \param[in] p_hw_tsi       : 指向tsi 寄存器块的指针
 * \param[in] inum           : 中断号
 *
 * \return 无
 */
void demo_fsl_hw_tsi_int_entry (amhw_fsl_tsi_t *p_hw_tsi,
                                int             inum);

/**
 * \brief  TSI HW层例程
 *
 * \param[in] p_hw_tsi       : 指向tsi 寄存器块的指针
 *
 * \return 无
 */
void demo_fsl_hw_tsi_entry (amhw_fsl_tsi_t *p_hw_tsi);

/**
 * \brief  串口硬件HW层中断收发例程
 *
 * \param[in] p_hw_uart   : 指向 uart 寄存器块的指针
 * \param[in] inum        : 中断号
 * \param[in] uart_clk    : uart外设时钟
 * \param[in] uart0_flog  : 如果使用UART0，则该值为1，否则为0
 *
 * \retval 无
 */
void demo_fsl_hw_uart_int_entry (amhw_fsl_uart_t *p_hw_uart,
                                 int              inum,
                                 uint32_t         uart_clk,
                                 am_bool_t        uart0_flog);

/**
 * \brief  串口硬件HW层查询收发例程
 *
 * \param[in] p_hw_uart   : 指向 uart 寄存器块的指针
 * \param[in] uart_clk    : uart外设时钟
 * \param[in] uart0_flog  : 如果使用UART0，则该值为1，否则为0
 *
 * \return 无
 */
void demo_fsl_hw_uart_polling_entry (amhw_fsl_uart_t *p_hw_uart,
                                     uint32_t         uart_clk,
                                     am_bool_t        uart0_flog);

/**
 * \brief  串口接收使用DMA例程
 * \return 无
 */
void demo_kl26_hw_uart_rx_dma_entry (void);

/**
 * \brief  串口发送使用DMA例程
 * \return 无
 */
void demo_kl26_hw_uart_tx_dma_entry (void);

/**
 * \brief WDT入口函数，HW层实现
 * \return 无
 */
void demo_kl26_hw_wdt_entry (void);

/**
 * \brief MiniPort-View + MiniPort-595 温度显示例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_hc595_digitron_temp_interface_entry (void);

/**
 * \brief MiniPort-LED 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_led_entry (void);

/**
 * \brief MiniPort-VIEW 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_digitron_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-LED 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_hc595_led_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_hc595_digitron_entry (void);

/**
 * \brief MiniPort-KEY
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_key_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-KEY 例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_hc595_digitron_key_entry (void);

/**
 * \brief MiniPort-595 + MiniPort-VIEW + LM75 温度显示例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_hc595_digitron_temp_entry (void);

/**
 * \brief MiniPort-ZLG72128显示例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_zlg72128_entry (void);

/**
 * \brief MiniPort-KEY + MiniPort-DIGITORN显示例程
 *
 * \param 无
 *
 * \return 无
 */
void demo_miniport_key_digitron_entry (void);

/**
 * \brief MicroPort DS1302 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_ds1302_entry (void);

/**
 * \brief MicroPort EEPROM 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_eeprom_entry (void);

/**
 * \brief MicroPort EEPROM 例程，通过 NVRAM 接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_eeprom_nvram_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_flash_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过 MTD 接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_flash_mtd_entry (void);

/**
 * \brief MicroPort FLASH 例程，通过 FTL 接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_flash_ftl_entry (void);

/**
 * \brief MicroPort RS485 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_rs485_entry (void);

/**
 * \brief MicroPort RTC 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_rtc_entry (void);

/**
 * \brief MicroPort RX8025T 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_rx8025t_entry (void);

/**
 * \brief MicroPort USB 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_microport_usb_entry (void);

/* end of file */
