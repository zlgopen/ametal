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
 * \brief 蜂鸣器例程，通过标准接口实现
 *
 *
 * - 操作步骤：
 *   1. 短接J7跳线帽，PIOC_3控制蜂鸣器;
 *   2. 需要在am_prj_config.h头文件里使能宏AM_CFG_BUZZER_ENABLE。
 *
 * - 实验现象：
 *   1. 蜂鸣器以0.5s的时间间隔鸣叫。
 *
 * \warning 蜂鸣器占用了TPM0_PWM2的PIOC_3引脚，如果用了该蜂鸣器，就不能将TPM0配置
 *          为其他非PWM功能。如需要在应用程序中使用非PWM功能，就不能调用蜂鸣器的API，
 *          需要在am_prj_config.h头文件里禁能宏AM_CFG_BUZZER_ENABLE。
 *       
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_buzzer.c src_amks16z_core_std_buzzer
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-09-30  sdy, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_amks16z_core_if_std_buzzer
 * \copydoc demo_amks16z_core_std_buzzer.c
 */
 
/** [src_amks16z_core_std_buzzer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_board.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_std_buzzer_entry (void)
{  
    AM_DBG_INFO("demo amks16z_core std buzzer!\r\n");

    demo_std_buzzer_entry();
}

/** [src_amks16z_core_std_buzzer] */

/* end of file */
