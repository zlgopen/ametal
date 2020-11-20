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
 * \brief HC32F460 CLK 用户配置文件
 * \sa am_hwconf_hc32f460_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-30  cds, first implementation
 * \endinternal
 */
#include "am_gpio.h"

#include "am_hc32f460_clk.h"
#include "hw/amhw_hc32f460_rcc.h"
/**
 * \addtogroup am_if_src_hwconf_hc32f460_clk
 * \copydoc am_hwconf_hc32f460_clk.c
 * @{
 */
/** \brief CLK 平台初始化 */
static void __hc32f460_clk_plfm_init (void)
{
}

/** \brief CLK 解平台初始化 */
static void __hc32f460_clk_plfm_deinit (void)
{
}

/*
 *
 * 系统时钟源的选择主要有五种，  HRC  ---  内部高速振荡器(16MHz/20MHz)
 *                       MRC  ---  外部中速振荡器(8MHz)
 *                       LRC  ---  内部低速振荡器(32.768KHz)
 *                       XTAL  ---  外部高速振荡器(晶振的频率范围：4 ~ 24MHz,外部时钟 输入：最高24MHz)
 *                       XTAL32  --- 晶振的频率范围：32.768KHz
 *                       MPLLP  --- MPLLP  = PLL输入时钟 / MPLL分频系数 * MPLL倍频系数 / MPLLP分频系数
 *
 * 系统时钟源选定，则得到以下关系
 *
 * SYSCLK = 被选择的系统时钟源
 * HCLK   = SYSCLK / HCLK_DIV
 * EXCLK  = SYSCLK / EXCLK_DIV
 * PCLK0  = SYSCLK / PCLK0_DIV
 * PCLK1  = SYSCLK / PCLK1_DIV
 * PCLK2  = SYSCLK / PCLK2_DIV
 * PCLK3  = SYSCLK / PCLK3_DIV
 * PCLK4  = SYSCLK / PCLK4_DIV
 *
 */
