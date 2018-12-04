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
 * \brief LPTMR实现标准定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED0以10Hz的频率闪烁。
 *
 * \note LPTMR时钟源有4种选择，在am_kl26_hwconfig_lptmr_timing.c中进行配置，选择不同大小
 *       的时钟源，定时的范围和精度存在差别。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_lptmr_timing.c src_amks16z_core_std_lptmr_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_amks16z_core_if_std_lptmr_timing
 * \copydoc demo_amks16z_core_std_lptmr_timing.c
 */

/** [src_amks16z_core_std_lptmr_timing] */
#include "am_kl26_inst_init.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_fsl_lptmr_timing.h"
#include "am_board.h"
#include "demo_std_entries.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief AMetal 应用程序入口
 */
void demo_amks16z_core_std_lptmr_timing_entry (void)
{
    am_timer_handle_t lptmr_timing_handle;

    lptmr_timing_handle = am_kl26_lptmr_timing_inst_init();

    demo_std_timer_timing_entry(lptmr_timing_handle, 0);
}

/** [src_amks16z_core_std_lptmr_timing] */

/* end of file */
