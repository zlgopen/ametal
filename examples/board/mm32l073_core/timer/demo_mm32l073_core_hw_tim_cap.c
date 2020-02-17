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
 *   1. 使用杜邦线，将 PIOA_0 与 PIOA_6 连接。
 *
 * - 实验现象：
 *   1. TIM2 通过 PIOA_0 引脚输出 2KHz 的 PWM；
 *   2. TIM3 捕获输入通道 1 使用 PIOA_6 引脚捕获；
 *   3. 串口打印出利用捕获功能得到的 PWM 信号的周期和频率。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_hw_tim_cap.c src_mm32l073_core_hw_tim_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 16-04-22  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_hw_tim_cap
 * \copydoc demo_mm32l073_core_hw_tim_cap.c
 */

/** [src_mm32l073_core_hw_tim_cap] */
#include "ametal.h"
#include "am_clk.h"
#include "am_pwm.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_mm32l073.h"
#include "am_mm32l073_clk.h"
#include "am_mm32l073_inst_init.h"
#include "demo_mm32_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_mm32l073_core_hw_tim_cap_entry (void)
{
    am_pwm_handle_t pwm_handle = am_mm32l073_tim2_pwm_inst_init();

    AM_DBG_INFO("demo mm32l073_core hw tim cap!\r\n");

    /* TIM2 输出频率为 2KHz 的 PWM */
    am_pwm_config(pwm_handle, 0, 500000 / 2, 500000);
    am_pwm_enable(pwm_handle, 0);

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_6, PIOA_6_TIM3_CH1 | PIOA_6_INPUT_FLOAT);

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM3);

    /* 复位定时器 */
    am_mm32l073_clk_reset(CLK_TIM3);

    demo_mm32_hw_tim_cap_entry(MM32L073_TIM3,
                              AMHW_MM32_TIM_TYPE1,
                              0,
                              am_clk_rate_get(CLK_TIM3),
                              INUM_TIM3);
}
/** [src_mm32l073_core_hw_tim_cap] */

/* end of file */
