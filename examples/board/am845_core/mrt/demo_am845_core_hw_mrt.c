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
 * \brief 多频率定时器（MRT）例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 以 10Hz 的频率闪烁；
 *   2. PIO0_17 引脚的输出电平状态以 8KHz 的频率变换，产生 4KHz 的波形。
 *
 * \note
 *    1. 由于 MRT 默认作为系统1滴答使用，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_SOFTIMER_ENABLE、 AM_CFG_SYSTEM_TICK_ENABLE 以及
 *       AM_CFG_KEY_GPIO_ENABLE 定义为 0，不使用软件定时器、系统嘀嗒、板载独立按键。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_mrt.c src_am845_core_hw_mrt
 *
 * \internal
 * \par History
 *
 * - 1.01 15-12-02  mem ,modified
 * - 1.00 15-07-15  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_mrt
 * \copydoc demo_am845_core_hw_mrt.c
 */

/** [src_am845_core_hw_mrt] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_mrt_entry (void)
{
    am_kprintf("demo am845_core hw mrt!\r\n");
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_MRT);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_MRT);

    am_gpio_pin_cfg(PIO0_17, AM_GPIO_OUTPUT_INIT_HIGH);

    demo_lpc845_hw_mrt_entry(LPC84X_MRT, INUM_MRT);
}
/** [src_am845_core_hw_mrt] */

/* end of file */
