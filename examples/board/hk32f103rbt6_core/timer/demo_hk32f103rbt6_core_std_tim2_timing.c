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
 * \brief TIM2 定时器 TIMING 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED0 以 10Hz 的频率翻转。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIOC_9 控制；
 *    2. 16 位定时器定时计数值不为 65536 的倍数，则定时会有一些偏差。
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_std_tim2_timing.c src_hk32f103rbt6_std_tim2_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-25  zcb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hk32f103rbt6_std_tim2_timing
 * \copydoc demo_hk32f103rbt6_std_tim2_timing.c
 */

/** [src_hk32f103rbt6_std_tim2_timing] */
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "am_hk32f103rbt6_inst_init.h"
#include "demo_hk32f103rbt6_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_core_std_tim2_timing_entry (void)
{
    AM_DBG_INFO("demo hk32f103rbt6_core std tim2 timing!\r\n");

    demo_std_timer_timing_entry(am_hk32f103rbt6_tim2_timing_inst_init(), 0);
}
/** [src_hk32f103rbt6_std_tim2_timing] */

/* end of file */
