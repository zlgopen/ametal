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
 * \brief KL26 CLK 用户配置文件。
 * \sa am_kl26_hwconfig_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-30  mkr, first implementation.
 * \endinternal
 */

#include "am_kl26_clk.h"
#include "hw/amhw_kl26_sim.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "am_bitops.h"
#include "hw/amhw_kl26_mcg.h"
#include "../../../../../soc/freescale/kl26/kl26_periph_map.h"
#include "../../../../../soc/freescale/kl26/kl26_pin.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_clk
 * \copydoc am_kl26_hwconfig_clk.c
 * @{
 */

/**
 * \brief 时钟工作模式,
 *
 *   可定义为AM_KL26_CLK_MODE_FEI或AM_KL26_CLK_MODE_PEE
 *
 *  - FEI mode: 使用内部慢速 IRC(32.768KHz)作FLL参考时钟 ，经FLL倍频后(47.972352MHz)作主时钟，
 *              Core clock = 47.972352MHz，
 *              Bus clock  = 23.986171MHz
 *
 *  - PEE mode: 使用外部时钟XTAL作PLL参考时钟 ，经PLL倍频后(96MHz)作主时钟，
 *              Core clock = 48MHz，
 *              Bus clock  = 24Hz
 *
 * \note 改变时钟模式时必面改变配置设备信息的core_clk_div成员, 当为AM_KL26_CLK_MODE_FEI，该值为1，当为AM_KL26_CLK_MODE_PEE，其值为2
 *
 */
#define __CLK_MODE    AM_KL26_CLK_MODE_FEI

/**
 * \brief CLK 平台初始化
 */
static void __kl26_clk_plfm_init (void)
{
    /** \brief  设置外部晶振OSC引脚功能 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_PORTA);

    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOA_18, PIOA_18_EXTAL0);
    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOA_19, PIOA_19_XTAL0);
}

/**
 * \brief CLK 平台去初始化
 */
static void __kl26_clk_plfm_deinit (void)
{
    /* 时钟无去初始化操作*/
    return;
}

/**
 * \brief CLK 设备信息
 */
static const am_kl26_clk_devinfo_t __g_clk_devinfo = {

    /* 时钟工作模式 */
    __CLK_MODE,

    /* AM_KL26_CLK_MODE_PEE模式的外部晶振频率，可用8-32MHz, 4的整数倍 */
    (8 * 1000 * 1000),

    /* 内核时钟core_clk分频系数(1-16),
     * CORE_CLK = MCGOUTCLK / core_clk_div
     * Core clock upto 48MHz
     **************************************************
     * mode    core_clk_div   Core clock    Bus clock *
     * FEI        1          47.972352MHz 23.986171MHz*
     * PEE        2             48MHz        24MHz    *
     **************************************************
     */
    1,

    /* CLK平台初始化 */
	__kl26_clk_plfm_init,

	/*  CLK 平台去初始化 */
	__kl26_clk_plfm_deinit,

};

/** \brief CLK设备实例   */
static am_kl26_clk_dev_t __g_clk_dev;

/**
 * \brief  CLK 实例初始化，初始化系统时钟
 *
 * \retval AM_OK : 时钟成功初始化
 */
int am_kl26_clk_inst_init (void)
{
    return am_kl26_clk_init(&__g_clk_dev, &__g_clk_devinfo);
}

/**
 * @}
 */

/* end of file */
