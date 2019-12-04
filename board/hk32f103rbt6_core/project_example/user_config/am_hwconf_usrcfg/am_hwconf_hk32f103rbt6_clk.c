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
 * \brief HK32F103RBT6 CLK 用户配置文件
 * \sa am_hwconf_hk32f103rbt6_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-03  bob, first implementation
 * \endinternal
 */
#include "hk32f103rbt6_pin.h"
#include "am_gpio.h"
#include "am_hk32f103rbt6_clk.h"
#include "amhw_hk32f103rbt6_rcc.h"
/**
 * \addtogroup am_if_src_hwconf_hk32f103rbt6_clk
 * \copydoc am_hwconf_hk32f103rbt6_clk.c
 * @{
 */
/** \brief CLK 平台初始化 */
static void __hk32f103rbt6_clk_plfm_init (void)
{
}

/** \brief CLK设备信息 */
static const am_hk32f103rbt6_clk_devinfo_t __g_clk_devinfo =
{
    /**
     * \brief 系统时钟源选择
     *
     * AM_HK32F103RBT6_SYSCLK_HSI : HSI 作为系统时钟
     * AM_HK32F103RBT6_SYSCLK_HSE : HSE 作为系统时钟
     * AM_HK32F103RBT6_SYSCLK_PLL : PLL 输出作为系统时钟
     * AM_HK32F103RBT6_SYSCLK_LSI : LSI 输出作为系统时钟
     * AM_HK32F103RBT6_SYSCLK_LSE : LSE 输出作为系统时钟
     */
    AM_HK32F103RBT6_SYSCLK_PLL,

    /**
     * \brief HSEOSC外部晶振频率
     *
     *  如果pllin_src选择 AMHW_HK32F103RBT6_PLLCLK_HSE 则 PLLIN = hse_osc）
     */
    12000000,

    /**
     * \brief LSEOSC外部晶振频率
     *
     *  如果rtc_src选择 AMHW_HK32F103RBT6_RTCCLK_LSE 则 rtcclk = lse_osc）
     */
    0,

    /** \brief
     *    PLL 时钟源选择
     *    -# AMHW_HK32F103RBT6_PLLCLK_HSI : HSI 作为PLL输入时钟
     *    -# AMHW_HK32F103RBT6_PLLCLK_HSE : HSE 作为PLL输入时钟
     */
    AMHW_HK32F103RBT6_PLLCLK_HSE,

    /**
     * \brief PLL 倍频系数，可选2-17
     *        PLLOUT = PLLIN * pll_mul
     */
     6,

    /** \brief USB分频系数，
     *
     *  AMHW_HK32F103RBT6_PLL_USB_DIV1P5 : PLL 1.5 分频作为usb输入时钟 ,USBCLK = PLLOUT / 1.5;
     *  AMHW_HK32F103RBT6_PLL_USB_DIV1   : PLL 1 分频作为usb输入时钟 ,  USBCLK = PLLOUT / 1。
     *
     *  建议配置成48Mhz
     */
    AMHW_HK32F103RBT6_PLL_USB_DIV1,

    /**
     * \brief AHB分频系数，AHBCLK = PLLOUT / DIV,AHB最大频率为80Mhz
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
     *        APB1最大频率为36Mhz
     */
    1,

    /**
     * \brief APB2分频系数，APB2CLK = AHBCLK / (2 ^ apb2_div)
     *        APB2最大频率为72Mhz
     */
    0,

    /**
     * \brief ADC分频系数，ADCCLK = APB2 / DIV
     *    adc_div |  DIV
     *   -----------------
     *          0 | 2 分频
     *          1 | 4 分频
     *          2 | 8 分频
     *          3 | 16分频
     *
     *        ADC最大频率为14Mhz
     */
    0,

    /** \brief 平台初始化函数，配置引脚等工作 */
    __hk32f103rbt6_clk_plfm_init,

    /** \brief 平台解初始化函数 */
    NULL,
};

/** \brief 时钟设备实例 */
static am_hk32f103rbt6_clk_dev_t __g_clk_dev;

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \retval AM_OK : 时钟成功初始化
 */
int am_hk32f103rbt6_clk_inst_init (void)
{
    return am_hk32f103rbt6_clk_init(&__g_clk_dev, &__g_clk_devinfo);
}

/**
 * @}
 */

/* end of file */
