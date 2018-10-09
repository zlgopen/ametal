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
 * \brief TPM实现PWM标准接口例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 定时器通道对应的引脚输出频率为 2KHz，占空比为 50% 的 PWM 波
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_tpm_pwm.c src_amks16z_core_std_tpm_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_amks16z_core_if_std_tpm_pwm
 * \copydoc demo_amks16z_core_std_tpm_pwm.c
 */

/** [src_amks16z_core_std_tpm_pwm] */
#include "am_kl26_inst_init.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_board.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_std_tpm_pwm_entry (void)
{

    /* 初始化定时器0为PWM功能 */
    am_pwm_handle_t tpm0_pwm_handle = am_kl26_tpm0_pwm_inst_init();

    demo_std_timer_pwm_entry(tpm0_pwm_handle, 0);
}

/** [src_amks16z_core_std_tpm_pwm] */

/* end of file */
