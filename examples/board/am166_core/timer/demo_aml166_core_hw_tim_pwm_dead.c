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
 * \brief 定时器带死区时间的互补 PWM 输出例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. PIOA_8(TIM1_CH1) 和 PIOA_7(TIM1_CH1N) 输出互补 PWM，频率为 100KHz，
 *      占空比为 40%，死区时间为 100ns。
 *
 * \par 源代码
 * \snippet demo_aml166_core_hw_tim_pwm_dead.c src_aml166_core_hw_tim_pwm_dead
 *
 * \internal
 * \par Modification history
 * - 1.00 18-06-06  pea, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_aml166_core_hw_tim_pwm_dead
 * \copydoc demo_aml166_core_hw_tim_pwm_dead.c
 */

/** [src_aml166_core_hw_tim_pwm_dead] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zlg116.h"
#include "am_zlg116_clk.h"
#include "demo_zlg_entries.h"
#include "demo_aml166_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml166_core_hw_tim_pwm_dead_entry (void)
{
    AM_DBG_INFO("demo aml166_core hw tim pwm dead!\r\n");

    /* 配置引脚功能 */
    am_gpio_pin_cfg(PIOA_7, PIOA_7_TIM1_CH1N | PIOA_7_AF_PP);
    am_gpio_pin_cfg(PIOA_8, PIOA_8_TIM1_CH1 | PIOA_8_AF_PP);

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM1);

    /* 复位定时器 */
    am_zlg116_clk_reset(CLK_TIM1);

    demo_zlg_hw_tim_pwm_dead_entry(ZLG116_TIM1,
                                   AMHW_ZLG_TIM_TYPE0,
                                   0,
                                   am_clk_rate_get(CLK_TIM1));
}
/** [src_aml166_core_hw_tim_pwm_dead] */

/* end of file */
