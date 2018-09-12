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
 * - 实验现象：
 *   1. 间隔 0.5s，LED0 闪烁 5 次；
 *   2. 之后，LED0 以 0.2s 的间隔一直闪烁。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_am824_hw_gpio.c src_am824_hw_gpio
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_gpio
 * \copydoc demo_am824_hw_gpio.c
 */

/** [src_am824_hw_gpio] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc82x_gpio.h"
#include "demo_nxp_entries.h"

/** \brief LED0 引脚 */
#define __LED0_PIN  PIO0_20

/**
 * \brief 例程入口
 */
void demo_am824_hw_gpio_entry (void)
{
  
    am_kprintf("demo am824_core hw gpio!\r\n");
  
    /* 配置引脚方向为输出 */
    amhw_lpc82x_gpio_pin_dir_output(LPC82X_GPIO, __LED0_PIN);

    demo_lpc824_hw_gpio_entry(LPC82X_GPIO, __LED0_PIN);
}
/** [src_am824_hw_gpio] */

/* end of file */
