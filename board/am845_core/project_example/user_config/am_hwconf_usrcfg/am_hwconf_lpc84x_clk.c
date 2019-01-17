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
 * \brief LPC84X 时钟用户配置文件
 * \sa am_hwconf_lpc84x_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  lnk, first implementation
 * \endinternal
 */

#include <am_lpc84x.h>
#include "ametal.h"
#include "am_lpc84x_clk.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_swm.h"
#include "hw/amhw_lpc84x_iocon.h"
#include "hw/amhw_lpc84x_syscon.h"

/**
 * \addtogroup am_if_src_hwconf_lpc84x_clk
 * \copydoc am_hwconf_lpc84x_clk.c
 * @{
 */

/**
 * \brief PLL 时钟源
 *
 * - 内部 IRC（12MHz）: AMHW_LPC84X_CLK_PLLIN_SRC_IRC
 * - 外部晶振（12MHz）: AMHW_LPC84X_CLK_PLLIN_SRC_SYSOSC
 * - 外部引脚输入时钟: AMHW_LPC84X_CLK_PLLIN_SRC_CLKIN
 *
 * \note 默认使用内部 IRC，若要使用外部晶振，需要在开发板上焊接 12MHz 晶振，
 *       并修改宏值为 AMHW_LPC84X_CLK_PLLIN_SRC_SYSOSC，若使用外部引脚时钟输入，
 *       将 12MHz 时钟从 PIO0_1 引脚输入
 */
#define __LPC84X_CLK_PLL_SRC     AMHW_LPC84X_CLK_PLLIN_SRC_EXTERNAL_CLK
#define __LPC84X_EXTERNAL_SRC    AMHW_LPC84X_EXTERNAL_CLK_PLLIN_SRC_SYS_OSC_CLK

/**
 * \brief CLK 平台初始化
 */
am_local void __lpc84x_clk_plfm_init (void)
{
    if(__LPC84X_CLK_PLL_SRC == AMHW_LPC84X_CLK_PLLIN_SRC_EXTERNAL_CLK &&
    		__LPC84X_EXTERNAL_SRC ==  AMHW_LPC84X_EXTERNAL_CLK_PLLIN_SRC_SYS_OSC_CLK) {

        /*
         * 外部晶振和系统振荡器作为时钟源
         */

        /* 将 PIO0_8,、PIO0_9 配置为消极模式（无上拉和下拉） */
        amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SWM);

		amhw_lpc84x_iocon_mode_set(LPC84X_IOCON,
						           PIO0_8,
						           AMHW_LPC84X_IOCON_MODE_INACTIVE);
		amhw_lpc84x_iocon_mode_set(LPC84X_IOCON,
						           PIO0_9,
						           AMHW_LPC84X_IOCON_MODE_INACTIVE);

        amhw_lpc84x_swm_fixed_func_enable(LPC84X_SWM,
        		                          AMHW_LPC84X_SWM_PIO0_8_XTALIN);
        amhw_lpc84x_swm_fixed_func_enable(LPC84X_SWM,
        		                          AMHW_LPC84X_SWM_PIO0_9_XTALOUT);

    } else if (__LPC84X_CLK_PLL_SRC == AMHW_LPC84X_CLK_PLLIN_SRC_EXTERNAL_CLK &&
    		__LPC84X_EXTERNAL_SRC ==  AMHW_LPC84X_EXTERNAL_CLK_PLLIN_SRC_CLK_IN) {

        /*
         * 外部管脚输入作为时钟源
         */

        /* 将 PIO0_1 配置为消极模式（无上拉和下拉） */

        amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_IOCON);

        amhw_lpc84x_iocon_mode_set(LPC84X_IOCON,PIO0_1,
                                   AMHW_LPC84X_IOCON_MODE_INACTIVE);

        /* 使能 PIO0_1 开关矩阵固定引脚功能 CLKIN */

        amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SWM);

        amhw_lpc84x_swm_fixed_func_enable(LPC84X_SWM,
                                          AMHW_LPC84X_SWM_PIO0_1_CLKIN);
    } else {

        /*
         * 使用内部IRC 作时钟源
         */
    }
}

