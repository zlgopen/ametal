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
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *    每次按键时串口打印 "the gpio interrupt happen!"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOE_4 引脚连接 PC 串口的 TXD，
 *       PIOE_5 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_gpio_trigger.c src_hc32f460_hw_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 20-01-16  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_gpio_trigger
 * \copydoc demo_hc32f460_hw_gpio_trigger.c
 */

/** [src_hc32f460_hw_gpio_trigger] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "hw/amhw_hc32f460_gpio.h"
#include "demo_hc32f460_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_gpio_trigger_entry (void)
{

    AM_DBG_INFO("demo hc32f460_core hw gpio trigger!\r\n");

    demo_hc32f460_hw_gpio_trigger_entry(HC32F460_GPIO, PIOD_3);
}
/** [src_hc32f460_hw_gpio_trigger] */

/* end of file */
