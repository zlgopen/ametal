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
 * \brief HC32F460系列外设驱动的所有例程函数入口声明
 * \sa    demo_hc32f460_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-02  tee, first implementation
 * \endinternal
 */

#ifndef __DEMO_HC32F460_ENTRIES_H
#define __DEMO_HC32F460_ENTRIES_H

#include "am_clk.h"
#include "am_timer.h"
#include "am_can.h"
#include "hw/amhw_hc32f460_gpio.h"
#include "hw/amhw_hc32f460_adc.h"
#include "hw/amhw_hc32f460_uart.h"
#include "hw/amhw_hc32f460_dma.h"
#include "am_hc32f460_dma.h"
#include "am_hc32f460_qspi.h"
#include "hw/amhw_hc32f460_spi.h"
#include "hw/amhw_hc32f460_wdt.h"
#include "hw/amhw_hc32f460_hash.h"
#include "hw/amhw_hc32f460_trng.h"
#include "hw/amhw_hc32f460_aes.h"
#include "hw/amhw_hc32f460_dcu.h"
#include "hw/amhw_hc32f460_cmp.h"
#include "am_hc32f460_hash.h"
#include "am_hc32f460_trng.h"
#include "am_hc32f460_aes.h"
#include "am_hc32f460_swdt.h"
#include "am_hc32f460_wdt.h"
#include "am_wdt.h"
#include "hw/amhw_hc32f460_adc.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief CLK 例程，通过 HW 层接口实现
 *
 * \param[in] p_clk_id_buf   指向要打印的时钟ID的数组
 * \param[in] buf_lenth      时钟ID数组的长度
 *
 * \return 无
 */
void demo_hc32f460_hw_clk_entry (am_clk_id_t  *p_clk_id_buf,
                                 uint8_t       buf_lenth,
                                 char         *p_clk_name,
                                 uint8_t       name_length);

/**
 * \brief ADC INT 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adc   指向ADC外设寄存器块的指针
 * \param[in] int_num    中断编号
 * \param[in] chan       ADC通道号
 * \param[in] vref_mv    参考电压
 *
 * \return 无
 */
void demo_hc32f460_hw_adc_int_entry (void    *p_hw_adc,
                                     int      int_num,
                                     uint8_t  chan,
                                     uint32_t vref_mv);

/**
 * \brief ADC多通道DMA传输例程，通过驱动层接口实现
 *
 * \param[in] p_hw_adc   指向 ADC外设寄存器块的指针
 * \param[in] p_adc_chan ADC通道属性配置
 * \param[in] p_dma_dev  指向 DMA设备的指针
 * \param[in] chan_num   ADC通道号
 * \param[in] vref_mv    参考电压
 * \param[in] dma_chan   DMA通道号
 *
 * \return 无
 */
void demo_hc32f460_hw_adc_dma_entry (void                  *p_hw_adc,
                                     amhw_adc_chmux_t      *p_adc_chan,
                                     am_hc32f460_dma_dev_t *p_dma_dev,
                                     int                    chan_num,
                                     uint32_t               vref_mv,
                                     int                    dma_chan);


/**
 * \brief ADC 多通道转换 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adc   指向 ADC外设寄存器块的指针
 * \param[in] int_num    中断编号
 * \param[in] vref_mv    参考电压
 * \param[in] p_adc_chan ADC通道属性配置
 * \param[in] chan_num   ADC通道号
 *
 * \return 无
 */
void demo_hc32f460_hw_multichan_adc_entry (void    *p_hw_adc,
                                           int      int_num,
                                           uint32_t vref_mv,
                                           amhw_adc_chmux_t     *p_adc_chan,
                                           uint8_t  chan_num);

/**
 * \brief ADC 多通道连续转换 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adc   指向 ADC外设寄存器块的指针
 * \param[in] int_num    中断编号
 * \param[in] vref_mv    参考电压
 * \param[in] p_adc_chan ADC通道属性配置
 * \param[in] chan_num   ADC通道号
 *
 * \return 无
 */
