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
 * \brief SCT 32 位 PWM 例程，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. PIO0_23(SCT_OUT0) 输出 500Hz 的 PWM，占空比为 25%；
 *   2. LED0 状态翻转频率为 500Hz。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 该例程下，事件 0 每 2000000ns 将输出 IO 通道 0 置高，并产生中断，用作周期，
 *       事件 1 每 500000ns（占周期 25%）将输出 IO 通道 0 清零，用作占空比；
 *    3. 该例程下，每发生事件 0 时，事件 1 也会同时发生，此时两个事件均控制输出 IO
 *       通道 0，且产生电平冲突，此时需要制定解决冲突的办法，若不指定，将保持上次
 *       电平不变；
 *    4. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_am824_drv_sct_timing_pwm.c src_am824_drv_sct_timing_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  hgo, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_drv_sct_timing_pwm
 * \copydoc demo_am824_drv_sct_timing_pwm.c
 */

/** [src_am824_drv_sct_timing_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x_inst_init.h"
#include "demo_nxp_entries.h"


/**
 * \brief 例程入口
 */
void demo_am824_core_drv_sct_timing_pwm_entry (void)
{
    am_lpc_sct_handle_t lpc82x_sct_handle = NULL;

    AM_DBG_INFO("demo am824_core sct pwm service\r\n");
    AM_DBG_INFO("The PIO0_23(SCT_OUT0) PWM: freq is 500Hz, "
                "The duty ratio is 25%% \r\n");

    lpc82x_sct_handle = am_lpc82x_sct0_inst_init();

    /* 将 PIO0_23 关联到 SCT 的输出通道 0 */
    am_gpio_pin_cfg(PIO0_23, PIO_FUNC_SCT_OUT0);

    /* 设置 SCT 状态为 0 */
    am_lpc_sct_state_set(lpc82x_sct_handle, 0);

  
    demo_lpc_drv_sct_timing_pwm_entry(lpc82x_sct_handle);

}
/** [src_am824_drv_sct_timing_pwm] */

/* end of file */
