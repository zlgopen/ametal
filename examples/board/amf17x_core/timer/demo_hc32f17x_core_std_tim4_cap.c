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
 * \brief 高级定时器tim4 CAP 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 使用杜邦线，将 PIOB_3 与 PIOB_2 连接。
 *
 * - 实验现象：
 *   1. TIM3 通过 PIOB_3 引脚输出 2KHz 的 PWM；
 *   2. TIM4 捕获输入通道 0 使用 PIOB_2 引脚捕获；
 *   3. 串口打印出利用捕获功能得到的 PWM 信号的周期和频率。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f17x_std_tim4_cap.c src_hc32f17x_std_tim4_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-09  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f17x_std_tim4_cap
 * \copydoc demo_hc32f17x_std_tim4_cap.c
 */

/** [src_hc32f17x_std_tim4_cap] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f17x_inst_init.h"
#include "demo_std_entries.h"
#include "demo_amf17x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f17x_core_std_tim4_cap_entry (void)
{

    am_pwm_handle_t tim3_pwm_handle = am_hc32_tim3_pwm_inst_init();
    am_cap_handle_t tim4_cap_handle = am_hc32_tim4_cap_inst_init();
    
    AM_DBG_INFO("demo amf17x_core std tim4 cap!\r\n");

    /* TIM3 输出频率为 2KHz 的 PWM */
    am_pwm_config(tim3_pwm_handle, 0, 500000 / 2, 500000);
    am_pwm_enable(tim3_pwm_handle, 0);

    demo_std_timer_cap_entry(tim4_cap_handle, 0);
}
/** [src_hc32f17x_std_tim4_cap] */

/* end of file */
