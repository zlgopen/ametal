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
 * \brief
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-04  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_RCC_H
#define __AMHW_ZSL42x_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "zsl42x_regbase.h"

/*
 * \brief 匿名结构体段的开始
 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_zsl42x_if_rcc
 * \copydoc amhw_zsl42x_rcc.h
 * @{
 */

/**
 * \brief CRC寄存器块结构体
 */
typedef struct amhw_zsl42x_rcc {
  __IO uint32_t sysctrl[3];       /**< \brief 系统控制寄存器0、1、2 */
  __IO uint32_t rch_cr;           /**< \brief 内部高速时钟控制寄存器 */
  __IO uint32_t xth_cr;           /**< \brief 外部高速时钟控制寄存器 */
  __IO uint32_t rcl_cr;           /**< \brief 内部低速时钟控制寄存器 */
  __IO uint32_t xtl_cr;            /**< \brief 外部低速时钟控制寄存器 */
  __IO uint32_t reserve0;         /**< \brief 保留位 */
  __IO uint32_t peri_clken[2];    /**< \brief 外设时钟使能寄存器 */
  __IO uint32_t reserve1[5];         /**< \brief 保留位 */
  __IO uint32_t pll_cr;           /**< \brief PLL时钟控制寄存器 */
} amhw_zsl42x_rcc_t;

#define ZSL42x_RCCCTRL            ((amhw_zsl42x_rcc_t *)ZSL42x_RCC_BASE)

/**
 * \brief 外设枚举
 */
typedef enum {

    /* peri_clken[0]寄存器控制的外设使能位  */
    AMHW_ZSL42x_RCC_FLASH   = 31,
    AMHW_ZSL42x_RCC_DMA     = 29,
    AMHW_ZSL42x_RCC_GPIO    = 28,
    AMHW_ZSL42x_RCC_AES     = 27,
    AMHW_ZSL42x_RCC_CRC     = 26,
    AMHW_ZSL42x_RCC_SWD     = 25,
    AMHW_ZSL42x_RCC_TICK    = 24,
    AMHW_ZSL42x_RCC_LCD     = 22,
    AMHW_ZSL42x_RCC_TRIM    = 21,
    AMHW_ZSL42x_RCC_RTC     = 20,
    AMHW_ZSL42x_RCC_PCNT    = 19,
    AMHW_ZSL42x_RCC_RNG     = 18,
    AMHW_ZSL42x_RCC_VC      = 17,
    AMHW_ZSL42x_RCC_ADC     = 16,
    AMHW_ZSL42x_RCC_WDT     = 15,
    AMHW_ZSL42x_RCC_PCA     = 14,
    AMHW_ZSL42x_RCC_OPA     = 13,
    AMHW_ZSL42x_RCC_TIM3    = 11,
    AMHW_ZSL42x_RCC_ADVTIM  = 10,
    AMHW_ZSL42x_RCC_LPTIM0  = 9,
    AMHW_ZSL42x_RCC_BASETIM = 8,
    AMHW_ZSL42x_RCC_SPI1    = 7,
    AMHW_ZSL42x_RCC_SPI0    = 6,
    AMHW_ZSL42x_RCC_I2C1    = 5,
    AMHW_ZSL42x_RCC_I2C0    = 4,
    AMHW_ZSL42x_RCC_LPUART1 = 3,
    AMHW_ZSL42x_RCC_LPUART0 = 2,
    AMHW_ZSL42x_RCC_UART1   = 1,
    AMHW_ZSL42x_RCC_UART0   = 0,

    /* peri_clken[1]寄存器控制的外设使能位  */
    AMHW_ZSL42x_RCC_UART3   = 9 + 31,
    AMHW_ZSL42x_RCC_UART2   = 8 + 31,
    AMHW_ZSL42x_RCC_LPTIM1  = 4 + 31,
    AMHW_ZSL42x_RCC_DAC     = 3 + 31,
}amhw_zsl42x_peripheral;

/**
 * \brief 系统时钟源
 */
typedef enum {
    AMHW_ZSL42x_SYSCLK_RCH = 0,   /**< \brief 内部高速时钟作为系统时钟 */
    AMHW_ZSL42x_SYSCLK_XTH,       /**< \brief 外部高速时钟作为系统时钟 */
    AMHW_ZSL42x_SYSCLK_RCL,       /**< \brief 内部低速时钟作为系统时钟 */
    AMHW_ZSL42x_SYSCLK_XTL,       /**< \brief 外部低速时钟作为系统时钟 */
    AMHW_ZSL42x_SYSCLK_PLL = 4,   /**< \brief 内部PLL作为系统时钟 */
}amhw_zsl42x_sys_clk_src;

