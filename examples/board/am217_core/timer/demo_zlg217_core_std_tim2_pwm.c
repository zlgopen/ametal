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
 * \brief TIM2 定时器 PWM 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. PIOA_15(TIM2_CH1)输出 4KHz 的 PWM，占空比为 50%；
 *
 * \par 源代码
 * \snippet demo_zlg217_std_tim2_pwm.c src_zlg217_std_tim2_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-28  zcb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg217_std_tim2_pwm
 * \copydoc demo_zlg217_std_tim2_pwm.c
 */

/** [src_zlg217_std_tim2_pwm] */

#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "am_zlg217_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_zlg217_core_std_tim2_pwm_entry (void)
{
    AM_DBG_INFO("demo am217_core std tim2 pwm!\r\n");

    demo_std_timer_pwm_entry(am_zlg217_tim2_pwm_inst_init(), 0);
}
/** [src_zlg217_std_tim2_pwm] */

/* end of file */
