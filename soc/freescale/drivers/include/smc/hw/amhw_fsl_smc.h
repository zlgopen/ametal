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
 * \brief SMC 硬件操作接口
 * 
 * 1. 系统模式控制；
 * 2. 系统模式获取。
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-14  mkr, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_SMC_H
#define __AMHW_FSL_SMC_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "am_bitops.h"
/**
 * \addtogroup amhw_fsl_if_smc
 * \copydoc amhw_fsl_smc.h
 * @{
 */
 
/**
 * \brief SMC寄存器块结构体
 */
typedef struct amhw_fsl_smc {
    __IO uint8_t  pmprot;           /**< \brief SMC 模式保护寄存器 */
    __IO uint8_t  pmctrl;           /**< \brief SMC 模式控制寄存器 */
    __IO uint8_t  stopctrl;         /**< \brief SMC 停止控制寄存器 */
    __I  uint8_t  pmstat;           /**< \brief SMC 模式状态寄存器 */
} amhw_fsl_smc_t;

/**
 * \name 系统允许进入模式
 * @{
 */

#define AMHW_FSL_SMC_PROTECT_AVLP_ALLOW  AM_BIT(5) /**< \brief 允许进入VLPx模式  */
#define AMHW_FSL_SMC_PROTECT_ALLS_ALLOW  AM_BIT(3) /**< \brief 允许进入LLS模式   */
#define AMHW_FSL_SMC_PROTECT_AVLLS_ALLOW AM_BIT(1) /**< \brief 允许进入VLLSx模式 */

/** @} */

/**
 * \name 运行模式
 * @{
 */

#define AMHW_FSL_SMC_RUNMODE_RUN    AM_SBF(0x00, 5) /**< \brief 正常运行模式    */
#define AMHW_FSL_SMC_RUNMODE_VLPR   AM_SBF(0x02, 5) /**< \brief 低功耗运行模式  */

/** @} */

/**
 * \name 停止模式
 * @{
 */

/** \brief 正常停止模式 */
#define AMHW_FSL_SMC_STOPMODE_STOP      (AM_SBF(0, 8) | AM_SBF(0, 6))

/** \brief 停止模式1 */
#define AMHW_FSL_SMC_STOPMODE_STOP1     (AM_SBF(0, 8) | AM_SBF(1, 6))

/** \brief 停止模式2 */
#define AMHW_FSL_SMC_STOPMODE_STOP2     (AM_SBF(0, 8) | AM_SBF(2, 6))

/** \brief 低功耗停止模式 */
#define AMHW_FSL_SMC_STOPMODE_VLPS      (AM_SBF(2, 8) | 0)

/** \brief 低漏电停止模式 */
#define AMHW_FSL_SMC_STOPMODE_LLS       (AM_SBF(3, 8) | 0)

/** \brief 超低漏电停止模式3 */
#define AMHW_FSL_SMC_STOPMODE_VLLS3     (AM_SBF(4, 8) | AM_SBF(3, 0))

/** \brief 超低漏电停止模式1 */
#define AMHW_FSL_SMC_STOPMODE_VLLS1     (AM_SBF(4, 8) | AM_SBF(1, 0))

/** \brief 超低漏电停止模式0 */
#define AMHW_FSL_SMC_STOPMODE_VLLS0     (AM_SBF(4, 8) | AM_SBF(0, 0))

/** @} */

/**
 * \name 芯片模式
 * @{
 */

#define AMHW_FSL_SMC_MODE_RUN    AM_BIT(0)   /**< \brief 运行模式        */
#define AMHW_FSL_SMC_MODE_STOP   AM_BIT(1)   /**< \brief 停止模式        */
#define AMHW_FSL_SMC_MODE_VLPR   AM_BIT(2)   /**< \brief 低功耗运行模式  */
#define AMHW_FSL_SMC_MODE_VLPW   AM_BIT(3)   /**< \brief 低功耗等待模式  */
#define AMHW_FSL_SMC_MODE_VLPS   AM_BIT(4)   /**< \brief 低功耗停止模式  */
#define AMHW_FSL_SMC_MODE_LLS    AM_BIT(5)   /**< \brief 地漏电模式模式  */
#define AMHW_FSL_SMC_MODE_VLLS   AM_BIT(6)   /**< \brief 超低漏电模式    */