void demo_hc32f460_hw_multichan_seq_adc_entry (void             *p_hw_adc,
                                               int               int_num,
                                               uint32_t          vref_mv,
                                               amhw_adc_chmux_t *p_adc_chan,
                                               uint8_t           chan_num);

/**
 * \brief ADC 序列A事件触发 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adc   指向 ADC外设寄存器块的指针
 * \param[in] int_num    中断编号
 * \param[in] chan_num   ADC通道号
 * \param[in] vref_mv    参考电压
 *
 * \return 无
 */
void demo_hc32f460_hw_adc_seq_a_event_trigger_entry (void    *p_hw_adc,
                                                     int      int_num,
                                                     uint8_t  chan,
                                                     uint32_t vref_mv);

/**
 * \brief ADC 序列A、B双序列事件触发 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adc 指向 ADC外设寄存器块的指针
 * \param[in] int_num  中断编号
 * \param[in] chan_a   ADC序列A通道号
 * \param[in] chan_b   ADC序列B通道号
 * \param[in] vref_mv  参考电压
 *
 * \return 无
 */
void demo_hc32f460_hw_adc_seq_a_b_event_trigger_entry (void    *p_hw_adc,
                                                       int      int_num,
                                                       uint8_t  chan_a,
                                                       uint8_t  chan_b,
                                                       uint32_t vref_mv);

/**
 * \brief 多ADC 协同工作 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adc1    指向 ADC1外设寄存器块的指针
 * \param[in] p_hw_adc2    指向 ADC2外设寄存器块的指针
 * \param[in] int_num_adc1 ADC1中断编号
 * \param[in] int_num_adc2 ADC2中断编号
 * \param[in] chan_adc1    ADC1通道号
 * \param[in] chan_adc2    ADC2通道号
 * \param[in] vref_mv      参考电压
 *
 * \return 无
 */
void demo_hc32f460_hw_adc_sync_entry (void    *p_hw_adc1,
                                      void    *p_hw_adc2,
                                      int      int_num_adc1,
                                      int      int_num_adc2,
                                      uint8_t  chan_adc1,
                                      uint8_t  chan_adc2,
                                      uint32_t vref_mv);

/**
 * \brief ADC PGA 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adc  指向 ADC外设寄存器块的指针
 * \param[in] int_num   ADC中断编号
 * \param[in] chan      ADC通道号
 * \param[in] vref_mv   参考电压
 *
 * \return 无
 */
void demo_hc32f460_hw_adc_pga_entry (void    *p_hw_adc,
                                     int      int_num,
                                     uint8_t  chan,
                                     uint32_t vref_mv);

/**
 * \brief ADC PGA 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_adc  指向 ADC外设寄存器块的指针
 * \param[in] int_num   EOC中断编号
 * \param[in] chan      SEQCMP中断编号
 * \param[in] chan      ADC通道号
 * \param[in] vref_mv   参考电压
 *
 * \return 无
 */
void demo_hc32f460_hw_adc_awd_entry (void    *p_hw_adc,
                                     int      int_num_eoc,
                                     int      int_num_seqcmp,
                                     uint8_t  chan,
                                     uint32_t vref_mv);


/**
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio  指向 GPIO外设寄存器块的指针
 * \param[in] input_pin  输入引脚号
 * \param[in] output_pin 输出引脚号
 *
 * \return 无
 */
void demo_hc32f460_hw_gpio_entry (void    *p_hw_gpio,
                                  int32_t  input_pin,
                                  int32_t  output_pin);

/**
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_gpio     指向 GPIO 外设寄存器块的指针
 * \param[in] pin           中断引脚号
 *
 * \return 无
 */
void demo_hc32f460_hw_gpio_trigger_entry (void *p_hw_gpio, int pin);


/**
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * \param[in] dma_chan DMA 通道号
 *
 * \return 无
 */
void demo_hc32f460_drv_dma_m2m_entry (am_hc32f460_dma_dev_t *p_dev, uint32_t dma_chan);

/**
 * \brief DMA 内存到内存连锁传输例程，通过驱动层接口实现
 *
 * \param[in] p_dev    DMA 设备指针
 * \param[in] dma_chan DMA 通道号
 *
 * \return 无
 */
