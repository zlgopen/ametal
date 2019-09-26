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
 * \brief AM237_CORE
 *
 * \internal
 * \par Modification history
 * - 1.00 14-12-01  tee, first implementation
 * \endinternal
 */
#ifndef __AM_BOARD_H
#define __AM_BOARD_H

#include "ametal.h"
#include "am_prj_config.h"
#include "am_delay.h"
#include "am_led.h"
#include "am_pwm.h"
#include "am_timer.h"
#include "am_vdebug.h"

/** \brief LED 索引 ID 号 */
#define LED0          0
#define LED1          1

#if (AM_CFG_BUZZER_ENABLE == 1)

/**
 * \brief 蜂鸣器使用的定时器的 handle
 *
 * 当使用蜂鸣器时，默认将使用 TIM3 的 OUT1 输出 PWM 波形，因此已经将 TIM3 初始化为
 * PWM 功能
 */
extern am_pwm_handle_t g_buzzer_pwm_handle;

#endif

#if (AM_CFG_SOFTIMER_ENABLE == 1)

/**
 * \brief 系统滴答使用的定时器的 handle
 *
 * 当使用系统滴答时，默认使用 TIM4 的通道 0 作为基础定时单元，因此已经将 TIM4 初始化
 * 为定时器功能
 */
extern am_timer_handle_t g_system_tick_timer_handle;

#endif

/**
 * \brief 板级初始化
 *
 * \return 无
 *
 * \note 该函数会初始化系统时钟、GPIO、中断、蜂鸣器、LED 和延时函数
 */
void am_board_init (void);

#endif /* __AM_BOARD_H */

/* end of file */
