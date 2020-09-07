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
 * \brief TIM3 定时器 PWM 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. PIOB_4(TIM3_CH1)输出 2KHz 的 PWM，占空比为 50%；
 *
 * \par 源代码
 * \snippet demo_aml165_core_std_tim3_pwm.c src_aml165_core_std_tim3_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-22  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_aml165_core_std_tim3_pwm
 * \copydoc demo_aml165_core_std_tim3_pwm.c
 */

/** [src_aml165_core_std_tim3_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg_tim_pwm.h"
#include "demo_std_entries.h"
#include "am_aml165_inst_init.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_std_tim3_pwm_entry (void)
{
    AM_DBG_INFO("demo aml165_core std tim3 pwm!\r\n");

    demo_std_timer_pwm_entry(am_zml165_tim3_pwm_inst_init(), AM_ZLG_TIM_PWM_CH1);
}
/** [src_aml165_core_std_tim3_pwm] */

/* end of file */
