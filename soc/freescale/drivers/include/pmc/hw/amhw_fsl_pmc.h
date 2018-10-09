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
 * \brief PMU 硬件操作接口
 * 
 * 1. 支持低电压报警中断；
 * 2. 支持低电压复位；
 * 3. VLLSx唤醒确认控制；
 * 4. 能带隙(缓冲)使能控制。
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-14  ari, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_PMC_H
#define __AMHW_FSL_PMC_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "am_bitops.h"
/**
 * \addtogroup amhw_fsl_if_pmc
 * \copydoc amhw_fsl_pmc.h
 * @{
 */

/**
 * \brief PMC寄存器块结构体
 */
typedef struct amhw_fsl_puc {
    __IO uint8_t lvdsc1;      /**< \brief PMC 低电压状态控制寄存器1 */
    __IO uint8_t lvdsc2;      /**< \brief PMC 低电压状态控制寄存器2 */
    __IO uint8_t regsc;       /**< \brief PMC 调整控制寄存器        */
} amhw_fsl_pmc_t;

/**
 * \name PMC中断标志
 * @{
 */

#define AMHW_FSL_PMC_FLAG_LVDF  AM_BIT(7)   /**< \brief 低电压检测标志 */
#define AMHW_FSL_PMC_FLAG_LVWF  AM_BIT(6)   /**< \brief 低电压报警标志 */

/** @} */

/**
 * \name PMC中断选项
 * @{
 */

#define AMHW_FSL_PMC_IRQ_LVD    AM_BIT(7)   /**< \brief 低电压检测中断 */
#define AMHW_FSL_PMC_IRQ_LVW    AM_BIT(6)   /**< \brief 低电压报警中断 */

/** @} */

/**
 * \name PMC确认选项
 * @{
 */

#define AMHW_FSL_PMC_ACK_LVD    AM_BIT(7)   /**< \brief 低电压检测确认 */
#define AMHW_FSL_PMC_ACK_LVW    AM_BIT(6)   /**< \brief 低电压报警确认 */
#define AMHW_FSL_PMC_ACK_VLLS   AM_BIT(5)   /**< \brief VLLS唤醒确认   */

/** @} */

/** \brief 低电压检测阈值 */
typedef enum amhw_fsl_pmc_lvd_sel {
    AMHW_FSL_PMC_LVD_VLDL = 0,              /**< \brief 低阈值检测 */
    AMHW_FSL_PMC_LVD_VLDH                   /**< \brief 高阈值检测 */
} amhw_fsl_pmc_lvd_sel_t;

/** \brief 低电压报警阈值 */
typedef enum amhw_fsl_pmc_lvw_sel {
    AMHW_FSL_PMC_LVW_VLW1 = 0,              /**< \brief 报警阈值1 */
    AMHW_FSL_PMC_LVW_VLW2,                  /**< \brief 报警阈值2 */
    AMHW_FSL_PMC_LVW_VLW3,                  /**< \brief 报警阈值3 */
    AMHW_FSL_PMC_LVW_VLW4                   /**< \brief 报警阈值4 */
} amhw_fsl_pmc_lvw_sel_t;


/**
 * \brief 获取PMC中断标志
 * \param[in] p_hw_pmc : 指向PMC寄存器块的指针
 * \return 中断标志
 */
am_static_inline
uint8_t amhw_fsl_pmc_int_flags_get (amhw_fsl_pmc_t *p_hw_pmc)
{
    uint8_t flags = (p_hw_pmc->lvdsc1 & 0x80);

    flags |= (p_hw_pmc->lvdsc2 & 0x80) >> 1;

    return flags;
}

