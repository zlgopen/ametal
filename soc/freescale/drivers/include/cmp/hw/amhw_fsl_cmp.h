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
 * \brief CMP 硬件操作接口
 *
 * 1. 可在 2.7 V 至 5.5 V 的整个电源电压范围上操作；
 * 2. 片上 6 位分辨率 DAC，可从 VDD 或内部带隙中选择基准电压；
 * 3. 可配置迟滞；
 * 4. 可在比较器输出的上升沿、下降沿、两个上升沿或两个下降沿时选择中断；
 * 5. 可选择翻转比较器输出；
 * 6. 可在停止模式下操作；
 * 7. 支持触发DMA传输。
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-14  mkr, first implementation.
 * \endinternal
 */
#ifndef  __AMHW_FSL_CMP_H
#define  __AMHW_FSL_CMP_H

#ifdef __cplusplus
extern "C" {
#endif
#include "am_types.h"

/**
 * \addtogroup amhw_fsl_if_cmp
 * \copydoc amhw_fsl_cmp.h
 * @{
 */

/**
 * \brief CMP 寄存器块结构体
 */
typedef struct amhw_fsl_cmp {
    __IO    uint8_t     ctrl0;          /**< \brief CMP 比较器控制寄存器0     */
    __IO    uint8_t     ctrl1;          /**< \brief CMP 比较器控制寄存器1     */
    __IO    uint8_t     fpr;            /**< \brief CMP 滤波周期寄存器               */
    __IO    uint8_t     scr;            /**< \brief CMP 状态与控制寄存器     */
    __IO    uint8_t     daccr;          /**< \brief CMP DAC控制寄存器     */
    __IO    uint8_t     muxcr;          /**< \brief CMP MUX控制寄存器     */
} amhw_fsl_cmp_t;

/**
 * \name CMP 控制寄存器0位域定义
 * \anchor grp_amhw_fsl_cmp_ctrl0
 * @{
 */
#define AMHW_FSL_CMP_CTRL0_FILTER_CNT_MASK    (7UL << 4)    /**< \brief 滤波采样数掩码 */
#define AMHW_FSL_CMP_CTRL0_HYSTCTR_MASK       (3UL << 0)    /**< \brief 迟滞控制掩码 */

#define AMHW_FSL_CMP_CTRL0_FILTER_CNT_CFG(x) \
    (((x)<< 4) & AMHW_FSL_CMP_CTRL0_FILTER_CNT_MASK) /**< \brief 滤波采样数配置 */
#define AMHW_FSL_CMP_CTRL0_HYSTCTR_CFG(x) \
    (((x)<< 0) & AMHW_FSL_CMP_CTRL0_HYSTCTR_MASK) /**< \brief 迟滞控制配置 */
/** @} */

/**
 * \name CMP 控制寄存器1位域定义
 * \anchor grp_amhw_cmp_ctrl1
 * @{
 */
#define AMHW_FSL_CMP_CTRL1_EN_MASK    (1UL << 0)    /**< \brief 模块使能掩码 */
#define AMHW_FSL_CMP_CTRL1_OPE_MASK   (1UL << 1)    /**< \brief 引脚输出使能掩码 */
#define AMHW_FSL_CMP_CTRL1_COS_MASK   (1UL << 2)    /**< \brief 输出选择掩码 */
#define AMHW_FSL_CMP_CTRL1_INV_MASK   (1UL << 3)    /**< \brief 比较反转掩码 */
#define AMHW_FSL_CMP_CTRL1_PMODE_MASK (1UL << 4)    /**< \brief 功耗模式掩码 */
#define AMHW_CMP_FSL_CTRL1_TRIGM_MASK (1UL << 5)    /**< \brief 触发使能使能掩码 */
#define AMHW_FSL_CMP_CTRL1_WE_MASK    (1UL << 6)    /**< \brief 窗口使能掩码 */
#define AMHW_FSL_CMP_CTRL1_SE_MASK    (1UL << 7)    /**< \brief 采样使能掩码 */
/** @} */

/**
 * \name CMP 滤波周期位域定义
 * \anchor grp_amhw_cmp_fpr
 * @{
 */
#define AMHW_FSL_CMP_FPR_MASK    0xFF    /**< \brief 滤波周期掩码 */
#define AMHW_FSL_CMP_FPR_CFG(x) ((x) & AMHW_FSL_CMP_FPR_MASK) /**< \brief 滤波周期配置 */
/** @} */

/**
 * \name CMP 状态控制寄存器位域定义
 * \anchor grp_amhw_cmp_scr
 * @{
 */
#define AMHW_FSL_CMP_SCR_COUT_MASK    (1UL << 0)    /**< \brief 模拟输出掩码 */
#define AMHW_FSL_CMP_SCR_CFF_MASK     (1UL << 1)    /**< \brief 下降沿标志掩码 */
#define AMHW_FSL_CMP_SCR_CFR_MASK     (1UL << 2)    /**< \brief 上升沿标志掩码 */
#define AMHW_FSL_CMP_SCR_IEF_MASK     (1UL << 3)    /**< \brief 使能CFF中断掩码 */
#define AMHW_FSL_CMP_SCR_IER_MASK     (1UL << 4)    /**< \brief 使能CFR中断掩码 */
#define AMHW_FSL_CMP_SCR_DMAEN_MASK   (1UL << 6)    /**< \brief 触发DMA传输掩码 */
/** @} */

/**
 * \name CMP DAC控制寄存器位域定义
 * \anchor grp_amhw_cmp_daccr
 * @{
 */
#define AMHW_FSL_CMP_DACCR_VOSEL_MASK    0x0F   /**< \brief 电压选择掩码 */
#define AMHW_FSL_CMP_DACCR_VRSEL_MASK    0x40   /**< \brief 供电电压选择源掩码 */
#define AMHW_FSL_CMP_DACCR_DACEN_MASK    0x80   /**< \brief DAC模块使能掩码 */

#define AMHW_FSL_CMP_DACCR_VOSEL_CFG(x) \
    ((x) & AMHW_FSL_CMP_DACCR_VOSEL_MASK) /**< \brief DAC输出电压配置 */
/** @} */

/**
 * \name CMP 复选控制寄存器位域定义
 * \anchor grp_amhw_fsl_cmp_muxcr
 * @{
 */
#define AMHW_FSL_CMP_MUXCR_MSEL_MASK    (7UL << 0)    /**< \brief 反相输入选择掩码 */
#define AMHW_FSL_CMP_MUXCR_PSEL_MASK    (7UL << 3)    /**< \brief 同相输入选择掩码 */
#define AMHW_FSL_CMP_MUXCR_PSTM_MASK    (1UL << 7)    /**< \brief 旁路控制掩码 */

#define AMHW_FSL_CMP_MUXCR_MSEL_CFG(x) \
    ((x) & AMHW_FSL_CMP_MUXCR_MSEL_MASK) /**< \brief 反相输入选择配置 */
#define AMHW_FSL_CMP_MUXCR_PSEL_CFG(x) \
    (((x) << 3) & AMHW_FSL_CMP_MUXCR_PSEL_MASK) /**< \brief 反相输入选择配置 */
/** @} */


/**
 * \brief 设置迟滞电平配置参数
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \param[in] level    : 迟滞电平
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_hyst_set (amhw_fsl_cmp_t *p_hw_cmp, uint8_t level)
{
    p_hw_cmp->ctrl0 &= ~AMHW_FSL_CMP_CTRL0_HYSTCTR_MASK;
    p_hw_cmp->ctrl0 |= AMHW_FSL_CMP_CTRL0_HYSTCTR_CFG(level);
}

/**
 * \brief 获取迟滞电平配置参数
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return    迟滞电平
 */
am_static_inline
uint8_t amhw_fsl_cmp_hyst_get (amhw_fsl_cmp_t *p_hw_cmp)
{
    return (uint8_t)(p_hw_cmp->ctrl0 & AMHW_FSL_CMP_CTRL0_HYSTCTR_MASK);
}
/**
 * \brief 设置滤波采样个数
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \param[in] cnt      : 滤波采样个数
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_filter_val_set (amhw_fsl_cmp_t *p_hw_cmp, uint8_t cnt)
{
    p_hw_cmp->ctrl0 &= ~AMHW_FSL_CMP_CTRL0_FILTER_CNT_MASK;
    p_hw_cmp->ctrl0 |= AMHW_FSL_CMP_CTRL0_FILTER_CNT_CFG(cnt);
}

/**
 * \brief 获取滤波采样个数
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return  滤波采样个数
 */
am_static_inline
uint8_t amhw_fsl_cmp_filter_val_get (amhw_fsl_cmp_t *p_hw_cmp)
{
    return (uint8_t)((p_hw_cmp->ctrl0 & AMHW_FSL_CMP_CTRL0_FILTER_CNT_MASK) >> 4);
}


/**
 * \brief 使能控制寄存器1某位
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \param[in] mask     : 滤波采样个数, 值为AMHW_FSL_CMP_CTRL1_*一个或多个的或值
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_ctrl1_set (amhw_fsl_cmp_t *p_hw_cmp, uint8_t mask)
{
    p_hw_cmp->ctrl1 |= mask;
}
/**
 * \brief 清除控制寄存器1某位
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \param[in] mask     : 滤波采样个数, 值为AMHW_FSL_CMP_CTRL1_*一个或多个的或值
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_ctrl1_clr (amhw_fsl_cmp_t *p_hw_cmp, uint8_t mask)
{
    p_hw_cmp->ctrl1 &= ~mask;
}

/**
 * \brief 配置控制寄存器1
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \param[in] cfg     : 滤波采样个数, 值为AMHW_FSL_CMP_CTRL1_*一个或多个的或值
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_ctrl1_cfg (amhw_fsl_cmp_t *p_hw_cmp, uint8_t cfg)
{
    p_hw_cmp->ctrl1 = cfg;
}

/**
 * \brief  使能采样
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_sample_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 |= AMHW_FSL_CMP_CTRL1_SE_MASK;
}

/**
 * \brief 禁能采样
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_sample_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 &= ~AMHW_FSL_CMP_CTRL1_SE_MASK;
}

/**
 * \brief  使能窗口采样
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_window_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 |= AMHW_FSL_CMP_CTRL1_WE_MASK;
}

/**
 * \brief 禁能窗口采样
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_window_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 &= ~AMHW_FSL_CMP_CTRL1_WE_MASK;
}

/**
 * \brief  使能触发模式
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_trigm_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 |= AMHW_CMP_FSL_CTRL1_TRIGM_MASK;
}

/**
 * \brief 禁能触发模式
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_trigm_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 &= ~AMHW_CMP_FSL_CTRL1_TRIGM_MASK;
}

/**
 * \brief  高速模式
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_pmode_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 |= AMHW_FSL_CMP_CTRL1_PMODE_MASK;
}

/**
 * \brief 低速模式
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_pmode_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 &= ~AMHW_FSL_CMP_CTRL1_PMODE_MASK;
}

/**
 * \brief  使能反转
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_inv_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 |= AMHW_FSL_CMP_CTRL1_INV_MASK;
}

/**
 * \brief 不反转
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_inv_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 &= ~AMHW_FSL_CMP_CTRL1_INV_MASK;
}

/**
 * \brief  选择输出COUTA
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_cos_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 |= AMHW_FSL_CMP_CTRL1_COS_MASK;
}

/**
 * \brief 选择输出COUT
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_cos_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 &= ~AMHW_FSL_CMP_CTRL1_COS_MASK;
}
/**
 * \brief  使能比较输出引脚
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_ope_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 |= AMHW_FSL_CMP_CTRL1_OPE_MASK;
}

/**
 * \brief 禁能比较输出引脚
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_ope_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 &= ~AMHW_FSL_CMP_CTRL1_OPE_MASK;
}

/**
 * \brief  使能比较模块
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 |= AMHW_FSL_CMP_CTRL1_EN_MASK;
}

/**
 * \brief 禁能比较模块
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->ctrl1 &= ~AMHW_FSL_CMP_CTRL1_EN_MASK;
}

/**
 * \brief 设置滤波采样周期
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \param[in] period   : 滤波采样周期
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_fpr_set (amhw_fsl_cmp_t *p_hw_cmp, uint8_t period)
{
    p_hw_cmp->fpr = period;
}

/**
 * \brief 获取滤波采样周期
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return    滤波采样周期
 */
am_static_inline
uint8_t amhw_fsl_cmp_fpr_get (amhw_fsl_cmp_t *p_hw_cmp)
{
    return (uint8_t)(p_hw_cmp->fpr);
}

/**
 * \brief 获取COUT状态
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \retval AM_TRUE  : 状态为1
 * \retval AM_FALSE : 状态为0
 *
 */
am_static_inline
uint8_t amhw_fsl_cmp_cout_get (amhw_fsl_cmp_t *p_hw_cmp)
{
    return (p_hw_cmp->scr & AMHW_FSL_CMP_SCR_COUT_MASK) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 获取CFF状态
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \retval AM_TRUE  : 状态为1
 * \retval AM_FALSE : 状态为0
 */
am_static_inline
uint8_t amhw_fsl_cmp_cff_get (amhw_fsl_cmp_t *p_hw_cmp)
{
    return (p_hw_cmp->scr & AMHW_FSL_CMP_SCR_CFF_MASK) ? AM_TRUE : AM_FALSE;
}
/**
 * \brief 获取CFR状态
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \retval AM_TRUE  : 状态为1
 * \retval AM_FALSE : 状态为0
 */
am_static_inline
uint8_t amhw_fsl_cmp_cfr_get (amhw_fsl_cmp_t *p_hw_cmp)
{
    return (p_hw_cmp->scr & AMHW_FSL_CMP_SCR_CFR_MASK) ? AM_TRUE : AM_FALSE;
}
/**
 * \brief 清除CFF标志
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_cff_clr (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->scr |= AMHW_FSL_CMP_SCR_CFF_MASK;
}
/**
 * \brief 清除CFR标志
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_cfr_clr (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->scr |= AMHW_FSL_CMP_SCR_CFR_MASK;
}

/**
 * \brief  使能CFF中断
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_ief_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->scr |= AMHW_FSL_CMP_SCR_IEF_MASK;
}

/**
 * \brief 禁能CFF中断
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_ief_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->scr &= ~AMHW_FSL_CMP_SCR_IEF_MASK;
}

/**
 * \brief  使能CFR中断
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_ier_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->scr |= AMHW_FSL_CMP_SCR_IER_MASK;
}

/**
 * \brief 禁能CFR中断
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_ier_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->scr &= ~AMHW_FSL_CMP_SCR_IER_MASK;
}

/**
 * \brief  使能触发DMA传输
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_dma_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->scr |= AMHW_FSL_CMP_SCR_DMAEN_MASK;
}

/**
 * \brief 禁止触发DMA传输
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_dma_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->scr &= ~AMHW_FSL_CMP_SCR_DMAEN_MASK;
}

/**
 * \brief 设置DAC输出电压
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \param[in] vosel    : 电压参数
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_dac_vol_set (amhw_fsl_cmp_t *p_hw_cmp, uint8_t vosel)
{
    p_hw_cmp->daccr &= ~AMHW_FSL_CMP_DACCR_VOSEL_MASK;
    p_hw_cmp->daccr |= AMHW_FSL_CMP_DACCR_VOSEL_CFG(vosel);
}

/**
 * \brief 获取DAC输出电压
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return    迟滞电平
 */
am_static_inline
uint8_t amhw_fsl_cmp_dac_vol_get (amhw_fsl_cmp_t *p_hw_cmp)
{
    return (uint8_t)(p_hw_cmp->daccr & AMHW_FSL_CMP_DACCR_VOSEL_MASK);
}

/**
 * \brief  使用Vin2作为供电参考电压输入
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_dac_vin2_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->daccr |= AMHW_FSL_CMP_DACCR_VRSEL_MASK;
}

/**
 * \brief 使用Vin1作为供电参考电压输入
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_dac_vin2_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->daccr &= ~AMHW_FSL_CMP_DACCR_VRSEL_MASK;
}

/**
 * \brief  使能6位DAC模块
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_dac_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->daccr |= AMHW_FSL_CMP_DACCR_DACEN_MASK;
}

/**
 * \brief 禁能6位DAC模块
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_dac_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->daccr &= ~AMHW_FSL_CMP_DACCR_DACEN_MASK;
}

/**
 * \brief 设置反相输入端接入通道
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \param[in] ch       : 接入通道
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_msel_set (amhw_fsl_cmp_t *p_hw_cmp, uint8_t ch)
{
    p_hw_cmp->muxcr &= ~AMHW_FSL_CMP_MUXCR_MSEL_MASK;
    p_hw_cmp->muxcr |= AMHW_FSL_CMP_MUXCR_MSEL_CFG(ch);
}

/**
 * \brief 获取反相输入端接入通道
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return    接入通道
 */
am_static_inline
uint8_t amhw_fsl_cmp_msel_get (amhw_fsl_cmp_t *p_hw_cmp)
{
    return (uint8_t)(p_hw_cmp->muxcr & AMHW_FSL_CMP_MUXCR_MSEL_MASK);
}
/**
 * \brief 设置同相输入端接入通道
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \param[in] ch       : 接入通道
 * \return    无
 */
am_static_inline
void amhw_fsl_cmp_psel_set (amhw_fsl_cmp_t *p_hw_cmp, uint8_t ch)
{
    p_hw_cmp->muxcr &= ~AMHW_FSL_CMP_MUXCR_PSEL_MASK;
    p_hw_cmp->muxcr |= AMHW_FSL_CMP_MUXCR_PSEL_CFG(ch);
}

/**
 * \brief 获取同相输入端接入通道
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return    接入通道
 */
am_static_inline
uint8_t amhw_fsl_cmp_psel_get (amhw_fsl_cmp_t *p_hw_cmp)
{
    return (uint8_t)(p_hw_cmp->muxcr & AMHW_FSL_CMP_MUXCR_PSEL_MASK);
}

/**
 * \brief  使能旁路复选模块
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_pstm_enable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->muxcr |= AMHW_FSL_CMP_MUXCR_PSTM_MASK;
}

/**
 * \brief 禁能旁路复选模块
 *
 * \param[in] p_hw_cmp : 指向CMP寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_cmp_pstm_disable (amhw_fsl_cmp_t *p_hw_cmp)
{
    p_hw_cmp->muxcr &= ~AMHW_FSL_CMP_MUXCR_PSTM_MASK;
}

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /*__AMHW_FSL_CMP_H */

/* end of file */
