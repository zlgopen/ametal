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
 * \brief TPM实现捕获功能，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 PWM 输出连接到捕获输入引脚。
 *
 * - 实验现象：
 *   1. 调试串口输出捕获到的 PWM 信号的周期和频率。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_tpm_cap.c src_amks16z_core_std_tpm_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_amks16z_core_if_std_tpm_cap
 * \copydoc demo_amks16z_core_std_tpm_cap.c
 */

/** [src_amks16z_core_std_tpm_cap] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_kl26_inst_init.h"
#include "am_board.h"
#include "am_int.h"
#include "demo_std_entries.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_std_tpm_cap_entry (void)
{

    am_cap_handle_t cap_handle;

    cap_handle = am_kl26_tpm1_cap_inst_init();

    demo_std_timer_cap_entry(cap_handle, 1);
}

/** [src_amks16z_core_std_tpm_cap] */

/* end of file */
