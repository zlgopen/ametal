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
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f19x_hw_gpio_trigger.c src_hc32f19x_hw_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f19x_hw_gpio_trigger
 * \copydoc demo_hc32f19x_hw_gpio_trigger.c
 */

/** [src_hc32f19x_hw_gpio_trigger] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_hc32.h"
#include "hw/amhw_hc32_gpio.h"
#include "demo_hc32_entries.h"
#include "demo_amf19x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f19x_core_hw_gpio_trigger_entry (void)
{

    AM_DBG_INFO("demo am217_core hw gpio trigger!\r\n");

    /* 开启 GPIOC 端口时钟 */
    am_clk_enable(CLK_GPIO);

    demo_hc32_hw_gpio_trigger_entry((void *)HC32_GPIO0, PIOA_7);
}
/** [src_hc32f19x_hw_gpio_trigger] */

/* end of file */
