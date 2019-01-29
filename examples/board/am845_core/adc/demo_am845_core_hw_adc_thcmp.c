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
 * \brief ADC 阈值比较例程，通过 HW 层接口实现
 *
 * - 本例程展示了 2 种阈值比较：
 *   1. 阈值比较 0: 范围比较；
 *   2. 阈值比较 1: 穿越比较。
 *
 * - 操作步骤： (由于没有外围的电路调节电压，该demo占时无法使用)
 *   1. PIO0_7(ADC 通道 0) 连接模拟输入 1；
 *   2. PIO0_6(ADC 通道 1) 连接模拟输入 2；
 *   4. 将两个通道的模拟输入电压分别从 0 调到 2500mV。
 *
 * - 实验现象：
 *   1. 模拟输入 1 的电压低于 1100mV 或高于 2000mV 时，串口会输出电压值及比较结果；
 *   2. 模拟输入 2 的电压变化经过 1100mV 时，串口会输出电压值及比较结果。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *    PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_adc_thcmp.c src_am845_core_hw_adc_thcmp
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-07-23  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_adc_thcmp
 * \copydoc demo_am845_core_hw_adc_thcmp.c
 */

/** [src_am845_core_hw_adc_thcmp] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_adc_thcmp_entry (void)
{

    am_kprintf("demo am845_core hw adc thcmp!\r\n");

    /* 平台初始化 */
    amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_ADC0);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_ADC);

    /* 配置通道，使用通道 0 和通道 1，引脚配置为消极模式 INACTIVE */
    am_gpio_pin_cfg(PIO0_7, PIO0_7_ADC_0 | PIO0_7_INACTIVE);
    am_gpio_pin_cfg(PIO0_6, PIO0_6_ADC_1 | PIO0_6_INACTIVE);


    demo_lpc845_hw_adc_thcmp_entry(LPC84X_ADC0,
                                   INUM_ADC0_THCMP, 
                                   2500);
}
/** [src_am845_core_hw_adc_thcmp] */

/* end of file */
