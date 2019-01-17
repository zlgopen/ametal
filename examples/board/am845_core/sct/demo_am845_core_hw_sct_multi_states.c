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
 * \brief SCT 操作多状态例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. PIO0_16(SCT_OUT5)引脚输出波形。
 *
 * \code
 *  |---         状态0        ---|--- 状态1 ---|--- 状态0      ......
 *             _____     _____     ___         __            ___
 *            |     |   |     |   |   |       |  |          |     ......
 *  __________|     |___|     |___|   |_______|  |__________|
 *
 *  0         10    15  18    23  26  3      10  12         10     (ms)
 *                              (状态1, 0)     (状态0, 0)
 *          事件 0  1   2      3   4   5      6  7
 * \endcode
 *
 * \note
 *    1. 连接 PIO0_16(SCT_OUT5) 引脚到逻辑分析仪，将会看到该波形，前 10ms 为低电平；
 *    2. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_sct_multi_states.c src_am845_core_hw_sct_multi_states
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-08  hgo, modified
 * - 1.00 14-12-09  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_sct_multi_states
 * \copydoc demo_am845_core_hw_sct_multi_states.c
 */

/** [src_am845_core_hw_sct_multi_states] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "lpc84x_pin.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_sct_multi_states_entry (void)
{
    uint32_t frq = 0;

    am_kprintf("demo am845_core hw sct multi!\r\n");
    /* 平台初始化 */
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SCT);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SCT);

    /* 选择 SCT_OUT5 对应引脚 PIO0_16 */
    am_gpio_pin_cfg(PIO0_16, PIO_FUNC_SCT_OUT5);

    frq = amhw_lpc84x_clk_system_clkrate_get();
  
    demo_lpc_hw_sct_multi_states_entry(LPC84X_SCT0, frq);
}
/** [src_am845_core_hw_sct_multi_states] */

/* end of file */
