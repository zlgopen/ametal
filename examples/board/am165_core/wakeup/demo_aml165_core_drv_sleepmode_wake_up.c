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
 * \brief 睡眠模式例程，通过驱动层接口实现
 *
 * - 操作步骤：
 *   1. 将 J14 的 KEY 和 PIOA_13 短接在一起。
 *
 * - 实现现象
 *   1. MCU 进入停止模式时， J-Link 调试断开；
 *   2. 按下 KEY/RES 键唤醒 MCU，串口输出 wake_up，程序继续运行。
 *
 * \note
 *   由于 TIM14 默认初始化并作为系统滴答使用，会定期产生中断导致唤醒， 测试本例程
 *   之前应将 am_prj_config.h 中的宏 AM_CFG_SYSTEM_TICK_ENABLE、
 *   AM_CFG_SOFTIMER_ENABLE 和   AM_CFG_KEY_GPIO_ENABLE 设置为 0。
 *
 * \par 源代码
 * \snippet demo_aml165_core_drv_sleepmode_wake_up.c src_aml165_core_drv_sleepmode_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_drv_sleepmode_wake_up
 * \copydoc demo_aml165_core_drv_sleepmode_wake_up.c
 */

/** [src_aml165_core_drv_sleepmode_wake_up] */
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_zlg_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_drv_sleepmode_wake_up_entry (void)
{
    AM_DBG_INFO("demo aml165_core drv sleepmode wake up!\r\n");

    /* 初始化 PWR */
    am_zml165_pwr_inst_init();

    demo_zml165_drv_sleepmode_wake_up_entry();
}
/** [src_aml165_core_drv_sleepmode_wake_up] */

/* end of file */
