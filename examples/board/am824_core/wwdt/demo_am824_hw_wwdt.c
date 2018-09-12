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
 * \brief WWDT 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 屏幕打印看门狗时钟频率；
 *   2. 前 5 次报警中断发生时，会执行喂狗操作,串口打印当前喂狗次数；
 *   3. 第 6 次报警中断发生时，不执行喂狗操作，会发生看门狗复位；
 *   4. 复位后，LED0 亮 500ms，串口打印 "Watchdog reset occurred!"。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. WDT 时钟，0.6MHz，64 分频，时钟频率 9.375KHz，WDT 计数器固定 4 分频时钟，
 *       当 WDT 时钟频率大于 400K 时，WDT 窗口功能不能正常使用。
 *
 * \par 源代码
 * \snippet demo_am824_hw_wwdt.c src_am824_hw_wwdt
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-14  hgo, del amhw_lpc_wwdt_option_clr()
 * - 1.00 15-07-22  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_wwdt
 * \copydoc demo_am824_hw_wwdt.c
 */

/** [src_am824_hw_wwdt] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"
#include "hw/amhw_lpc_wwdt.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_hw_wwdt_entry (void)
{
    uint32_t wdt_freq;
  
    am_kprintf("demo am824_core hw wwdt!\r\n");

    /* 使能 WWDT 时钟 */
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_WWDT);

    /* 设置 WDT 时钟，0.6MHz，64 分频，时钟频率 9.375KHz */
    amhw_lpc82x_clk_wdtoscc_cfg(AMHW_LPC82X_CLK_WDTOSC_RATE_0_6MHZ, 64);

    amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_WDT_OSC);

    AM_DBG_INFO("\r\nThe wdt osc freq is %d .\r\n",
                amhw_lpc82x_clk_wdt_rate_get());

    /* 看门狗定时器频率为输入时钟频率 4 分频 */
    wdt_freq = amhw_lpc82x_clk_wdt_rate_get() / 4;

    /* 关闭 LED0 */
    am_led_off(LED0);

    demo_lpc_hw_wwdt_entry(LPC82X_WWDT, INUM_WDT, wdt_freq);
}
/** [src_am824_hw_wwdt] */

/* end of file */
