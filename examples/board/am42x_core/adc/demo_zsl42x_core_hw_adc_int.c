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
 *   1. PIOA_3连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 使用 ADC 模块功能，必须保证 ADC 模块上电；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zsl42x_hw_adc_int.c src_zsl42x_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-25  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_adc_int
 * \copydoc demo_zsl42x_hw_adc_int.c
 */

/** [src_zsl42x_hw_adc_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zsl42x.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "demo_zsl42x_entries.h"
#include "demo_am42x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zsl42x_core_hw_adc_int_entry (void)
{
    AM_DBG_INFO("demo am42x_core hw adc int!\r\n");

    /* 配置引脚 */
    am_gpio_pin_cfg(PIOC_0, PIOC_0_GPIO | PIOC_0_AIN);

    /* 使能时钟 */
    am_clk_enable(CLK_ADC_BGR);

    demo_zsl42x_hw_adc_int_entry(ZSL42x_ADC,
                                 INUM_ADC_DAC,
                                 AMHW_ZSL42x_CHAN_AIN10_PC0,
                                 3300);
}
/** [src_zsl42x_hw_adc_int] */

/* end of file */