void demo_hc32f460_drv_dma_m2m_chain_entry (am_hc32f460_dma_dev_t *p_dev, uint32_t dma_chan);

/**
 * \brief UART 中断发送例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] pfn_init  指向 UART 引脚初始化函数的指针
 * \param[in] clk_rate  UART 时钟源频率
 * \param[in] uart_base UART 基地址
 * \param[in] inum_uart UART 中断号
 *
 * \return 无
 */
void demo_hc32f460_hw_uart_int_entry (void             *p_hw_uart,
                                      void (* pfn_init)(void),
                                      int              clk_rate,
                                      unsigned long    uart_base);

/**
 * \brief UART 轮询方式例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 *
 * \return 无
 */
void demo_hc32f460_hw_uart_polling_entry (void     *p_hw_uart,
                                          uint32_t  clk_rate);


/**
 * \brief UART DMA发送例程，通过 HW 层接口实现
 *

 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 *
 * \return 无
 */
void demo_hc32f460_hw_uart_tx_dma_entry (void                   *p_hw_uart,
                                         uint32_t                clk_rate,
                                         am_hc32f460_dma_dev_t  *p_dma_dev,
                                         int32_t                 dma_chan);
/**
 * \brief UART DMA接收例程，通过 HW 层接口实现
 *

 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] clk_rate  UART 时钟源频率
 *
 * \return 无
 */
void demo_hc32f460_hw_uart_rx_dma_entry (void    *p_hw_uart,
                                         uint32_t clk_rate,
                                         am_hc32f460_dma_dev_t *p_dma_dev,
                                         int32_t  dma_chan);

/**
 * \brief QSPI读写测试例程
 *
 * \param[in] p_hw_uart 指向 QSPI设备的指针
 *
 * \return 无
 */
void demo_hc32f460_drv_qspi_entry (am_hc32f460_qspi_dev_t *p_dev);

/**
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart 指向 UART 外设寄存器块的指针
 * \param[in] cs_pin    片选引脚
 *
 * \return 无
 */
void demo_hc32f460_hw_spi_master_entry (void    *p_hw_spi,
                                        int32_t  cs_pin);

/**
 * \brief WDT 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_wdt     指向 WDT 外设寄存器块的指针
 * \param[in] time_out_ms  WDT超时时间
 * \param[in] feed_time_ms 喂狗间隔时间
 * \param[in] clk_rate     wdt时钟频率
 *
 * \return 无
 */
void demo_hc32f460_hw_wdt_entry (void     *p_hw_wdt,
                                 uint32_t  time_out_ms,
                                 uint32_t  feed_time_ms,
                                 uint32_t  clk_rate);

/**
 * \brief WDT DRV例程，通过 HW 层接口实现
 *
 * \param[in] handle       WDT handler
 * \param[in] p_dev        指向WDT设备的指针
 * \param[in] time_out_ms  WDT超时时间
 * \param[in] feed_time_ms 喂狗间隔时间
 *
 * \return 无
 */
void demo_hc32f460_drv_wdt_int_entry (am_wdt_handle_t        handle,
                                      am_hc32f460_wdt_dev_t *p_dev,
                                      uint32_t               time_out_ms,
                                      uint32_t               feed_time_ms);

/**
 * \brief SWDT DRV例程，通过 HW 层接口实现
 *
 * \param[in] handle       SWDT handler
 * \param[in] p_dev        指向SWDT设备的指针
 * \param[in] feed_time_ms 喂狗间隔时间
 *
 * \return 无
 */
void demo_hc32f460_drv_swdt_int_entry (am_wdt_handle_t         handle,
                                       am_hc32f460_swdt_dev_t *p_dev,
                                       uint32_t                feed_time_ms);


/**
 * \brief MPU 例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_mpu     MPU寄存器结构体指针
 *
 * \return 无
 */
void demo_hc32f460_hw_mpu_entry (void *p_hw_mpu);

/**
 * \brief HASH 消息摘要产生例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_hash     指向 HASH 外设寄存器块的指针
 * \param[in] data_len      原始数据长度（字节）
 * \param[in] p_hash_data   指向存放原始数据的BUFFER的指针
 * \param[in] p_hash_digest 指向存放消息摘要的BUFFER的指针
 *
 * \return 无
 */
