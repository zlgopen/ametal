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
 * \brief KL26 核心板
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-23  nwt, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_prj_config.h"

#include "am_delay.h"
#include "am_led.h"
#include "am_buzzer_pwm.h"
#include "am_timer.h"

/** \brief LED 索引ID号 */
#define LED0          0
#define LED1          1

#if (AM_CFG_BUZZER_ENABLE == 1)

/** 
 * \brief TPM_PWM 的handle
 *
 * 当使用蜂鸣器时，默认将使用TPM0的OUT2输出PWM波形，因此已经将TPM0初始化为PWM功能，
 * 由于TPM0以输出六路PWM，因此可以使用该handle，以便使用其它5路PWM。
 */
extern am_pwm_handle_t g_sct0_pwm_handle;

#endif

#if (AM_CFG_SOFTIMER_ENABLE == 1)

/** 
 * \brief PIT 的handle
 *
 * 当使用软件定时器时，默认将使用PIT作为基础定时单元，因此已经将PIT初始化为定时器功能，

 */
extern am_timer_handle_t g_mrt_handle;

#endif


/**
 * \brief 板级初始化(kl26)
 * \return 无
 * \note 该函数会初始化系统时钟、GPIO、中断、蜂鸣器、LED和延时函数
 */
void am_board_init(void);
    
/* end of file */
