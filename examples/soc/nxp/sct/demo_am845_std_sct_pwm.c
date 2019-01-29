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
 * \brief SCT PWM 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. PIO0_23(SCT_OUT0) 输出 4KHz 的 PWM，占空比为 50%；
 *   2. PIO0_27(SCT_OUT4) 输出 4KHz 的 PWM，占空比为 25%；
 *   3. LED0 以 0.2s 的时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_lpc82x_std_sct_pwm.c src_lpc82x_std_sct_pwm
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-07  hgo, modified
 * - 1.00 15-07-15  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc82x_std_sct_pwm
 * \copydoc demo_lpc82x_std_sct_pwm.c
 */

/** [src_lpc82x_std_sct_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_lpc84x_std_sct_pwm_entry (void)
{
    am_pwm_handle_t sct_pwm_handle = NULL;

    AM_DBG_INFO("The SCT demo for standard pwm service\r\n");
    AM_DBG_INFO("The PIO0_23(SCT_OUT0) PWM: freq is 4kHz, "
                "The duty ratio is 50%% \r\n");

    AM_DBG_INFO("The PIO0_27(SCT_OUT4) PWM: freq is 4kHz, "
                "The duty ratio is 25%% \r\n");

    sct_pwm_handle = am_lpc84x_sct0_pwm_inst_init();

    /* 通道 0 使用 PIO0_23 */
    am_pwm_config(sct_pwm_handle, 0, 125000, 250000);
    am_pwm_enable(sct_pwm_handle, 0);

    /* 通道 4 使用 PIO0_27 */
    am_pwm_config(sct_pwm_handle, 4, 62500, 250000);
    am_pwm_enable(sct_pwm_handle, 4);

    AM_FOREVER {
        am_led_toggle(LED0);
        am_mdelay(200);
    }
}
/** [src_lpc82x_std_sct_pwm] */

/* end of file */
