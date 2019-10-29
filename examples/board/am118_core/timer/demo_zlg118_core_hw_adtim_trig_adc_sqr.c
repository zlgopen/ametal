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
 * \brief 高级定时器触发控制ADC顺序扫描转换例程，通过 HW层接口实现
 *
 * - 实验现象：
 *   1.ADC以10Hz的频率进行顺序扫描转换（定时器定时100ms），每转换一次LED翻转一次（闪烁频率10/2=5Hz）
 *   2.串口数据打印的频率与ADC转换的时间间隔不同，串口打印间隔为500ms。
 *
 * \note
 *   1.如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *     PIOA_9 引脚连接 PC 串口的 RXD。
 *   2.adc通道数量（sqr_num）的设定范围为1~16
 *   3.在PCLK16MHz下，定时器周期时间（period_us）的设定范围为0~262140us（0~262ms之间），如果想设
 *     定其他时间值，则需要更改demo_zlg118_hw_adtim_trig_adc_sqr.c中定时器分频系数的设定。
 *   4.不同情况下，ADC的转换速度有限，需要考虑定时器的周期时间是否大于ADC转换时间（多个通道则需要累加转换时间）
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_adtim_trig_adc_sqr.c src_zlg118_hw_adtim_trig_adc_sqr
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-11  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_adtim_trig_adc_sqr
 * \copydoc demo_zlg118_hw_adtim_trig_adc_sqr.c
 */

/** [src_zlg118_hw_adtim_trig_adc_sqr] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zlg118.h"
#include "hw/amhw_zlg118_adtim.h"
#include "hw/amhw_zlg118_adc.h"
#include "am_zlg118_inst_init.h"
#include "demo_zlg_entries.h"
#include "demo_am118_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg118_core_hw_adtim_trig_adc_sqr_entry (void)
{
    /* 16个顺序通道ADC采集 */
    int adc_chan[] = {AMHW_ZLG118_CHAN_AIN0_PA0,
                      AMHW_ZLG118_CHAN_AIN1_PA1,
                      AMHW_ZLG118_CHAN_AIN2_PA2,
                      AMHW_ZLG118_CHAN_AIN3_PA3,
                      AMHW_ZLG118_CHAN_AIN4_PA4,
                      AMHW_ZLG118_CHAN_AIN5_PA5,
                      AMHW_ZLG118_CHAN_AIN6_PA6,
                      AMHW_ZLG118_CHAN_AIN7_PA7,
                      AMHW_ZLG118_CHAN_AIN8_PB0,
                      AMHW_ZLG118_CHAN_AIN9_PB1,
                      AMHW_ZLG118_CHAN_AIN10_PC0,
                      AMHW_ZLG118_CHAN_AIN11_PC1,
                      AMHW_ZLG118_CHAN_AIN12_PC2,
                      AMHW_ZLG118_CHAN_AIN13_PC3,
                      AMHW_ZLG118_CHAN_AIN14_PC4,
                      AMHW_ZLG118_CHAN_AIN15_PC5};
    
    AM_DBG_INFO("demo am118_core hw adtim trig adc sqr!\r\n");

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

    /* 时钟使能  */
    am_clk_enable(CLK_ADC_BGR);
    am_clk_enable(CLK_TIM456);

    demo_zlg118_hw_adtim_trig_adc_sqr_entry(ZLG118_TIM4,     //高级定时器4
                                            100000,          //100000us = 100ms
                                            ZLG118_ADC,      //ADC
                                            INUM_ADC_DAC,    //中断号
                                            adc_chan,        //ADC通道编号
                                            6);              //使用的ADC通道数量
}
/** [src_zlg118_hw_adtim_trig_adc_sqr] */

/* end of file */