/**
 * \brief 使能PMC中断
 *
 * \param[in] p_hw_pmc  : 指向PMC寄存器块的指针
 * \param[in] irq_flags : 中断定义，AMHW_FSL_PMC_IRQ_*宏值(#AMHW_FSL_PMC_IRQ_LVW)
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_int_enable (amhw_fsl_pmc_t *p_hw_pmc, uint8_t irq_flags)
{
    if (irq_flags & AMHW_FSL_PMC_IRQ_LVD) {
        p_hw_pmc->lvdsc1 |= 0x20;
    }
    if (irq_flags & AMHW_FSL_PMC_IRQ_LVW) {
        p_hw_pmc->lvdsc2 |= 0x20;
    }
}

/**
 * \brief 禁能PMC中断
 *
 * \param[in] p_hw_pmc  : 指向PMC寄存器块的指针
 * \param[in] irq_flags : 中断定义，AMHW_FSL_PMC_IRQ_*宏值(#AMHW_FSL_PMC_IRQ_LVW)
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_int_disable (amhw_fsl_pmc_t *p_hw_pmc, uint8_t irq_flags)
{
    if (irq_flags & AMHW_FSL_PMC_IRQ_LVD) {
        p_hw_pmc->lvdsc1 &= (~0x20);
    }
    if (irq_flags & AMHW_FSL_PMC_IRQ_LVW) {
        p_hw_pmc->lvdsc2 &= (~0x20);
    }
}

/**
 * \brief 使能低电压复位功能
 * \param[in] p_hw_pmc  : 指向PMC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_lvdrest_enable (amhw_fsl_pmc_t *p_hw_pmc)
{
    p_hw_pmc->lvdsc1 |= 0x10;
}

/**
 * \brief 禁能低电压复位功能
 * \param[in] p_hw_pmc  : 指向PMC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_lvdrest_disable (amhw_fsl_pmc_t *p_hw_pmc)
{
    p_hw_pmc->lvdsc1 &= (~0x10);
}

/**
 * \brief ACK确认操作
 *
 * \param[in] p_hw_pmc  : 指向PMC寄存器块的指针
 * \param[in] ack_flags : ACK确认选项标志，AMHW_FSL_PMC_ACK_*宏值(#AMHW_FSL_PMC_ACK_LVD)
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_ack_set (amhw_fsl_pmc_t *p_hw_pmc, uint8_t ack_flags)
{
    if (ack_flags & AMHW_FSL_PMC_ACK_LVD) {
        p_hw_pmc->lvdsc1 |= 0x40;
    }
    if (ack_flags & AMHW_FSL_PMC_ACK_LVW) {
        p_hw_pmc->lvdsc2 |= 0x40;
    }
    if (ack_flags & AMHW_FSL_PMC_ACK_VLLS) {
        p_hw_pmc->regsc |= 0x08;
    }
}

/**
 * \brief 低电压检测阈值选择
 *
 * \param[in] p_hw_pmc : 指向PMC寄存器块的指针
 * \param[in] sel      : 阈值选择
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_lvd_cfg (amhw_fsl_pmc_t *p_hw_pmc, amhw_fsl_pmc_lvd_sel_t sel)
{
    p_hw_pmc->lvdsc1 = (sel | (p_hw_pmc->lvdsc1 & 0x3));
}

/**
 * \brief 低电压报警阈值选择
 *
 * \param[in] p_hw_pmc : 指向PMC寄存器块的指针
 * \param[in] sel      : 阈值选择
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_lvw_cfg (amhw_fsl_pmc_t *p_hw_pmc, amhw_fsl_pmc_lvw_sel_t sel)
{
    p_hw_pmc->lvdsc2 = (sel | (p_hw_pmc->lvdsc1 & 0x3));
}

/**
 * \brief 使能能带隙，当处于VLPX模式
 * \param[in] p_hw_pmc : 指向PMC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_bandgap_enable (amhw_fsl_pmc_t *p_hw_pmc)
{
    p_hw_pmc->regsc |= 0x10;
}

/**
 * \brief 禁能能带隙，当处于VLPX模式
 * \param[in] p_hw_pmc : 指向PMC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_bandgap_disable (amhw_fsl_pmc_t *p_hw_pmc)
{
    p_hw_pmc->regsc &= (~0x10);
}

/**
 * \brief 使能能带隙缓冲区
 * \param[in] p_hw_pmc : 指向PMC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_bandgap_buf_enable (amhw_fsl_pmc_t *p_hw_pmc)
{
    p_hw_pmc->regsc |= 0x01;
}

/**
 * \brief 禁能能带隙缓冲区
 * \param[in] p_hw_pmc : 指向PMC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_pmc_bandgap_buf_disable (amhw_fsl_pmc_t *p_hw_pmc)
{
    p_hw_pmc->regsc &= (~0x01);
}

/**
 * \brief 返回调整器工作状态
 *
 * \param[in] p_hw_pmc : 指向PMC寄存器块的指针
 *
 * \retval AM_TRUE  : 调整器工作
 * \retval AM_FALSE : 调整器没有工作或者在过渡状态
 */
am_static_inline
am_bool_t amhw_fsl_pmc_regulator_wstat_get (amhw_fsl_pmc_t *p_hw_pmc)
{
    return (p_hw_pmc->regsc & 0x04) ? AM_TRUE : AM_FALSE;
}

/** 
 * @} amhw_fsl_if_pmc
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_PMC_H */

/*end of file */
