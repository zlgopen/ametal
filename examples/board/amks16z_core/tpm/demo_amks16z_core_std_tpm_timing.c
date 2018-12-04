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
 * \brief TPM实现标准定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED以10Hz的频率闪烁。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_tpm_timing.c src_amks16z_core_std_tpm_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_amks16z_core_if_std_tpm_timing
 * \copydoc demo_amks16z_core_std_tpm_timing.c
 */

/** [src_amks16z_core_std_tpm_timing] */
#include "am_kl26_inst_init.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_board.h"
#include "demo_std_entries.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_std_tpm_timing_entry (void)
{

    am_timer_handle_t tpm_timing_handle;

    tpm_timing_handle = am_kl26_tpm0_timing_inst_init();

    demo_std_timer_timing_entry(tpm_timing_handle, 0);
}

/** [src_amks16z_core_std_tpm_timing] */

/* end of file */
