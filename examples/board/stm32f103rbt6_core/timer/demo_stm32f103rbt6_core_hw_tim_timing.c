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
 *   1. LED0 以 10Hz 的频率进行翻转。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIOC_9 控制；
 *    2. 16 位定时器定时计数值不为 65536 的倍数，则定时会有一些偏差。
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_hw_tim_timing.c src_stm32f103rbt6_hw_tim_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-25  zcb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_stm32f103rbt6_hw_tim_timing
 * \copydoc demo_stm32f103rbt6_hw_tim_timing.c
 */

/** [src_stm32f103rbt6_hw_tim_timing] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_stm32f103rbt6.h"
#include "am_stm32f103rbt6_clk.h"
#include "am_stm32f103rbt6_inst_init.h"
#include "demo_stm32f103rbt6_entries.h"
#include "demo_stm32f103rbt6_core_entries.h"

/**
 * \brief 定时器timing HW层例程入口
 */
void demo_stm32f103rbt6_core_hw_tim_timing_entry (void)
{
    AM_DBG_INFO("demo stm32f103rbt6_core hw tim timing!\r\n");

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM2);

    /* 复位定时器 */
    am_stm32f103rbt6_clk_reset(CLK_TIM2);

    demo_stm32f103rbt6_hw_tim_timing_entry(STM32F103RBT6_TIM2,
                                 AMHW_STM32F103RBT6_TIM_TYPE0,
                                 am_clk_rate_get(CLK_TIM2),
                                 INUM_TIM2);
}
/** [src_stm32f103rbt6_hw_tim_timing] */

/* end of file */
