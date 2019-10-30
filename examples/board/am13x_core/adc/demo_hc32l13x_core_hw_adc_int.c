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
 * \snippet demo_zlg118_hw_adc_int.c src_zlg118_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-25  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_adc_int
 * \copydoc demo_zlg118_hw_adc_int.c
 */

/** [src_zlg118_hw_adc_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg118.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "demo_zlg_entries.h"
#include "demo_am118_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg118_core_hw_adc_int_entry (void)
{
    AM_DBG_INFO("demo am118_core hw adc int!\r\n");

    /* 配置引脚 */
    am_gpio_pin_cfg(PIOA_3, PIOA_3_GPIO | PIOA_3_AIN);

    /* 使能时钟 */
    am_clk_enable(CLK_ADC_BGR);

    demo_zlg118_hw_adc_int_entry(ZLG118_ADC,
                                 INUM_ADC_DAC,
                                 AMHW_ZLG118_CHAN_AIN3_PA3,
                                 3300);
}
/** [src_zlg118_hw_adc_int] */

/* end of file */