/**
 * \brief HCLK时钟源
 */
typedef enum {
    AMHW_ZSL42x_HCLK_SYSCLK = 0,        /**< \brief 系统时钟/1  作为HCLK源 */
    AMHW_ZSL42x_HCLK_SYSCLK_DIV2,       /**< \brief 系统时钟/2  作为HCLK源 */
    AMHW_ZSL42x_HCLK_SYSCLK_DIV4,       /**< \brief 系统时钟/4  作为HCLK源 */
    AMHW_ZSL42x_HCLK_SYSCLK_DIV8,       /**< \brief 系统时钟/8  作为HCLK源 */
    AMHW_ZSL42x_HCLK_SYSCLK_DIV16,      /**< \brief 系统时钟/16 作为HCLK源 */
    AMHW_ZSL42x_HCLK_SYSCLK_DIV32,      /**< \brief 系统时钟/32 作为HCLK源 */
    AMHW_ZSL42x_HCLK_SYSCLK_DIV64,      /**< \brief 系统时钟/64 作为HCLK源 */
    AMHW_ZSL42x_HCLK_SYSCLK_DIV128,     /**< \brief 系统时钟/128作为HCLK源 */
}amhw_zsl42x_hclk_src;

/**
 * \brief PCLK时钟源
 */
typedef enum {
    AMHW_ZSL42x_PCLK_HCLK = 0,        /**< \brief HCLK时钟/1  作为PCLK源 */
    AMHW_ZSL42x_PCLK_HCLK_DIV2,       /**< \brief HCLK时钟/2  作为PCLK源 */
    AMHW_ZSL42x_PCLK_HCLK_DIV4,       /**< \brief HCLK时钟/4  作为PCLK源 */
    AMHW_ZSL42x_PCLK_HCLK_DIV8,       /**< \brief HCLK时钟/8  作为PCLK源 */
}amhw_zsl42x_pclk_src;

/**
 * \brief SYSCTRL2 寄存器 SYSCTL0，SYSCTRL1保护系列控制寄存器，
 *
 * \param[in]  data ：写入数据
 *
 * \note 寄存器 SYSCTL0，SYSCTRL1保护系列控制寄存器，
 *       对 SYSCTRL2先写0x5A5A，再写 0xA5A5，启动对于寄存器 SYSCTL0，SYSCTRL1的写操作。
 *       只要对寄存器 SYSCTL0， SYSCTRL1写操作了，这个保护位自动回复保护状态，需要重新写入系列打开保护。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_set_start (uint16_t data)
{
    ZSL42x_RCCCTRL->sysctrl[2] = data;
}

/**
 * \brief  wakeup_byRCH  从deepsleep唤醒后，系统时钟源更换为RCH，原时钟继续使能
 *
 * \return 无
 *
 */
am_static_inline
void amhw_zsl42x_rcc_wakeup_sysclk_rch (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] |= (1ul << 15);
}

/**
 * \brief wakeup_byRCH  从deepsleep唤醒后，不改变系统时钟源
 *
 * \return 无
 *
 */
am_static_inline
void amhw_zsl42x_rcc_wakeup_sysclk_nochange (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] &= ~(1ul << 15);
}

/**
 * \brief PCLK_PRS PCLK时钟源切换
 *
 * \param[in]  src ：PCLK时钟源
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_pclk_set (amhw_zsl42x_pclk_src src)
{

    ZSL42x_RCCCTRL->sysctrl[0] = (ZSL42x_RCCCTRL->sysctrl[0] & ~(0x3ul << 11)) |
                                 ((src & 0x3ul) << 11);
}

/**
 * \brief HCLK_PRS HCLK时钟源切换
 *
 * \param[in]  src ：HCLK时钟源
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_hclk_set (amhw_zsl42x_hclk_src src)
{

    ZSL42x_RCCCTRL->sysctrl[0] = (ZSL42x_RCCCTRL->sysctrl[0] & ~(0x7ul << 8)) |
                                 ((src & 0x7ul) << 8);
}

/**
 * \brief CLKSW 系统时钟源切换
 *
 * \param[in]  src ：系统时钟源
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_sys_clk_set (amhw_zsl42x_sys_clk_src src)
{

    ZSL42x_RCCCTRL->sysctrl[0] &= ~(0x7ul << 5);

    amhw_zsl42x_rcc_set_start(0x5A5A);
    amhw_zsl42x_rcc_set_start(0xA5A5);
    ZSL42x_RCCCTRL->sysctrl[0] |= (src  << 5);
}

/**
 * \brief PLL_EN PLL禁能
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_pll_disable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] &= ~(1ul << 4);
}

/**
 * \brief PLL_EN PLL使能
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_pll_enable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] |= (1ul << 4);
}

/**
 * \brief XTL_EN 外部低速晶振XTL禁能
 *
 * \note 需要将PC14、 PC15设置成模拟端口.
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xtl_disable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] &= ~(1ul << 3);
}

/**
 * \brief XTL_EN 外部低速晶振XTL使能
 *
 * \note 需要将PC14、 PC15设置成模拟端口.
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xtl_enable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] |= (1ul << 3);
}

/**
 * \brief RCL_EN 内部低速时钟RCL禁能
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rcl_disable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] &= ~(1ul << 2);
}

/**
 * \brief RCL_EN 内部低速时钟RCL使能
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rcl_enable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] |= (1ul << 2);
}

/**
 * \brief 获取内部低速时钟RCL是否使能
 *
 * \return 无
 */
