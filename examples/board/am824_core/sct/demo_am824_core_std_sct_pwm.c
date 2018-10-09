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
 *   1. 定时器通道对应的引脚输出频率为 2KHz，占空比为 50% 的 PWM 波
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_am824_std_sct_pwm.c src_am824_std_sct_pwm
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-07  hgo, modified
 * - 1.00 15-07-15  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_sct_pwm
 * \copydoc demo_am824_std_sct_pwm.c
 */

/** [src_am824_std_sct_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_std_sct_pwm_entry (void)
{
    AM_DBG_INFO("The SCT demo for standard pwm service\r\n");

    demo_std_timer_pwm_entry(am_lpc82x_sct0_pwm_inst_init(), 0);
}
/** [src_am824_std_sct_pwm] */

/* end of file */
