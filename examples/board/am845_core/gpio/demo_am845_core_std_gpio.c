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
 * - 实验现象：
 *   1. 间隔 0.5s，LED0 闪烁 5 次；
 *   2. 之后，LED0 以 0.2s 的间隔一直闪烁。
 *
 * \par 源代码
 * \snippet demo_am845_core_std_gpio.c src_am845_core_std_gpio
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-10  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_gpio
 * \copydoc demo_am845_core_std_gpio.c
 */

/** [src_am845_core_std_gpio] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_lpc84x_gpio.h"
#include "demo_std_entries.h"

/** \brief LED0 引脚 */
#define __LED0_PIN  PIO0_0

/** \brief 按键引脚 */
#define __KEY_PIN  PIO0_4

/**
 * \brief 例程入口
 */
void demo_am845_core_std_gpio_entry (void)
{

    AM_DBG_INFO("demo am845_core std gpio!\r\n");

    demo_std_gpio_entry(__KEY_PIN, __LED0_PIN);
}
/** [src_am845_core_std_gpio] */

/* end of file */
