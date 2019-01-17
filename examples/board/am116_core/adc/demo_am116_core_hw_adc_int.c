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
 *   1. PIOA_1 (ADC 通道 1) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 使用 ADC 模块功能，必须保证 ADC 模块上电与 BOD 模块上电；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am116_core_hw_adc_int.c src_am116_core_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-18  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am116_core_hw_adc_int
 * \copydoc demo_am116_core_hw_adc_int.c
 */

/** [src_am116_core_hw_adc_int] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zlg116.h"
#include "demo_zlg_entries.h"
#include "demo_am116_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_am116_core_hw_adc_int_entry (void)
{
    AM_DBG_INFO("demo am116_core hw adc int!\r\n");

    /* 运行硬件层例程前的环境设置 ，配置引脚，使能时钟等 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_AIN);
    am_clk_enable(CLK_ADC1);

    demo_zlg_hw_adc_int_entry(ZLG116_ADC,
                              INUM_ADC_COMP,
                              AMHW_ZLG_ADC_CHANNEL1,
                              3300);
}
/** [src_am116_core_hw_adc_int] */

/* end of file */
