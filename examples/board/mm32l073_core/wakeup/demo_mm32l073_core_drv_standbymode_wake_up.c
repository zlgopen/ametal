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
 * \brief 待机模式例程，通过驱动层接口实现
 *
 * - 操作步骤：
 *   1. 上电前将 WAKE_UP(PIOA_0) 连接到低电平；
 *   2. 等待 MCU 进入待机模式；
 *   3. 将 PIOA_0 连接到高电平，使 PIOA_0 上产生上升沿唤醒 MCU。
 *
 * - 实现现象
 *   1. 成功进入待机模式时，串口输出"enter standby!"，且 J-Link 调试断开；
 *      否则串口输出"WAKE_UP(PIOA_0) must be low!"；
 *   2. PIOA_0 产生上升沿后，MCU 复位运行。
 *
 * \note
 *    只有在 WAKE_UP(PIOA_0) 为低电平的时候才能进入待机模式
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_drv_standbymode_wake_up.c src_mm32l073_core_drv_standbymode_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_mm32l073_core_drv_standbymode_wake_up
 * \copydoc demo_mm32l073_core_drv_standbymode_wake_up.c
 */
 
/** [src_mm32l073_core_drv_standbymode_wake_up] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_mm32l073_inst_init.h"
#include "demo_mm32_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_mm32l073_core_drv_standbymode_wake_up_entry (void)
{
    AM_DBG_INFO("demo mm32l073_core drv standbymode wake up!\r\n");

    demo_mm32l073_drv_standbymode_wake_up_entry();
}
/** [src_mm32l073_core_drv_standbymode_wake_up] */

/* end of file */
