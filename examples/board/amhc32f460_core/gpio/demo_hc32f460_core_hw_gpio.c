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
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *
 * - 实验现象：
 *   1. 按一次按键 LED0 灯熄灭，再按一次按键 LED0 灯亮，如此反复。
 *
 *
 * \par 源代码
 * \snippet demo_hc32f460_core_hw_gpio.c src_hc32f460_core_hw_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 20-01-16  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_core_hw_gpio
 * \copydoc demo_hc32f460_core_hw_gpio.c
 */

/** [src_hc32f460_core_hw_gpio] */
#include <hc32f460_pin.h>
#include <hw/amhw_hc32f460_gpio.h>
#include "ametal.h"
#include "am_clk.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "demo_hc32f460_core_entries.h"
#include "demo_hc32f460_entries.h"

#define INPUT_PIN  PIOD_3 /**< \brief 输入引脚 */
#define OUTPUT_PIN PIOE_6 /**< \brief 输出引脚 */

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_gpio_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core hw gpio!\r\n");

    demo_hc32f460_hw_gpio_entry((void*)HC32F460_GPIO, INPUT_PIN, OUTPUT_PIN);
}
/** [src_hc32f460_core_hw_gpio] */

/* end of file */
