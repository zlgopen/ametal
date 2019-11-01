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
 *   16个顺序扫描转换通道，使能DMA块传输（16个数据）
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *    2. 使用的是顺序扫描转换，最多支持16通道。如果需要使用插入扫描转换，需要更改
 *       demo_hc32f03x_hw_adc_dma.c中的部分代码。
 *
 * \par 源代码
 * \snippet demo_hc32f03x_hw_adc_dma.c src_hc32f03x_hw_adc_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-25  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f03x_hw_adc_dma
 * \copydoc demo_hc32f03x_hw_adc_dma.c
 */

/** [src_hc32f03x_hw_adc_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "demo_hc32_entries.h"
#include "demo_amf03x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f03x_core_hw_adc_dma_entry (void)
{     
    /* 16个顺序通道ADC采集 */
    int adc_chan[16] = {AMHW_HC32_CHAN_AIN0_PA0,
                        AMHW_HC32_CHAN_AIN1_PA1,
                        AMHW_HC32_CHAN_AIN2_PA2,
                        AMHW_HC32_CHAN_AIN3_PA3,
                        AMHW_HC32_CHAN_AIN4_PA4,
                        AMHW_HC32_CHAN_AIN5_PA5,
                        AMHW_HC32_CHAN_AIN6_PA6,
                        AMHW_HC32_CHAN_AIN7_PA7,
                        AMHW_HC32_CHAN_AIN8_PB0,
                        AMHW_HC32_CHAN_AIN9_PB1,
                        AMHW_HC32_CHAN_AIN10_PC0,
                        AMHW_HC32_CHAN_AIN11_PC1,
                        AMHW_HC32_CHAN_AIN12_PC2,
                        AMHW_HC32_CHAN_AIN13_PC3,
                        AMHW_HC32_CHAN_AIN14_PC4,
                        AMHW_HC32_CHAN_AIN15_PC5};
    
    AM_DBG_INFO("demo amf03x_core hw adc dma!\r\n");

    /* 配置引脚 */
    am_gpio_pin_cfg(PIOA_0, PIOA_0_GPIO | PIOA_0_AIN);
    am_gpio_pin_cfg(PIOA_1, PIOA_1_GPIO | PIOA_1_AIN);
    am_gpio_pin_cfg(PIOA_2, PIOA_2_GPIO | PIOA_2_AIN);
    am_gpio_pin_cfg(PIOA_3, PIOA_3_GPIO | PIOA_3_AIN);
    am_gpio_pin_cfg(PIOA_4, PIOA_4_GPIO | PIOA_4_AIN);
    am_gpio_pin_cfg(PIOA_5, PIOA_5_GPIO | PIOA_5_AIN);
    am_gpio_pin_cfg(PIOA_6, PIOA_6_GPIO | PIOA_6_AIN);
    am_gpio_pin_cfg(PIOA_7, PIOA_7_GPIO | PIOA_7_AIN);
    am_gpio_pin_cfg(PIOB_0, PIOB_0_GPIO | PIOB_0_AIN);
    am_gpio_pin_cfg(PIOB_1, PIOB_1_GPIO | PIOB_1_AIN);
    am_gpio_pin_cfg(PIOC_0, PIOC_0_GPIO | PIOC_0_AIN);
    am_gpio_pin_cfg(PIOC_1, PIOC_1_GPIO | PIOC_1_AIN);
    am_gpio_pin_cfg(PIOC_2, PIOC_2_GPIO | PIOC_2_AIN);
    am_gpio_pin_cfg(PIOC_3, PIOC_3_GPIO | PIOC_3_AIN);
    am_gpio_pin_cfg(PIOC_4, PIOC_4_GPIO | PIOC_4_AIN);
    am_gpio_pin_cfg(PIOC_5, PIOC_5_GPIO | PIOC_5_AIN);

    am_clk_enable(CLK_ADC_BGR);

    demo_hc32_hw_adc_dma_entry(HC32_ADC,
                                 adc_chan,
                                 16,
                                 3300,
                                 DMA_CHAN_1,
                                 HC32_DMA_SRC_TYPE_ADC_SRQ0_RX);
}
/** [src_hc32f03x_hw_adc_dma] */

/* end of file */
