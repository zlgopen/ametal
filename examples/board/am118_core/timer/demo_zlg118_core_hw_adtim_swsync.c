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
 * \brief 高级定时器软件软件同步开始、同步停止、同步清零例程，通过 HW层接口实现
 *
 * - 实验现象：
 *   1.串口打印四行数据信息；
 *   2.第1行代表（state）参与软件同步的定时器状态   1：参与同步     0：不参与软件同步
 *     第2行开始（start）时每个定时器的计数值（此行打印数据应该是等差，定时器已启动，
 *                                  分别读取寄存器值的过程中依旧在计数）
 *     第3行停止（stop）时每个定时器的计数值
 *     第4行清零（clear）时每个定时器的计数值
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_adtim_swsync.c src_zlg118_hw_adtim_swsync
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_adtim_swsync
 * \copydoc demo_zlg118_hw_adtim_swsync.c
 */

/** [src_zlg118_hw_adtim_swsync] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "hw/amhw_zlg118_adtim.h"
#include "am_zlg118_inst_init.h"
#include "demo_zlg_entries.h"
#include "demo_am118_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg118_core_hw_adtim_swsync_entry (void)
{
    amhw_zlg118_adtim_sw_sync_t adtim_sync;

    AM_DBG_INFO("demo am118_core hw adtim swsync!\r\n");

    /* 软件同步配置使能    AM_TRUE：参与软件同步动作      AM_FALSE：不参与软件同步动作*/
    adtim_sync.adtim4 = AM_TRUE;
    adtim_sync.adtim5 = AM_TRUE;
    adtim_sync.adtim6 = AM_TRUE;

    demo_zlg118_hw_adtim_swsync_entry(&adtim_sync);
}
/** [src_zlg118_hw_adtim_swsync] */

/* end of file */
