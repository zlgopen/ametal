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
 * \brief 定时器 CAP 捕获例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 使用杜邦线，将 PIOB_3 与 PIOA_0 连接。
 *
 * - 实验现象：
 *   1. TIM3 通过 PIOB_3 引脚输出 2KHz 的 PWM；
 *   2. TIM1 捕获输入通道 0 使用 PIOA_0 引脚捕获；
 *   3. 串口打印出利用捕获功能得到的 PWM 信号的周期和频率。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *    2. 由于 TIM2 默认初始化并作为系统滴答使用，使用本 Demo 之前必须在
 *       am_prj_config.h 内将 AM_CFG_DELAY_ENABLE、AM_CFG_SOFTIMER_ENABLE
 *       和 AM_CFG_SYSTEM_TICK_ENABLE 定义为 0。
 *
 * \par 源代码
 * \snippet demo_hc32f17x_hw_tim_cap.c src_hc32f17x_hw_tim_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f17x_hw_tim_cap
 * \copydoc demo_hc32f17x_hw_tim_cap.c
 */

/** [src_hc32f17x_hw_tim_cap] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_hc32.h"
#include "am_hc32_clk.h"
#include "am_hc32f17x_inst_init.h"
#include "demo_hc32_entries.h"
#include "demo_amf17x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f17x_core_hw_tim_cap_entry (void)
{
    am_pwm_handle_t tim3_pwm_handle = am_hc32_tim3_pwm_inst_init();

    AM_DBG_INFO("demo amf17x_core hw tim cap!\r\n");

    /* TIM3 输出频率为 2KHz 的 PWM */
    am_pwm_config(tim3_pwm_handle, 0, 500000 / 2, 500000);

    am_pwm_enable(tim3_pwm_handle, 0);

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_0,  PIOA_0_TIM1_CHA  | PIOA_0_INPUT_FLOAT);

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM012);

    demo_hc32_hw_tim_cap_entry(HC32_TIM1,
                               HC32_TIM_CH0A,
                               am_clk_rate_get(CLK_TIM012),
                               INUM_TIM1);
}
/** [src_hc32f17x_hw_tim_cap] */

/* end of file */
