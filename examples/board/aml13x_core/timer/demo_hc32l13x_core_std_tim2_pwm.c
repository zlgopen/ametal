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
 *   1. PIOA_7(TIM2_CH0)输出 2KHz 的 PWM，占空比为 50%；
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *    2. 由于 TIM2 默认初始化并作为系统滴答使用，使用本 Demo 之前必须在
 *       am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE、AM_CFG_SOFTIMER_ENABLE
 *       和 AM_CFG_SYSTEM_TICK_ENABLE 定义为 0。
 *
 * \par 源代码
 * \snippet demo_hc32l13x_std_tim2_pwm.c src_hc32l13x_std_tim2_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-23  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l13x_std_tim2_pwm
 * \copydoc demo_hc32l13x_std_tim2_pwm.c
 */

/** [src_hc32l13x_std_tim2_pwm] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32_tim_pwm.h"
#include "am_hc32l13x_inst_init.h"
#include "demo_std_entries.h"
#include "demo_aml13x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32l13x_core_std_tim2_pwm_entry (void)
{

    AM_DBG_INFO("demo aml13x_core std tim2 pwm!\r\n");

    demo_std_timer_pwm_entry(am_hc32_tim2_pwm_inst_init(), AM_HC32_TIM_PWM_CH0A);
}
/** [src_hc32l13x_std_tim2_pwm] */

/* end of file */
