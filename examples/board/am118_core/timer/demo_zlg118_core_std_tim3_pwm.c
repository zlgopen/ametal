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
 * \brief TIM1 定时器 PWM 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. PIOA_3(TIM3_CH1)输出 2KHz 的 PWM，占空比为 50%；
 *
 * \par 源代码
 * \snippet demo_zlg118_std_tim3_pwm.c src_zlg118_std_tim3_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-23  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_std_tim3_pwm
 * \copydoc demo_zlg118_std_tim3_pwm.c
 */

/** [src_zlg118_std_tim3_pwm] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg118_tim_pwm.h"
#include "am_zlg118_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am118_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg118_core_std_tim3_pwm_entry (void)
{

    AM_DBG_INFO("demo am118_core std tim3 pwm!\r\n");

    demo_std_timer_pwm_entry(am_zlg118_tim3_pwm_inst_init(), AM_ZLG118_TIM_PWM_CH1A);
}
/** [src_zlg118_std_tim3_pwm] */

/* end of file */
