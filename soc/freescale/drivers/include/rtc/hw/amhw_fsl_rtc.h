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
 * \brief RTC 硬件操作接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-13  sdy, first implementation.
 * \endinternal
 */


#ifndef __AMHW_FSL_RTC_H
#define __AMHW_FSL_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_fsl_if_rtc
 * \copydoc amhw_fsl_rtc.h
 * @{
 */

/**
 *  \name RTC 位操作相关的宏定义
 *  @{
 */

/******************************************************************************
  RTC 校准寄存器操作相关
******************************************************************************/

/** \brief 校准间歇计数器当前值起始位偏移量 */
#define AMHW_FSL_RTC_CIC_SHIFT  (24UL)

/** \brief 当前的校准值起始位偏移量 */
#define AMHW_FSL_RTC_TCV_SHIFT  (16UL)

/** \brief 校准间歇设置起始位偏移量 */
#define AMHW_FSL_RTC_CIR_SHIFT  (8UL)

/** \brief 校准量起始位偏移量 */
#define AMHW_FSL_RTC_TCR_SHIFT  (0UL)


/*******************************************************************************
  RTC 控制寄存器操作相关宏
*******************************************************************************/

/** \brief 2pF 负载配置位 */
#define AMHW_FSL_RTC_SC2P       (1 << 13UL)

/** \brief 4pF 负载配置位 */
#define AMHW_FSL_RTC_SC4P       (1 << 12UL)

/** \brief 8pF 负载配置位 */
#define AMHW_FSL_RTC_SC8P       (1 << 11UL)

/** \brief 16pF 负载配置位 */
#define AMHW_FSL_RTC_SC16P      (1 << 10UL)

/** \brief 32kHz 时钟输出到其他设备使能位 */
#define AMHW_FSL_RTC_CLKO       (1 <<  9UL)

/** \brief 32.768 kHz时钟使能位 */
#define AMHW_FSL_RTC_OSCE       (1 <<  8UL)

/** \brief 唤醒引脚选择位 */
#define AMHW_FSL_RTC_WPS        (1 <<  4UL)

/** \brief 允许强制更新使能位 */
#define AMHW_FSL_RTC_UM         (1 <<  3UL)

/** \brief 仅特权模式下可写使能位 */
#define AMHW_FSL_RTC_SUP        (1 <<  2UL)

/** \brief 唤醒引脚使能位 */
#define AMHW_FSL_RTC_WPE        (1 <<  1UL)

/** \brief RTC 模块软件复位 */
#define AMHW_FSL_RTC_SWR        (1 <<  0UL)


/*******************************************************************************
  RTC 状态寄存器操作相关宏
*******************************************************************************/

/** \brief RTC 已使能标志位 */
#define AMHW_FSL_RTC_TCE        (1 <<  4UL)

/** \brief 闹钟发生标志位 */
#define AMHW_FSL_RTC_TAF        (1 <<  2UL)

/** \brief RTC 时钟计数器溢出标志位 */
#define AMHW_FSL_RTC_TOF        (1 <<  1UL)

/** \brief RTC 时钟模块无效标志位 */
#define AMHW_FSL_RTC_TIF        (1 <<  0UL)



/******************************************************************************
  RTC 锁定寄存器操作相关宏
******************************************************************************/

/** \brief RTC上锁使能位 */
#define AMHW_FSL_RTC_LRL        (1 <<  6UL)

/** \brief RTC上锁状态位 */
#define AMHW_FSL_RTC_SRL        (1 <<  5UL)

/** \brief RTC控制寄存器上锁使能位 */
#define AMHW_FSL_RTC_CRL        (1 <<  4UL)

/** \brief RTC校准上锁使能位 */
#define AMHW_FSL_RTC_TCL        (1 <<  3UL)


/*******************************************************************************
   RTC 中断使能寄存器操作相关宏
*******************************************************************************/

/** \brief 唤醒引脚开启位 */
#define AMHW_FSL_RTC_WPON       (1 << 7UL)

/** \brief 每秒中断使能位 */
#define AMHW_FSL_RTC_TSIE       (1 << 4UL)

/** \brief 闹钟中断使能 */
#define AMHW_FSL_RTC_TAIE       (1 << 2UL)

/** \brief 溢出中断使能 */
#define AMHW_FSL_RTC_TOIE       (1 << 1UL)

/** \brief 时钟模块无效中断使能位 */
#define AMHW_FSL_RTC_TIIE       (1 << 0UL)


/**
 *  @}
 */

/**
 *  \brief RTC 寄存器结构体定义
 */
typedef struct amhw_fsl_rtc {
    __IO uint32_t tsr;    /**< \brief RTC 秒寄存器 */
    __IO uint32_t tpr;    /**< \brief RTC 预分频寄存器 */
    __IO uint32_t tar;    /**< \brief RTC 闹钟寄存器 */
    __IO uint32_t tcr;    /**< \brief RTC 校准寄存器 */
    __IO uint32_t cr;     /**< \brief RTC 控制寄存器 */
    __IO uint32_t sr;     /**< \brief RTC 状态寄存器 */
    __IO uint32_t lr;     /**< \brief RTC 锁定寄存器 */
    __IO uint32_t ier;    /**< \brief RTC 中断使能寄存器 */
} amhw_fsl_rtc_t;


/*******************************************************************************
    内联函数定义
*******************************************************************************/

/**
 *  \brief 写秒计数寄存器
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \param second 要写入的秒的数值
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_second_set (amhw_fsl_rtc_t *p_hw_rtc, uint32_t second)
{
    p_hw_rtc->tsr = second;
}

/**
 *  \brief 读秒计数寄存器
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 秒计数寄存器的值
 */
am_static_inline
uint32_t amhw_fsl_rtc_second_get (amhw_fsl_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->tsr;
}

/**
 *  \brief 写预分频寄存器
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \param[in] prescaler 要写入的预分频的数值
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_prescaler_set (amhw_fsl_rtc_t *p_hw_rtc, uint16_t prescaler)
{
    p_hw_rtc->tpr = prescaler & 0xffff;
}

/**
 *  \brief 读预分频寄存器
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 预分频寄存器
 */
am_static_inline
uint16_t amhw_fsl_rtc_prescaler_get (amhw_fsl_rtc_t *p_hw_rtc)
{
    return (p_hw_rtc->tpr & 0xffff);
}

/**
 *  \brief 设置闹钟寄存器的值
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \param alarm_second 要设置的闹钟寄存器值
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_alarm_set (amhw_fsl_rtc_t *p_hw_rtc, uint32_t alarm_second)
{
    p_hw_rtc->tar = alarm_second;
}

/**
 *  \brief 读取闹钟寄存器的值
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 闹钟寄存器的值
 */
am_static_inline
uint32_t amhw_fsl_rtc_alarm_get (amhw_fsl_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->tar;
}

/**
 *  \brief 设置每次的校准量
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \param value 每次校准的量
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_compensate_value_set (amhw_fsl_rtc_t *p_hw_rtc,
                                        uint32_t        value)
{
    AM_BITS_SET(p_hw_rtc->tcr, AMHW_FSL_RTC_TCR_SHIFT, 8, value);
}

/**
 *  \brief 设置校准间歇
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \param interval 校准间歇，每多少秒调整一次
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_compensate_interval_set (amhw_fsl_rtc_t *p_hw_rtc,
                                           uint32_t        interval)
{
    AM_BITS_SET(p_hw_rtc->tcr, AMHW_FSL_RTC_CIR_SHIFT, 8, interval);
}

/**
 *  \brief 获取当前的校准量
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 当前校准量
 */
am_static_inline
uint8_t amhw_fsl_rtc_current_compensate_value_get (amhw_fsl_rtc_t *p_hw_rtc)
{
    return (uint8_t)((p_hw_rtc->tcr & (0xff << AMHW_FSL_RTC_TCV_SHIFT))
                      >> AMHW_FSL_RTC_TCV_SHIFT);
}
/**
 *  \brief 设置当前的校准量
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *  \param value 校准量，每次校准减少的时钟个数
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_current_compensate_value_set (amhw_fsl_rtc_t *p_hw_rtc,
                                                uint8_t         value)
{
    p_hw_rtc->tcr |= (((p_hw_rtc->tcr                   &
                     (~(0xff << AMHW_FSL_RTC_TCV_SHIFT))))) |
                     (value << AMHW_FSL_RTC_TCV_SHIFT);
}
/**
 *  \brief 设置当前调整间歇计数器的值
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *  \param count 设置的当前计数器值
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_current_compensate_counter_set (amhw_fsl_rtc_t *p_hw_rtc,
                                                  uint8_t         count)
{
    p_hw_rtc->tcr |= ((p_hw_rtc->tcr                     &
                     (~(0xfful << AMHW_FSL_RTC_CIC_SHIFT)))) |
                     (count << AMHW_FSL_RTC_CIC_SHIFT);
}
/**
 *  \brief 设置调整间歇和调整量
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *  \param interval 调整间歇
 *  \param value 调整值
 *
 *  \return 无
 *
 */
am_static_inline
void amhw_fsl_rtc_current_interval_and_value_set (amhw_fsl_rtc_t *p_hw_rtc,
                                                  uint8_t         interval,
                                                  uint8_t         value)
{
    p_hw_rtc->tcr = (interval << 8) | value;
}

/**
 *  \brief 使能 2pF的振荡器负载
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_2pf_load_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_SC2P;
}

/**
 *  \brief 除能 2pF的振荡器负载
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_2pf_load_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr &= (~ AMHW_FSL_RTC_SC2P);
}

/**
 *  \brief 使能 4pF的振荡器负载
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_4pf_load_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_SC4P;
}

/**
 *  \brief 除能 4pF的振荡器负载
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_4pf_load_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr &= (~ AMHW_FSL_RTC_SC4P);
}

/**
 *  \brief 使能 8pF的振荡器负载
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_8pf_load_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_SC8P;
}

/**
 *  \brief 除能 8pF的振荡器负载
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_8pf_load_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr &= (~ AMHW_FSL_RTC_SC8P);
}

/**
 *  \brief 使能 16pF的振荡器负载
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_16pf_load_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_SC16P;
}

/**
 *  \brief 除能 16pF的振荡器负载
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_16pf_load_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr &= (~ AMHW_FSL_RTC_SC16P);
}

/**
 *  \brief 32kHz时钟输出到其它设备使能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_clk_output_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_CLKO;
}

/**
 *  \brief 32kHz时钟输出到其它设备除能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_clk_output_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr &= (~AMHW_FSL_RTC_CLKO);
}

/**
 *  \brief RTC 32.768kHz 振荡器使能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_osc_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_OSCE;
}

/**
 *  \brief RTC 32.768kHz 振荡器除能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_osc_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_OSCE;
}

/**
 *  \brief 允许在锁定的状态下写状态寄存器(SR)
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_update_mode_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_UM;
}

/**
 *  \brief 设置不允许在锁定的状态下写状态寄存器(SR)
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_update_mode_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr &= (~AMHW_FSL_RTC_UM);
}

/**
 *  \brief 使能唤醒引脚
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_wakeup_pin_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_WPE;
}

/**
 *  \brief 使能唤醒引脚
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_wakeup_pin_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr &= (~AMHW_FSL_RTC_WPE);
}

/**
 *  \brief 软件复位RTC模块
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_soft_reset (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_SWR;
}

/**
 *  \brief 清除软件复位位
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_soft_rstbit_clear (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->cr |= AMHW_FSL_RTC_SWR;
}

/**
 *  \brief 时间计数器状态使能位获取
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \retval 0 没有使能
 *  \retval AMHW_FSL_RTC_TCE 已经使能
 */
am_static_inline
uint32_t amhw_fsl_rtc_time_counter_status_get (amhw_fsl_rtc_t *p_hw_rtc)
{
    return (p_hw_rtc->sr & AMHW_FSL_RTC_TCE);
}

/**
 *  \brief 置1计数器状态使能位,即使能计数器
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_time_counter_status_set (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->sr |= AMHW_FSL_RTC_TCE;
}

/**
 *  \brief 清0计数器状态使能位，即除能计数器
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_time_counter_status_clr (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->sr &= ~AMHW_FSL_RTC_TCE;
}

/**
 *  \brief 闹钟状态获取
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \retval 0 闹钟没有发生过
 *  \retval AMHW_FSL_RTC_TAF 闹钟发生过
 */
am_static_inline
uint32_t amhw_fsl_rtc_alarm_status_get (amhw_fsl_rtc_t *p_hw_rtc)
{
    return (p_hw_rtc->sr & AMHW_FSL_RTC_TAF);
}

/**
 *  \brief 秒计数器溢出状态获取
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \retval 0 没有溢出
 *  \retval AMHW_FSL_RTC_TOF 发生过溢出
 */
am_static_inline
uint32_t amhw_fsl_rtc_count_over_status_get (amhw_fsl_rtc_t *p_hw_rtc)
{
    return (p_hw_rtc->sr & AMHW_FSL_RTC_TOF);
}

/**
 *  \brief RTC 模块是否为无效状态获取
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \retval 0 模块有效
 *  \retval AMHW_FSL_RTC_TIF 模块无效
 */
am_static_inline
uint32_t amhw_fsl_rtc_is_time_invalid (amhw_fsl_rtc_t *p_hw_rtc)
{
    return (p_hw_rtc->sr & AMHW_FSL_RTC_TIF);
}

/**
 *  \brief 对锁定寄存器(LR)上锁
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_lock_reg_lock (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->lr &= ~AMHW_FSL_RTC_LRL;
}

/**
 *  \brief 对控制寄存器(CR)上锁
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_control_reg_lock (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->lr &= ~AMHW_FSL_RTC_CRL;
}

/**
 *  \brief 对状态寄存器(SR)上锁
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_status_reg_lock (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->lr &= ~AMHW_FSL_RTC_SRL;
}

/**
 *  \brief 对校准寄存器(TCR)上锁
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_compensate_reg_lock (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->lr &= ~AMHW_FSL_RTC_TCL;
}

/**
 *  \brief 解除对锁定寄存器(LR)上锁
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_lock_reg_unlock (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->lr |= AMHW_FSL_RTC_LRL;
}

/**
 *  \brief 解除对控制寄存器(CR)上锁
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_control_reg_unlock (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->lr |= AMHW_FSL_RTC_CRL;
}

/**
 *  \brief 解除对状态寄存器(SR)上锁
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_status_reg_unlock (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->lr |= AMHW_FSL_RTC_SRL;
}

/**
 *  \brief 解除对校准寄存器(TCR)上锁
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_compensate_reg_unlock (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->lr |= AMHW_FSL_RTC_TCL;
}

/**
 *  \brief 唤醒引脚使能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_wakeup_pin_on (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier |= AMHW_FSL_RTC_WPON;
}

/**
 *  \brief 秒中断使能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_sec_int_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier |= AMHW_FSL_RTC_TSIE;
}

/**
 *  \brief 闹钟中断使能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_alarm_int_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier |= AMHW_FSL_RTC_TAIE;
}

/**
 *  \brief 溢出中断使能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_overflow_int_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier |= AMHW_FSL_RTC_TOIE;
}

/**
 *  \brief 模块中断无效使能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_invalid_int_enable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier |= AMHW_FSL_RTC_TIIE;
}

/**
 *  \brief 唤醒引脚除能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_wakeup_pin_off (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier &= ~AMHW_FSL_RTC_WPON;
}

/**
 *  \brief 秒中断除能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_sec_int_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier &= ~AMHW_FSL_RTC_TSIE;
}

/**
 *  \brief 闹钟中断除能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_alarm_int_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier &= ~AMHW_FSL_RTC_TAIE;
}

/**
 *  \brief 溢出中断除能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_overflow_int_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier &= ~AMHW_FSL_RTC_TOIE;
}

/**
 *  \brief 模块无效中断除能
 *
 *  \param p_hw_rtc 指向RTC寄存器块的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_rtc_invalid_int_disable (amhw_fsl_rtc_t *p_hw_rtc)
{
    p_hw_rtc->ier &= ~AMHW_FSL_RTC_TIIE;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AMHW_FSL_RTC_H */
