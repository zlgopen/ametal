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
 * \snippet demo_lpc_drv_sct_timing_pwm.c src_lpc_drv_sct_timing_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  hgo, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_drv_sct_timing_pwm
 * \copydoc demo_lpc_drv_sct_timing_pwm.c
 */

/** [src_lpc_drv_sct_timing_pwm] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc_sct.h"


/**
 * \brief SCT 事件中断
 *
 * \param[in] p_arg 使用 (#AM_LPC_SCT_EVT_OUT_INT) 中断回调宏指定的回调参数
 *
 * return 无
 */
am_local void __sct_evt_irq (void *p_arg)
{
    am_led_toggle(LED0);    /* 翻转 LED0 状态 */
}

void demo_lpc_drv_sct_timing_pwm_entry (am_lpc_sct_handle_t sct_handle)
{
    uint32_t            period_mat_val    = 0;    /* 周期匹配值   */
    uint32_t            duty_mat_val      = 0;    /* 占空比匹配值 */
    am_lpc_sct_evt_t    lpc_sct_evt;           /* SCT 事件     */
    /* 事件初始化 */
    am_lpc_sct_evt_init(&lpc_sct_evt);

    /* 获取 500Hz 时的配置值, 500Hz 周期值为 2000000ns */
    period_mat_val = am_lpc_sct_time_to_match(sct_handle, 2000000);

    /* 配置一个事件用于产生 500Hz 的中断，以提供 PWM 周期 */
    am_lpc_sct_evt_build(&lpc_sct_evt,

                          /* 使用匹配通道 0，设定匹配值，只检测匹配条件 */
                          AM_LPC_SCT_EVT_COND_MAT(0, period_mat_val) |
                          AM_LPC_SCT_EVT_COND_OPR_MATCH,

                          /* 该事件在状态 0 下可发生 */
                          AM_LPC_SCT_EVT_STAT_EN(0),

                          /* 该事件发生后，SCT 状态不变 */
                          AM_LPC_SCT_EVT_OUT_STAT_ADD(0),

                          /* 该事件发生时，SCT 计数器清零 */
                          AM_LPC_SCT_EVT_OUT_CNT_LIMIT,

                          /* 该事件不使用 DMA 功能 */
                          AM_LPC_SCT_EVT_FUNCS_NOT_USE,

                          /* 该事件控制输出 IO 通道 0 置位 */
                          AM_LPC_SCT_EVT_OUT_IO_SET(0),

                          /* 该事件产生中断，并设置它的回调函数和参数 */
                          AM_LPC_SCT_EVT_OUT_INT(__sct_evt_irq, NULL));

    /* 上述事件编号为 0， 并添加到 SCT */
    am_lpc_sct_evt_add(sct_handle, 0, &lpc_sct_evt);

    /* 获取占 500Hz 时的 25% 周期值的匹配值，即时间为(2000000 * 25% = 500000)ns */
    duty_mat_val = am_lpc_sct_time_to_match(sct_handle, 500000);

    /* 配置一个事件用于产生 500Hz 的中断，以及提供 PWM 周期 */
    am_lpc_sct_evt_build(&lpc_sct_evt,

                          /* 使用匹配通道 1，设定匹配值，只检测匹配条件 */
                          AM_LPC_SCT_EVT_COND_MAT(1, duty_mat_val) |
                          AM_LPC_SCT_EVT_COND_OPR_MATCH,

                          /* 该事件在状态 0 下可发生 */
                          AM_LPC_SCT_EVT_STAT_EN(0),

                          /* 该事件发生后，SCT 状态不变 */
                          AM_LPC_SCT_EVT_OUT_STAT_ADD(0),

                          /* 该事件不控制计数器 */
                          AM_LPC_SCT_EVT_FUNCS_NOT_USE,

                          /* 该事件不使用 DMA 功能 */
                          AM_LPC_SCT_EVT_FUNCS_NOT_USE,

                          /* 该事件控制输出 IO 通道 0 清零 */
                          AM_LPC_SCT_EVT_OUT_IO_CLR(0),

                          /* 该事件不产生中断 */
                          AM_LPC_SCT_EVT_FUNCS_NOT_USE);

    /* 上述事件编号为 1， 并添加到 SCT */
    am_lpc_sct_evt_add(sct_handle, 1, &lpc_sct_evt);

    /**
     * 因为事件 0 和事件 1 会引起输出 IO 通道 0 的电平冲突，所以需指定冲突解决方法
     * 该冲突发生时，经过一个 PWM 周期，IO 需要重新拉高
     */
    am_lpc_sct_conflict_res_config(sct_handle,
                                   0,
                                   AM_LPC_SCT_RES_OUT_SET);

    /* 启动 SCT */
    am_lpc_sct_start(sct_handle);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_lpc_drv_sct_timing_pwm] */

/* end of file */
