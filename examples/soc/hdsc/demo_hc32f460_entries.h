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

#ifdef __cplusplus
extern "C" {
#endif


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
                                         int32_t                 dma_chan);


#ifdef __cplusplus
}
#endif

#endif /* __DEMO_HC32F460_ENTRIES_H */

/* end of file */
