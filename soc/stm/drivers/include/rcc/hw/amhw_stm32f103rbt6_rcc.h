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
 * - 1.00 19-06-27  yrh, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG237_RCC_H
#define __AMHW_ZLG237_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "zlg237_periph_map.h"

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
 * \addtogroup amhw_zlg237_if_rcc
 * \copydoc amhw_zlg237_rcc.h
 * @{
 */

/**
 * \brief CRC寄存器块结构体
 */
typedef struct amhw_zlg237_rcc {
  __IO uint32_t cr;        /**< \brief 时钟控制寄存器 */
  __IO uint32_t cfgr;      /**< \brief 时钟配置寄存器 */
  __IO uint32_t cir;       /**< \brief 时钟中断寄存器 */
  __IO uint32_t apb2rstr;  /**< \brief APB2外设复位寄存器 */
  __IO uint32_t apb1rstr;  /**< \brief APB1外设复位寄存器 */
  __IO uint32_t ahbenr;    /**< \brief AHB外设时钟使能寄存器 */
  __IO uint32_t apb2enr;   /**< \brief APB2外设时钟使能寄存器 */
  __IO uint32_t apb1enr;   /**< \brief APB1外设时钟使能寄存器 */
  __IO uint32_t bdcr;      /**< \brief 备份控制寄存器 */
  __IO uint32_t csr;       /**< \brief 控制状态寄存器 */
  __IO uint32_t wts;       /**< \brief hse时钟源控制寄存器 */
  __I  uint32_t reserve;   /**< \brief 保留 */
  __IO uint32_t lpclk_ctl; /**< \brief 低功耗时钟配置寄存器 */

} amhw_zlg237_rcc_t;

/**
 * \brief 系统时钟源
 */
typedef enum {
    AMHW_ZLG237_SYSCLK_HSI,       /**< \brief HSI 作为系统时钟 */
    AMHW_ZLG237_SYSCLK_HSE,       /**< \brief HSE 作为系统时钟 */
    AMHW_ZLG237_SYSCLK_PLL,       /**< \brief PLL 输出作为系统时钟 */
    AMHW_ZLG237_SYSCLK_LSI_OR_LSE /**< \brief LSI 或  LSE 输出作为系统时钟 */
}amhw_zlg237_sys_clk_src;

/**
 * \brief 低速系统时钟
 */
typedef enum {
    AMHW_ZLG237_LCLK_LSI,  /**< \brief 低速内部时钟 */
    AMHW_ZLG237_LCLK_LSE,  /**< \brief 低速外部时钟 */
}amhw_zlg237_lclk;


/**
 * \brief RTC时钟源
 */
typedef enum {
    AMHW_ZLG237_RTCCLK_LSE = 1,       /**< \brief PLL 输出作为系统时钟 */
    AMHW_ZLG237_RTCCLK_LSI = 2,       /**< \brief HSI 振荡器  6 分频作为系统时钟 */
    AMHW_ZLG237_RTCCLK_HSE_DIV128 =3, /**< \brief HSE 作为系统时钟 */
}amhw_zlg237_rtc_clk_src;

/**
 * \brief PLL时钟源
 */
typedef enum {
    AMHW_ZLG237_PLLCLK_HSI = 0,  /**< \brief HSI 作为PLL输入时钟 */
    AMHW_ZLG237_PLLCLK_HSE = 1,  /**< \brief HSE 作为PLL输入时钟 */
}amhw_zlg237_pll_clk_src;

/**
 * \brief usb 分频系数
 */
typedef enum {
    AMHW_ZLG237_PLL_USB_DIV1P5 = 0, /**< \brief PLL 1.5 分频作为usb输入时钟 */
    AMHW_ZLG237_PLL_USB_DIV1   = 1, /**< \brief PLL 1 分频作为usb输入时钟 */
}amhw_zlg237_pll_usb_clk_div;

/**
 * \brief 时钟中断标志
 */
typedef enum {
    AMHW_ZLG237_CLKINT_INDEX_LSI = 0,  /**< \brief LSI标志 */
    AMHW_ZLG237_CLKINT_INDEX_LSE = 1,  /**< \brief LSE标志 */
    AMHW_ZLG237_CLKINT_INDEX_HSI,      /**< \brief HSI标志 */
    AMHW_ZLG237_CLKINT_INDEX_HSE,      /**< \brief HSE标志 */
    AMHW_ZLG237_CLKINT_INDEX_PLL,      /**< \brief PLL标志 */
    AMHW_ZLG237_CLKINT_INDEX_CSSF = 7, /**< \brief CSSF(时钟安全系统中断)标志 */
}amhw_zlg237_clk_int_index;

/**
 * \brief APB2外设枚举
 */
typedef enum {
    AMHW_ZLG237_RCC_APB2_AFIO   = 0,  /**< \brief AFIO */
    AMHW_ZLG237_RCC_APB2_IOPA   = 2,  /**< \brief IOPA */
    AMHW_ZLG237_RCC_APB2_IOPB   = 3,  /**< \brief IOPB */
    AMHW_ZLG237_RCC_APB2_IOPC   = 4,  /**< \brief IOPC */
    AMHW_ZLG237_RCC_APB2_IOPD   = 5,  /**< \brief IOPD */
    AMHW_ZLG237_RCC_APB2_ADC1   = 9,  /**< \brief ADC1 */
    AMHW_ZLG237_RCC_APB2_ADC2   = 10, /**< \brief ADC2 */
    AMHW_ZLG237_RCC_APB2_TIM1   = 11, /**< \brief TIM1 */
    AMHW_ZLG237_RCC_APB2_SPI1   = 12, /**< \brief SPI1 */
    AMHW_ZLG237_RCC_APB2_UART1  = 14, /**< \brief UART1 */
}amhw_zlg237_apb2_peripheral;

/**
 * \brief APB1外设枚举
 */
typedef enum {
    AMHW_ZLG237_RCC_APB1_TIM2   = 0,    /**< \brief TIM2定时器 */
    AMHW_ZLG237_RCC_APB1_TIM3   = 1,    /**< \brief TIM3定时器 */
    AMHW_ZLG237_RCC_APB1_TIM4   = 2,    /**< \brief TIM4定时器 */
    AMHW_ZLG237_RCC_APB1_WWDG   = 11,   /**< \brief WWDG窗口看门狗 */
    AMHW_ZLG237_RCC_APB1_SPI2   = 14,   /**< \brief SPI2 */
    AMHW_ZLG237_RCC_APB1_USART2  = 17,  /**< \brief USART2 */
    AMHW_ZLG237_RCC_APB1_USART3  = 18,  /**< \brief USART3 */
    AMHW_ZLG237_RCC_APB1_I2C1   = 21,   /**< \brief I2C1 */
    AMHW_ZLG237_RCC_APB1_I2C2   = 22,   /**< \brief I2C2 */
    AMHW_ZLG237_RCC_APB1_USB    = 23,   /**< \brief USB */
    AMHW_ZLG237_RCC_APB1_CAN    = 25,   /**< \brief CAN */
    AMHW_ZLG237_RCC_APB1_BKP    = 27,   /**< \brief BKP */
    AMHW_ZLG237_RCC_APB1_PWR    = 28,   /**< \brief 电源接口 */
}amhw_zlg237_apb1_peripheral;

/**
 * \brief AHB外设枚举
 */
typedef enum {
    AMHW_ZLG237_RCC_AHB_DMA   = 0,   /**< \brief DMA */
    AMHW_ZLG237_RCC_AHB_SRAM  = 2,   /**< \brief SRAM */
    AMHW_ZLG237_RCC_AHB_FLITF = 4,   /**< \brief FLITF */
    AMHW_ZLG237_RCC_AHB_CRC   = 6,   /**< \brief CRC */
}amhw_zlg237_ahb_peripheral;

/**
 * \brief 复位标志枚举
 */
typedef enum {
    AMHW_ZLG237_RCC_CSR_PINRSTF   = (1ul << 26),      /**< \brief NRST管脚复位标志 */
    AMHW_ZLG237_RCC_CSR_PORRSTF   = (1ul << 27),      /**< \brief 上电/掉电复位标志 */
    AMHW_ZLG237_RCC_CSR_SFTRSTF   = (1ul << 28),      /**< \brief 软件复位标志 */
    AMHW_ZLG237_RCC_CSR_IWDGRSTF  = (1ul << 29),      /**< \brief 独立看门狗复位标志 */
    AMHW_ZLG237_RCC_CSR_WWDGRSTF  = (1ul << 30),      /**< \brief 窗口看门狗复位标志 */
    AMHW_ZLG237_RCC_CSR_LPWRRSTF  = (int)(1ul << 31), /**< \brief 低功耗管理复位标志 */
}amhw_zlg237_reset_flag_t;


/**
 * \brief 内部高速时钟使能
 *
 * \return 无
 *
 */
am_static_inline
void amhw_zlg237_rcc_hsion_enable (void)
{
    ZLG237_RCC->cr |= 1ul;
}

/**
 * \brief 内部高速时钟就绪标志读取
 *
 * \retval TRUE  : 内部 8MHz时钟没有就绪
 * \retval FALSE : 内部 8MHz时钟就绪
 */
am_static_inline
am_bool_t amhw_zlg237_rcc_hsirdy_read (void)
{
    return (am_bool_t)((ZLG237_RCC->cr & 0x2ul) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 外部高速时钟(HSE 振荡器)使能
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_hseon_enable (void)
{
    ZLG237_RCC->cr |= (1ul << 16);
}

/**
 * \brief 外部高速时钟就绪标志读取
 *
 * \retval TRUE  : 外部时钟没有就绪
 * \retval FALSE : 外部时钟就绪
 */
am_static_inline
am_bool_t amhw_zlg237_rcc_hserdy_read (void)
{
    return (am_bool_t)((ZLG237_RCC->cr & (1ul << 17)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 外部高速时钟旁路使能
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_hsebyp_enable (void)
{
    ZLG237_RCC->cr |= (1ul << 18);
}

/**
 * \brief 时钟安全系统使能
 *
 * \details 如果外部 8-24MHz振荡器就绪，时钟监测器开启。
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_ccson_enable (void)
{
    ZLG237_RCC->cr |= (1ul << 19);
}


/**
 * \brief PLL 使能
 *
 * \details 当进入待机和停止模式时，该位由硬件清零。
 *          当 PLL 时钟被用作或被选择将要作为系统时钟时,
 *          该位不能被清零。
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_pll_enable (void)
{
    ZLG237_RCC->cr |= (1ul << 24);
}

/**
 * \brief PLL 禁能
 *
 * \details 当进入待机和停止模式时，该位由硬件清零。
 *          当 PLL 时钟被用作或被选择将要作为系统时钟时,
 *          该位不能被清零。
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_pll_disable (void)
{
    ZLG237_RCC->cr &= ~(1ul << 24);
}

/**
 * \brief PLL 时钟就绪标志读取
 *
 * \retval TRUE  : PLL 未锁定
 * \retval FALSE : PLL 锁定
 */
am_static_inline
am_bool_t amhw_zlg237_rcc_pllrdy_read (void)
{
    return (am_bool_t)((ZLG237_RCC->cr & (1ul << 25)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 系统时钟切换
 *
 * \param[in]  src ：系统时钟源
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_sys_clk_set (amhw_zlg237_sys_clk_src src)
{

    ZLG237_RCC->cfgr = ((src & 0x3ul) << 0) |
                              (ZLG237_RCC->cfgr & ~(0x3ul << 0));
}

/**
 * \brief 当前系统时钟获取
 *
 * \return 当前系统时钟源
 */
am_static_inline
amhw_zlg237_sys_clk_src amhw_zlg237_rcc_sys_clk_get (void)
{

    return (amhw_zlg237_sys_clk_src)((ZLG237_RCC->cfgr >> 2ul) & 0x3ul);
}

/**
 * \brief 设置AHB 预分频
 *
 * \param[in]  div ：预分频值
 *
 *       - 0-7 ： 不分频
 *       - 8   ： 2分频
 *       - 9   ： 4分频
 *       - 10  ： 8分频
 *       - 11  ： 16分频
 *       - 12  ： 64分频
 *       - 13  ： 128分频
 *       - 14  ： 256分频
 *       - 15  ： 512分频
 *
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_ahb_div_set (uint8_t div)
{

    ZLG237_RCC->cfgr = ((div & 0xful) << 4) |
                              (ZLG237_RCC->cfgr & ~(0xful << 4));
}

/**
 * \brief 设置 低速 APB 预分频（APB1）
 *
 * \param[in]  div ：预分频值
 *
 *       - 0-3 ： 不分频
 *       - 4   ： 2分频
 *       - 5   ： 4分频
 *       - 6   ： 8分频
 *       - 7   ： 16分频
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb1_div_set (uint8_t div)
{

    ZLG237_RCC->cfgr = ((div & 0x7ul) << 8) |
                              (ZLG237_RCC->cfgr & ~(0x7ul << 8));
}

/**
 * \brief 获取 低速 APB 预分频（APB1）
 *
 * \return 预分频值
 *       - 0-3 ： 不分频
 *       - 4   ： 2分频
 *       - 5   ： 4分频
 *       - 6   ： 8分频
 *       - 7   ： 16分频
 */
am_static_inline
uint8_t amhw_zlg237_rcc_apb1_div_get (void)
{

    return (uint8_t)((ZLG237_RCC->cfgr >> 8) & 0x7ul);
}

/**
 * \brief 设置 高速 APB 预分频（APB2）
 *
 * \param[in]  div ：预分频值
 *
 *       - 0-3 ： 不分频
 *       - 4   ： 2分频
 *       - 5   ： 4分频
 *       - 6   ： 8分频
 *       - 7   ： 16分频
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb2_div_set (uint8_t div)
{

    ZLG237_RCC->cfgr = ((div & 0x7ul) << 11) |
                              (ZLG237_RCC->cfgr & ~(0x7ul << 11));
}

/**
 * \brief 获取 高速 APB 预分频（APB2）
 *
 * \return 预分频值
 *       - 0-3 ： 不分频
 *       - 4   ： 2分频
 *       - 5   ： 4分频
 *       - 6   ： 8分频
 *       - 7   ： 16分频
 */
am_static_inline
uint8_t amhw_zlg237_rcc_apb2_div_get (void)
{

    return (uint8_t)((ZLG237_RCC->cfgr >> 11) & 0x7ul);
}


/**
 * \brief 设置 ADC 预分频
 *
 * \param[in]  div ：预分频值
 *
 *       - 0   ： 2 分频
 *       - 1   ： 4 分频
 *       - 2   ： 8 分频
 *       - 3   ： 16分频
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_adc_div_set (uint8_t div)
{

    ZLG237_RCC->cfgr = ((div & 0x3ul) << 14) |
                              (ZLG237_RCC->cfgr & ~(0x3ul << 14));
}

/**
 * \brief 获取 ADC 预分频
 *
 * \return 预分频值
 *       - 0   ： 2 分频
 *       - 1   ： 4 分频
 *       - 2   ： 8 分频
 *       - 3   ： 16分频
 */
am_static_inline
uint8_t amhw_zlg237_rcc_adc_div_get (void)
{

    return (uint8_t)((ZLG237_RCC->cfgr >> 14) & 0x3ul);
}


/**
 * \brief PLL输入时钟选择
 *
 * \param[in]  src ：PLL输入时钟源
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_pll_clk_set (amhw_zlg237_pll_clk_src src)
{

    ZLG237_RCC->cfgr = ((src & 0x3ul) << 16) |
                              (ZLG237_RCC->cfgr & ~(0x3ul << 16));
}

/**
 * \brief 当前PLL输入时钟获取
 *
 * \return 当前PLL输入时钟源
 */
am_static_inline
amhw_zlg237_pll_clk_src amhw_zlg237_rcc_pll_clk_get (void)
{

    return (amhw_zlg237_pll_clk_src)((ZLG237_RCC->cfgr >> 16ul) & 0x3ul);
}

/**
 * \brief 选择 HSE 分频器作为 PLL 输入
 *
 * \param[in]  div ：分频值
 *
 *      - 0  ： 不分频
 *      - 1  ： 2分频
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_hseforpll_set (uint8_t div)
{

    ZLG237_RCC->cfgr = ((div & 0x1ul) << 17) |
                              (ZLG237_RCC->cfgr & ~(0x1ul << 17));
}


///**
// * \brief PLL分频系数设置
// *
// * \param[in]  div ：分频值
// *
// * \return 无
// */
//am_static_inline
//void amhw_zlg237_rcc_plldm_set (uint8_t div)
//{
//    ZLG237_RCC->cr = ((div & 0x7ul) << 20) |
//                            (ZLG237_RCC->cr & ~(0x7ul << 20));
//}

/**
 * \brief PLL倍频系数设置
 *
 * \param[in]  mul ：倍频值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_plldn_set (uint8_t mul)
{

    ZLG237_RCC->cfgr = ((mul & 0xful) << 18) |
                            (ZLG237_RCC->cfgr & ~(0xful << 18));
}


/**
 * \brief 选择 USB 预分频
 *
 * \param[in]  div ：分频值
 *
 *      - AMHW_ZLG237_PLL_USB_DIV1P5 ：PLL 时钟1.5分频作为 USB 时钟
 *      - AMHW_ZLG237_PLL_USB_DIV1   ： PLL 时钟直接作为 USB时钟
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_pllforusb_set (amhw_zlg237_pll_usb_clk_div index)
{

    ZLG237_RCC->cfgr = ((index & 0x1ul) << 22) |
                              (ZLG237_RCC->cfgr & ~(0x1ul << 22));
}

/**
 * \brief USB 预分频获取
 *
 * \retval  0  ： PLL 时钟直接作为 USB 时钟
 * \retval  1  ： PLL 时钟2 分频作为 USB时钟
 * \retval  2  ： PLL 时钟3 分频作为 USB时钟
 * \retval  3  ： PLL 时钟4 分频作为 USB时钟
 */
am_static_inline
uint8_t amhw_zlg237_rcc_pllforusb_get (void)
{

    return (uint8_t)((ZLG237_RCC->cfgr >> 22ul) & 0x1ul);
}

/**
 * \brief 微控制器时钟输出选择
 *
 * \details 1.该时钟输出在启动和切换 MCO 时钟源时可能会被截断,
 *          2.在系统时钟作为输出至 MCO 管脚时，请保证输出时钟频
 *            率不超过 50MHz（IO口最高频率）
 *
 * \param[in]  src ：时钟源
 *
 *      - 0-3 ： 没有时钟输出
 *      - 4   ： 系统时钟（SYSCLK）输出
 *      - 5   ： HSI 时钟输出
 *      - 6   ： HSE 时钟输出
 *      - 7   ： PLL 时钟2 分频后输出
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_mco_src_set (uint8_t src)
{

    ZLG237_RCC->cfgr = ((src & 0x7ul) << 24ul) |
                              (ZLG237_RCC->cfgr & ~(0x7ul << 24ul));
}

/**
 * \brief 选择 HSI 分频器作为 PLL 输入参考时钟
 *
 * \param[in]  div ：分频值
 *
 *      - 0  ： 2分频
 *      - 1  ： 不分频
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_pllcrpre_set (uint8_t div)
{

    ZLG237_RCC->cfgr = ((div & 0x1ul) << 31ul) |
                              (ZLG237_RCC->cfgr & ~(0x1ul << 31ul));
}


/**
 * \brief 就绪中断使能
 *
 * \param[in]  index ：就绪标志
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_clkint_enable (amhw_zlg237_clk_int_index index)
{

    ZLG237_RCC->cir |= ((index & 0x1ful) << 8);
}

/**
 * \brief 就绪中断禁能
 *
 * \param[in]  index ：就绪标志
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_clkint_disable (amhw_zlg237_clk_int_index index)
{

    ZLG237_RCC->cir &= ~((index & 0x1ful) << 8);
}

/**
 * \brief 读取中断标志
 *
 * \return 中断标志
 */
am_static_inline
amhw_zlg237_clk_int_index amhw_zlg237_rcc_clkint_flag_read (void)
{

    return (amhw_zlg237_clk_int_index)(ZLG237_RCC->cir & 0x9ful);
}

/**
 * \brief 清除中断标志
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_clkint_flag_clear (amhw_zlg237_clk_int_index index)
{

    ZLG237_RCC->cir |= ((index & 0x9ful) << 16);
}

/**
 * \brief 复位APB2外设
 *
 * \param[in]  reset ：需要复位的外设
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb2_reset (amhw_zlg237_apb2_peripheral reset)
{

    ZLG237_RCC->apb2rstr |= (1ul << reset);
}

/**
 * \brief 停止复位APB2外设
 *
 * \param[in]  reset ：需要复位的外设
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb2_reset_stop (amhw_zlg237_apb2_peripheral reset)
{

    ZLG237_RCC->apb2rstr &= ~(1ul << reset);
}

/**
 * \brief 复位APB1外设
 *
 * \param[in]  reset ：需要复位的外设
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb1_reset (amhw_zlg237_apb1_peripheral reset)
{

    ZLG237_RCC->apb1rstr |= (1ul << reset);
}

/**
 * \brief 停止复位APB1外设
 *
 * \param[in]  reset ：需要复位的外设
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb1_reset_stop (amhw_zlg237_apb1_peripheral reset)
{

    ZLG237_RCC->apb1rstr &= ~(1ul << reset);
}

/**
 * \brief 使能APB1外设
 *
 * \param[in]  peri ：需要使能的外设
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb1_enable (amhw_zlg237_apb1_peripheral peri)
{

    ZLG237_RCC->apb1enr |= (1ul << peri);
}

/**
 * \brief 禁能APB1外设
 *
 * \param[in]  peri ：需要禁能的外设
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb1_disable (amhw_zlg237_apb1_peripheral peri)
{

    ZLG237_RCC->apb1enr &= ~(1ul << peri);
}

/**
 * \brief 使能APB2外设
 *
 * \param[in]  peri ：需要使能的外设
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb2_enable (amhw_zlg237_apb2_peripheral peri)
{

    ZLG237_RCC->apb2enr |= (1ul << peri);
}

/**
 * \brief 禁能APB2外设
 *
 * \param[in]  peri ：需要禁能的外设
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_apb2_disable (amhw_zlg237_apb2_peripheral peri)
{

    ZLG237_RCC->apb2enr &= ~(1ul << peri);
}

/**
 * \brief 使能AHB外设
 *
 * \param[in]  peri ：需要使能的外设
 *
 * \return 无
 */
void amhw_zlg237_rcc_ahb_enable (amhw_zlg237_ahb_peripheral peri);

/**
 * \brief 禁能AHB外设
 *
 * \param[in]  peri ：需要禁能的外设
 *
 * \return 无
 *
 */
void amhw_zlg237_rcc_ahb_disable (amhw_zlg237_ahb_peripheral peri);

/**
 * \brief 使能LSI
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_lsi_enable (void)
{

    ZLG237_RCC->csr |= (1ul << 0);
}

/**
 * \brief 禁能LSI
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_lsi_disable (void)
{

    ZLG237_RCC->csr &= ~(1ul << 0);
}

/**
 * \brief 内部低速时钟就绪标志读取
 *
 * \retval TRUE  : 内部 40kHz 振荡器时钟就绪
 * \retval FALSE : 内部 40kHz 振荡器时钟未就绪
 */
am_static_inline
am_bool_t amhw_zlg237_rcc_lsirdy_read (void)
{
    return (am_bool_t)((ZLG237_RCC->csr & (1ul << 1)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 获取复位标志
 *
 * \return 复位标志
 */
am_static_inline
amhw_zlg237_reset_flag_t amhw_zlg237_rcc_reset_flag (void)
{

    return (amhw_zlg237_reset_flag_t)(ZLG237_RCC->csr & 0xfc000000);
}

/**
 * \brief 清除复位标志
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_reset_flag_clear (void)
{

    ZLG237_RCC->csr |= (1ul << 24);
}

/**
 * \brief 外部低速振荡器使能
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_lse_enable (void)
{

    ZLG237_RCC->bdcr |= (1ul << 0);
}
/**
 * \brief 外部低速振荡器禁能
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_lse_disable (void)
{
    ZLG237_RCC->bdcr &= ~(1ul << 0);
}

/**
 * \brief 外部低速时钟就绪标志读取
 *
 * \retval TRUE  : 外部 32kHz 振荡器时钟就绪
 * \retval FALSE : 外部 32kHz 振荡器时钟未就绪
 */
am_static_inline
am_bool_t amhw_zlg237_rcc_lserdy_read (void)
{
    return (am_bool_t)((ZLG237_RCC->bdcr & (1ul << 1)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 外部低速时钟旁路状态
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_bdcr_lsebyp_set (uint32_t lsebyp)
{
    ZLG237_RCC->bdcr |= (lsebyp << 2);
}
/**
 * \brief 外部低速时钟旁路状态
 *
 * \retval TRUE  : LSE 时钟被旁路
 * \retval FALSE : LSE 时钟未被旁路
 */
am_static_inline
am_bool_t amhw_zlg237_rcc_bdcr_lsebyp_get (void)
{
    return (am_bool_t)((ZLG237_RCC->bdcr & 0x4ul) ? AM_TRUE : AM_FALSE);
}
/**
 * \brief RTC输入时钟选择
 *
 * \param[in]  src ：RTC输入时钟源
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_bdcr_rtc_clk_set (amhw_zlg237_rtc_clk_src src)
{

    ZLG237_RCC->bdcr = ((src & 0x3ul) << 8) |
                              (ZLG237_RCC->bdcr & ~(0x3ul << 8));
}

/**
 * \brief RTC输入时钟获取
 *
 * \retval  rtc时钟源获取
 */
am_static_inline
amhw_zlg237_rtc_clk_src amhw_zlg237_rcc_bdcr_rtc_clk_get (void)
{
    return (amhw_zlg237_rtc_clk_src)((ZLG237_RCC->bdcr >> 8) & 0x3ul);
}

/**
 * \brief RTC时钟使能
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_bdcr_rtc_enable (void)
{

    ZLG237_RCC->bdcr |= (1ul << 15);
}
/**
 * \brief RTC时钟禁能
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_bdcr_rtc_disable (void)
{

    ZLG237_RCC->bdcr &= ~(1ul << 15);
}

/**
 * \brief 备份域软件复位
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_bdcr_bdrst_reset (void)
{

    ZLG237_RCC->bdcr |= (1ul << 16);
}
/**
 * \brief 备份域软件复位结束
 * \return 无
 */
am_static_inline
void amhw_zlg237_rcc_bdcr_bdrst_reset_end (void)
{

    ZLG237_RCC->bdcr &= ~(1ul << 16);
}

/**
 * \brief HSE 晶振启动等待时间设置
 *
 * \param[in]  value ：输入值
 *
 * 等待时间 = value * 8 * hse时钟周期
 */
am_static_inline
void amhw_zlg237_rcc_hse_wt_set(uint16_t value)
{
    ZLG237_RCC->wts = ((value & 0x1ffful) << 0) |
                                 (ZLG237_RCC->wts & ~(0x1ffful << 0));
}

/**
 * \brief XCH_IOp[2:0]  HSE晶振驱动能力配置
 */
typedef enum amhw_zlg237_rcc_hse_iop_set {
    AMHW_ZLG237_RCC_HSE_IOP_20 = 0,/**< \brief 驱动电流大约为20uA，推荐使用0.5~3MHz晶振 */
    AMHW_ZLG237_RCC_HSE_IOP_30,    /**< \brief 驱动电流大约为30uA，推荐使用0.5~4MHz晶振 */
    AMHW_ZLG237_RCC_HSE_IOP_40,    /**< \brief 驱动电流大约为40uA，推荐使用0.5~5MHz晶振 */
    AMHW_ZLG237_RCC_HSE_IOP_50,    /**< \brief 驱动电流大约为50uA，推荐使用1~6MHz晶振 */
    AMHW_ZLG237_RCC_HSE_IOP_60,    /**< \brief 驱动电流大约为60uA，推荐使用2~12MHz晶振 */
    AMHW_ZLG237_RCC_HSE_IOP_80,    /**< \brief 驱动电流大约为80uA，推荐使用2~15MHz晶振 */
    AMHW_ZLG237_RCC_HSE_IOP_100,   /**< \brief 驱动电流大约为100uA，推荐使用3~20MHz晶振 */
    AMHW_ZLG237_RCC_HSE_IOP_200,   /**< \brief 驱动电流大约为200uA，推荐使用5~30MHz晶振 */
} amhw_zlg237_rcc_hse_iop_set_t;
/**
 * \brief XCH_IOp[2:0]  HSE晶振驱动能力配置
 *
 * \param[in] status_flag：HSE晶振驱动能力配置，
 *                        值为 amhw_zlg237_rcc_hse_iop_set_t 这一枚举类型
 *
 */
am_static_inline
void amhw_zlg237_rcc_hse_iop_set (amhw_zlg237_rcc_hse_iop_set_t hse_iop_set)
{
    ZLG237_RCC->wts = (ZLG237_RCC->wts & ~(0x7ul << 16)) |
                      ((hse_iop_set & 0x7ul) << 16) ;

}

/**
 * \brief HSE 片内模拟滤噪电器bypass控制
 *
 *
 *  0 ：HSE 时钟信号经过片内模拟滤噪器后被使用。
 *  1 ：bypass 片内HSE 模拟滤噪器。
 */
am_static_inline
void amhw_zlg237_rcc_hsenf_byp_cfg(uint8_t index)
{
    ZLG237_RCC->wts = ((index & 0x1ul) << 24) |
                                       (ZLG237_RCC->wts & ~(0x1ul << 24));
}

/**
 * \brief 低速系统时钟选择
 *
 * \param[in]  index : 低速时钟
 *
 *      - AMHW_ZLG237_LCLK_LSI  ： 低速内部LSI时钟
 *      - AMHW_ZLG237_LCLK_LSE  ： 低速外部LSE时钟
 */
am_static_inline
void amhw_zlg237_rcc_lclk_sel(amhw_zlg237_lclk index)
{
    ZLG237_RCC->lpclk_ctl = ((index & 0x1ul) << 0) |
                                       (ZLG237_RCC->lpclk_ctl & ~(0x1ul << 0));
}

/**
 * \brief USB1.1时钟使能
 *
 */
am_static_inline
void amhw_zlg237_rcc_usbclk_enable(void)
{
    ZLG237_RCC->lpclk_ctl |= ( 0x1ul << 8);
}

/**
 * \brief USB1.1时钟禁能
 *
 */
am_static_inline
void amhw_zlg237_rcc_usbclk_disable(void)
{
    ZLG237_RCC->lpclk_ctl &= ~( 0x1ul << 8);
}

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
#endif /* __ZLG237_RCC_H */

#endif

/* end of file */
