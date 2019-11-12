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
 *   1. LED0 和 PIOA_2 引脚以 10Hz 的频率进行翻转。
 *
 * \par 源代码
 * \snippet demo_hc32l13x_hw_tim_cmp_toggle.c src_hc32l13x_hw_tim_cmp_toggle
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l13x_hw_tim_cmp_toggle
 * \copydoc demo_hc32l13x_hw_tim_cmp_toggle.c
 */

/** [src_hc32l13x_hw_tim_cmp_toggle] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32.h"
#include "am_hc32_clk.h"
#include "am_hc32l13x_inst_init.h"
#include "demo_hc32_entries.h"
#include "demo_aml13x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32l13x_core_hw_tim_cmp_toggle_entry (void)
{
    AM_DBG_INFO("demo aml13x_core hw tim cmp toggle!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_TIM0_CHA | PIOA_2_OUT_PP);

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM012);

    demo_hc32_hw_tim_cmp_toggle_entry((void *)HC32_TIM0,
                                        AMHW_HC32_TIM_TYPE_TIM0,
                                        HC32_TIM_CH0A,
                                        am_clk_rate_get(CLK_TIM012),
                                        AMHW_HC32_TIM_CLK_DIV64,
                                        INUM_TIM0);
}
/** [src_hc32l13x_hw_tim_cmp_toggle] */

/* end of file */
