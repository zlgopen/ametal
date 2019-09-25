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
 *   1. PIOA_4 (ADC 通道 4) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 使用 ADC 模块功能，必须保证 ADC 模块上电；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_9引脚连接 PC串口的RXD，
 *       PIOA_10引脚连接 PC串口的TXD。
 *
 * \par 源代码
 * \snippet demo_zlg237_hw_adc_int.c src_zlg237_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-28  fra, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg237_hw_adc_int
 * \copydoc demo_zlg237_hw_adc_int.c
 */

/** [src_zlg237_hw_adc_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg237.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "demo_zlg_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg237_core_hw_adc_int_entry (void)
{
    AM_DBG_INFO("demo am237_core hw adc int!\r\n");

    /* 运行硬件层例程前的环境设置 ，配置引脚，使能时钟等 */
    am_gpio_pin_cfg(PIOA_0, PIOA_0_AIN);
    am_clk_enable(CLK_ADC1);

    demo_zlg237_hw_adc_int_entry((void *)ZLG237_ADC1,
                                 INUM_ADC1_2,
                                 AMHW_ZLG237_ADC_CHAN_0,
                                 3300);
}
/** [src_zlg237_hw_adc_int] */

/* end of file */
