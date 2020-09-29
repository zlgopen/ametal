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
 * \brief HC32 CLK 用户配置文件
 * \sa am_hwconf_hc32_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-04  zp, first implementation
 * \endinternal
 */
#include "am_gpio.h"
#include "am_zlg116_clk.h"
#include "hw/amhw_zlg116_rcc.h"
#include "zlg116_pin.h"
/**
 * \addtogroup am_if_src_hwconf_hc32_clk
 * \copydoc am_hwconf_hc32_clk.c
 * @{
 */
/** \brief CLK 平台初始化 */
static void __zlg_clk_plfm_init (void)
{
}

/** \brief CLK设备信息 */
static const am_zlg116_clk_devinfo_t __g_clk_devinfo =
{
    /**
     * \brief HSEOSC外部晶振频率
     *
     *  如果pllin_src选择 AMHW_ZLG116_PLLCLK_HSE 则 PLLIN = hse_osc）
     *  PLLIN = 12000000
     */
    16000000,
    /** \brief
     *    PLL 时钟源选择
     *    -# AMHW_ZLG116_PLLCLK_HSI_DIV4 : HSI 振荡器  4 分频作为PLL输入时钟
     *    -# AMHW_ZLG116_PLLCLK_HSE      : HSE 作为PLL输入时钟
     */
    AMHW_ZLG116_PLLCLK_HSE,
    /**
     * \brief PLL 倍频系数，可选1-64
     *        PLLOUT = PLLIN * pll_mul / pll_div
     *        PLLOUT  = 12000000 * 4 / 1 = 48Mhz
     */
    3,
    /**
     * \brief PLL 分频系数，可选1-8
     *        PLLOUT = PLLIN * pll_mul / pll_div
     *        PLLOUT  = 12000000 * 4 / 1 = 48Mhz
     */
    1,
    /** \brief USB分频系数，USBCLK = PLLOUT / (usb_div + 1),建议配置成48Mhz */
    0,
    /**
     * \brief AHB分频系数，AHBCLK = PLLOUT / DIV,AHB最大频率为48Mhz
     *
     *    ahb_div |  DIV
     *   ---------------------
     *        0-7 |   1
     *        8   |   2
     *        9   |   4
     *        10  |   8
     *        11  |   16
     *        12  |   64
     *        13  |  128
     *        14  |  256
     *        15  |  512
     */
    0,
    /**
     * \brief APB1分频系数，APB1CLK = AHBCLK / (2 ^ apb1_div)
     *        APB1最大频率为24Mhz
     */
    1,
    /**
     * \brief APB2分频系数，APB2CLK = AHBCLK / (2 ^ apb2_div)
     *        APB2最大频率为48Mhz
     */
    0,
    /* 平台初始化函数，配置时钟引脚等操作 */
    __zlg_clk_plfm_init,
    /* CLK无平台去初始化函数 */
    NULL
};

/** \brief 时钟设备实例 */
static am_zlg116_clk_dev_t __g_clk_dev;

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \retval AM_OK : 时钟成功初始化
 */
int am_zlg116_clk_inst_deinit (void)
{
    return am_zlg116_clk_init(&__g_clk_dev, &__g_clk_devinfo);
}

/**
 * @}
 */

/* end of file */
