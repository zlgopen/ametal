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
 * \brief LPTMR实现定时器例程，通过HW层接口实现
 *
 * - 实验现象：
 *   1. LED0以10Hz的频率闪烁。
 *
 * \note LPTMR时钟源有4种选择，在amdr_hwconfig_lptmr_timing.c中进行配置，选择不同大小
 *       的时钟源，定时的范围和精度存在差别。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_lptmr_timing.c src_amks16z_core_hw_lptmr_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_lptmr_timing
 * \copydoc demo_amks16z_core_hw_lptmr_timing.c
 */

/** [src_amks16z_core_hw_lptmr_timing] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_kl26_clk.h"
#include "hw/amhw_fsl_lptmr.h"
#include "hw/amhw_kl26_sim.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_lptmr_timing_entry (void)
{
    uint32_t lptmr_clock;

    /* 初始化时钟 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_LPTMR);

    /* 获取输入频率 */
    lptmr_clock = am_kl26_clk_periph_rate_get(KL26_LPTMR);

    demo_fsl_hw_lptmr_timing_entry(KL26_LPTMR, INUM_LPTMR0, lptmr_clock);
}

/** [src_amks16z_core_hw_lptmr_timing] */

/* end of file */
