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
 * \brief 定时器 PWM 输出功能，通过标准接口实现
 *
 * - 实验现象：
 *   1. 定时器通道对应的引脚输出频率为 2KHz，占空比为 50% 的 PWM 波
 *
 * \par 源代码
 * \snippet demo_std_timer_pwm.c src_std_timer_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-22  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_std_timer_pwm
 * \copydoc demo_std_timer_pwm.c
 */

/** [src_std_timer_pwm] */
#include "ametal.h"
#include "am_pwm.h"

/**
 * \brief 例程入口
 */
void demo_std_timer_pwm_entry (am_pwm_handle_t pwm_handle, int pwm_chan)
{
    am_pwm_config(pwm_handle, pwm_chan, 10000000 / 2, 10000000); /* 频率为 1000000000 / 10000000 = 100Hz */
    am_pwm_enable(pwm_handle, pwm_chan);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_timer_pwm] */

/* end of file */