/**
 * \brief CLK 设备信息
 *
 * 默认使用内部 fro（24MHz), 经过 PLL 倍频后输出到主时钟，主时钟
 * main_rate = 60 MHz, 系统时钟 system_clk = 30MHz
 *
 * \note 可以通过修改 __g_lpc84x_clk_devinfo 配置主时钟源和频率（主时钟使用 PLL
 *       输出时，修改 MSEL，PSEL 实现），系统时钟由主时钟分频后得到，默认 2 分频
 */
am_local am_lpc84x_clk_devinfo_t __g_lpc84x_clk_devinfo = {

	/** \brief fro 时钟选择  */
	AMHW_LPC84X_CLK_FRO_24000000,

    /*
     * 外部 时钟源
     * 使用io输入时钟还是晶振输入时钟
     * 如PLL为选择外部时钟源则此配置无效
     */
	AMHW_LPC84X_EXTERNAL_CLK_PLLIN_SRC_SYS_OSC_CLK,

    /*
     * PLL 时钟源
     * 内部 IFRO（默认24MHz）: AMHW_LPC84X_CLK_PLLIN_SRC_FRO
     * 外部时钟(选择外部时钟为PLL输入源，则选择的外部 时钟源有效): AMHW_LPC84X_CLK_PLLIN_SRC_EXTERNAL_CLK
     *  看门狗振荡器: AMHW_LPC84X_CLK_PLLIN_SRC_WTD_CLK
     * 内部 IFRO_DIV（默认12MHz）：AMHW_LPC84X_CLK_PLLIN_SRC_WTD_CLK
     */
	AMHW_LPC84X_CLK_PLLIN_SRC_FRO_DIV,

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
     * 主时钟源选择
     * - 默认不经过PLL时钟: AMHW_LPC84X_MAIN_CLK_PLLIN_SRC_MAIN_CLK_PRE_PLL
     * - 经过PLL时钟 ：AMHW_LPC84X_CLK_PLLIN_SRC_EXTERNAL_SYS_PLL
     */
	AMHW_LPC84X_CLK_PLLIN_SRC_EXTERNAL_SYS_PLL,

    /*
     * 主时钟源通过分频后作为系统时钟，分频值可填 1- 255 之间的数值
     * system_clk = main_clk / div = 60MHz / 2 = 30 MHz
     */
    2,

    /*
     * WDTOSC 频率分频系数，可填 2 - 64 之间的偶数
     * 若主时钟源选择看门狗振荡器时，需要配置 WDTOSC 频率分频系数和频率选择
     * 这里为默认值 4.6MHz，2 分频，时钟频率 2.3MHz
     */
    2,

    /*
     * WDTOSC 频率选择
     * wdtosc_freq = 600000UL Hz
     */
	AMHW_LPC84X_CLK_WDTOSC_RATE_4_6MHZ,

    /*
     * FRG0 时钟源频率选择
     * - FRO：AMHW_LPC84X_CLK_FRG_FRO
     * - 主时钟：AMHW_LPC84X_CLK_FRG_MAIN_CLK
     * - PLL输出时钟： AMHW_LPC84X_CLK_FRG_SYS_PLL0_CLK
     */
	AMHW_LPC84X_CLK_FRG_MAIN_CLK,

    /*
     * FRG1 时钟源频率选择
     * - FRO：AMHW_LPC84X_CLK_FRG_FRO
     * - 主时钟：AMHW_LPC84X_CLK_FRG_MAIN_CLK
     * - PLL输出时钟： AMHW_LPC84X_CLK_FRG_SYS_PLL0_CLK
     */
	AMHW_LPC84X_CLK_FRG_MAIN_CLK,

    /* 平台初始化函数 */
    __lpc84x_clk_plfm_init,

    /* 无平台解初始化函数 */
    NULL
};

/** \brief CLK 设备实例 */
am_local am_lpc84x_clk_dev_t __g_lpc84x_clk_dev;

/**
 * \brief CLK 实例初始化
 */
int am_lpc84x_clk_inst_init (void)
{

    return am_lpc84x_clk_init(&__g_lpc84x_clk_dev, &__g_lpc84x_clk_devinfo);
}

/**
 * @}
 */

/* end of file */
