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
 * \brief 停止模式例程，通过驱动层接口实现
 *
 * - 实现现象
 *   1. MCU 进入停止模式时， J-Link 调试断开；
 *   2. 按下 KEY/RES 键唤醒 MCU，串口输出 wake_up，程序继续运行。
 *
 * \par 源代码
 * \snippet demo_am116_core_drv_stopmode_wake_up.c src_am116_core_drv_stopmode_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am116_core_drv_stopmode_wake_up
 * \copydoc demo_am116_core_drv_stopmode_wake_up.c
 */

/** [src_am116_core_drv_stopmode_wake_up] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg116_inst_init.h"
#include "demo_zlg_entries.h"
#include "demo_am116_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_am116_core_drv_stopmode_wake_up_entry (void)
{
    AM_DBG_INFO("demo am116_core drv stopmode wake up!\r\n");

    /* 初始化 PWR */
    am_zlg116_pwr_inst_init();
    
    demo_zlg116_drv_stopmode_wake_up_entry();
}
/** [src_am116_core_drv_stopmode_wake_up] */

/* end of file */
