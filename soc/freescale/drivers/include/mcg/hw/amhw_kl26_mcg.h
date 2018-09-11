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
 * \brief 系统时钟源选择和控制
 *
 * \internal
 * \par History
 * - 1.00 16-09-13  ari, first implementation.
 * \endinternal
 */

#ifndef __KL26_TPM2_H
#define __KL26_TPM2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "../../../../kl26/kl26_periph_map.h"

/**
 * \addtogroup amhw_kl26_if_mcg
 * \copydoc amhw_kl26_mcg.h
 * @{
 */

/*******************************************************************************
* defines
*******************************************************************************/

/**
  * \brief 系统控制寄存器块结构体
  */
typedef struct amhw_kl26_mcg {
    __IO uint8_t c1;             /**< \brief  MCG控制寄存器 1 */
    __IO uint8_t c2;             /**< \brief  MCG控制寄存器 2 */
    __IO uint8_t c3;             /**< \brief  MCG控制寄存器 3 */
    __IO uint8_t c4;             /**< \brief  MCG控制寄存器 4 */
    __IO uint8_t c5;             /**< \brief  MCG控制寄存器 5 */
    __IO uint8_t c6;             /**< \brief  MCG控制寄存器 6 */
    __IO uint8_t s;              /**< \brief  MCG状态寄存器   */
    __I  uint8_t reserved0[1];   /**< \brief  保留            */
    __IO uint8_t sc;             /**< \brief  MCG状态/控制寄存器  */
    __I  uint8_t reserved1[1];   /**< \brief  保留            */
    __IO uint8_t atcvh;          /**< \brief  MCG比较值高位   */
    __IO uint8_t atcvl;          /**< \brief  MCG比较值低位   */
    __IO uint8_t reserved2[1];   /**< \brief  保留            */
    __IO uint8_t c8;             /**< \brief  MCG控制寄存器 8 */
    __I  uint8_t c9;             /**< \brief  MCG控制寄存器 9 */
    __I  uint8_t c10;            /**< \brief  MCG控制寄存器 10 */
} amhw_kl26_mcg_t;

/**
  * \brief 振荡器寄存器块结构体
  */
typedef struct amhw_kl26_osc {
    __IO uint8_t cr;            /**< \brief  OSC控制寄存器 1 */
} amhw_kl26_osc_t;

/**
 * \brief MCGOUTCLK时钟源
 */
typedef enum amhw_kl26_mcg_outclk_src {
    KL26_TPM2_OUTCLK_SRC_PLLS = 0,    /**< \brief FLL或PLL,由PLLS决定 */
    KL26_TPM2_OUTCLK_SRC_IRC  = 1,    /**< \brief IRC内部参考时钟  */
    KL26_TPM2_OUTCLK_SRC_ERC  = 2     /**< \brief ERC外部参考时钟  */
} amhw_kl26_mcg_outclk_src_t;

/**
 * \brief MCGOUTCLK时钟状态
 */
typedef enum amhw_kl26_mcg_outclk_stat {
    KL26_TPM2_OUTCLK_SATA_FLL = 0,    /**< \brief FLL时钟 */
    KL26_TPM2_OUTCLK_SATA_IRC = 1,    /**< \brief IRC内部参考时钟  */
    KL26_TPM2_OUTCLK_SATA_ERC = 2,    /**< \brief ERC外部参考时钟  */
    KL26_TPM2_OUTCLK_SATA_PLL = 3     /**< \brief PLL时钟  */
} amhw_kl26_mcg_outclk_stat_t;

/**
 * \brief PLLS时钟源
 */
typedef enum amhw_kl26_mcg_plls_src {
    KL26_TPM2_PLLS_SRC_FLL = 0,    /**< \brief FLL */
    KL26_TPM2_PLLS_SRC_PLL = 1     /**< \brief PLL */
} amhw_kl26_mcg_plls_src_t;

/**
 * \brief FLL时钟源
 */
