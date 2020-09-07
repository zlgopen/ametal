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
 *   1. 测量16个外部输入
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOE_4引脚连接 PC串口的RXD，
 *       PIOE_5引脚连接 PC串口的TXD。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_multichan_adc.c src_hc32f460_hw_multichan_adc
 *
 * \internal
 * \par Modification History
 * - 1.00 20-04-22  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_multichan_adc
 * \copydoc demo_hc32f460_hw_multichan_adc.c
 */

/** [src_hc32f460_hw_multichan_adc] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "demo_hc32f460_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_multichan_seq_adc_entry (void)
{
	uint8_t i = 0;

    /* 16个通道ADC采集 */
    amhw_adc_chmux_t adc_chan[] = {
                                      {0,  PIOA_0 , AMHW_HC32F460_ADC_CH_MUX_ADC1_IN0},
                                      {1,  PIOA_1 , AMHW_HC32F460_ADC_CH_MUX_ADC1_IN1},
                                      {2,  PIOA_2 , AMHW_HC32F460_ADC_CH_MUX_ADC1_IN2},
                                      {3,  PIOA_3 , AMHW_HC32F460_ADC_CH_MUX_ADC1_IN3},
                                      {4,  PIOA_4 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN4},
                                      {5,  PIOA_5 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN5},
                                      {6,  PIOA_6 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN6},
                                      {7,  PIOA_7 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN7},
                                      {8,  PIOB_0 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN8},
                                      {9,  PIOB_1 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN9},
                                      {10, PIOC_0 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN10},
                                      {11, PIOC_1 , AMHW_HC32F460_ADC_CH_MUX_ADC12_IN11},
                                      {12, PIOC_2 , AMHW_HC32F460_ADC_CH_MUX_ADC1_IN12},
                                      {13, PIOC_3 , AMHW_HC32F460_ADC_CH_MUX_ADC1_IN13},
                                      {14, PIOC_4 , AMHW_HC32F460_ADC_CH_MUX_ADC1_IN14},
                                      {15, PIOC_5 , AMHW_HC32F460_ADC_CH_MUX_ADC1_IN15}
                                  };
    
    AM_DBG_INFO("demo hc32f460_core hw multichan seq adc int !\r\n");

    /* 配置引脚 */
    for(i = 0; i < sizeof(adc_chan) / sizeof(adc_chan[0]); i++) {
        am_gpio_pin_cfg(adc_chan[i].pin, GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN));
    }

    /* 时钟使能  */
    am_clk_enable(CLK_ADC1);

    demo_hc32f460_hw_multichan_seq_adc_entry(HC32F460_ADC1,
                                             INT_VECTOR_16,
                                             3300,
                                             adc_chan,
                                             sizeof(adc_chan) / sizeof(adc_chan[0]));
}
/** [src_hc32f460_hw_multichan_seq_adc] */

/* end of file */
