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
 * \brief Systick 延时例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED0 以 10Hz 的频率闪烁。
 *
 * \par 源代码
 * \snippet demo_am845_core_std_delay.c src_am845_core_std_delay
 *
 * \internal
 * \par Modification History
 * - 1.00 14-12-04  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_delay
 * \copydoc demo_am845_core_std_delay.c
 */

/** [src_am845_core_std_delay] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_std_delay_entry (void)
{
    AM_DBG_INFO("demo am845 std delay!\r\n");
    demo_std_delay_entry(LED0);
}
/** [src_am845_core_std_delay] */

/* end of file */
