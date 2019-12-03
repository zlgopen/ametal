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
 *   1. 将 J14 的 KEY 和 PIOC_7 短接在一起。
 *
 * - 实验现象：
 *   1. 按下按键 KEY/RES，串口输出 "the gpio interrupt happen!"。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_mm32l373_std_gpio_trigger.c src_mm32l373_std_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-17  nwt, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l373_std_gpio_trigger
 * \copydoc demo_mm32l373_std_gpio_trigger.c
 */

/** [src_mm32l373_std_gpio_trigger] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_mm32l373.h"
#include "demo_std_entries.h"
#include "demo_mm32l373_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_mm32l373_core_std_gpio_trigger_entry (void)
{
    AM_DBG_INFO("demo mm32l373_core std gpio trigger!\r\n");

    demo_std_gpio_trigger_entry(PIOC_7);
}
/** [src_mm32l373_std_gpio_trigger] */

/* end of file */