typedef enum amhw_kl26_mcg_fll_src {
    KL26_TPM2_FLL_SRC_ERC = 0,      /**< \brief ERC外部参考时钟   */
    KL26_TPM2_FLL_SRC_IRC_SLOW = 1  /**< \brief 慢速（32KHZ）内部参考时钟  */
} amhw_kl26_mcg_fll_src_t;

/**
 * \brief IRC内部参考时钟源
 */
typedef enum amhw_kl26_mcg_irc_src {
    KL26_TPM2_IRC_SRC_SLOW = 0,   /**< \brief 慢速（32KHZ）内部参考时钟  */
    KL26_TPM2_IRC_SRC_FAST = 1    /**< \brief 快速（4MHZ）内部参考时钟  */
} amhw_kl26_mcg_irc_src_t;

/**
 * \brief IRC内部参考时钟状态
 */
typedef enum amhw_kl26_mcg_irc_stat {
    KL26_TPM2_IRC_SATA_SLOW = 0,   /**< \brief 慢速（32KHZ）内部参考时钟  */
    KL26_TPM2_IRC_SATA_FAST = 1    /**< \brief 快速（4MHZ）内部参考时钟  */
} amhw_kl26_mcg_irc_stat_t;

/**
 * \brief OSCER外部参考时钟源
 */
typedef enum amhw_kl26_mcg_osc_src {
    KL26_TPM2_OSC_SRC_CLKIN = 0,   /**< \brief 外部时钟引脚输入  */
    KL26_TPM2_OSC_SRC_OSC   = 1    /**< \brief 晶体振荡器时钟    */
} amhw_kl26_mcg_osc_src_t;

/**
 * \brief OSC振荡器频率范围
 */
typedef enum amhw_kl26_mcg_osc_range {
    KL26_TPM2_OSC_RANGE_LOW       = 0,   /**< \brief 1K~32.768KHZ */
    KL26_TPM2_OSC_RANGE_HIGH      = 1,   /**< \brief 3~8MHZ       */
    KL26_TPM2_OSC_RANGE_VERYHIGH  = 2,   /**< \brief 8~32MHZ      */
    KL26_TPM2_OSC_RANGE_VERYHIGH1 = 3    /**< \brief 8~32MHZ      */
} amhw_kl26_mcg_osc_range_t;

/**
 * \brief OSC振荡器增益选择
 */
typedef enum amhw_kl26_mcg_osc_gain {
    KL26_TPM2_OSC_GAIN_LOW  = 0,   /**< \brief 低功耗模式 */
    KL26_TPM2_OSC_GAIN_HIGH = 1    /**< \brief 高增益     */
} amhw_kl26_mcg_osc_gain_t;

/**
 * \brief OSC振荡器内部负载电容
 */
typedef enum amhw_kl26_mcg_osc_cap {
    KL26_TPM2_OSC_CAP_DISABLED = 0,    /**< \brief diabled   */
    KL26_TPM2_OSC_CAP_2PF      = 8,    /**< \brief 2PF       */
    KL26_TPM2_OSC_CAP_4PF      = 4,    /**< \brief 4PF       */
    KL26_TPM2_OSC_CAP_8PF      = 2,    /**< \brief 8PF       */
    KL26_TPM2_OSC_CAP_16PF     = 1     /**< \brief 16PF      */
} amhw_kl26_mcg_osc_cap_t;

/**
 * \brief OSC振荡器监控模式
 */
typedef enum amhw_kl26_mcg_osc_monitor_mode {
    KL26_TPM2_OSC_MONITOR_INT   = 0,   /**< \brief 时钟丢失时产生中断 */
    KL26_TPM2_OSC_MONITOR_RESET = 1    /**< \brief 时钟丢失时复位     */
} amhw_kl26_mcg_osc_monitor_mode_t;

/**
 * \brief FLL DCO输出倍频系数F
 */
