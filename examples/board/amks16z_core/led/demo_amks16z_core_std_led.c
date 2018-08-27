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
 * \brief LED例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 短接J9/J10跳线帽，PIOC_4/PIOC_8控制LED0/LED1;
 *   2. 需要在am_prj_config.h头文件里使能AM_CFG_LED_ENABLE。
 *
 * - 实验现象：
 *   1. LED以0.3s的时间间隔闪烁。
 * 
 * \par 源代码
 * \snippet demo_amks16z_core_std_led.c src_amks16z_core_std_led
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-09-21  nwt, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_amks16z_core_if_std_led
 * \copydoc demo_amks16z_core_std_led.c
 */

/** [src_amks16z_core_std_led] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_std_led_entry (void)
{
    AM_DBG_INFO("demo amks16z_core std led!\r\n");

    demo_std_led_entry(LED0);
}

/** [src_amks16z_core_std_led] */

/* end of file */
