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
 * \brief MM32L373 CLK 用户配置文件
 * \sa am_hwconf_mm32l373_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */
#include "am_gpio.h"

#include "am_mm32l373_clk.h"
#include "amhw_mm32l373_rcc.h"
#include "mm32l373_pin.h"
/**
 * \addtogroup am_if_src_hwconf_mm32l373_clk
 * \copydoc am_hwconf_mm32l373_clk.c
 * @{
 */
/** \brief CLK 平台初始化 */
static void __mm32l373_clk_plfm_init (void)
{
}

/** \brief CLK设备信息 */
static const am_mm32l373_clk_devinfo_t __g_clk_devinfo =
{
    /**
     * \brief HSEOSC外部晶振频率
     *
     *  如果pllin_src选择 AMHW_MM32L373_PLLCLK_HSE 则 PLLIN = hse_osc）
     */
    12000000,

    /**
     * \brief LSEOSC外部晶振频率
     *
     *  如果rtc_src选择 AMHW_MM32L373_RTCCLK_LSE 则 rtcclk = lse_osc）
     */
    0,

    /** \brief
     *    PLL 时钟源选择
     *    -# AMHW_MM32L373_PLLCLK_HSI_DIV4 : HSI 振荡器 4 分频作为PLL输入时钟
     *    -# AMHW_MM32L373_PLLCLK_HSE      : HSE 作为PLL输入时钟
     */
    AMHW_MM32L373_PLLCLK_HSE,

    /**
     * \brief PLL 倍频系数，可选1-64
     *        PLLOUT = PLLIN * pll_mul / pll_div
     */
    8,

    /**
     * \brief PLL 分频系数，可选1-8
     *        PLLOUT = PLLIN * pll_mul / pll_div
     */
    1,

    /** \brief USB分频系数，USBCLK = PLLOUT / (usb_div + 1),建议配置成48Mhz */
    1,

    /**
     * \brief AHB分频系数，AHBCLK = PLLOUT / DIV,AHB最大频率为96Mhz
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
     *        APB1最大频率为96Mhz
     */
    1,

    /**
     * \brief APB2分频系数，APB2CLK = AHBCLK / (2 ^ apb2_div)
     *        APB2最大频率为96Mhz
     */
    0,

    /**
     * \brief ADC分频系数，ADCCLK = APB2 / (2 ^ adc_div)
     *        ADC最大频率为48Mhz
     */
    1,

    /** \brief 平台初始化函数，配置引脚等工作 */
    __mm32l373_clk_plfm_init,

    /** \brief 平台解初始化函数 */
    NULL,
};

/** \brief 时钟设备实例 */
static am_mm32l373_clk_dev_t __g_clk_dev;

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \retval AM_OK : 时钟成功初始化
 */
int am_mm32l373_clk_inst_init (void)
{
    return am_mm32l373_clk_init(&__g_clk_dev, &__g_clk_devinfo);
}

/**
 * @}
 */

/* end of file */
