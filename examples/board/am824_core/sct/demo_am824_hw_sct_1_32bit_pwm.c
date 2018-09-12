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
 * \brief SCT 32 位 PWM 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. PIO0_23(SCT_OUT0) 输出 4kHz 的 PWM，占空比为 50%；
 *   2. PIO0_27(SCT_OUT4) 输出 4kHz 的 PWM，占空比为 25%；
 *   3. LED0 以 0.2s 的时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. SCT 使用 32 位计数器，只能产生一组独立的 PWM，所有 PWM 输出使用同一频率，
 *       计数器和最大匹配值为 0xFFFFFFFF；
 *    3. SCT 使用 16 位计数器，可以产生 2 组独立的 PWM，每一组 PWM 的输出使用同一
 *       频率，计数器和最大匹配值为 0xFFFF；
 *    4. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_am824_hw_sct_1_32bit_pwm.c src_am824_hw_sct_1_32bit_pwm
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-19  hgo, modified
 * - 1.00 15-07-13  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_sct_1_32bit_pwm
 * \copydoc demo_am824_hw_sct_1_32bit_pwm.c
 */

/** [src_am824_hw_sct_1_32bit_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc82x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_hw_sct_1_32bit_pwm_entry (void)
{
    uint32_t frq = 0;
  
    AM_DBG_INFO("demo am824_core 1_32bit hw pwm\r\n");

    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SCT);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);

    /* PIO0_23 关联到 SCT 的输出通道 0、PIO0_27 关联到 SCT 的输出通道 4 */
    am_gpio_pin_cfg(PIO0_23, PIO_FUNC_SCT_OUT0);
    am_gpio_pin_cfg(PIO0_27, PIO_FUNC_SCT_OUT4);

    AM_DBG_INFO("The demo for SCT output PWM:\r\n");
    AM_DBG_INFO("The PIO0_23(SCT_OUT0) PWM: freq is 4kHz, "
                "The duty ratio is 50%% \r\n");
    AM_DBG_INFO("The PIO0_27(SCT_OUT4) PWM: freq is 4kHz, "
                "The duty ratio is 25%% \r\n");

    frq = amhw_lpc82x_clk_system_clkrate_get();
    demo_lpc_hw_sct_1_32bit_pwm_entry(LPC82X_SCT0, frq);

}
/** [src_am824_hw_sct_1_32bit_pwm] */

/* end of file */
