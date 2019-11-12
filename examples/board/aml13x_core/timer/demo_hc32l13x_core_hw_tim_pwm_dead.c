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
 * \brief 通用定时器带死区的互补PWM例程，通过 HW层接口实现
 *
 * - 实验现象：
 *   1.串口打印提示信息.
 *   2.PIOA_2(TIM0_CHA)和PIOA_1(TIM0_CHB)输出占空比25%、20Hz的PWM波，死区时间为100us。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *    2. 使用示波器或逻辑分析仪测量互补PWM的死区时间。
 *    3. 死区时间应小于PWM的周期时间。死区时间的设定范围（6——2686us），如果需要设定其他死区时间，则需
 *       要更改demo_hc32l13x_hw_tim_pwm_dead.c中的分频系数，外设时钟两个参数。
 *
 * \par 源代码
 * \snippet demo_hc32l13x_hw_tim_pwm_dead.c src_hc32l13x_hw_tim_pwm_dead
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l13x_hw_tim_pwm_dead
 * \copydoc demo_hc32l13x_hw_tim_pwm_dead.c
 */

/** [src_hc32l13x_hw_tim_pwm_dead] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_hc32.h"
#include "hw/amhw_hc32_adtim.h"
#include "am_hc32l13x_inst_init.h"
#include "demo_hc32_entries.h"
#include "demo_aml13x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32l13x_core_hw_tim_pwm_dead_entry (void)
{

    AM_DBG_INFO("demo aml13x_core hw tim PWM dead!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_TIM0_CHA | PIOA_2_OUT_PP);
    am_gpio_pin_cfg(PIOA_1, PIOA_1_TIM0_CHB | PIOA_1_OUT_PP);

    /* 定时器时钟使能 */
    am_clk_enable(CLK_TIM012);

    demo_hc32_hw_tim_pwm_dead_entry((void *)HC32_TIM0,
                                       AMHW_HC32_TIM_TYPE_TIM0,
                                       HC32_TIM_CH0A,
                                       HC32_TIM_CH0B,
                                       50000 / 4,
                                       50000,
                                       100);
}
/** [src_hc32l13x_hw_tim_pwm_dead] */

/* end of file */
