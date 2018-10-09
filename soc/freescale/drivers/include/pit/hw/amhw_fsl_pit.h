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
 * \brief 周期性中断定时器（PIT）操作接口
 *
 * 1. 定时器产生DMA触发脉冲的能力;
 * 2. 定时器产生中断的能力;
 * 3. 可屏蔽中断;
 * 4. 每个定时器都有独立的超时周期.
 *
 * \note 该模块没有外部引脚
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-18  sdy, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_PIT_H
#define __AMHW_FSL_PIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup amhw_fsl_if_pit
 * \copydoc amhw_fsl_pit.h
 * @{
 */

/**
 * \brief TPM通道数
 */
#define AMHW_FSL_PIT_CHANNELS_NUM    (2)

/**
 * \brief PIT通道ID,用于操作具体的某个通道，
 * 有效范围：0 ~ (AMHW_FSL_PIT_CHANNELS_NUM - 1)
 */
#define AMHW_FSL_PIT_CH(n)           ((n) < AMHW_FSL_PIT_CHANNELS_NUM ? (n) : 0)


/**
 * \brief TPM 寄存器块结构体
 */
typedef struct amhw_fsl_pit {
    __IO uint32_t module_ctl;           /**< \brief 模块控制 */
    __I  uint32_t reserved_0[55];       /**< \brief 保留，不应使用 */
    __I  uint32_t lifetime64_h;         /**< \brief 生命周期定时器高32位 */
    __I  uint32_t lifetime64_l;         /**< \brief 生命周期定时器低32位 */
    __I  uint32_t reserved_1[6];        /**< \brief 保留，不应使用 */

    struct {
        __IO uint32_t load_value;           /**< \brief 装载值 */
        __I  uint32_t current_value;        /**< \brief 当前值 */
        __IO uint32_t control;              /**< \brief 定时器控制 */
        __IO uint32_t flag;                 /**< \brief 定时器标志 */
    } channel[2];                           /**< \brief tpm通道描述   */
} amhw_fsl_pit_t;


/**
 * \name 模块控制寄存器位定义(R/W)
 * @{
 */

#define AMHW_FSL_PIT_MSC_MASK     (0x3u)         /**< \brief 状态和控制寄存器掩码 */
#define AMHW_FSL_PIT_MSC_MDIS     (1UL << 1)     /**< \brief 模块时钟禁能 */
#define AMHW_FSL_PIT_MSC_FRZ      (1UL << 0)     /**< \brief 调试模式下定时器暂停 */

/** @} */

/**
 * \brief PIT模块使能
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_pit_module_enable (amhw_fsl_pit_t *p_hw_pit)
{
    p_hw_pit->module_ctl &= ~AMHW_FSL_PIT_MSC_MDIS;
}

/**
 * \brief PIT模块禁能
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_pit_module_disable (amhw_fsl_pit_t *p_hw_pit)
{
    p_hw_pit->module_ctl |= AMHW_FSL_PIT_MSC_MDIS;
}

/**
 * \name 声明周期寄存器,分为高，低两部分，组成一个64位定时器(只读)
 * @{
 */

#define AMHW_FSL_PIT_LTH_MASK     (0xFFFFFFFFu)   /**< \brief 生命周期定时器高位部分 */
#define AMHW_FSL_PIT_LTL_MASK     (0xFFFFFFFFu)   /**< \brief 生命周期定时器低位部分 */

/** @} */

/**
 * \brief 获取生命周期定时器高32位
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \return 生命周期定时器高32位
 */
am_static_inline
uint32_t amhw_fsl_pit_ltmr64h_get (amhw_fsl_pit_t *p_hw_pit)
{
    return p_hw_pit->lifetime64_h;
}

/**
 * \brief 获取生命周期定时器低位
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \return 生命周期定时器低32位
 */
am_static_inline
uint32_t amhw_fsl_pit_ltmr64l_get (amhw_fsl_pit_t *p_hw_pit)
{
    return p_hw_pit->lifetime64_l;
}

/**
 * \brief 装载值设定
 *
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_PIT_CH(n)
 * \param[in] val      : 需要写入的值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_pit_ldval_set (amhw_fsl_pit_t *p_hw_pit,
                              uint8_t          chan,
                              uint32_t         val)
{
    p_hw_pit->channel[chan].load_value = val;
}

/**
 * \brief 装载值获取
 *
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_PIT_CH(n)
 *
 * \return 装载值
 */
am_static_inline
uint32_t amhw_fsl_pit_ldval_get (amhw_fsl_pit_t *p_hw_pit, uint8_t chan)
{
    return p_hw_pit->channel[chan].load_value;
}

/**
 * \brief 当前值获取
 *
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_PIT_CH(n)
 *
 * \return 当前值
 */
am_static_inline
uint32_t amhw_fsl_pit_cval_get (amhw_fsl_pit_t *p_hw_pit, uint8_t chan)
{
    return p_hw_pit->channel[chan].current_value;
}

/**
 * \name PIT控制寄存器定义(R/W)
 * @{
 */

#define AMHW_FSL_PIT_TCTL_MASK          (0x7u)           /**< \brief 模数掩码 */
#define AMHW_FSL_PIT_TCTL_CHAIN_MODE    (1UL << 2)       /**< \brief 链模式 */
#define AMHW_FSL_PIT_TCTL_TI_EN         (1UL << 1)       /**< \brief 定时器中断使能 */
#define AMHW_FSL_PIT_TCTL_TMR_EN        (1UL << 0)       /**< \brief 定时器使能 */

/** @} */

/**
 * \brief 定时器控制寄存器置位
 *
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_PIT_CH(n)
 * \param[in] flags    : AMHW_FSL_PIT_TCTL_* 宏值或多个
 *                       AMHW_FSL_PIT_TCTL_*宏的 或（OR）值(#AMHW_FSL_PIT_TCTL_TI_EN)
 * \return 无
 */
am_static_inline
void amhw_fsl_pit_timer_ctrl_set (amhw_fsl_pit_t *p_hw_pit,
                                   uint8_t          chan,
                                   uint32_t         flags)
{
    p_hw_pit->channel[chan].control |= flags & AMHW_FSL_PIT_TCTL_MASK;
}

/**
 * \brief 定时器控制寄存器清零
 *
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_PIT_CH(n)
 * \param[in] flags    : AMHW_FSL_PIT_TCTL_* 宏值或多个
 *                       AMHW_FSL_PIT_TCTL_*宏的 或（OR）值(#AMHW_FSL_PIT_TCTL_TI_EN)
 * \return 无
 */
am_static_inline
void amhw_fsl_pit_timer_ctrl_clear (amhw_fsl_pit_t *p_hw_pit,
                                     uint8_t          chan,
                                     uint32_t         flags)
{
    p_hw_pit->channel[chan].control &= ~(flags & AMHW_FSL_PIT_TCTL_MASK);
}

/**
 * \name PIT标志寄存器定义(R/W)
 * @{
 */

#define AMHW_FSL_PIT_TMR_FLAG        (1UL << 0)             /**< \brief 定时器使能 */

/** @} */

/**
 * \brief 定时器中断标志
 *
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_PIT_CH(n)
 *
 * \retval  AM_TRUE  : 超时已发生
 * \retval  AM_FALSE : 超时未发生
 */
am_static_inline
am_bool_t amhw_fsl_pit_timerout_flag_check (amhw_fsl_pit_t *p_hw_pit,
                                           uint8_t          chan)
{
    return (am_bool_t)(p_hw_pit->channel[chan].flag & AMHW_FSL_PIT_TMR_FLAG);
}

/**
 * \brief 定时器中断标志清除
 *
 * \param[in] p_hw_pit : 指向PIT寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_PIT_CH(n)
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_pit_timerout_flag_clr (amhw_fsl_pit_t *p_hw_pit,
                                      uint8_t          chan)
{
    p_hw_pit->channel[chan].flag = AMHW_FSL_PIT_TMR_FLAG;
}

/**
 * @} amhw_fsl_if_pit
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_PIT_H */

/* end of file */

