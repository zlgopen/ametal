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
 * \brief ADC例程，利用中断方式获取软件触发的ADC转换结果，通过HW层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（此时参考电压为2.5v）；
 *   4. PIOE_29(ADC通道0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. ADC模块以444.44kHz采样率(总线时钟为24MHz)，进行16位采样;
 *   2. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_adc_int.c src_amks16z_core_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-18  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_adc_int
 * \copydoc demo_amks16z_core_hw_adc_int.c
 */

/** [src_amks16z_core_hw_adc_int] */
#include "demo_fsl_entrys.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_board.h"
#include "hw/amhw_kl26_sim.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief 中断方式获取软件触发的ADC转换结果，通过HW层接口实现
 * \return 无
 */
void demo_amks16z_core_hw_adc_int_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw adc int!\r\n");

    /* 配置ADC 选择模拟输入引脚 */
    am_gpio_pin_cfg(PIOE_29, PIOE_29_ADC0_SE4B);

    /* 使能ADC时钟                  */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_ADC0);

    demo_fsl_hw_adc_int_entry(KL26_ADC0,
                              INUM_ADC0,
                              AMHW_FSL_ADC_CHAN_4,
                              CLK_BUS,
                              2500);
}
/** [src_amks16z_core_hw_adc_int] */

/* end of file */
