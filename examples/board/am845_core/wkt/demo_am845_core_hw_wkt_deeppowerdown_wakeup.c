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
 * \brief WKT 深度掉电唤醒例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 运行程序，一秒后 LED0 闪烁一次后进入深度掉电模式；
 *   2. 等待 5s 后，WKT 计时时间到，CPU 被唤醒，如果通用寄存器数据校验正确，LED0
 *      闪烁一次；如果通用寄存器数据校验错误，LED0 持续闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 进入深度掉电模式后，只有 WAKEUP 引脚和 WKT 定时器能唤醒芯片。对于默认状态，
 *       WAKEUP 引脚唤醒使能，所以一定确保 WAKEUP 外部上拉到电源，保证有效的高电平
 *       状态，否则会引起误唤醒操作，而观察不到 WKT 唤醒 CPU；而本例程将 WAKEUP 引脚
 *       (PIO0_4)唤醒功能禁止了，所以不需要将 PIO0_4 上拉；
 *    3. 使用该程序后，会导致下次程序烧写的时候检测不到芯片（由于芯片进入深度掉电
 *       模式将 SWD 调试模式关闭），下次下载时将 P0_12 拉低或按动复位键之后一秒内
 *       即可正常下载调试。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_wkt_deeppowerdown_wakeup.c src_am845_core_hw_wkt_deeppowerdown_wakeup
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-01  sky, modified
 * - 1.00 15-07-14  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_wkt_deeppowerdown_wakeup
 * \copydoc demo_am845_core_hw_wkt_deeppowerdown_wakeup.c
 */

/** [src_am845_core_hw_wkt_deeppowerdown_wakeup] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_wkt_deeppowerdown_wakeup_entry (void)
{
    am_kprintf("demo am_845core hw wkt deeppowerdown wakeup!\r\n");
    /* 初始化 WKT AHB 时钟 */
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_WKT);

    /* 复位 WFT */
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_WKT);

    /* 低功耗时钟源 */
    amhw_lpc_wkt_clksel_cfg(LPC84X_WKT, AMHW_LPC_WKT_LOW_POWER_CLOCK);

    demo_lpc845_hw_wkt_deeppowerdown_wakeup_entry(LPC84X_PMU,
                                                  LPC84X_WKT,
                                                  INUM_WKT, 
                                                  5000);
}
/** [src_am845_core_hw_wkt_deeppowerdown_wakeup] */

/* end of file */
