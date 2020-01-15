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
 *   1. PIOA_4 引脚以 10Hz 的频率进行翻转。
 *
 * \note
 *    由于 TIM14 默认初始化并作为系统滴答使用，使用本 Demo 之前必须在
 *    am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE、AM_CFG_SOFTIMER_ENABLE
 *    和 AM_CFG_SYSTEM_TICK_ENABLE 定义为 0。
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_hw_tim_cmp_toggle.c src_mm32l073_core_hw_tim_cmp_toggle
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-22  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_hw_tim_cmp_toggle
 * \copydoc demo_mm32l073_core_hw_tim_cmp_toggle.c
 */

/** [src_mm32l073_core_hw_tim_cmp_toggle] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_mm32l073.h"
#include "am_mm32l073_clk.h"
#include "demo_mm32_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_mm32l073_core_hw_tim_cmp_toggle_entry (void)
{
    AM_DBG_INFO("demo mm32l073_core hw tim cmp toggle!\r\n");

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_4, PIOA_4_TIM14_CH1 | PIOA_4_AF_PP);

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM14);

    /* 复位定时器 */
    am_mm32l073_clk_reset(CLK_TIM14);

    demo_mm32_hw_tim_cmp_toggle_entry(MM32L073_TIM14,
                                     AMHW_MM32_TIM_TYPE2,
                                     0,
                                     am_clk_rate_get(CLK_TIM14),
                                     INUM_TIM14);
}
/** [src_mm32l073_core_hw_tim_cmp_toggle] */

/* end of file */