am_static_inline
am_bool_t amhw_zsl42x_rcc_rcl_enable_get (void)
{
    return (ZSL42x_RCCCTRL->sysctrl[0] & (1ul << 2)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief XTH_EN 外部高速晶振XTH禁能
 *
 * \note 当系统进入DeepSleep，此高速时钟会自动关闭。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xth_disable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] &= ~(1ul << 1);
}

/**
 * \brief XTH_EN 外部高速晶振XTH使能
 *
 * \note 当系统进入DeepSleep，此高速时钟会自动关闭。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xth_enable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] |= (1ul << 1);
}

/**
 * \brief RCH_EN 内部高速时钟RCH禁能
 *
c
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rch_disable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] &= ~(1ul << 0);
}

/**
 * \brief RCH_EN 内部高速时钟RCH使能
 *
 * \note 当系统进入DeepSleep，此高速时钟会自动关闭。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rch_enable (void)
{
    ZSL42x_RCCCTRL->sysctrl[0] |= (1ul << 0);
}

/**
 * \brief 获取内部高速时钟RCH是否使能
 *
 * \return 无
 */
am_static_inline
am_bool_t amhw_zsl42x_rcc_rch_enable_get (void)
{
    return (ZSL42x_RCCCTRL->sysctrl[0] & (1ul << 0)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief RTC高速时钟补偿时钟频率选择
 */
typedef enum {
    AMHW_ZSL42x_RTCCLK_ADJUST_4M = 0,    /**< \brief 4  MHz补偿时钟频率 */
    AMHW_ZSL42x_RTCCLK_ADJUST_6M,        /**< \brief 6  MHz补偿时钟频率 */
    AMHW_ZSL42x_RTCCLK_ADJUST_8M,        /**< \brief 8  MHz补偿时钟频率 */
    AMHW_ZSL42x_RTCCLK_ADJUST_10M,       /**< \brief 10 MHz补偿时钟频率 */
    AMHW_ZSL42x_RTCCLK_ADJUST_16M,       /**< \brief 16 MHz补偿时钟频率 */
    AMHW_ZSL42x_RTCCLK_ADJUST_20M,       /**< \brief 20 MHz补偿时钟频率 */
    AMHW_ZSL42x_RTCCLK_ADJUST_24M,       /**< \brief 24 MHz补偿时钟频率 */
    AMHW_ZSL42x_RTCCLK_ADJUST_32M,       /**< \brief 32 MHz补偿时钟频率 */
}amhw_zsl42x_rtcclk_adjust;

/**
 * \brief RTC_FREQ_ADJUST RTC高速时钟补偿时钟频率选择
 *
 * \param[in]  choice ：补偿时钟频率
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rtcclk_adjust_set (amhw_zsl42x_rtcclk_adjust choice)
{
    ZSL42x_RCCCTRL->sysctrl[1] = (ZSL42x_RCCCTRL->sysctrl[1] & (~(0x7ul << 9))) |
                                 ((choice & 0x7ul) << 9);
}


/**
 * \brief SWD_USE_IO SWD端口功能配置（SWD）
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_swdio_debug (void)
{
    ZSL42x_RCCCTRL->sysctrl[1] &= ~(1ul << 8);
}

/**
 * \brief SWD_USE_IO SWD端口功能配置（GPIO）
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_swdio_gpio (void)
{
    ZSL42x_RCCCTRL->sysctrl[1] |= (1ul << 8);
}

/**
 * \brief LOCKUP_EN Cortex-M0+ LockUp禁能
 *
 * \note 使能后，CPU读到无效指令时会复位MCU，可增强系统可靠性。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_lockup_disable (void)
{
    ZSL42x_RCCCTRL->sysctrl[1] &= ~(1ul << 6);
}

/**
 * \brief LOCKUP_EN Cortex-M0+ LockUp使能
 *
 * \note 使能后，CPU读到无效指令时会复位MCU，可增强系统可靠性。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_lockup_enable (void)
{
    ZSL42x_RCCCTRL->sysctrl[1] |= (1ul << 6);
}

/**
 * \brief RTC_LPW RTC模块低功耗禁能
 *
 * \note 使能后， RTC模块进低功耗状态，其寄存器不可以读写。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rtc_lowpower_disable (void)
{
    ZSL42x_RCCCTRL->sysctrl[1] &= ~(1ul << 5);
}

/**
 * \brief RTC_LPW RTC模块低功耗使能
 *
 * \note 使能后， RTC模块进低功耗状态，其寄存器不可以读写。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rtc_lowpower_enable (void)
{
    ZSL42x_RCCCTRL->sysctrl[1] |= (1ul << 5);
}

/**
 * \brief XTL_ALWAYS_ON XTL高级使能控制禁能
 *
 * \note 使能后， sysctrl[0].XTL_EN只可置位。
 *       禁能后， sysctrl[0].XTL_EN可置位可清零。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xtl_alwayson_disable (void)
{
    ZSL42x_RCCCTRL->sysctrl[1] &= ~(1ul << 3);
}

/**
 * \brief XTL_ALWAYS_ON XTL高级使能控制使能
 *
 * \note 使能后， sysctrl[0].XTL_EN只可置位。
 *       禁能后， sysctrl[0].XTL_EN可置位可清零。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xtl_alwayson_enable (void)
{
    ZSL42x_RCCCTRL->sysctrl[1] |= (1ul << 3);
}


/**
 * \brief 外部 XTL时钟输入控制
 */
typedef enum {
    AMHW_ZSL42x_XTL_INPUT_XTAL = 0,    /**< \brief XTL输入由晶振产生 */
    AMHW_ZSL42x_XTL_INPUT_PC14,        /**< \brief XTL输入由 PC14输入 */
}amhw_zsl42x_xtl_input;

/**
 * \brief EXTL_EN XTL时钟输入控制
 *
 * \param[in]  choice ：XTL时钟输入选择
 *
 * \note 使用PC14输入时钟时，需设置sysctrl[0].XTL_EN为1。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xtl_input_set (amhw_zsl42x_xtl_input choice)
{
    ZSL42x_RCCCTRL->sysctrl[1] = (ZSL42x_RCCCTRL->sysctrl[1] & (~(0x1ul << 2))) |
                                 ((choice & 0x1ul) << 2);
}

/**
 * \brief 外部 XTH时钟输入控制
 */
typedef enum {
    AMHW_ZSL42x_XTH_INPUT_XTAL = 0,    /**< \brief XTH输入由晶振产生 */
    AMHW_ZSL42x_XTH_INPUT_PF00,        /**< \brief XTH输入由PF00输入 */
}amhw_zsl42x_xth_input;

/**
 * \brief EXTH_EN XTH时钟输入控制
 *
 * \param[in]  choice ：XTH时钟输入选择
 *
 * \note 使用PC14输入时钟时，需设置sysctrl[0].XTH_EN为1。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xth_input_set (amhw_zsl42x_xth_input choice)
{
    ZSL42x_RCCCTRL->sysctrl[1] = (ZSL42x_RCCCTRL->sysctrl[1] & (~(0x1ul << 1))) |
                                 ((choice & 0x1ul) << 1);
}


/**
 * \brief stable RCH时钟稳定标志位
 *
 * \retval TRUE  : 已经稳定，可以被内部电路使用。
 * \retval FALSE : 未稳定，不可以被内部电路使用。
 */
am_static_inline
am_bool_t amhw_zsl42x_rcc_rch_state_get (void)
{
    return (am_bool_t)((ZSL42x_RCCCTRL->rch_cr & (0x1ul << 11)) ?
                       AM_TRUE :
                       AM_FALSE);
}

typedef enum {
    AMHW_ZSL42x_RCH_FRE_24MHz = 0x00100C00,
    AMHW_ZSL42x_RCH_FRE_22MHz = 0x00100C02,   // 22.12MHz
    AMHW_ZSL42x_RCH_FRE_16MHz = 0x00100C04,
    AMHW_ZSL42x_RCH_FRE_8MHz = 0x00100C06,
    AMHW_ZSL42x_RCH_FRE_4MHz = 0x00100C08,
}amhw_zsl42x_rch_fre_t;
/**
 * \brief TRIM 时钟频率调整
 *
 * \param[in]  data ：写入数据
 *
 * \note 时钟频率调整， Flash中保存了5组频率的校准值。
 *       将Flash内的校准值读出并写入RCH_CR.TRIM即可获得精准的频率。
 *
 *       24M校准值地址         ： 0x00100C00 - 0x00100C01
 *       22.12M校准值地址： 0x00100C02 - 0x00100C03
 *       16M校准值地址         ： 0x00100C04 - 0x00100C05
 *       8M校准值地址            ： 0x00100C06 - 0x00100C07
 *       4M校准值地址            ： 0x00100C08 - 0x00100C09
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rch_trim_set (uint16_t data)
{
    ZSL42x_RCCCTRL->rch_cr = (ZSL42x_RCCCTRL->rch_cr & (~0x07FFul)) |
                         (data & 0x07FFul);
}

/**
 * \brief TRIM 时钟频率获取
 *
 * \return 寄存器内部的时钟频率校准值
 */
am_static_inline
uint16_t amhw_zsl42x_rcc_rch_trim_get (void)
{
    return (ZSL42x_RCCCTRL->rch_cr & 0x07FFul);
}

/**
 * \brief stable XTH时钟稳定标志位
 *
 * \retval TRUE  : 已经稳定，可以被内部电路使用。
 * \retval FALSE : 未稳定，不可以被内部电路使用。
 */
am_static_inline
am_bool_t amhw_zsl42x_rcc_xth_state_get (void)
{
    return (am_bool_t)((ZSL42x_RCCCTRL->xth_cr & (0x1ul << 6)) ?
                       AM_TRUE :
                       AM_FALSE);
}


/**
 * \brief 外部高速时钟XTH稳定时间选择
 */
typedef enum {
    AMHW_ZSL42x_XTH_WAITTIME_256 = 0,       /**< \brief 稳定时间为256个周期 */
    AMHW_ZSL42x_XTH_WAITTIME_1024,          /**< \brief 稳定时间为1024个周期 */
    AMHW_ZSL42x_XTH_WAITTIME_4096,          /**< \brief 稳定时间为4096个周期 */
    AMHW_ZSL42x_XTH_WAITTIME_16384,         /**< \brief 稳定时间为16384个周期 */
}amhw_zsl42x_xth_waittime;

/**
 * \brief Startup 外部高速时钟XTH稳定时间选择
 *
 * \param[in]  data ：写入数据
 *
 * \note 强烈建议将 XTH 的稳定时间设置为 11（AMHW_ZSL42x_XTH_WAITTIME_16384）。
 *       如果XTH稳定时间不足，在进行时钟切换时或从深度休眠唤醒时，系统不能稳定工作。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xth_waittime_set (amhw_zsl42x_xth_waittime time_set)
{
    ZSL42x_RCCCTRL->xth_cr = (ZSL42x_RCCCTRL->xth_cr & (~(0x3ul << 4))) |
                         ((time_set & 0x3ul) << 4);
}

/**
 * \brief 外部晶振工作频率选择
 */
typedef enum {
    AMHW_ZSL42x_XTH_XTAL_FRE_4_8 = 0,       /**< \brief 4M~6M */
    AMHW_ZSL42x_XTH_XTAL_FRE_8_16,          /**< \brief 6M~12M */
    AMHW_ZSL42x_XTH_XTAL_FRE_16_24,         /**< \brief 12M~20M */
    AMHW_ZSL42x_XTH_XTAL_FRE_24_32,         /**< \brief 20M~32M */
}amhw_zsl42x_xth_xtal_fre;

/**
 * \brief xth_fsel 外部晶振工作频率选择
 *
 * \param[in]  data ：写入数据
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xth_xtal_fre_set (amhw_zsl42x_xth_xtal_fre fre)
{
    ZSL42x_RCCCTRL->xth_cr = (ZSL42x_RCCCTRL->xth_cr & (~(0x3ul << 2))) |
                         ((fre & 0x3ul) << 2);
}

/**
 * \brief 外部晶振驱动能力选择
 */
typedef enum {
    AMHW_ZSL42x_XTH_XTAL_DRIVER_MIN = 0,       /**< \brief 最弱驱动能力 */
    AMHW_ZSL42x_XTH_XTAL_DRIVER_SMALL,         /**< \brief 弱驱动能力 */
    AMHW_ZSL42x_XTH_XTAL_DRIVER_DEFAULT,       /**< \brief 默认驱动能力 (推荐值) */
    AMHW_ZSL42x_XTH_XTAL_DRIVER_MAX,           /**< \brief 最强驱动能力 */
}amhw_zsl42x_xth_xtal_driver;

/**
 * \brief Driver 外部晶振驱动能力选择
 *
 * \param[in]  data ：写入数据
 *
 * \note 需要根据晶振特性、负载电容以及电路板的寄生参数选择适当的驱动能力。
 *       驱动能力越大则功耗越大；驱动能力越弱则功耗越小。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xth_xtal_driver_set (amhw_zsl42x_xth_xtal_driver driver_set)
{
    ZSL42x_RCCCTRL->xth_cr = (ZSL42x_RCCCTRL->xth_cr & (~(0x3ul << 0))) |
                             ((driver_set & 0x3ul) << 0);
}

/**
 * \brief stable RCL时钟稳定标志位
 *
 * \retval TRUE  : 已经稳定，可以被内部电路使用。
 * \retval FALSE : 未稳定，不可以被内部电路使用。
 */
am_static_inline
am_bool_t amhw_zsl42x_rcc_rcl_state_get (void)
{
    return (am_bool_t)((ZSL42x_RCCCTRL->rcl_cr & (0x1ul << 12)) ?
                       AM_TRUE :
                       AM_FALSE);
}

/**
 * \brief 内部低速时钟RCL稳定时间选择
 */
typedef enum {
    AMHW_ZSL42x_RCL_WAITTIME_4 = 0,       /**< \brief 稳定时间为4个周期 */
    AMHW_ZSL42x_RCL_WAITTIME_16,          /**< \brief 稳定时间为16个周期 */
    AMHW_ZSL42x_RCL_WAITTIME_64,          /**< \brief 稳定时间为64个周期 */
    AMHW_ZSL42x_RCL_WAITTIME_256,         /**< \brief 稳定时间为256个周期 */
}amhw_zsl42x_rcl_waittime;

/**
 * \brief Startup 内部低速时钟RCL稳定时间选择
 *
 * \param[in]  data ：写入数据
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rcl_waittime_set (amhw_zsl42x_rcl_waittime time_set)
{
    ZSL42x_RCCCTRL->rcl_cr = (ZSL42x_RCCCTRL->rcl_cr & (~(0x3ul << 10))) |
                             ((time_set & 0x3ul) << 10);
}

typedef enum {
    AMHW_ZSL42x_RCl_FRE_38400Hz = 0x00100C20,
    AMHW_ZSL42x_RCl_FRE_32768Hz = 0x00100C22,
}amhw_zsl42x_rcl_fre_t;

/**
 * \brief TRIM 时钟频率调整
 *
 * \param[in]  data ：写入数据
 *
 * \note 内部低速时钟频率调整， Flash中保存了2组频率的校准值。
 *       将Flash内的校准值读出并写入RCL_CR.TRIM即可获得精准的频率。
 *
 *       38.4K校准值地址      ： 0x00100C20 - 0x00100C21
 *       32.768K校准值地址： 0x00100C22 - 0x00100C23
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_rcl_trim_set (uint16_t data)
{
    ZSL42x_RCCCTRL->rcl_cr = (ZSL42x_RCCCTRL->rcl_cr & (~0x03FFul)) |
                         (data & 0x03FFul);
}

/**
 * \brief TRIM 时钟频率获取
 *
 * \return 寄存器内部的时钟频率校准值
 */
am_static_inline
uint16_t amhw_zsl42x_rcc_rcl_trim_get (void)
{
    return (ZSL42x_RCCCTRL->rcl_cr & 0x07FFul);
}

/**
 * \brief stable XTL时钟稳定标志位
 *
 * \retval TRUE  : 已经稳定，可以被内部电路使用。
 * \retval FALSE : 未稳定，不可以被内部电路使用。
 */
am_static_inline
am_bool_t amhw_zsl42x_rcc_xtl_state_get (void)
{
    return (am_bool_t)((ZSL42x_RCCCTRL->xtl_cr & (0x1ul << 6)) ?
                       AM_TRUE :
                       AM_FALSE);
}

/**
 * \brief 内部低速时钟RCL稳定时间选择
 */
typedef enum {
    AMHW_ZSL42x_XTL_WAITTIME_256 = 0,       /**< \brief 稳定时间为256个周期 */
    AMHW_ZSL42x_XTL_WAITTIME_1024,          /**< \brief 稳定时间为1024个周期 */
    AMHW_ZSL42x_XTL_WAITTIME_4096,          /**< \brief 稳定时间为4096个周期 */
    AMHW_ZSL42x_XTL_WAITTIME_16384,         /**< \brief 稳定时间为16384个周期 */
}amhw_zsl42x_xtl_waittime;

/**
 * \brief Startup 内部低速时钟XTL稳定时间选择
 *
 * \param[in]  data ：写入数据
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xtl_waittime_set (amhw_zsl42x_xtl_waittime time_set)
{
    ZSL42x_RCCCTRL->xtl_cr = (ZSL42x_RCCCTRL->xtl_cr & (~(0x3ul << 4))) |
                             ((time_set & 0x3ul) << 4);
}

/**
 * \brief XTL晶振振荡幅度的调整
 */
typedef enum {
    AMHW_ZSL42x_XTL_XTAL_AMPLITUDE_MIN = 0,       /**< \brief 最小振幅 */
    AMHW_ZSL42x_XTL_XTAL_AMPLITUDE_NORMAL,        /**< \brief 正常振幅 */
    AMHW_ZSL42x_XTL_XTAL_AMPLITUDE_BIG_DEFAULT,   /**< \brief 较大振幅（推荐值） */
    AMHW_ZSL42x_XTL_XTAL_AMPLITUDE_MAX,           /**< \brief 最大振幅 */
}amhw_zsl42x_xtl_xtal_amplitude;

/**
 * \brief Driver XTL晶振振荡幅度的调整
 *
 * \param[in]  data ：写入数据
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xtl_xtal_amplitude_set (amhw_zsl42x_xtl_xtal_amplitude driver_set)
{
    ZSL42x_RCCCTRL->xtl_cr = (ZSL42x_RCCCTRL->xtl_cr & (~(0x3ul << 2))) |
                             ((driver_set & 0x3ul) << 2);
}

/**
 * \brief 外部晶振驱动能力选择
 */
typedef enum {
    AMHW_ZSL42x_XTL_XTAL_DRIVER_MIN = 0,       /**< \brief 最弱驱动能力 */
    AMHW_ZSL42x_XTL_XTAL_DRIVER_DEFAULT,       /**< \brief 一般驱动能力 （推荐值） */
    AMHW_ZSL42x_XTL_XTAL_DRIVER_BIG,           /**< \brief 较强驱动能力 */
    AMHW_ZSL42x_XTL_XTAL_DRIVER_MAX,           /**< \brief 最强驱动能力 */
}amhw_zsl42x_xtl_xtal_driver;

/**
 * \brief Driver 外部低速晶振驱动能力选择
 *
 * \param[in]  data ：写入数据
 *
 * \note 需要根据晶振特性、负载电容以及电路板的寄生参数选择适当的驱动能力。
 *       驱动能力越大则功耗越大；驱动能力越弱则功耗越小。
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_xtl_xtal_driver_set (amhw_zsl42x_xtl_xtal_driver driver_set)
{
    ZSL42x_RCCCTRL->xtl_cr = (ZSL42x_RCCCTRL->xtl_cr & (~(0x3ul << 0))) |
                             ((driver_set & 0x3ul) << 0);
}

/**
 * \brief stable PLL稳定标志
 *
 * \retval TRUE  : 已经稳定，可以被内部电路使用。
 * \retval FALSE : 未稳定，不可以被内部电路使用。
 */
am_static_inline
am_bool_t amhw_zsl42x_rcc_pll_state_get (void)
{
    return (am_bool_t)((ZSL42x_RCCCTRL->pll_cr & (0x1ul << 18)) ?
                       AM_TRUE :
                       AM_FALSE);
}

/**
 * \brief PLL稳定时间选择
 */
typedef enum {
    AMHW_ZSL42x_PLL_WAITTIME_128 = 0,      /**< \brief 稳定时间为128个周期 */
    AMHW_ZSL42x_PLL_WAITTIME_256,          /**< \brief 稳定时间为256个周期 */
    AMHW_ZSL42x_PLL_WAITTIME_512,          /**< \brief 稳定时间为512个周期 */
    AMHW_ZSL42x_PLL_WAITTIME_1024,         /**< \brief 稳定时间为1024个周期 */
    AMHW_ZSL42x_PLL_WAITTIME_2048,         /**< \brief 稳定时间为2048个周期 */
    AMHW_ZSL42x_PLL_WAITTIME_4096,         /**< \brief 稳定时间为4096个周期 */
    AMHW_ZSL42x_PLL_WAITTIME_8192,         /**< \brief 稳定时间为8192个周期 */
    AMHW_ZSL42x_PLL_WAITTIME_16384,        /**< \brief 稳定时间为16384个周期 */
}amhw_zsl42x_pll_waittime;

/**
 * \brief Startup PLL稳定时间选择
 *
 * \param[in]  data ：写入数据
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_pll_waittime_set (amhw_zsl42x_pll_waittime time_set)
{
    ZSL42x_RCCCTRL->pll_cr = (ZSL42x_RCCCTRL->pll_cr & (~(0x7ul << 15))) |
                         ((time_set & 0x7ul) << 15);
}

/**
 * \brief PLL输入频率选择
 */
typedef enum {
    AMHW_ZSL42x_PLL_INPUT_FRE_4_6 = 0,       /**< \brief 4M~6M */
    AMHW_ZSL42x_PLL_INPUT_FRE_6_12,          /**< \brief 6M~12M */
    AMHW_ZSL42x_PLL_INPUT_FRE_12_20,         /**< \brief 12M~20M */
    AMHW_ZSL42x_PLL_INPUT_FRE_20_24,         /**< \brief 20M~32M */
}amhw_zsl42x_pll_input_fre;

/**
 * \brief FRSEL PLL输入频率选择
 *
 * \param[in]  data ：写入数据
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_pll_input_fre_set (amhw_zsl42x_pll_input_fre fre)
{
    ZSL42x_RCCCTRL->pll_cr = (ZSL42x_RCCCTRL->pll_cr & (~(0x3ul << 13))) |
                             ((fre & 0x3ul) << 13);
}

/**
 * \brief DIVN PLL输出时钟的倍频系数
 *
 * \param[in]  data ：写入数据, 允许范围0X2~0XC
 *
 * \note 输出频率 = DIVN*输入频率，
 *       data 允许范围0X2~0XC
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_pll_divn_set (uint8_t data)
{
    ZSL42x_RCCCTRL->pll_cr = (ZSL42x_RCCCTRL->pll_cr & (~(0xful << 5))) |
                             ((data & 0xful) << 5);
}

/**
 * \brief PLL输出频率范围选择
 */
typedef enum {
    AMHW_ZSL42x_PLL_OUTPUT_FRE_8_12 = 0,      /**< \brief 8M~12M  */
    AMHW_ZSL42x_PLL_OUTPUT_FRE_12_18,         /**< \brief 12M~18M */
    AMHW_ZSL42x_PLL_OUTPUT_FRE_18_24,         /**< \brief 18M~24M */
    AMHW_ZSL42x_PLL_OUTPUT_FRE_24_36,         /**< \brief 24M~36M */
    AMHW_ZSL42x_PLL_OUTPUT_FRE_36_48,         /**< \brief 36M~48M */
}amhw_zsl42x_pll_output_fre;

/**
 * \brief FOSC PLL输出频率范围选择
 *
 * \param[in]  data ：写入数据
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_pll_output_fre_set (amhw_zsl42x_pll_output_fre fre)
{
    ZSL42x_RCCCTRL->pll_cr = (ZSL42x_RCCCTRL->pll_cr & (~(0x7ul << 2))) |
                             ((fre & 0x7ul) << 2);
}

/**
 * \brief PLL输入时钟选择
 */
typedef enum {
    AMHW_ZSL42x_PLL_INPUT_FRE_SRC_XTH_XTAL = 0,   /**< \brief XTH晶振生成的时钟  */
    AMHW_ZSL42x_PLL_INPUT_FRE_SRC_XTH_PF00 = 1,   /**< \brief XTH从管脚PF00输入的时钟 */
    AMHW_ZSL42x_PLL_INPUT_FRE_SRC_RCH      = 3,   /**< \brief RCH时钟 */
}amhw_zsl42x_pll_input_fre_src;

/**
 * \brief FRSEL PLL输入时钟选择
 *
 * \param[in]  data ：写入数据
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_rcc_pll_input_fre_src_set (amhw_zsl42x_pll_input_fre_src fre)
{
    ZSL42x_RCCCTRL->pll_cr = (ZSL42x_RCCCTRL->pll_cr & (~0x3ul)) |
                             (fre & 0x3ul);
}

/**
 * \brief 使能外设
 *
 * \param[in]  peri ：需要使能的外设
 *
 * \return 无
 *
 */
void amhw_zsl42x_rcc_peripheral_enable (amhw_zsl42x_peripheral peri);

/**
 * \brief 检查某外设时钟是否使能
 *
 * \param[in]  peri ：需要使能的外设
 *
 * \return 1: 当前外设时钟已使能，无需再次使能
 *         0: 当前外设时钟未使能
 *
 */
int amhw_zsl42x_rcc_peripheral_enable_check (amhw_zsl42x_peripheral peri);

/**
 * \brief 禁能外设
 *
 * \param[in]  peri ：需要禁能的外设
 *
 * \return 无
 *
 */
void amhw_zsl42x_rcc_peripheral_disable (amhw_zsl42x_peripheral peri);

/**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __ZSL42x_RCC_H */

#endif

/* end of file */
