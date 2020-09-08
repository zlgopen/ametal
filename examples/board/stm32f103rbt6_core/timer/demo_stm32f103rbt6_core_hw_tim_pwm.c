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
 *   1. PIOB_11(TIM2_CH4) 输出  5HZ 的 PWM，占空比为 50%；
 *   2. LED0 以 200ms 间隔进行翻转。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIOC_9 控制。
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_hw_tim_pwm.c src_stm32f103rbt6_hw_tim_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-04  zcb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_stm32f103rbt6_hw_tim_pwm
 * \copydoc demo_stm32f103rbt6_hw_tim_pwm.c
 */

/** [src_stm32f103rbt6_hw_tim_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_stm32f103rbt6.h"
#include "am_stm32f103rbt6_clk.h"
#include "am_stm32f103rbt6_tim_pwm.h"
#include "am_stm32f103rbt6_inst_init.h"
#include "demo_stm32f103rbt6_entries.h"
#include "demo_stm32f103rbt6_core_entries.h"

/**
 * \brief 定时器PWM输出HW层例程入口
 */
void demo_stm32f103rbt6_core_hw_tim_pwm_entry (void)
{

    AM_DBG_INFO("demo stm32f103rbt6_core hw tim cmp toggle!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOB_11, PIOB_11_TIM2_CH4_REMAP2 | PIOB_11_AF_PP);

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM2);

    /* 复位定时器 */
    am_stm32f103rbt6_clk_reset(CLK_TIM2);

    demo_stm32f103rbt6_hw_tim_pwm_entry(STM32F103RBT6_TIM2,
                              AMHW_STM32F103RBT6_TIM_TYPE1,
                              AM_STM32F103RBT6_TIM_PWM_CH4,
                              am_clk_rate_get(CLK_TIM2),
                              INUM_TIM2);
}
/** [src_stm32f103rbt6_hw_tim_pwm] */

/* end of file */
