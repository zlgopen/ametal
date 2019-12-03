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
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 睡眠模式例程，使用定时器周期唤醒，通过驱动层接口实现
 *
 * - 实现现象
 *   1. 串口输出"enter sleep!"，系统时钟源切换为 LSI，启动定时器，
 *      进入睡眠模式后，J-Link 调试断开，此时用户可测量睡眠模式的功耗；
 *   2. 等待定时时间到，MCU 被唤醒，串口输出"wake_up!"，然后重新进入
 *      睡眠模式。
 *
 * \note
 *   由于 TIM14 默认初始化并作为系统滴答使用，会定期产生中断导致唤醒， 测试本例程
 *   之前应将 am_prj_config.h 中的宏 AM_CFG_SYSTEM_TICK_ENABLE、
 *   AM_CFG_SOFTIMER_ENABLE 和   AM_CFG_KEY_GPIO_ENABLE 设置为 0。
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_drv_sleepmode_timer_wake_up.c src_mm32l073_core_drv_sleepmode_timer_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 18-05-16  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_drv_sleepmode_timer_wake_up
 * \copydoc demo_mm32l073_core_drv_sleepmode_timer_wake_up.c
 */

/** [src_mm32l073_core_drv_sleepmode_timer_wake_up] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_mm32l073_inst_init.h"
#include "demo_mm32_entries.h"
#include "demo_mm32l073_core_entries.h"

/** \brief LSI 时钟频率 */
#define    __LSI_CLK    (40000UL)

/**
 * \brief 例程入口
 */
void demo_mm32l073_core_drv_sleepmode_timer_wake_up_entry (void)
{
    AM_DBG_INFO("demo mm32l073_core drv sleepmode timer wake up!\r\n");

    /* 初始化 PWR */
    am_mm32l073_pwr_inst_init();

    demo_mm32l073_drv_sleepmode_timer_wake_up_entry(am_mm32l073_tim14_timing_inst_init(),
                                                  __LSI_CLK);
}
/** [src_mm32l073_core_drv_sleepmode_timer_wake_up] */

/* end of file */
