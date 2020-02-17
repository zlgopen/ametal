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
 * \brief GPIO 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 J14 的 KEY 和 PIOA_8 短接在一起。
 *
 * - 实验现象：
 *   1. 按一次按键 LED0 灯熄灭，再按一次按键 LED0 灯亮，如此反复。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIOB_1 控制。
 *
 * \par 源代码
 * \snippet demo_am159_core_std_gpio.c src_am159_core_std_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am159_core_std_gpio
 * \copydoc demo_am159_core_std_gpio.c
 */

/** [src_am159_core_std_gpio] */
#include <hc32f460_pin.h>
#include "ametal.h"
#include "am_vdebug.h"
//#include "am_zmf159.h"
//#include "demo_std_entries.h"
//#include "demo_zmf159_core_entries.h"
extern void demo_std_gpio_entry (int input_pin, int output_pin);

#define INPUT_PIN  PIOD_3 /**< \brief 输入引脚 */
#define OUTPUT_PIN PIOE_6 /**< \brief 输出引脚 */

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_std_gpio_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core std gpio!\r\n");

    demo_std_gpio_entry(INPUT_PIN, OUTPUT_PIN);
}
/** [src_am159_core_std_gpio] */

/* end of file */
