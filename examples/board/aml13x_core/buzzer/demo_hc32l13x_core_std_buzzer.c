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
 * - 操作步骤：
 *   1. 短接 J7 跳线帽，PIOA_0 控制蜂鸣器。
 *
 * - 实验现象：
 *   1. 蜂鸣器以 0.5s 的时间间隔鸣叫。
 *
 * \note
 *    测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_BUZZER_ENABLE 
 *    定义为 1。但该宏已经默认配置为 1， 用户不必再次配置；
 *
 * \par 源代码
 * \snippet demo_hc32l13x_std_buzzer.c src_std_buzzer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-20  win, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l13x_std_buzzer
 * \copydoc demo_hc32l13x_std_buzzer.c
 */

/** [src_std_buzzer] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_aml13x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32l13x_core_std_buzzer_entry (void)
{
    AM_DBG_INFO("demo aml13x_core std buzzer!\r\n");

    demo_std_buzzer_entry();
}
/** [src_std_buzzer] */

/* end of file */
