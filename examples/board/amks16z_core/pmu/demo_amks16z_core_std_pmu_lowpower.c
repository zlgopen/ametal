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
 * \brief amks16z_core系统模式配置演示例程，通过PMU驱动层实现
 *
 * - 操作步骤:
 *   1. 根据串口输出内容提示进行操作；
 *   2. 波特率：115200。
 *
 * - 注意:
 *   1. 进入lls, vlls之前，lptmr也会定时唤醒MCU，
 *   2. 其中vlls模式唤醒会复位
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_pmu_lowpower.c src_amks16z_core_std_pmu_lowpower
 *
 * \internal
 * \par History
 * - 1.00 16-09-24  ari, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_amks16z_core_std_pmu_lowpower
 * \copydoc demo_amks16z_core_std_pmu_lowpower.c
 */

/** [src_amks16z_core_std_pmu_lowpower] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief 主函数
 */
void demo_amks16z_core_std_pmu_lowpower_entry (void)
{
    AM_DBG_INFO("demo amks16z_core std pmu lowpower!\r\n");

    demo_kl26_std_pmu_lowpower_entry();
}

/** [src_amks16z_core_std_pmu_lowpower] */

/* end of file */