typedef enum amhw_kl26_mcg_dco_div {
    KL26_TPM2_DOC_DIV_640  = 0,  /**< \brief ref:31.25-39.0625KHZ,out:20-25MHZ */
    KL26_TPM2_DOC_DIV_732  = 4,  /**< \brief ref:32.768KHZ,out:24MHZ */
    KL26_TPM2_DOC_DIV_1280 = 1,  /**< \brief ref:31.25-39.0625KHZ,out:40-50MHZ  */
    KL26_TPM2_DOC_DIV_1464 = 5,  /**< \brief ref:32.768KHZ out:48MHZ */
    KL26_TPM2_DOC_DIV_1920 = 2,  /**< \brief ref:31.25-39.0625KHZ,out:60-75MHZ */
    KL26_TPM2_DOC_DIV_2197 = 6,  /**< \brief ref:32.768KHZ,out:72MHZ */
    KL26_TPM2_DOC_DIV_2560 = 3,  /**< \brief ref:31.25-39.0625KHZ,out:80-100MHZ */
    KL26_TPM2_DOC_DIV_2929 = 7   /**< \brief ref:32.768KHZ,out:96MHZ  */
} amhw_kl26_mcg_dco_div_t;

/**
 * \brief ATM修正时钟源
 */
typedef enum amhw_kl26_mcg_atm_src {
    KL26_TPM2_ATM_SRC_32KHZ = 0,   /**< \brief OSCCLK         */
    KL26_TPM2_ATM_SRC_4MHZ  = 1    /**< \brief 32KHZ RTC OSC  */
} amhw_kl26_mcg_atm_src_t;

/*******************************************************************************
* API
*******************************************************************************/

/**
 * \brief STOP模式下，内部参考时钟使能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_irc_stop_enable (void)
{
    KL26_MCG->c1 |= (1UL << 0);
}

/**
 * \brief STOP模式下，内部参考时钟禁能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_irc_stop_disable (void)
{
    KL26_MCG->c1 &= ~(1UL << 0);
}

/**
 * \brief MCGIRCLK时钟使能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_irc_enable (void)
{
    KL26_MCG->c1 |= (1UL << 1);
}

/**
 * \brief MCGIRCLK时钟禁能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_irc_disable (void)
{
    KL26_MCG->c1 &= ~(1UL << 1);
}

/**
 * \brief 内部参考时钟源选择
 * \param[in] src : 内部参考时钟源
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_irc_src_set (amhw_kl26_mcg_irc_src_t src)
{
    KL26_MCG->c2 = (KL26_MCG->c2 & (~(1UL << 0))) |
                        (src << 0);
}

/**
 * \brief 内部参考时钟源获取
 * \return 内部参考时钟源
 * \note
 */
am_static_inline
amhw_kl26_mcg_irc_src_t amhw_kl26_mcg_irc_src_get (void)
{
    return (amhw_kl26_mcg_irc_src_t) ((KL26_MCG->c2 >> 0) & 0x01);
}

/**
 * \brief 内部参考时钟状态获取
 * \return 内部参考时钟使用的源
 * \note
 */
am_static_inline
amhw_kl26_mcg_irc_stat_t amhw_kl26_mcg_irc_stat_get (void)
{
    return (amhw_kl26_mcg_irc_stat_t) (KL26_MCG->s & 0x01);
}

/**
 * \brief FCRDIV快速内部参考时钟分频系数设置
 * \param[in] div : FCRDIV分频系数0~7
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_irc_fast_div_set (uint32_t div)
{
    KL26_MCG->sc = (KL26_MCG->sc & (~(7UL << 1))) |
                        (div << 1);
}

/**
 * \brief 获取FCRDIV快速内部参考时钟分频系数
 * \return  FCRDIV分频系数0~7
 */
am_static_inline
uint32_t amhw_kl26_mcg_irc_fast_div_get (void)
{
    return ((KL26_MCG->sc >> 1) & 0x07);
}

/**
 * \brief 快速内部参考时钟修正配置
 * \return 无
 * \note 复位时自动加载,
 */
