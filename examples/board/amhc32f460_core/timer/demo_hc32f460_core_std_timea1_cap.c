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
 * \brief TIMA1 定时器 CAP 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 使用杜邦线，将 PIOE_2 与 PIOE_9 连接。
 *
 * - 实验现象：
 *   1. TIMEA1 通过 PIOE_2 引脚输出 2KHz 的 PWM；
 *   2. TIMEA1 捕获输入通道 2 使用 PIOE_9 引脚捕获；
 *   3. 串口打印出利用捕获功能得到的 PWM 信号的周期和频率。
 *
 * \note
 *    
 *       
 *
 * \par 源代码
 * \snippet demo_hc32f460_std_tim0_cap.c src_hc32f460_std_tim0_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_std_tim0_cap
 * \copydoc demo_hc32f460_std_tim0_cap.c
 */

/** [src_hc32f460_std_tim0_cap] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f460_timea_cap.h"
#include "am_hc32f460_inst_init.h"
#include "demo_std_entries.h"
#include "demo_hc32f460_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_std_timea1_cap_entry (void)
{
    am_pwm_handle_t timea3_pwm_handle = am_hc32f460_timea3_pwm_inst_init();
    am_cap_handle_t timea1_cap_handle = am_hc32f460_timea1_cap_inst_init();
    
    AM_DBG_INFO("demo hc32f460_core std timea1 cap!\r\n");

    /* TIM0 输出频率为 2KHz 的 PWM */
    am_pwm_config(timea3_pwm_handle, TIMERA_CH5, 500000 / 2, 500000);
    am_pwm_enable(timea3_pwm_handle, TIMERA_CH5);

    demo_std_timer_cap_entry(timea1_cap_handle, TIMERA_CH1);
}
/** [src_hc32f460_std_tim0_cap] */

/* end of file */
