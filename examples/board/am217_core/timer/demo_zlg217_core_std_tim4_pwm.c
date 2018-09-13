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
 *   1. PIOB_6(TIM4_CH1)输出 4KHz 的 PWM，占空比为 50%；
 *
 * \note
 *    由于 TIM4 默认初始化并作为系统滴答使用，使用本 Demo 之前必须在
 *    am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE、AM_CFG_SOFTIMER_ENABLE
 *    和 AM_CFG_SYSTEM_TICK_ENABLE 定义为 0。
 *
 * \par 源代码
 * \snippet demo_zlg217_std_tim4_pwm.c src_zlg217_std_tim4_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-28  zcb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg217_std_tim4_pwm
 * \copydoc demo_zlg217_std_tim4_pwm.c
 */

/** [src_zlg217_std_tim4_pwm] */

#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "am_zlg217_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_zlg217_core_std_tim4_pwm_entry (void)
{
    AM_DBG_INFO("demo am217_core std tim14 pwm!\r\n");

    demo_std_timer_pwm_entry(am_zlg217_tim4_pwm_inst_init(), 0);
}
/** [src_zlg217_std_tim4_pwm] */

/* end of file */
