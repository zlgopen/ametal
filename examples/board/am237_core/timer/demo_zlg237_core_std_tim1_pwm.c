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
 *   1. PIOA_8(TIM1_CH1)输出 2KHz 的 PWM，占空比为 50%；
 *
 * \par 源代码
 * \snippet demo_zlg237_std_tim1_pwm.c src_zlg237_std_tim1_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-28  zcb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg237_std_tim1_pwm
 * \copydoc demo_zlg237_std_tim1_pwm.c
 */

/** [src_zlg237_std_tim1_pwm] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg_tim_pwm.h"
#include "am_zlg237_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg237_core_std_tim1_pwm_entry (void)
{

    AM_DBG_INFO("demo am237_core std tim1 pwm!\r\n");

    demo_std_timer_pwm_entry(am_zlg237_tim1_pwm_inst_init(), AM_ZLG_TIM_PWM_CH1);
}
/** [src_zlg237_std_tim1_pwm] */

/* end of file */
