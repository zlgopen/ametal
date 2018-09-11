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
 * \brief WWDT 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 若 WWDT 使芯片复位，串口提示 "The chip Reset by WWDT\r\n"；
 *   2. 修改宏定义 __WWDT_FEED_TIME_MS 的值，超过 1500ms(存在 5ms 误差)，芯片复位；
 *   3. 修改宏定义 __WWDT_FEED_TIME_MS 的值，小于 1500ms(存在 5ms 误差)，程序正常运行。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. WDT 时钟，0.6MHz，64 分频，时钟频率 9.375KHz，WDT 计数器固定 4 分频时钟；
 *    4. 片内 WDTOSC 频率产生存在最大 40% 的误差。
 *
 * \par 源代码
 * \snippet demo_am824_std_wwdt.c src_am824_std_wwdt_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-09  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_wwdt
 * \copydoc demo_am824_std_wwdt.c
 */

/** [src_am824_std_wwdt_wdt] */
#include "ametal.h"
#include "am_wdt.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x_inst_init.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"
#include "demo_std_entries.h"

/**
 * \brief 看门狗超时时间
 *
 * \note 因为看门狗内部时钟 WDTOSC 存在误差，所以 __WWDT_TIMEOUT_MS 对应的实际时间
 *       存在误差
 */
#define __WWDT_TIMEOUT_MS       1500

/**
 * \brief 看门狗喂狗时间，若喂狗时间超过 __WWDT_TIMEOUT_MS 的值（大于 5ms 以上）,
 *        会产生看门狗事件
 */
#define __WWDT_FEED_TIME_MS     1000

/**
 * \brief 例程入口
 */
void demo_am824_std_wwdt_entry (void)
{
    am_kprintf("demo am824_core std wwdt!\r\n");

    if ((amhw_lpc82x_syscon_rst_stat_get() & AMHW_LPC82X_SYSCON_RSTSTAT_WDT) != 0) {

        amhw_lpc82x_syscon_rst_stat_clr(AMHW_LPC82X_SYSCON_RSTSTAT_WDT);

        AM_DBG_INFO("The chip Reset by WWDT\r\n");
    } else {
        AM_DBG_INFO("The chip Reset by External Reset Pin \r\n");
    }

    AM_DBG_INFO("The wdt osc freq is %d\r\n", amhw_lpc82x_clk_wdt_rate_get());

    demo_std_wdt_entry(am_lpc82x_wwdt_inst_init(),
                       __WWDT_TIMEOUT_MS,
                       __WWDT_FEED_TIME_MS);
}
/** [src_am824_std_wwdt_wdt] */

/* end of file */
