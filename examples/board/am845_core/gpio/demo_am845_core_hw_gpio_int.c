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
 * \brief GPIO 引脚中断例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. PIO0_1 的电平由高电平改变为低电平（按键 KEY/RES 按下）时，LED0 状态改变。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_gpio_int.c src_am845_core_hw_gpio_int
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_gpio_int
 * \copydoc demo_am845_core_hw_gpio_int.c
 */

/** [src_am845_core_hw_gpio_int] */
#include "ametal.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "hw/amhw_lpc84x_gpio.h"
#include "hw/amhw_lpc82x_pint.h"
#include "hw/amhw_lpc84x_iocon.h"
#include "demo_nxp_entries.h"
#include "lpc84x_inum.h"

/** \brief LED0 引脚 */
#define __LED0_PIN  PIO0_0

/** \brief 按键引脚 */
#define __KEY_PIN  PIO0_4

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_gpio_int_entry (void)
{

    am_kprintf("demo am845 hw gpio int!\r\n");

    /* 配置 LED 引脚方向为输出 */
    amhw_lpc84x_gpio_pin_dir_output(LPC84X_GPIO, __LED0_PIN);

    /* 配置 KEY 引脚方向为输入 */
    amhw_lpc84x_gpio_pin_dir_input(LPC84X_GPIO, __KEY_PIN);

    /* 配置 KEY 引脚为上拉 */
    amhw_lpc84x_iocon_mode_set(LPC84X_IOCON, __KEY_PIN,
                               AMHW_LPC84X_IOCON_MODE_PULLUP);

    /* 配置为下降沿触发 */
    amhw_lpc82x_pint_trigger_set(LPC84X_PINT,
                                 AMHW_LPC82X_PINT_CHAN_0,
                                 AMHW_LPC82X_PINT_TRIGGER_FALL);

    demo_lpc845_hw_gpio_int_entry(__KEY_PIN,
                                  __LED0_PIN,
                                  AMHW_LPC82X_PINT_CHAN_0,
                                  INUM_PIN_INT0);

}
/** [src_am845_core_hw_gpio_int] */

/* end of file */
