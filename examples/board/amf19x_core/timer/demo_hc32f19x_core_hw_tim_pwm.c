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
 * \brief 定时器 PWM 输出例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. PIOA_2(TIM0_CHA) 输出  10HZ 的 PWM，占空比为 50%；
 *   2. LED0 以 100ms 间隔进行翻转。
 *
 * \par 源代码
 * \snippet demo_hc32f19x_hw_tim_pwm.c src_hc32f19x_hw_tim_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f19x_hw_tim_pwm
 * \copydoc demo_hc32f19x_hw_tim_pwm.c
 */

/** [src_hc32f19x_hw_tim_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32.h"
#include "am_hc32_clk.h"
#include "am_hc32_tim_pwm.h"
#include "am_hc32f19x_inst_init.h"
#include "demo_hc32_entries.h"
#include "demo_amf19x_core_entries.h"

/**
 * \brief 定时器PWM输出HW层例程入口
 */
void demo_hc32f19x_core_hw_tim_pwm_entry (void)
{

    AM_DBG_INFO("demo am217_core hw tim cmp toggle!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_TIM0_CHA | PIOA_2_OUT_PP);

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM012);

    demo_hc32_hw_tim_pwm_entry((void *)HC32_TIM0,
                                 AMHW_HC32_TIM_TYPE_TIM0,
                                 HC32_TIM_CH0A,
                                 am_clk_rate_get(CLK_TIM012),
                                 AMHW_HC32_TIM_CLK_DIV64,
                                 INUM_TIM0);
}
/** [src_hc32f19x_hw_tim_pwm] */

/* end of file */
