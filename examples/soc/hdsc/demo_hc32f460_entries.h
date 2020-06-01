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
#include "hw/amhw_hc32f460_uart.h"
#include "hw/amhw_hc32f460_dma.h"
#include "am_hc32f460_dma.h"
#include "am_hc32f460_qspi.h"
#include "hw/amhw_hc32f460_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

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
void demo_hc32f460_drv_dma_m2m_entry (uint32_t dma_chan);

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

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_HC32F460_ENTRIES_H */

/* end of file */
