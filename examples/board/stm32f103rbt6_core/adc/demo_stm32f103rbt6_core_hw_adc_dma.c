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
 * \brief ADC INT 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_0 (ADC 通道 0) 连接模拟输入。
 *   2. PIOA_1 (ADC 通道 1) 连接模拟输入。
 *   3. PIOA_2 (ADC 通道 2) 连接模拟输入。
 *   4. PIOA_3 (ADC 通道 3) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 使用 ADC模块功能，必须保证 ADC模块上电；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_9引脚连接 PC串口的RXD，
 *       PIOA_10引脚连接 PC串口的TXD。
 *
 * \par 源代码
 * \snippet demo_zlg237_hw_adc_dma.c src_zlg237_hw_adc_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 19-02-28  ipk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg237_hw_adc_dma
 * \copydoc demo_zlg237_hw_adc_dma.c
 */

/** [src_zlg237_hw_adc_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg237.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "demo_zlg_entries.h"
#include "demo_am237_core_entries.h"

#define  __ADC1_DMA_CHAN1         DMA_CHAN_1 /* ADC1对应DMA通道1 */
                                             /* 仅ADC1能发起DMA请求 */

/**
 * \brief 例程入口
 */
void demo_zlg237_core_hw_adc_dma_entry (void)
{
    AM_DBG_INFO("demo am237_core hw adc dma!\r\n");

    int adc_chan[] = {0, 1, 2, 3};

    /* 运行硬件层例程前的环境设置 ，配置引脚，使能时钟等 */
    am_gpio_pin_cfg(PIOA_0, PIOA_0_AIN | PIOA_0_ADC12_IN0); /* ADC12通道0 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_AIN | PIOA_1_ADC12_IN1); /* ADC12通道1 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_AIN | PIOA_2_ADC12_IN2); /* ADC12通道2 */
    am_gpio_pin_cfg(PIOA_3, PIOA_3_AIN | PIOA_3_ADC12_IN3); /* ADC12通道3 */

    am_clk_enable(CLK_ADC1);

    demo_zlg237_hw_adc_dma_entry((void *)ZLG237_ADC1,
                                 adc_chan,
                                 sizeof(adc_chan)/sizeof(adc_chan[0]),
                                 __ADC1_DMA_CHAN1);
}
/** [src_zlg237_hw_adc_dma] */

/* end of file */
