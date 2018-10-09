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
 * \brief WKT 定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED0 以 1s 间隔闪烁。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_am824_std_wkt_timer.c src_am824_std_wkt_timer
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-02  sky, modified
 * - 1.00 15-09-22  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_wkt_timer
 * \copydoc demo_am824_std_wkt_timer.c
 */

/** [src_am824_std_wkt_timer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x_inst_init.h"
#include "demo_nxp_entries.h"


/**
 * \brief 例程入口
 */
void demo_am824_core_std_wkt_timer_entry (void)
{
    am_timer_handle_t      timer_handle;
    uint32_t               clkin;
    
    am_kprintf("demo am824 std wkt timer!\r\n");
    timer_handle = am_lpc82x_wkt_inst_init();
  
    am_timer_clkin_freq_get(timer_handle, &clkin);
  
    demo_lpc_std_wkt_timer_entry(timer_handle,
                                 0,
                                 clkin);
}
/** [src_am824_std_wkt_timer] */

/* end of file */