am_static_inline
void amhw_kl26_mcg_irc_trim_set (void)
{
    if ( *((uint8_t*) 0x03FFU) != 0xFFU) {
        KL26_MCG->c3 = *((uint8_t*) 0x03FFU);
        KL26_MCG->c4 = (KL26_MCG->c4 & 0xFEU)      |
        		            ((*((uint8_t*) 0x03FEU)) & 0x01U);
  }
}

/**
 * \brief 获取ATM状态
 * \retval 0 : ATM修正正常完成
 * \retval 1 : ATM修正失败
 * \note
 */
am_static_inline
uint32_t amhw_kl26_mcg_atm_status_get (void)
{
    return (KL26_MCG->sc & (1UL << 5));
}

/**
 * \brief ATM自动修正时钟源设置
 * \param[in] src ：ATM时钟源
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_atm_src_set (amhw_kl26_mcg_atm_src_t src)
{
    KL26_MCG->sc = (KL26_MCG->sc & (~(1UL << 6))) |
                        (src << 6);
}

/**
 * \brief ATM自动修正机制使能
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_atm_enable (void)
{
    KL26_MCG->sc |= (1UL << 7);
}

/**
 * \brief ATM自动修正机制禁能
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_atm_disable (void)
{
    KL26_MCG->sc &= ~(1UL << 7);
}

/**
 * \brief ATM自动修正比较值设置
 * \param[in] value ：ATM比较值16bits
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_atm_value_set (uint32_t value)
{
    KL26_MCG->atcvh = (value >> 8) & 0xff;
    KL26_MCG->atcvl = value & 0xff;
}

/**
 * \brief FLL参考时钟源选择
 * \param[in] src : FLL参考时钟源
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_fll_src_set (amhw_kl26_mcg_fll_src_t src)
{
    KL26_MCG->c1 = (KL26_MCG->c1 & (~(1UL << 2))) |
                        (src << 2);
}

/**
 * \brief FLL参考时钟源选择
 * \return FLL参考时钟源
 */
am_static_inline
amhw_kl26_mcg_fll_src_t amhw_kl26_mcg_fll_src_get (void)
{
    return (amhw_kl26_mcg_fll_src_t)((KL26_MCG->c1 >> 2) & 0x01);
}

/**
 * \brief 获取FLL时钟状态
 * \retval 0 : 外部参考时钟作FLL参考时钟
 * \retval 1 : 内部参考时钟作FLL参考时钟
 */
am_static_inline
am_bool_t amhw_kl26_mcg_fll_stat_get (void)
{
    return (am_bool_t)((KL26_MCG->s & (1ul << 4)));
}

/**
 * \brief FRDIV分频系数设置
 * \param[in] div : FRDIV分频系数（0～7）
 * \return 无
 * \note FLL使用外部时钟时，必须分频至31.25 kHz to 39.0625 kHz之间
 */
am_static_inline
void amhw_kl26_mcg_fll_erc_div_set (uint32_t div)
{
    KL26_MCG->c1 = (KL26_MCG->c1 & (~(7UL << 3))) |
                        (div << 3);
}

/**
 * \brief FRDIV分频系数获取
 * \return FRDIV分频系数
 */
am_static_inline
uint32_t amhw_kl26_mcg_fll_erc_div_get (void)
{
    return ((KL26_MCG->c1 >> 3) & 0x07);
}

/**
 * \brief FLL DCO输出倍频系数设置
 * \param[in] div : DCO倍频系数
 * \return 无
 * \note  以内部参考时钟32.7768KHZ的基数倍频
 */
am_static_inline
void amhw_kl26_mcg_fll_dco_div_set (amhw_kl26_mcg_dco_div_t div)
{
    KL26_MCG->c4 = (KL26_MCG->c4 & (~(7UL << 5))) |
                        (div << 5);
}

/**
 * \brief FLL DCO输出倍频系数获取
 * \return 无
 */
am_static_inline
amhw_kl26_mcg_dco_div_t amhw_kl26_mcg_fll_dco_div_get (void)
{
    return (amhw_kl26_mcg_dco_div_t)((KL26_MCG->c4 >> 5) & 0x07);
}