/** \brief CLK设备信息 */
static const am_hc32f460_clk_devinfo_t __g_clk_devinfo =
{
    /**
     * \brief XTH外部高速时钟晶振频率（Hz）
     */
    8000000,

    /**
     * \brief XTL外部低速时钟晶振频率（Hz）
     */
    32768,

    /**
     * \brief HRC内部高速时钟晶振频率（Hz）
     *
     * \note 频率选择 20M、16M
     */
    AMHW_HC32F460_HRC_16M,

    /** \brief
     *    PLL时钟源选择
     *    -# AMHW_HC32F460_CLK_PLLSRC_XTAL : 选择外部高速振荡器作为输入时钟
     *    -# AMHW_HC32F460_CLK_PLLSRC_HRC : 选择内部高速振荡器作为输入时钟
     */
    AMHW_HC32F460_CLK_PLLSRC_HRC,

    /**
     * \brief MPLL 输出时钟配置
     *        MPLLP = PLLIN / mpll_div * mpll_mul / mpllp_div。
     *        MPLLQ = PLLIN / mpll_div * mpll_mul / mpllq_div。
     *        MPLLR = PLLIN / mpll_div * mpll_mul / mpllr_div。
     */
    2,     /* MPLL 输入时钟分频系数,1~24，应确保MPLLIN介于1MHz到12MHz之间 */
    42,    /* MPLL 输出时钟倍频系数，20~480倍,240M~480M */
    2,     /* MPLLP时钟分频系数，2~16 */
    2,     /* MPLLQ时钟分频系数，2~16 */
    2,     /* MPLLR时钟分频系数，2~16 */

    /**
     * \brief UPLL 输入时钟分频系数
     *        UPLLP = PLLIN / upll_div * upll_mul / upllp_div。
     *        UPLLQ = PLLIN / upll_div * upll_mul / upllq_div。
     *        UPLLR = PLLIN / upll_div * upll_mul / upllr_div。
     */
    4,     /* UPLL 输入时钟分频系数，应确保UPLLIN介于1MHz到12MHz之间 */
    84,    /* UPLL 输出时钟倍频系数，20~480倍,240M~480M */
    7,     /* UPLLP时钟分频系数，2~16 */
    7,     /* UPLLQ时钟分频系数，2~16 */
    7,     /* UPLLR时钟分频系数，2~16 */

    /** \brief
     *    系统 时钟源选择
     *    -# AMHW_HC32F460_SYSCLK_HRC : 内部高速振荡器作为系统时钟
     *    -# AMHW_HC32F460_SYSCLK_MRC : 外部中速振荡器作为系统时钟
     *    -# AMHW_HC32F460_SYSCLK_LRC : 内部低速时钟作为系统时钟
     *    -# AMHW_HC32F460_SYSCLK_XTAL ：  外部高速振荡器作为系统时钟
     *    -# AMHW_HC32F460_SYSCLK_XTAL32 ： 外部低速振荡器作为系统时钟
     *    -# AMHW_HC32F460_SYSCLK_MPLL ： MPLL作为系统时钟
     */
    AMHW_HC32F460_SYSCLK_MPLL,

    /** \brief
     *    USBCK时钟源选择
     *    -# AMHW_HC32F460_CLK_USBCKS_SYSCLK_DIV_2 : 系统时钟2分频
     *    -# AMHW_HC32F460_CLK_USBCKS_SYSCLK_DIV_3 : 系统时钟3分频
     *    -# AMHW_HC32F460_CLK_USBCKS_SYSCLK_DIV_4 : 系统时钟4分频
     *    -# AMHW_HC32F460_CLK_USBCKS_MPLL_P ： MPLL/P
     *    -# AMHW_HC32F460_CLK_USBCKS_MPLL_Q ： MPLL/Q
     *    -# AMHW_HC32F460_CLK_USBCKS_MPLL_R : MPLL/R
     *    -# AMHW_HC32F460_CLK_USBCKS_UPLL_P : UPLL/P
     *    -# AMHW_HC32F460_CLK_USBCKS_UPLL_Q : UPLL/Q
     *    -# AMHW_HC32F460_CLK_USBCKS_UPLL_R ：  UPLL/R
     */
    AMHW_HC32F460_CLK_USBCKS_UPLL_P,


    /*
     * HCLK = SYSCLK / HCLK_DIV，最高168MHz
     * 作用范围：CPU、DMAn(n=1,2)、EFM(主闪存)、SRAM0、SRAM1、SRAM2、SRAMHS、Ret-SRAM、
     *         MPU、GPIO、DCU、INTC、QSPI.
     **/
    AMHW_HC32F460_SYSCLK_DIV_1,

    /*
     * EXCLK = SYSCLK / EXCLK_DIV，最高84MHz
     * 作用范围：SDIOn(n=1,2)、 CAN
     **/
    AMHW_HC32F460_SYSCLK_DIV_2,

    /*
     *  PCLK0 = SYSCLK / PCLK0_DIV，最高168MHz
     *  作用范围：Timer6计数器用时钟
     **/
    AMHW_HC32F460_SYSCLK_DIV_1,

    /*
     * PCLK1 = SYSCLK / PCLK1_DIV，最高84MHz
     * 作用范围：USARTn(n = 1~4)、SPIn(n=1~4)、USBFS(控制逻辑)、Timer0n(n=1、2)、TimerAn(n=1~6)、
     *         Timer6(控制逻辑)、EMB、CRC、HASH、AES、I2Sn(n=1~4)控制逻辑
     **/
    AMHW_HC32F460_SYSCLK_DIV_2,

    /*
     * PCLK2 = SYSCLK / PCLK2_DIV，最高60MHz
     * 作用范围：AD变换时钟
     **/
    AMHW_HC32F460_SYSCLK_DIV_4,

    /*
     * PCLK3 = SYSCLK / PCLK3_DIV，最高42MHz
     * 作用范围：RTC(控制逻辑)、I2Cn(n=1、2、3)、CMP、WDT、SWDT(控制逻辑)
     **/
    AMHW_HC32F460_SYSCLK_DIV_4,

    /*
     * PCLK4 = SYSCLK / PCLK4_DIV，最高84MHz
     * 作用范围： ADC(控制逻辑)、TRNG
     **/
    AMHW_HC32F460_SYSCLK_DIV_2,

    /** \brief 平台初始化函数，配置引脚等工作 */
    __hc32f460_clk_plfm_init,

    /** \brief 平台解初始化函数 */
    __hc32f460_clk_plfm_deinit,
};

/** \brief 时钟设备实例 */
static am_hc32f460_clk_dev_t __g_clk_dev;

/**
 * \brief CLK 实例初始化，初始化系统时钟
 *
 * \retval AM_OK : 时钟成功初始化
 */
int am_hc32_clk_inst_deinit (void)
{
    return am_hc32f460_clk_init(&__g_clk_dev, &__g_clk_devinfo);
}

/**
 * @}
 */

/* end of file */
