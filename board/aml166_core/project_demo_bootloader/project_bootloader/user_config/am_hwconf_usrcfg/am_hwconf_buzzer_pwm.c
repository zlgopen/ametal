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
 * \brief 蜂鸣器配置文件（PWM驱动）
 * \sa am_hwconf_buzzer_pwm.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-20  pea, first implementation
 * \endinternal
 */
#include <am_aml166_inst_init.h>
#include "ametal.h"
#include "am_pwm.h"
#include "am_buzzer_pwm.h"

/**
 * \addtogroup am_if_src_hwconf_buzzer_pwm
 * \copydoc am_hwconf_buzzer_pwm.c
 * @{
 */

/** \brief 蜂鸣器的 PWM 频率，2500 代表 2500Hz */
#define __BUZZER_PWM_FREQ        2500

/** \brief PWM 的占空比，50 代表 50% */
#define __BUZZER_PWM_DUTY        50

/** \brief 使用通道 1, TIM16_CH1N, PIOB_6 */
#define __BUZZER_TIMER_CHAN      (1 - 1)

/** \brief 蜂鸣器实例初始化 */
void am_buzzer_pwm_inst_init (void)
{
    am_pwm_handle_t handle = am_zlg116_tim16_pwm_inst_init();

    if (NULL != handle) {
        am_buzzer_pwm_init(handle,
                           __BUZZER_TIMER_CHAN,
                           10000000 * __BUZZER_PWM_DUTY / __BUZZER_PWM_FREQ,
                           1000000000 / __BUZZER_PWM_FREQ);
    }
}

/**
 * @}
 */

/* end of file */
