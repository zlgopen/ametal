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
 * \brief 蜂鸣器用户配置文件（PWM驱动）
 * \sa am_hwconf_buzzer_pwm.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-20  pea, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_pwm.h"
#include "am_buzzer_pwm.h"
#include "am_zlg237_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_buzzer_pwm
 * \copydoc am_hwconf_buzzer_pwm.c
 * @{
 */

/** \brief 蜂鸣器的 PWM 频率，2500 代表 2500Hz */
#define __BUZZER_PWM_FREQ       2500

/** \brief PWM 的占空比，50 代表 50% */
#define __BUZZER_PWM_DUTY       50

/** \brief 使用通道2 */
#define __BUZZER_TIMER_CHAN     2

/** \brief 蜂鸣器实例初始化 */
am_pwm_handle_t am_buzzer_pwm_inst_init (void)
{
    am_pwm_handle_t handle = am_zlg237_tim3_pwm_inst_init();

    if (NULL != handle) {
        am_buzzer_pwm_init(handle,
                           __BUZZER_TIMER_CHAN,
                           10000000 * __BUZZER_PWM_DUTY / __BUZZER_PWM_FREQ,
                           1000000000 / __BUZZER_PWM_FREQ);
    }

    return handle;
}

/**
 * @}
 */

/* end of file */
