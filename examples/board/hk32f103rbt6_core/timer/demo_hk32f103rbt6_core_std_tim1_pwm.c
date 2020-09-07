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
 * \snippet demo_hk32f103rbt6_std_tim1_pwm.c src_hk32f103rbt6_std_tim1_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-28  zcb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hk32f103rbt6_std_tim1_pwm
 * \copydoc demo_hk32f103rbt6_std_tim1_pwm.c
 */

/** [src_hk32f103rbt6_std_tim1_pwm] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_hk32f103rbt6_tim_pwm.h"
#include "am_hk32f103rbt6_inst_init.h"
#include "demo_std_entries.h"
#include "demo_hk32f103rbt6_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_core_std_tim1_pwm_entry (void)
{

    AM_DBG_INFO("demo hk32f103rbt6_core std tim1 pwm!\r\n");

    demo_std_timer_pwm_entry(am_hk32f103rbt6_tim1_pwm_inst_init(), AM_HK32F103RBT6_TIM_PWM_CH1);
}
/** [src_hk32f103rbt6_std_tim1_pwm] */

/* end of file */