/**
 * \brief 改变时钟模式时，FLL滤波和频率保留使能
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_fll_fitprsrv_enable (void)
{
    KL26_MCG->sc |= 1UL << 4;
}

/**
 * \brief 改变时钟模式时，FLL滤波和频率保留禁能
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_fll_fitprsrv_disable (void)
{
    KL26_MCG->sc &= ~(1UL << 4);
}

/**
 * \brief MCGOUTCLK时钟源配置
 * \param[in] src : MCGOUTCLK时钟源
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_outclk_src_set (amhw_kl26_mcg_outclk_src_t src)
{
    KL26_MCG->c1 = (KL26_MCG->c1 & (~(3UL << 6))) |
                        (src << 6);
}

/**
 * \brief MCGOUTCLK时钟源获取
 * \return MCGOUTCLK时钟源
 * \note
 */
am_static_inline
amhw_kl26_mcg_outclk_src_t amhw_kl26_mcg_outclk_src_get (void)
{
    return (amhw_kl26_mcg_outclk_src_t) ((KL26_MCG->c1 >> 6) & 0x03);
}

/**
 * \brief MCGOUTCLK时钟源获取
 * \return MCGOUTCLK时钟源
 * \note
 */
am_static_inline
amhw_kl26_mcg_outclk_stat_t amhw_kl26_mcg_outclk_stat_get (void)
{
    return (amhw_kl26_mcg_outclk_stat_t) ((KL26_MCG->s >> 2) & 0x03);
}

/**
 * \brief OSC外部参考时钟源选择
 * \param[in] src : OSC外部参考时钟源
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_osc_src_set (amhw_kl26_mcg_osc_src_t src)
{
    KL26_MCG->c2 = (KL26_MCG->c2 & (~(1UL << 2))) |
                        (src << 2);
}

/**
 * \brief OSC外部参考时钟源获取
 * \return OSC外部参考时钟源
 * \note
 */
am_static_inline
amhw_kl26_mcg_osc_src_t amhw_kl26_mcg_osc_src_get (void)
{
    return (amhw_kl26_mcg_osc_src_t)((KL26_MCG->c2 >> 2) & 0x01);
}

/**
 * \brief OSC振荡器模式选择
 * \param[in] mode : OSC振荡器模式
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_osc_gain_set (amhw_kl26_mcg_osc_gain_t mode)
{
    KL26_MCG->c2 = (KL26_MCG->c2 & (~(1UL << 3))) |
                        (mode << 3);
}

/**
 * \brief 获取OSC振荡器模式
 * \return OSC振荡器模式
 * \note
 */
am_static_inline
amhw_kl26_mcg_osc_gain_t amhw_kl26_mcg_osc_gain_get (void)
{
    return (amhw_kl26_mcg_osc_gain_t)((KL26_MCG->c2 >> 3)& 0x01);
}

/**
 * \brief OSC振荡器频率范围设置
 * \param[in] range : OSC振荡器频率范围
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_osc_range_set (amhw_kl26_mcg_osc_range_t range)
{
    KL26_MCG->c2 = (KL26_MCG->c2 & (~(3UL << 4))) |
                        (range << 4);
}

/**
 * \brief 获取OSC振荡器频率范围
 * \return OSC振荡器频率范围
 * \note
 */
am_static_inline
amhw_kl26_mcg_osc_range_t amhw_kl26_mcg_osc_range_get (void)
{
    return (amhw_kl26_mcg_osc_range_t)((KL26_MCG->c2 >> 4) & 0x03);
}

