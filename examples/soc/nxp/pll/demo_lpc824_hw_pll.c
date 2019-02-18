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
 * \brief PLL 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 打印 PLL 期望频率、实际输出频率和 CLKOUT 引脚输出频率；
 *   2. CLKOUT 引脚(PIO0_27 引脚)输出 __CLKOUT_DIV 分频后的主时钟频率；
 *   3. LED0 以 0.5s 的时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_pll.c src_lpc824_hw_pll
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-20  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_pll
 * \copydoc demo_lpc824_hw_pll.c
 */

/** [src_lpc824_hw_pll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_syscon.h"
#include "hw/amhw_lpc82x_iocon.h"
#include "hw/amhw_lpc82x_swm.h"
#include "hw/amhw_lpc82x_clk.h"
#include "am_lpc82x_inst_init.h"

/**
 * \brief PLL 时钟源，为以下一种：
 *            AMHW_LPC82X_CLK_PLLIN_SRC_IRC
 *            AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC
 *            AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN
 */
#define __PLL_CLKSRC                AMHW_LPC82X_CLK_PLLIN_SRC_IRC

/** \name PLL 频率控制
 *
 * M = AM_CFG_SYSPLLCTRL_M + 1
 * P = 2^(AM_CFG_SYSPLLCTRL_P)
 *
 * PLLOUT = M * PLLIN;
 * FCCO = 2 * P * PLLOUT;
 * @{
 */

/** \brief PLL 控制寄存器 PSEL 值 */
#define __PLL_CFG_PSEL               1

/** \brief PLL 控制寄存器 MSEL 值 */
#define __PLL_CFG_MSEL               3

/** \brief PLL 期望频率 */
#define __PLL_DESIREDRATE            (48000000UL)

/** \brief 选择输出时钟源 */
#define __CLKOUT_SRC                 AMHW_LPC82X_CLK_CLKOUT_SRC_MAINCLK

/** \brief 输出时钟源分频 */
#define __CLKOUT_DIV                 48
/**
 * @}
 */

/**
 * \brief 例程入口
 */
void demo_lpc824_hw_pll_entry (void)
{
    volatile uint32_t i = 0;

    /* 开启内部 IRC */
    if (__CLKOUT_SRC == AMHW_LPC82X_CLK_CLKOUT_SRC_IRC) {
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_IRC);
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_IRC_OUT);

    /* 开启 WDT 时钟振荡器 */
    } else if (__CLKOUT_SRC == AMHW_LPC82X_CLK_CLKOUT_SRC_WDTOSC) {
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_WDT_OSC);

    /* 开启系统振荡器 */
    } else if (__CLKOUT_SRC == AMHW_LPC82X_CLK_CLKOUT_SRC_SYSOSC) {
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_SYS_OSC);
     }

    /* 配置 PLL 之前，改变主时钟源为 IRC */
    amhw_lpc82x_clk_main_src_set(AMHW_LPC82X_CLK_MAIN_SRC_IRC);

    /* 设置 PLL 时钟源前的准备 */
    if (__PLL_CLKSRC == AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC) {

        /* 系统振荡器作为 PLL 时钟源 */

        /* 将 PIO0_8,、PIO0_9 配置为消极模式（无上拉和下拉） */
        amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                                   PIO0_8,
                                   AMHW_LPC82X_IOCON_MODE_INACTIVE);

        amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                                   PIO0_9,
                                   AMHW_LPC82X_IOCON_MODE_INACTIVE);

        /* 使能 PIO0_8_XTALIN，PIO0_9_XTALOUT 开关矩阵固定引脚功能 */
        amhw_lpc82x_swm_fixed_func_enable(LPC82X_SWM,
                                          AMHW_LPC82X_SWM_PIO0_8_XTALIN);

        amhw_lpc82x_swm_fixed_func_enable(LPC82X_SWM,
                                          AMHW_LPC82X_SWM_PIO0_9_XTALOUT);

        /* 外部 XTAL < 15MHz */
        amhw_lpc82x_clk_pll_bypass_set( AM_FALSE,  AM_FALSE);

         /* 使能系统 OSC */
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_SYS_OSC);

        for (i = 0; i < 200; i++);
    }

    if (__PLL_CLKSRC == AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN) {

        /* 外部管脚输入作为时钟源 */

        /* 将 PIO0_1 配置为消极模式（无上拉和下拉） */
        amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                                   PIO0_1,
                                   AMHW_LPC82X_IOCON_MODE_INACTIVE);

        /* 使能 PIO0_1 开关矩阵固定引脚功能 CLKIN */
        amhw_lpc82x_swm_fixed_func_enable(LPC82X_SWM,
                                          AMHW_LPC82X_SWM_PIO0_1_CLKIN);
        for (i = 0; i < 200; i++);
    }

    /* 设置 PLL 时钟源 */
    amhw_lpc82x_clk_pll_src_set(__PLL_CLKSRC);

    /* PLLOUT 时钟 */
    amhw_lpc82x_clk_pllctrl_set(__PLL_CFG_MSEL, __PLL_CFG_PSEL);
    amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_SYS_PLL);

    /* 等待 PLL 锁定 */
    while (!amhw_lpc82x_clk_pll_locked_chk());

    /* 配置主时钟为 PLL 输出 */
    amhw_lpc82x_clk_main_src_set(AMHW_LPC82X_CLK_MAIN_SRC_PLLOUT);

    /* 系统时钟分频为 48 */
    amhw_lpc82x_clk_system_clkdiv_set(__CLKOUT_DIV);

    am_bsp_delay_timer_init(am_arm_systick_inst_init(), 0);

    /* 因为时钟改变了，必须重新初始化串口 */
    am_debug_init(am_lpc82x_usart0_inst_init(), 115200);

    AM_DBG_INFO("PLL configure Success!\r\n");
    AM_DBG_INFO("The Desired rate is %8d Hz\r\n", __PLL_DESIREDRATE);
    AM_DBG_INFO("The Actual Desired rate is %8d Hz\r\n",
                amhw_lpc82x_clk_pllout_rate_get());

    /*  配置引脚为时钟输出 */
    am_gpio_pin_cfg(PIO0_27, PIO_FUNC_CLKOUT);

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
/** [src_lpc824_hw_pll] */

/* end of file */
