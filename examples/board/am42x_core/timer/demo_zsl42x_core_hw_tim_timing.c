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
 * \brief 定时器 TIMING 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 以 10Hz 的频率进行翻转。(计数周期是10Hz，led翻转周期为10/2=5Hz)
 *
 * \par 源代码
 * \snippet demo_zsl42x_hw_tim_timing.c src_zsl42x_hw_tim_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-23  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_tim_timing
 * \copydoc demo_zsl42x_hw_tim_timing.c
 */

/** [src_zsl42x_hw_tim_timing] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zsl42x.h"
#include "am_zsl42x_clk.h"
#include "am_zsl42x_inst_init.h"
#include "demo_zsl42x_entries.h"
#include "demo_am42x_core_entries.h"

/**
 * \brief 定时器timing HW层例程入口
 */
void demo_zsl42x_core_hw_tim_timing_entry (void)
{
    AM_DBG_INFO("demo am42x_core hw tim timing!\r\n");

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM012);

    demo_zsl42x_hw_tim_timing_entry(ZSL42x_TIM0,
    		                        AMHW_ZSL42x_TIM_TYPE_TIM0,
                                    am_clk_rate_get(CLK_TIM012),
                                    INUM_TIM0);
}
/** [src_zsl42x_hw_tim_timing] */

/* end of file */
