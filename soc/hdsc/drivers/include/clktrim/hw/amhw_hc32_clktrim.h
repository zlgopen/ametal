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
 * \brief 时钟校准模块（CLKTRIM）
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-29
 *
 * \endinternal
 */

#ifndef __AMHW_HC32_CLKTRIM_H
#define __AMHW_HC32_CLKTRIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zlg_if_clktrim
 * \copydoc amhw_zlg_clktrim.h
 * @{
 */

/**
 * \brief CLKTRIM - Register Layout Typedef
 */
typedef struct amhw_zlg_clktrim {
    __IO uint32_t cr;        /**< \brief 配置寄存器 */
    __IO uint32_t refcon;    /**< \brief 参考计数器初值配置寄存器 */
    __I  uint32_t refcnt;    /**< \brief 参考计数器值寄存器 */
    __I  uint32_t calcnt;    /**< \brief 校准计数器值寄存器 */
    __IO uint32_t ifr;       /**< \brief 中断标志位寄存器 */
    __IO uint32_t iclr;      /**< \brief 中断标志位清除寄存器 */
    __IO uint32_t calcon;    /**< \brief 校准计数器溢出值配置寄存器 */
} amhw_hc32_clktrim_t;

/* 待校准/监测时钟源 */
typedef enum amhw_clktrim_calclk_sel {
    AMHW_CLKTRIM_CALCLK_RCH = 0x0,
    AMHW_CLKTRIM_CALCLK_XTH = 0x1,
    AMHW_CLKTRIM_CALCLK_RCL = 0x2,
    AMHW_CLKTRIM_CALCLK_XTL = 0x3,
    AMHW_CLKTRIM_CALCLK_PLL = 0x10
} amhw_clktrim_calclk_sel_t;

/* 参考时钟源 */
typedef enum amhw_clktrim_refclk_sel {
    AMHW_CLKTRIM_REFCLK_RCH        = 0x0,
    AMHW_CLKTRIM_REFCLK_XTH        = 0x1,
    AMHW_CLKTRIM_REFCLK_RCL        = 0x2,
    AMHW_CLKTRIM_REFCLK_XTL        = 0x3,
    AMHW_CLKTRIM_REFCLK_IRC10K     = 0x4,
    AMHW_CLKTRIM_REFCLK_EXT_CLK_IN = 0x5
} amhw_clktrim_refclk_sel_t;

typedef enum amhw_clktrim_ifr {
    AMHW_CLKTRIM_STOP      = (1),       /**< \brief 参考计数器停止标志 */
    AMHW_CLKTRIM_CALCNT_OF = (1 << 1),  /**< \brief 校准计数器溢出标志 */
    AMHW_CLKTRIM_XTL_FAULT = (1 << 2),  /**< \brief XTL 失效标志 */
    AMHW_CLKTRIM_XTH_FAULT = (1 << 3),  /**< \brief XTH 失效标志 */
    AMHW_CLKTRIM_PLL_FAULT = (1 << 4)   /**< \brief PLL 失效标志 */
} amhw_clktrim_ifr_t;

/**
 * \brief 待校准/监测时钟选择
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 * \param[in] calclk_sel   : 待校准/监测时钟选项
 *
 * retval : none
 */
am_static_inline
void amhw_hc32_clktrim_calclk_sel (amhw_hc32_clktrim_t      *p_hw_clktrim,
                                   amhw_clktrim_calclk_sel_t   calclk_sel)
{
    p_hw_clktrim->cr &= 0x4f;
    p_hw_clktrim->cr |= calclk_sel << 4;
}

/**
 * \brief 中断使能控制
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 * \param[in] enable       : 使能选项   1，使能
 *                                    0，禁能
 *
 * retval : none
 */
am_static_inline
void amhw_hc32_clktrim_int_enable (amhw_hc32_clktrim_t *p_hw_clktrim,
                                   uint8_t                enable)
{
    if(enable != 0 && enable != 1) {
        return;
    }
    p_hw_clktrim->cr &= ~(1 << 7);
    p_hw_clktrim->cr |= (enable << 7);
}

/**
 * \brief 监测模式使能控制
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 * \param[in] enable       : 使能选项   1，使能
 *                                    0，禁能
 *
 * retval : none
 */
am_static_inline
void amhw_hc32_clktrim_mon_enable (amhw_hc32_clktrim_t *p_hw_clktrim,
                                   uint8_t                enable)
{
    if(enable != 0 && enable != 1) {
        return;
    }
    p_hw_clktrim->cr &= ~(1 << 6);
    p_hw_clktrim->cr |= (enable << 6);
}

/**
 * \brief 参考时钟选择
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 * \param[in] refclk_sel   : 待校准/监测时钟选项
 *
 * retval : none
 */
am_static_inline
void amhw_hc32_clktrim_refclk_sel (amhw_hc32_clktrim_t      *p_hw_clktrim,
                                   amhw_clktrim_refclk_sel_t   refclk_sel)
{
    p_hw_clktrim->cr &= ~0x0e;
    p_hw_clktrim->cr |= (refclk_sel << 1);
}

/**
 * \brief 校准/监测开始控制
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 * \param[in] start        : 开始选项   1，开始
 *                                    0，停止
 *
 * retval : none
 */
am_static_inline
void amhw_hc32_clktrim_trim_start (amhw_hc32_clktrim_t *p_hw_clktrim,
                                   uint8_t                start)
{
    if(start != 0 && start != 1) {
        return;
    }
    p_hw_clktrim->cr &= ~1;
    p_hw_clktrim->cr |= start;
}

/**
 * \brief 参考计数器初值配置
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 * \param[in] rcntval      : 参考计数器初始值
 *
 * retval : none
 */
am_static_inline
void amhw_hc32_clktrim_refcon_set (amhw_hc32_clktrim_t *p_hw_clktrim,
                                   uint32_t             rcntval)
{
    p_hw_clktrim->refcon = rcntval;
}

/**
 * \brief 获取参考计数器值
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 *
 * retval : 计数器值
 */
am_static_inline
uint32_t amhw_hc32_clktrim_refcnt_get (amhw_hc32_clktrim_t *p_hw_clktrim)
{
    return p_hw_clktrim->refcnt;
}

/**
 * \brief 获取校准计数器值
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 *
 * retval : 计数器值
 */
am_static_inline
uint32_t amhw_hc32_clktrim_calcnt_get (amhw_hc32_clktrim_t *p_hw_clktrim)
{
    return p_hw_clktrim->calcnt;
}

/**
 * \brief 获取中断标志
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 * \param[in] ifr          : 中断标志
 *
 * retval : 0，无中断标志
 *         非0，有中断标志
 */
am_static_inline
uint8_t amhw_hc32_clktrim_ifr_get (amhw_hc32_clktrim_t *p_hw_clktrim,
                                   amhw_clktrim_ifr_t     ifr)
{
    return p_hw_clktrim->ifr & ifr;
}

/**
 * \brief 清除中断标志
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 * \param[in] ifr          : 中断标志
 *
 * retval : AM_OK，成功
 *         -AM_EPERM，操作不允许
 */
am_static_inline
uint8_t amhw_hc32_clktrim_ifr_clear (amhw_hc32_clktrim_t *p_hw_clktrim,
                                     amhw_clktrim_ifr_t     ifr)
{
    if(ifr < 4) {
        return AM_EPERM;
    }
    p_hw_clktrim->ifr &= ~ifr;
    return AM_OK;
}

/**
 * \brief 校准计数器溢出值配置
 *
 * \param[in] p_hw_clktrim : 指向clktrim寄存器地址的指针
 * \param[in] ccntval      : 校准计数器溢出值
 *
 * retval : none
 */
am_static_inline
void amhw_hc32_clktrim_calcon_set (amhw_hc32_clktrim_t *p_hw_clktrim,
                                   uint32_t               ccntval)
{
    p_hw_clktrim->calcon = ccntval;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32_CLKTRIM_H */

/* end of file */
