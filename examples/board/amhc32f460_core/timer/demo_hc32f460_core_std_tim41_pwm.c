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
 * \brief TIM4 定时器 PWM 例程，通过标准接口实现
 *
 * - 实验现象：
 *   0. 本文件配置项TIMER4_PWM_U需要与hwconf_hc32f460_tim4_pwm配置相对应
 *   1. PIOE_9(TIMER4_PWM_U)输出 2KHz 的 PWM，占空比为 50%；
 *   2. PIOE_8(TIMER4_PWM_U)输出 2KHz 的 PWM，占空比为 50%；
 * \par 源代码
 * \snippet demo_hc32f460_std_timea1_pwm.c src_hc32f460_std_tim3_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32lf460_std_timea1_pwm
 * \copydoc demo_hc32f460_std_timea1_pwm.c
 */

/** [src_hc32f460_std_tim3_pwm] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f460_tim4_pwm.h"
#include "am_hc32f460_inst_init.h"
#include "demo_std_entries.h"
#include "demo_hc32f460_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_std_tim41_pwm_entry (void)
{

    AM_DBG_INFO("demo hc32f460_core std tim41 pwm!\r\n");

    demo_std_timer_pwm_entry(am_hc32f460_tim41_pwm_inst_init(), TIMER4_PWM_U);
}
/** [src_hc32f460_std_tim3_pwm] */

/* end of file */
