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
 * \brief ADC1 和 ADC2同时采集的例程，通过 HW 层接口实现
 *
 *        ADC1 使用 规则组 DMA通道, ADC2 使用 注入组 中断通道
 *
 * - 操作步骤：
 *   1. PIOA_2 (ADC 通道 2) 连接模拟输入。
 *   2. PIOA_3 (ADC 通道 3) 连接模拟输入。
 *   3. PIOA_4 (ADC 通道 4) 连接模拟输入。
 *   4. PIOA_5 (ADC 通道 5) 连接模拟输入。
 *   5. PIOA_6 (ADC 通道 6) 连接模拟输入。
 *   6. PIOA_7 (ADC 通道 7) 连接模拟输入。
 *   7. PIOC_1 (ADC 通道 11) 连接模拟输入。
 *   8. PIOC_2 (ADC 通道 12) 连接模拟输入。
 *   9. PIOC_3 (ADC 通道 13) 连接模拟输入。
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
 * \snippet demo_hk32f103rbt6_core_hw_adc_dma_int.c src_hk32f103rbt6_core_hw_adc_dma_int
 *
 * \internal
 * \par Modification History
 * - 1.00 19-02-28  ipk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hk32f103rbt6_core_hw_adc_dma_int
 * \copydoc demo_hk32f103rbt6_core_hw_adc_dma_int.c
 */

/** [src_hk32f103rbt6_core_hw_adc_dma_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hk32f103rbt6.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "demo_hk32f103rbt6_entries.h"
#include "demo_hk32f103rbt6_core_entries.h"

#define  __ADC1_DMA_CHAN1         DMA_CHAN_1 /* ADC1对应DMA通道1 */
                                             /* 仅ADC1能发起DMA请求 */

/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_core_hw_adc_dma_double_entry (void)
{
    AM_DBG_INFO("demo hk32f103rbt6_core hw adc dma!\r\n");

    int adc1_chan[] = {2, 3, 4, 5, 6, 7};
    int adc2_chan[] = {11, 12, 13};

    /* 运行硬件层例程前的环境设置 ，配置引脚，使能时钟等 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_AIN | PIOA_2_ADC12_IN2); /* ADC1通道2 */
    am_gpio_pin_cfg(PIOA_3, PIOA_3_AIN | PIOA_3_ADC12_IN3); /* ADC1通道3 */
    am_gpio_pin_cfg(PIOA_4, PIOA_4_AIN | PIOA_4_ADC12_IN4); /* ADC1通道0 */
    am_gpio_pin_cfg(PIOA_5, PIOA_5_AIN | PIOA_5_ADC12_IN5); /* ADC1通道1 */
    am_gpio_pin_cfg(PIOA_6, PIOA_6_AIN | PIOA_6_ADC12_IN6); /* ADC1通道2 */
    am_gpio_pin_cfg(PIOA_7, PIOA_7_AIN | PIOA_7_ADC12_IN7); /* ADC1通道3 */


    am_gpio_pin_cfg(PIOC_1, PIOC_1_AIN | PIOC_1_ADC12_IN11); /* ADC2通道11 */
    am_gpio_pin_cfg(PIOC_2, PIOC_2_AIN | PIOC_2_ADC12_IN12); /* ADC2通道12 */
    am_gpio_pin_cfg(PIOC_3, PIOC_3_AIN | PIOC_3_ADC12_IN13); /* ADC2通道13 */

    am_clk_enable(CLK_ADC1);
    am_clk_enable(CLK_ADC2);

    demo_hk32f103rbt6_hw_adc_dma_double_entry((void *)HK32F103RBT6_ADC1,
                                        adc1_chan,
                                        sizeof(adc1_chan)/sizeof(adc1_chan[0]),
                                        __ADC1_DMA_CHAN1,
                                        (void *)HK32F103RBT6_ADC2,
                                        adc2_chan,
                                        sizeof(adc2_chan)/sizeof(adc2_chan[0]));
}
/** [src_hk32f103rbt6_core_hw_adc_dma_int] */

/* end of file */
