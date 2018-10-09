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
 * \brief 时钟频率输出例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口打印 "The CLKOUT  rate is  xxx Hz\r\n"，xxx 为当前频率；
 *   2. PIO0_27 引脚（默认配置 CLKOUT 引脚输出）输出当前时钟频率 2.5MHz；
 *   3. LED0 以 0.2s 的时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_clkout.c src_lpc824_hw_clkout
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_clkout
 * \copydoc demo_lpc824_hw_clkout.c
 */

/** [src_lpc824_hw_clkout] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_clk.h"


/** \brief 时钟源定义 */
#define __CLKOUT_SRC  AMHW_LPC82X_CLK_CLKOUT_SRC_MAINCLK

/** \brief 时钟分频参数定义 */
#define __CLKOUT_DIV  24

/**
 * \brief 例程入口
 */
void demo_lpc824_hw_clkout_entry (void)
{
    /* 开启内部 IRC */
    if (__CLKOUT_SRC == AMHW_LPC82X_CLK_CLKOUT_SRC_IRC) {

        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_IRC);
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_IRC_OUT);

    /* 开启 WDT 时钟振荡器 */
    } else if (__CLKOUT_SRC == AMHW_LPC82X_CLK_CLKOUT_SRC_WDTOSC) {

        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_WDT_OSC);

    /* 系统振荡器 */
    } else if (__CLKOUT_SRC == AMHW_LPC82X_CLK_CLKOUT_SRC_SYSOSC) {

        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_SYS_OSC);
    }

    /* 配置输出时钟源和时钟分频参数 */
    amhw_lpc82x_clk_clkout_config(__CLKOUT_SRC, __CLKOUT_DIV);

    AM_DBG_INFO("The CLKOUT  rate is  %8d Hz\r\n",
                 amhw_lpc82x_clk_main_clkrate_get() / __CLKOUT_DIV);

    AM_FOREVER {
        am_led_on(LED0);
        am_mdelay(200);
        am_led_off(LED0);
        am_mdelay(200);
    }
}
/** [src_lpc824_hw_clkout] */

/* end of file */