/** @} */

/**
 * \brief 配置系统允许进入的模式
 *
 * \param[in] p_hw_smc : 指向SMC寄存器块的指针
 * \param[in] flags    : 模式允许宏定义，AMHW_SMC_PROTECT_*_ALLOW宏值或多个
 *                      （#AMHW_FSL_SMC_PROTECT_AVLP_ALLOW）
 *
 * \return 无
 *
 * \note 复位后，只可以操作一次
 */
am_static_inline
void amhw_fsl_smc_mode_allow_cfg (amhw_fsl_smc_t *p_hw_smc, uint8_t flags)
{
    p_hw_smc->pmprot = flags;
}

/**
 * \brief 运行模式选择
 *
 * \param[in] p_hw_smc : 指向SMC寄存器块的指针
 * \param[in] flags    : 运行模式宏定义，AMHW_FSL_SMC_RUNMODE_*宏值
 *                      （#AMHW_FSL_SMC_RUNMODE_RUN）
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_smc_run_mdoe_sel (amhw_fsl_smc_t *p_hw_smc, uint8_t flags)
{
    p_hw_smc->pmctrl = flags | (p_hw_smc->pmctrl & (~(0x03 << 5)));
}

/**
 * \brief 停止模式选择
 *
 * \param[in] p_hw_smc : 指向SMC寄存器块的指针
 * \param[in] flags    : 停止模式宏定义，AMHW_FSL_SMC_STOPMODE_*宏值
 *                      （#AMHW_FSL_SMC_STOPMODE_STOP）
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_smc_stop_mdoe_sel (amhw_fsl_smc_t *p_hw_smc, uint16_t flags)
{
    uint8_t pmctrl   = p_hw_smc->pmctrl & (~0x07);
    uint8_t stopctrl = p_hw_smc->stopctrl & (~0xC7); /* stop位和VLLS位 */

    pmctrl   |= flags >> 8;
    stopctrl |= flags;

    p_hw_smc->stopctrl = stopctrl;
    p_hw_smc->pmctrl   = pmctrl;
}

/**
 * \brief 判断进入停止模式是否被中断
 * \param[in] p_hw_smc : 指向SMC寄存器块的指针
 * \retval AM_TRUE : 被中断；AM_FALSE : 未被中断
 */
am_static_inline
am_bool_t amhw_fsl_smc_stop_mdoe_isabort (amhw_fsl_smc_t *p_hw_smc)
{
    return (p_hw_smc->pmctrl & 0x08) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 使能VLLS0模式电源检测电路
 * \param[in] p_hw_smc : 指向SMC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_smc_vlls0_por_enable (amhw_fsl_smc_t *p_hw_smc)
{
    p_hw_smc->stopctrl &= (~0x20);
}

/**
 * \brief 禁能VLLS0模式电源检测电路
 * \param[in] p_hw_smc : 指向SMC寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_smc_vlls0_por_disable (amhw_fsl_smc_t *p_hw_smc)
{
    p_hw_smc->stopctrl |= 0x20;
}

/**
 * \brief 获取当前系统模式
 * \param[in] p_hw_smc : 指向SMC寄存器块的指针
 * \return 芯片模式
 */
am_static_inline
uint8_t amhw_fsl_smc_mode_get (amhw_fsl_smc_t *p_hw_smc)
{
    return p_hw_smc->pmstat;
}

/** 
 * @} amhw_if_fsl_smc
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_SMC_H */

/*end of file */
