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
 * \brief SCT 作为 2 个 16 位定时器定时例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 以 10Hz 的频率闪烁;
 *   2. 蜂鸣器引脚(PIO0_24)状态以 8KHz 的频率翻转，输出波形为 4KHz，
 *      每次鸣叫 50 毫秒，鸣叫频率为 10Hz。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 蜂鸣器需要短接 J7 跳线帽，才能被 PIO0_24 控制；
 *    3. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不默认初始化蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_sct_2_16bit_timing.c src_am845_core_hw_sct_2_16bit_timing
 *
 * \internal
 * \par Modification history
 * - 1.02 15-12-14  hgo, change PIO0_15 to PIO0_24 .
 * - 1.01 15-11-19  hgo, modified
 * - 1.00 15-07-13  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_sct_2_16bit_timing
 * \copydoc demo_am845_core_hw_sct_2_16bit_timing.c
 */

/** [src_am845_core_hw_sct_2_16bit_timing] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"


/**
 * \brief 例程入口
 */
void demo_am845_core_hw_sct_2_16bit_timing_entry (void)
{
    AM_DBG_INFO("demo am845_core hw sct 2-16bit timing\r\n");
  
    uint32_t frq = 0;
    /* 平台初始化 */
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SCT);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SCT);

    AM_DBG_INFO("The LED0 blinking in 10Hz\r\n");
    AM_DBG_INFO("The PIO0_24 toggle in 8KHz, The output waveform is 4KHz\r\n");

    /* 初始化蜂鸣器 ，输出高，蜂鸣器不响 */
    am_gpio_pin_cfg(PIO0_24, AM_GPIO_OUTPUT_INIT_HIGH | AM_GPIO_PULLUP);

    frq = amhw_lpc84x_clk_system_clkrate_get();
    demo_lpc_hw_sct_2_16bit_timing_entry(LPC84X_SCT0,
                                         INUM_SCT0, 
                                         frq,
                                         PIO0_24);
}
/** [src_am845_core_hw_sct_2_16bit_timing] */

/* end of file */
