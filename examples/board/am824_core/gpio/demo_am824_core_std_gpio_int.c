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
 * - 操作步骤：
 *   1. 将 J14 的 KEY 和 PIO0_1 短接在一起。
 *
 * - 实验现象：
 *   1. PIO0_1 的电平由高电平改变为低电平（按键 KEY/RES 按下）时，LED0 状态改变。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 按键需要将 J14 跳线帽的 KEY 和 PIO0_1 短接才能使用。
 *
 * \par 源代码
 * \snippet demo_am824_std_gpio_int.c src_am824_std_gpio_int
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-10  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_gpio_int
 * \copydoc demo_am824_std_gpio_int.c
 */

/** [src_am824_std_gpio_int] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_lpc82x.h"
#include "demo_std_entries.h"

/** \brief 按键引脚 */
#define __KEY_PIN  PIO0_1

/**
 * \brief 例程入口
 */
void demo_am824_core_std_gpio_int_entry (void)
{
    AM_DBG_INFO("demo am824_core std gpio trigger!\r\n");

    demo_std_gpio_trigger_entry(__KEY_PIN);
}
/** [src_am824_std_gpio_int] */

/* end of file */
