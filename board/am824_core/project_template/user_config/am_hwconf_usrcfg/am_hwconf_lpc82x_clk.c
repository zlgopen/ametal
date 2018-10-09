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
 * \brief LPC82X 时钟用户配置文件
 * \sa am_hwconf_lpc82x_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  lnk, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc82x_clk.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_swm.h"
#include "hw/amhw_lpc82x_iocon.h"
#include "hw/amhw_lpc82x_syscon.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_clk
 * \copydoc am_hwconf_lpc82x_clk.c
 * @{
 */

/**
 * \brief PLL 时钟源
 *
 * - 内部 IRC（12MHz）: AMHW_LPC82X_CLK_PLLIN_SRC_IRC
 * - 外部晶振（12MHz）: AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC
 * - 外部引脚输入时钟: AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN
 *
 * \note 默认使用内部 IRC，若要使用外部晶振，需要在开发板上焊接 12MHz 晶振，
 *       并修改宏值为 AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC，若使用外部引脚时钟输入，
 *       将 12MHz 时钟从 PIO0_1 引脚输入
 */
#define __LPC82X_CLK_PLL_SRC    AMHW_LPC82X_CLK_PLLIN_SRC_IRC

/**
 * \brief CLK 平台初始化
 */
am_local void __lpc82x_clk_plfm_init (void)
{
    if(__LPC82X_CLK_PLL_SRC == AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC) {

        /*
         * 外部晶振和系统振荡器作为时钟源
         */

        /* 将 PIO0_8,、PIO0_9 配置为消极模式（无上拉和下拉） */
        amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_IOCON);

        amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                                   PIO0_8,
                                   AMHW_LPC82X_IOCON_MODE_INACTIVE);

        amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                                   PIO0_9,
                                   AMHW_LPC82X_IOCON_MODE_INACTIVE);

        /* 使能 PIO0_8_XTALIN，PIO0_9_XTALOUT 开关矩阵固定引脚功能 */

        amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SWM);

        amhw_lpc82x_swm_fixed_func_enable(LPC82X_SWM,
                                          AMHW_LPC82X_SWM_PIO0_8_XTALIN);

        amhw_lpc82x_swm_fixed_func_enable(LPC82X_SWM,
                                          AMHW_LPC82X_SWM_PIO0_9_XTALOUT);

    } else if (__LPC82X_CLK_PLL_SRC == AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN) {

        /*
         * 外部管脚输入作为时钟源
         */

        /* 将 PIO0_1 配置为消极模式（无上拉和下拉） */

        amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_IOCON);

        amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,PIO0_1,
                                   AMHW_LPC82X_IOCON_MODE_INACTIVE);

        /* 使能 PIO0_1 开关矩阵固定引脚功能 CLKIN */

        amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SWM);

        amhw_lpc82x_swm_fixed_func_enable(LPC82X_SWM,
                                          AMHW_LPC82X_SWM_PIO0_1_CLKIN);
    } else {

        /*
         * 使用内部IRC 作时钟源
         */
    }
}

/**
 * \brief CLK 设备信息
 *
 * 默认使用内部 IRC（12MHz), 经过 PLL 倍频后输出到主时钟，主时钟
 * main_rate = 60 MHz, 系统时钟 system_clk = 30MHz
 *
 * \note 可以通过修改 __g_lpc82x_clk_devinfo 配置主时钟源和频率（主时钟使用 PLL
 *       输出时，修改 MSEL，PSEL 实现），系统时钟由主时钟分频后得到，默认 2 分频
 */
am_local am_const am_lpc82x_clk_devinfo_t __g_lpc82x_clk_devinfo = {

    /*
     * PLL 时钟源
     * 内部 IRC（12MHz）: AMHW_LPC82X_CLK_PLLIN_SRC_IRC
     * 系统振荡器（12MHz）: AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC
     * 外部引脚输入时钟: AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN
     */
    __LPC82X_CLK_PLL_SRC,

    /*
     * PLL 控制寄存器 MSEL，使 FCLKOUT 在范围 100MHz 内
     * FCLKOUT = FCLKIN * (MSEL + 1) = 12MHz * 5 = 60 MHz
     */
    4,

    /*
     * PLL 控制寄存器 PSEL，使 FCCO 在范围 156MHz - 320MHz
     * FCCO = FCLKOUT * 2 * 2^(PSEL) = 60MHz * 2 * 2 = 240MHz
     */
    1,

    /*
     * 主时钟源选择 main_clk = FCLKOUT = 60MHz
     * - 内部 IRC: AMHW_LPC82X_CLK_MAIN_SRC_IRC
     * - PLL 输入: AMHW_LPC82X_CLK_MAIN_SRC_PLLIN
     * - 看门狗振荡器: AMHW_LPC82X_CLK_MAIN_SRC_WDTOSC
     * - PLL 输出: AMHW_LPC82X_CLK_MAIN_SRC_PLLOUT
     */
    AMHW_LPC82X_CLK_MAIN_SRC_PLLOUT,

    /*
     * 系统时钟分频数，可填 1- 255 之间的数值
     * system_clk = main_clk / div = 60MHz / 2 = 30 MHz
     */
    2,

    /*
     * WDTOSC 频率分频系数，可填 2 - 64 之间的偶数
     * 若主时钟源选择看门狗振荡器时，需要配置 WDTOSC 频率分频系数和频率选择
     * 这里为默认值 0.6MHz，64 分频，时钟频率 9.375KHz
     */
    64,

    /*
     * WDTOSC 频率选择
     * wdtosc_freq = 600000UL Hz
     */
    AMHW_LPC82X_CLK_WDTOSC_RATE_0_6MHZ,

    /* 平台初始化函数 */
    __lpc82x_clk_plfm_init,

    /* 无平台解初始化函数 */
    NULL
};

/** \brief CLK 设备实例 */
am_local am_lpc82x_clk_dev_t __g_lpc82x_clk_dev;

/**
 * \brief CLK 实例初始化
 */
int am_lpc82x_clk_inst_init (void)
{
    return am_lpc82x_clk_init(&__g_lpc82x_clk_dev, &__g_lpc82x_clk_devinfo);
}

/**
 * @}
 */

/* end of file */
