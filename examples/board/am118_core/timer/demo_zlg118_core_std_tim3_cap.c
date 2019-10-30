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
 * \brief TIM1 定时器 CAP 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 使用杜邦线，将 PIOA_2 与 PIOB_3 连接。
 *
 * - 实验现象：
 *   1. TIM0 通过 PIOA_2 引脚输出 2KHz 的 PWM；
 *   2. TIM3 捕获输入通道 0 使用 PIOB_3 引脚捕获；
 *   3. 串口打印出利用捕获功能得到的 PWM 信号的周期和频率。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zlg118_std_tim0_cap.c src_zlg118_std_tim0_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_std_tim0_cap
 * \copydoc demo_zlg118_std_tim0_cap.c
 */

/** [src_zlg118_std_tim0_cap] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg118_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am118_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg118_core_std_tim3_cap_entry (void)
{

    am_pwm_handle_t tim0_pwm_handle = am_zlg118_tim0_pwm_inst_init();
    am_cap_handle_t tim3_cap_handle = am_zlg118_tim3_cap_inst_init();
    
	  AM_DBG_INFO("demo am118_core std tim1 cap!\r\n");

    /* TIM0 输出频率为 2KHz 的 PWM */
    am_pwm_config(tim0_pwm_handle, 0, 50000 / 2, 100000);
    am_pwm_enable(tim0_pwm_handle, 0);

    demo_std_timer_cap_entry(tim3_cap_handle, 0);
}
/** [src_zlg118_std_tim0_cap] */

/* end of file */
