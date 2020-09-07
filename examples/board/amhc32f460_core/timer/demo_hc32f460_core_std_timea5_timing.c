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
 * \brief TIMA5 定时器 TIMING 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED0 以 5Hz 的频率闪烁。(计数周期是10Hz，led翻转周期为10/2=5Hz)
 *   2. Channel_num需要与硬件配置文件am_hwconf_hc32f460_tim_timing.c一致
 * \par 源代码
 * \snippet demo_hc32f460_std_tim0_timing.c src_hc32f460_std_tim0_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_std_tim0_timing
 * \copydoc demo_hc32f460_std_tim0_timing.c
 */

/** [src_hc32f460_std_tim0_timing] */

#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_std_timea5_timing_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core std timea5 timing!\r\n");

    demo_std_timer_timing_entry(am_hc32f460_timea5_timing_inst_init(), 0);
}
/** [src_hc32f460_std_tim0_timing] */

/* end of file */