void demo_hc32f460_hw_hash_entry(void     *p_hw_hash,
                                 uint32_t  data_len,
                                 uint8_t  *p_hash_data,
                                 uint8_t  *p_hash_digest);

/**
 * \brief HASH 消息摘要产生例程，通过 驱动 层接口实现
 *
 * \param[in] p_hw_trng 指向 HASH服务的handle
 *
 * \return 无
 */
void demo_hc32f460_drv_hash_generate_entry (am_hash_handle_t handle);

/**
 * \brief TRNG 随机数产生例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_trng 指向 TRNG 外设寄存器块的指针
 *
 * \return 无
 */
void demo_hc32f460_hw_trng_generate_entry (void *p_hw_trng);

/**
 * \brief TRNG 随机数产生例程，通过 DRV 层接口实现
 *
 * \param[in] p_hw_trng 指向 TRNG服务的handle
 *
 * \return 无
 */
void demo_hc32f460_drv_trng_generate_entry (am_trng_handle_t handle);

/**
 * \brief AES 加密解密例程，通过 HW 层接口实现
 *
 * \param[in] p_hw_uart  指向 AES 外设寄存器块的指针
 * \param[in] p_aes_data 指向 要加密的数据
 * \param[in] p_key_data 指向 密钥
 *
 * \return 无
 */
void demo_hc32f460_hw_aes_ency_decy_entry (void     *p_hw_aes,
                                           uint32_t *p_aes_data,
                                           uint32_t *p_key_data);

/**
 * \brief AES 加密解密例程，通过 DRV 层接口实现
 *
 * \param[in] p_hw_uart  指向 AES 服务的handle
 * \param[in] p_aes_data 指向 要加密的数据
 * \param[in] p_key_data 指向 密钥
 *
 * \return 无
 */
void demo_hc32f460_drv_aes_ency_decy_entry (am_aes_handle_t  handle,
                                            uint32_t        *p_aes_data,
                                            uint32_t        *p_key_data);

/**
 * \brief DCU 数据计算单元例程，通过 硬件 层接口实现
 *
 * \param[in] p_hw_dcu 指向 DCU 外设寄存器块的指针
 *
 * \return 无
 */
void demo_hc32f460_hw_dcu_entry (void *p_hw_dcu);

/**
 * \brief FLASH读写例程，通过 DRV 层接口实现
 *
 * \param[in] p_hw_dcu 指向 EFM 外设寄存器块的指针
 * \param[in] sector   扇区编号
 *
 * \return 无
 */
void demo_hc32f460_drv_flash_entry (void *p_hw_flash, uint16_t sector);

/**
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 */
void demo_hc32f460_hw_i2c_slave_poll_entry (void     *p_hw_i2c_addr,
                                            uint32_t  clk_id,
                                            int       slave_speed,
                                            int       slave_addr);

/**
 * \brief I2C 主机轮询例程，通过 HW 层接口实现
 */
void demo_hc32f460_hw_i2c_master_poll_entry (void     *p_hw_i2c_addr,
                                             uint32_t  clk_id,
                                             int       master_speed,
                                             int       master_addr);

/**
 * \brief USB模拟U盘例程，通过driver层的接口实现
 */
void demo_usbd_msc_entry (void* p_handle);

/**
 * \brief USB模拟鼠标例程，通过driver层的接口实现
 */
void demo_usbd_mouse_entry (void* p_handle);

/**
 * \brief CMP例程，通过 HW 层接口实现
 */
void demo_hc32f460_hw_cmp_entry (amhw_hc32f460_cmp_t *p_hw_cmp);
                                             
/**
 * \brief FLASH 写保护例程
 *
 * \param[in] p_hw_flash 指向 EFM 外设寄存器块的指针
 * \param[in] sector   扇区编号
 *
 * \return 无
 */
void demo_hc32f460_drv_flash_write_protect_entry (void *p_hw_flash, uint16_t sector);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_HC32F460_ENTRIES_H */

/* end of file */