/**
 * \brief OSC振荡器内部负载电容配置
 * \param[in] value ：内部负载电容大小
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_osc_cap_cfg (amhw_kl26_mcg_osc_cap_t value)
{
	KL26_OSC->cr = (KL26_OSC->cr & (~(0x0f << 0))) |
                   (value << 0);
}

/**
 * \brief OSC0时钟监控使能
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_osc_monitor_enable (amhw_kl26_mcg_osc_monitor_mode_t mode)
{
    KL26_MCG->c2 = ((KL26_MCG->c2 & (~(1ul << 7))) | (mode << 7));
    KL26_MCG->c6 |= (1UL << 5);
}

/**
 * \brief OSC0时钟监控禁能
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_osc_monitor_disable (void)
{
    KL26_MCG->c2 = KL26_MCG->c2 & (~(1ul << 7));
    KL26_MCG->c6 &= ~(1UL << 5);
}

/**
 * \brief 获取OSC失锁状态
 * \retval 0 : 失锁没有发生
 * \retval 1 : 失锁发生
 * \note
 */
am_static_inline
am_bool_t amhw_kl26_mcg_osc_losslock_stat_get (void)
{
    return (am_bool_t)(KL26_MCG->sc & 0x01);
}

/**
 * \brief 获取OSC稳定状态
 * \retval 0 : OSC未稳定
 * \retval 1 : OSC稳定
 * \note
 */
am_static_inline
am_bool_t amhw_kl26_mcg_osc_stable_stat_get (void)
{
    return (am_bool_t)(KL26_MCG->s & (1ul << 1));
}

/**
 * \brief MCGPLLCLK时钟使能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_pll_enable (void)
{
    KL26_MCG->c5 |= (1UL << 6);
}

/**
 * \brief MCGPLLCLK时钟禁能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_pll_disable (void)
{
    KL26_MCG->c5 &= ~(1UL << 6);
}

/**
 * \brief STOP模式下，PLL时钟使能
 * \return 无
 * \note 除了PEE和PBE模式，PLL时钟要使能
 */
am_static_inline
void amhw_kl26_mcg_pll_stopmode_enable (void)
{
    KL26_MCG->c5 |= (1UL << 5);
}

/**
 * \brief STOP模式下，PLL时钟禁能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_pll_stopmode_disable (void)
{
    KL26_MCG->c5 &= ~(1UL << 5);
}



/**
 * \brief PLL ERC分配系数（PRDIV0）设置
 * \param[in] div :  PRDIV0分配系数 0~24
 * \return 无
 * \note  pll输入的时钟必须分频后 2 – 4 MHz
 */
am_static_inline
void amhw_kl26_mcg_pll_erc_div_set (uint32_t div)
{
    KL26_MCG->c5 = (KL26_MCG->c5 & (~(0x1f << 0))) |
                        (div << 0);
}

/**
 * \brief PLL ERC分配系数（PRDIV0）获取
 * \return PRDIV0分配系数 0~24
 */
am_static_inline
uint32_t amhw_kl26_mcg_pll_erc_div_get (void)
{
    return ((KL26_MCG->c5 >> 0) & 0x1f);
}

/**
 * \brief PLL VCO倍频系数设置
 * \param[in] div :  VCO分配系数（0～31对应分频值24～55）
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_pll_vco_div_set (uint32_t div)
{
    KL26_MCG->c6 = (KL26_MCG->c6 & (~(0x1f << 0))) |
                        (div << 0);
}

/**
 * \brief PLL VCO倍频系数获取
 * \return VCO倍频系数
 * \note 实际倍频(24 + VCODIV)倍
 */
am_static_inline
uint32_t amhw_kl26_mcg_pll_vco_div_get (void)
{
    return ((KL26_MCG->c6 >> 0) & 0x1f);
}

/**
 * \brief PLL失锁中断使能
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_pll_losslock_int_enable (void)
{
    KL26_MCG->c6 |= (1UL << 7);
}

/**
 * \brief PLL失锁中断禁能
 * \return 无
 * \note
 */
am_static_inline
void amhw_kl26_mcg_pll_losslock_int_disable (void)
{
    KL26_MCG->c6 &= ~(1UL << 7);
}

