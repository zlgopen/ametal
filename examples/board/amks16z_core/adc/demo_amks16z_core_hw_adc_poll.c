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
 * \brief ADC例程，利用查询方式获取软件触发的ADC转换结果，通过HW层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（参考电压的大小和芯片VREF管脚连接的电平有关，此处的参考电压为2.5V）；
 *   4. PIOE_20(ADC通道0，单通道输入) 连接模拟输入。
 *
 * - 实验现象：
 *   1. ADC模块以444.44kHz采样率(总线时钟为24MHz)，进行16位采样;
 *   2. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_adc_poll.c src_amks16z_core_hw_adc_poll
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-14  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_adc_poll
 * \copydoc demo_amks16z_core_hw_adc_poll.c
 */

/** [src_amks16z_core_hw_adc_poll] */
#include "demo_fsl_entrys.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_board.h"
#include "hw/amhw_kl26_sim.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief ADC查询获取采样值，使用HW层接口函数
 * \return 无
 */
void demo_amks16z_core_hw_adc_poll_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw adc poll!\r\n");

    /* 配置ADC 通道0模拟输入引脚 */
    am_gpio_pin_cfg(PIOE_20, PIOE_20_ADC0_SE0);

    /* 使能ADC时钟                  */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_ADC0);

    demo_fsl_hw_adc_poll_entry(KL26_ADC0,
                               AMHW_FSL_ADC_CHAN_0,
                               CLK_BUS,
                               2500);
}
/** [src_amks16z_core_hw_adc_poll] */

/* end of file */
