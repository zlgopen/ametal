/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X 时钟用户配置文件
 * \sa am_hwconf_lpc82x_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  lnk, first implementation.
 * \endinternal
 */

#include "am_lpc82x.h"
#include "am_lpc82x_clk.h"
#include "hw/amhw_lpc82x_swm.h"
#include "hw/amhw_lpc82x_iocon.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_clk
 * \copydoc am_hwconf_lpc82x_clk.c
 * @{
 */

/**
 * \brief  开发板PLL时钟源
 *       - 内部IRC（12MHz）  : AMHW_LPC82X_CLK_PLLIN_SRC_IRC
 *       - 外部晶振（12MHz） : AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC
 *       - 外部引脚输入时钟  : AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN
 * \note 开发板使用内部IRC，若要使用外部晶振，需要在开发板上焊接12MHz的晶振，\n
 *       并修改宏值为AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC,若使用外部引脚时钟输入,\n
 *       将12MHz时钟从PIO0_1引脚输入。
 */
#define __LPC82X_CLK_PLL_SRC  AMHW_LPC82X_CLK_PLLIN_SRC_IRC


/** \brief CLK 平台初始化 */
static void __lpc82x_clk_plfm_init (void)
{
    if(__LPC82X_CLK_PLL_SRC == AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC) {
        /**
         *  外部晶振和系统振荡器作为时钟源
         */

        /* 将PIO0_8,、PIO0_9 配置为消极模式（无上拉和下拉）*/
        amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_IOCON);

        amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                                   PIO0_8,
                                   AMHW_LPC82X_IOCON_MODE_INACTIVE);

        amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                                   PIO0_9,
                                   AMHW_LPC82X_IOCON_MODE_INACTIVE);

        /* 使能PIO0_8_XTALIN，PIO0_9_XTALOUT 开关矩阵固定引脚功能 */

        amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SWM);

        amhw_lpc82x_swm_fixed_func_enable(LPC82X_SWM,
                                          AMHW_LPC82X_SWM_PIO0_8_XTALIN);

        amhw_lpc82x_swm_fixed_func_enable(LPC82X_SWM,
                                          AMHW_LPC82X_SWM_PIO0_9_XTALOUT);

        } else if (__LPC82X_CLK_PLL_SRC == AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN) {
        /**
         *  外部管脚输入作为时钟源
         */

        /* 将PIO0_1 配置为消极模式（无上拉和下拉）*/

        amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_IOCON);

        amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,PIO0_1,
                                   AMHW_LPC82X_IOCON_MODE_INACTIVE);

        /* 使能PIO0_1 开关矩阵固定引脚功能 CLKIN */

        amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SWM);

        amhw_lpc82x_swm_fixed_func_enable(LPC82X_SWM,
                                          AMHW_LPC82X_SWM_PIO0_1_CLKIN);
        } else {
        /* 使用内部IRC 作时钟源 */
        }

}

/** \brief CLK设备信息
 *  默认值： 配置板子使用内部IRC（12MHz), 经过PLL倍频后输出到主时钟，\n
 *           主时钟 main_rate = 60 MHz, 系统时钟 system_clk = 30MHz
 *
 *  \note 可以通过修改__g_clk_devinfo配置main主时钟源和频率（主时钟
 *        使用PLL输出时，修改MSEL,PSEL实现），系统时钟由主时钟分频后得到,
 *        默认2分频
 */
static const am_lpc82x_clk_devinfo_t __g_clk_devinfo =
{
    /**
     * \brief PLL 时钟源
     *        PLL时钟源使用外部晶振
     *        内部IRC（12MHz）:    AMHW_LPC82X_CLK_PLLIN_SRC_IRC
     *        系统振荡器（12MHz） : AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC
     *        外部引脚输入时钟 : AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN
     */
    __LPC82X_CLK_PLL_SRC,

    /**
     * \brief PLL 控制寄存器MSEL，使FCLKOUT 在范围 100MHz内
     *        FCLKOUT = FCLKIN * (MSEL + 1) = 12MHz * 5 = 60 MHz
     */
    4,

    /**
     * \brief PLL 控制寄存器PSEL ,使FCCO 在范围 156 - 320MHz
     *        FCCO = FCLKOUT * 2 * 2^(PSEL) = 60MHz * 2 * 2 = 240MHz
     */
    1,

    /**
     * \brief MAIN 时钟源选择
     *        - 内部IRC      AMHW_LPC82X_CLK_MAIN_SRC_IRC
     *        - PLL输入      AMHW_LPC82X_CLK_MAIN_SRC_PLLIN
     *        - 看门狗振荡器 AMHW_LPC82X_CLK_MAIN_SRC_WDTOSC
     *        - PLL输出      AMHW_LPC82X_CLK_MAIN_SRC_PLLOUT
     * \note         main_clk = FCLKOUT = 60MHz
     */
    AMHW_LPC82X_CLK_MAIN_SRC_PLLOUT,

    /**
     * \brief 系统时钟分频数，可填 1- 255 之间的数值
     *        system_clk = main_clk / div = 60MHz / 2 = 30 MHz
     */
    2,

    /* 若MAIN 时钟源选择看门狗振荡器时，需要配置WDTOSC频率分频系数和频率选择
     * 这里为默认值 0.6MHz，64分频，时钟频率9.375KHz
     */

    /**
     * \brief WDTOSC频率分频系数，可填 2 - 64 之间的偶数
     *
     */
    64,

    /**
     * \brief WDTOSC频率选择
     *        wdtosc_freq = 600000UL Hz
     */
    AMHW_LPC82X_CLK_WDTOSC_RATE_0_6MHZ,

    /* 平台初始化函数，配置时钟引脚等操作 */
    __lpc82x_clk_plfm_init,

    /* CLK无平台去初始化函数 */
    NULL

};

/** \brief 时钟设备实例 */
static am_lpc82x_clk_dev_t __g_clk_dev;

/**
 * \brief  CLK 实例初始化，初始化系统时钟
 * \retval AM_OK : 时钟成功初始化
 */
int am_lpc82x_clk_inst_init (void)
{
    return am_lpc82x_clk_init(&__g_clk_dev, &__g_clk_devinfo);
}

/**
 * @}
 */

/* end of file */
