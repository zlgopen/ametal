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
 * \brief 延时例程，通过标准接口实现
 *
 * - 实验现象:
 *   1. LED0 闪烁频率 10Hz。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIOC_9 控制。
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_std_delay.c src_stm32f103rbt6_std_delay
 *
 * \internal
 * \par Modification History
 * - 1.00 14-12-04  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_stm32f103rbt6_std_delay
 * \copydoc demo_stm32f103rbt6_std_delay.c
 */

/** [src_stm32f103rbt6_std_delay] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_stm32f103rbt6_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_core_std_delay_entry (void)
{
    AM_DBG_INFO("The demo for delay, "
                "The delay is implement through SYSTICK\r\n");

    demo_std_delay_entry(LED0);
}
/** [src_stm32f103rbt6_std_delay] */

/* end of file */
