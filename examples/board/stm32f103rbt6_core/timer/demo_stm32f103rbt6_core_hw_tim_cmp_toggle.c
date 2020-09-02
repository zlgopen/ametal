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
 * \brief 定时器通道比较匹配引脚翻转例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 和 PIOB_11 引脚以 10Hz 的频率进行翻转。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIOC_9 控制。
 *
 * \par 源代码
 * \snippet demo_zlg237_hw_tim_cmp_toggle.c src_zlg237_hw_tim_cmp_toggle
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-04  zcb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg237_hw_tim_cmp_toggle
 * \copydoc demo_zlg237_hw_tim_cmp_toggle.c
 */

/** [src_zlg237_hw_tim_cmp_toggle] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg237.h"
#include "am_zlg237_clk.h"
#include "am_zlg237_inst_init.h"
#include "demo_zlg_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg237_core_hw_tim_cmp_toggle_entry (void)
{
    AM_DBG_INFO("demo am237_core hw tim cmp toggle!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOB_11, PIOB_11_TIM2_CH4_REMAP2 | PIOB_11_AF_PP);

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM2);

    /* 复位定时器 */
    am_zlg237_clk_reset(CLK_TIM2);

    demo_zlg_hw_tim_cmp_toggle_entry(ZLG237_TIM2,
                                     AMHW_ZLG_TIM_TYPE0,
                                     3,
                                     am_clk_rate_get(CLK_TIM2),
                                     INUM_TIM2);
}
/** [src_zlg237_hw_tim_cmp_toggle] */

/* end of file */
