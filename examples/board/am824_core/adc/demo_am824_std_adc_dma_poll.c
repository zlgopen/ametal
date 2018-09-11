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
 * \brief ADC DMA 例程（轮询方式），通过标准接口实现
 *
 * - 操作步骤：
 *   1. 短接 J12 跳线帽，使得参考电压为 2.5V；
 *   2. PIO0_7(ADC 通道 0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_std_adc_dma_poll.c src_am824_std_adc_dma_poll
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-09-10  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_adc_dma_poll
 * \copydoc demo_am824_std_adc_dma_poll.c
 */

/** [src_am824_std_adc_dma_poll] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_adc.h"
#include "am_lpc82x_inst_init.h"
#include "demo_nxp_entries.h"

/** \brief 使用的 ADC 通道 */
#define __ADC_CHAN         0

/**
 * \brief 例程入口
 */
void demo_am824_std_adc_dma_poll_entry (void)
{

    am_adc_handle_t adc_handle = NULL;

    am_kprintf("demo am824_core std adc poll!\r\n");

    adc_handle = am_lpc82x_adc0_dma_inst_init();

    demo_lpc_std_adc_dma_poll_entry(adc_handle,
                                    __ADC_CHAN,
                                    10000);
}
/** [src_am824_std_adc_dma_poll] */

/* end of file */