/**
 * \brief PLL失锁复位使能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_pll_losslock_rest_enable (void)
{
    KL26_MCG->c8 |= 1UL << 6;
}

/**
 * \brief PLL失锁复位禁能
 * \return 无
 * \note 复位禁能时产生中断请求，要求PLL失锁中断使能
 */
am_static_inline
void amhw_kl26_mcg_pll_losslock_rest_disable (void)
{
    KL26_MCG->c8 &= ~(1UL << 6);
}

/**
 * \brief PLL失锁状态获取
 * \retval 0: PLL没有失锁
 * \retval 1: PLL已经失锁
 * \note
 */
am_static_inline
am_bool_t amhw_kl26_mcg_pll_losslock_stat_get (void)
{
    return (am_bool_t)(KL26_MCG->s & (1ul << 7));
}

/**
 * \brief PLL锁定状态
 * \retval 0: PLL没锁定
 * \retval 1: PLL锁定
 * \note
 */
am_static_inline
am_bool_t amhw_kl26_mcg_pll_locked_stat_get (void)
{
    return (am_bool_t)(KL26_MCG->s & (1ul << 6));
}

/**
 * \brief 设置PLLS的时钟源
 * \param[in] src : PLLS时钟源
 * \return 无
 * \note 先设置MCGOUTCLK时钟源为PLLS
 */
am_static_inline
void amhw_kl26_mcg_plls_src_set (amhw_kl26_mcg_plls_src_t src)
{
    KL26_MCG->c6 = (KL26_MCG->c6 & (~(1UL << 6))) |
                        (src << 6);
}

/**
 * \brief 获取PLLS的时钟源
 * \return PLLS时钟源
 */
am_static_inline
amhw_kl26_mcg_plls_src_t amhw_kl26_mcg_plls_src_get (void)
{
    return (amhw_kl26_mcg_plls_src_t)((KL26_MCG->c6 >> 6) & 0x01);
}

/**
 * \brief 获取PLLS时钟的状态
 * \retval 0 : MCGFLLCLK 作为 MCGOUTCLK输出
 * \retval 1 : MCGPLLCLK 作为 MCGOUTCLK输出
 */
am_static_inline
am_bool_t amhw_kl26_mcg_plls_stat_get (void)
{
    return (am_bool_t)(KL26_MCG->s & (1ul << 5));
}

/**
 * \brief 低功耗模式下FLL/PLL使能
 * \return 无
 * \note 仅FBE,PBE,BLPI,BLPE模式下有效
 */
am_static_inline
void amhw_kl26_mcg_lp_plls_enable (void)
{
    KL26_MCG->c2 &= ~(1UL << 1);
}

/**
 * \brief 低功耗模式下FLL/PLL禁能
 * \return 无
 * \note 仅FBE,PBE,BLPI,BLPE模式下有效
 */
am_static_inline
void amhw_kl26_mcg_lp_plls_disable (void)
{
    KL26_MCG->c2 |= 1UL << 1;
}

/**
 * \brief 低功耗模式下FLL/PLL状态
 * \retval 0：FLL/PLL 使能
 * \retval 1：FLL/PLL 禁能
 */
am_static_inline
am_bool_t amhw_kl26_mcg_lp_plls_get (void)
{
    return (am_bool_t) ((KL26_MCG->c2 >> 1) & 0x01);
}

/**
 * \brief STOP模式下，OSCERCLK时钟使能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_erc_stop_enable (void)
{
	KL26_OSC->cr |= (1UL << 5);
}

/**
 * \brief STOP模式下，OSCERCLK时钟禁能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_erc_stop_disable (void)
{
	KL26_OSC->cr &= ~(1UL << 5);
}

/**e
 * \brief OSCERCLK时钟使能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_erc_enable (void)
{
	KL26_OSC->cr |= (1UL << 7);
}

/**
 * \brief OSCERCLK时钟禁能
 * \return 无
 */
am_static_inline
void amhw_kl26_mcg_erc_disable (void)
{
	KL26_OSC->cr &= ~(1UL << 7);
}


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __KL26_TPM2_H */

/* end of file */
