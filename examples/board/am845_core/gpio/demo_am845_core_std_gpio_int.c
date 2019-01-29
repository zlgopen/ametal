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
 *   1. PIO0_4 的电平由高电平改变为低电平（按键 KEY/RES 按下）时，LED0 状态改变。
 *
 * \par 源代码
 * \snippet demo_am845_core_std_gpio_int.c src_am845_core_std_gpio_int
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-10  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_gpio_int
 * \copydoc demo_am845_core_std_gpio_int.c
 */

/** [src_am845_core_std_gpio_int] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_lpc84x.h"
#include "demo_std_entries.h"

/** \brief 按键引脚 */
#define __KEY_PIN  PIO0_4

/**
 * \brief 例程入口
 */
void demo_am845_core_std_gpio_int_entry (void)
{
    AM_DBG_INFO("demo am845_core std gpio trigger!\r\n");

    demo_std_gpio_trigger_entry(__KEY_PIN);
}
/** [src_am845_core_std_gpio_int] */

/* end of file */
