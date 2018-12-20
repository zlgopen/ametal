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
 * \brief ADC 例程，转换结果数据通过 DMA 传输，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 短接 J12 跳线帽，使得参考电压为 2.5V；
 *   2. PIO0_7(ADC 通道 0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. ADC 转换完成 100 次，串口打印出 100 个采样电压值。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_core_hw_adc_dma.c src_am824_core_hw_adc_dma
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-07-23  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_core_hw_adc_dma
 * \copydoc demo_am824_core_hw_adc_dma.c
 */

/** [src_am824_core_hw_adc_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_hw_adc_dma_entry (void)
{
    am_kprintf("demo am824_core hw adc dma!\r\n");

    /* 平台初始化 */
    amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_ADC0);
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_ADC0);
	
    /* 配置通道，使用通道 0，引脚配置为消极模式 INACTIVE */
    am_gpio_pin_cfg(PIO0_7, PIO0_7_ADC_0 | PIO0_7_INACTIVE);

    demo_lpc824_hw_adc_dma_entry(LPC82X_ADC0, 0, 2500);
}
/** [src_am824_hw_adc_dma] */

/* end of file */
