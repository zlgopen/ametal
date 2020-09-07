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
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-20  pea, first implementation
 * \endinternal
 */
#ifndef __AM_HWCONF_BUZZER_PWM_H
#define __AM_HWCONF_BUZZER_PWM_H

#include "ametal.h"
#include "am_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 蜂鸣器实例初始化
 *
 * 当使用蜂鸣器时，默认将使用 TIM16 的通道 1 输出 PWM 波形，因此会将 TIM16 初始
 * 化为PWM 功能
 */
void am_buzzer_pwm_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_BUZZER_PWM_H */

/* end of file */
