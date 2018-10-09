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
 * \brief PIT实现标准定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED以10Hz的频率闪烁。
 *
 * - 注意：
 *    如果要运行此例程，需要将am_prj_config.h中AM_CFG_SOFTIMER_ENABLE、
 *    AM_CFG_KEY_GPIO_ENABLE和AM_CFG_SYSTEM_TICK_ENABLE三个宏的值都设为0。
 *
 *    使用此demo需要将am_kl26_hwconfig_pit_timing.c用户配置文件中的设备信息结构体
 *    中的pit配置标志设为AM_FSL_PIT_TIMING_1_64BIT
 *
 * \note 假如PIT计数时钟源为24M,则32位定时器最长可计时 
 *           0xFFFFFFFF / 20M = 179秒，采用64位定时器则可定更长时间。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_pit_2_64bit_timing.c src_amks16z_core_std_pit_2_64bit_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_amks16z_core_if_std_pit_2_64bit_timing
 * \copydoc demo_amks16z_core_std_pit_2_64bit_timing.c
 */

/** [src_amks16z_core_std_pit_2_64bit_timing] */
#include "am_kl26_inst_init.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "demo_std_entries.h"


/**
 * \brief 例程入口
 */
void demo_amks16z_core_std_pit_2_64bit_timing_entry (void)
{
    am_timer_handle_t  pit_timing_handle;

    AM_DBG_INFO("demo amks16z_core std pit 1 64 bit timing!\r\n");

    /* 获取PIT TIMER操作句柄 */
    pit_timing_handle = am_kl26_pit_timing_inst_init();

    demo_std_timer_timing_entry(pit_timing_handle, 0);
}

/** [src_amks16z_core_std_pit_2_64bit_timing] */

/* end of file */
