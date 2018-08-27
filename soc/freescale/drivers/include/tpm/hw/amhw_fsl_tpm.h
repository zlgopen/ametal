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
 * \brief 定时器/PWM模块（TPM）操作接口
 *
 * 1. TPM的时钟模式是可选的：
 *    -能够在每个异步计数时钟沿上产生增量
 *    -能够在外部时钟输入同步到异步时钟计数器的上升沿上增量
 * 2. 预分频器分频因子可选1,2,4,8,16,32,64或128;
 * 3. TPM包括一个16位的计数器：
 *    -可以是一个自由运行的计数器或模计数器
 *    -可以配置成向上计数或者向上向下计数
 * 4. 包含6个通道，每个通道可以配置位输入捕获、输出比较或边沿对齐PWM模式：
 *    -在输入捕获模式中，捕获动作可以发生在上升沿、下降沿或者上升/下降沿
 *    -在输出比较模式中，匹配时可以使输出信号置位、清零、脉冲或者翻转
 *    -所有通道可以配置成中心对齐PWM模式
 * 5. 每个通道支持产生中断和DMA请求;
 * 6. 当计数器溢出时支持产生中断和DMA请求;
 * 7. 支持可选择的触发输入来复位或者导致计数器重新增加：
 *    -当计数器溢出时可以选择停止计数
 * 8. 当计数器溢出时，支持产生硬件触发;
 *
 *            TPM模式、边沿、电平配置表(stat_ctl寄存器和通道stat_ctl寄存器)
 * ==========================================================================
 * | CPWMS | MSnB:MSnA | ELSnB:ELSnA |      Mode        |       配置
 * ==========================================================================
 * |   x   |     00    |      00     |      None        |     通道禁止
 * --------------------------------------------------------------------------
 * |   x   |     01    |      00     | Software compare |      无引脚
 * --------------------------------------------------------------------------
 * |       |           |      01     |                  |  只捕获上升沿
 * -       -           ---------------                  ---------------------
 * |       |     00    |      10     |  input capture   |  只捕获下降沿
 * -       -           ---------------                  ---------------------
 * |       |           |      11     |                  |  捕获上升\下降沿
 * -       ------------------------------------------------------------------
 * |       |           |      01     |                  |  匹配时输出翻转
 * -       -           ---------------                  ---------------------
 * |   0   |     01    |      10     |  output compare  |  匹配时输出为低
 * -       -           ---------------                  ---------------------
 * |       |           |      11     |                  |  匹配时输出为高
 * -       ------------------------------------------------------------------
 * |       |           |      10     |                  |  High-true脉冲
 * -       -     10    --------------- Edge-aligned PWM ---------------------
 * |       |           |      X1     |                  |  Low-true脉冲
 * -       ------------------------------------------------------------------
 * |       |           |      10     |                  |  匹配时脉冲输出为低
 * -       -     11    --------------- Output compare   ---------------------
 * |       |           |      01     |                  |  匹配时脉冲输出为高
 * --------------------------------------------------------------------------
 * |       |           |      10     |                  |  High-true脉冲
 * -   1   -     10    --------------- Edge-aligned PWM ---------------------
 * |       |           |      01     |                  |  Low-true脉冲
 * --------------------------------------------------------------------------
 */

/*
 * \internal
 * \par Modification History
 * - 1.00 16-09-13  sdy, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_TPM_H
#define __AMHW_FSL_TPM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_fsl_if_tpm
 * \copydoc amhw_fsl_tpm.h
 * @{
 */

/* 使用匿名联合体段结束 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)

  /* 默认使能匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif


/**
 * \brief TPM通道数
 */
#define AMHW_FSL_TPM_CHANNELS_NUM      (6)

/**
 * \brief TPM通道ID,用于操作具体的某个通道，
 * 有效范围：0 ~ (AMHW_FSL_TPM_CHANNELS_NUM - 1)
 */
#define AMHW_FSL_TPM_CH(n)             ((n) < AMHW_FSL_TPM_CHANNELS_NUM ? (n) : 0)


/**
 * \brief TPM寄存器块结构体
 */
typedef struct amhw_fsl_tpm {
    __IO uint32_t stat_ctl;             /**< \brief 状态和控制 */
    __IO uint32_t counter;              /**< \brief 计数值 */
    __IO uint32_t modulo;               /**< \brief 模数 */

    struct {
        __IO uint32_t stat_ctl;         /**< \brief 通道控制 */
        __IO uint32_t value;            /**< \brief 通道值 */
    } chan_ctl[6];                      /**< \brief tpm通道描述 */

    __I  uint32_t reserved_0[5];        /**< \brief 保留 */
    __IO uint32_t status;               /**< \brief 状态寄存器 */
    __I  uint32_t reserved_1[12];       /**< \brief 保留 */
    __IO uint32_t config;               /**< \brief 配置寄存器 */

} amhw_fsl_tpm_t;


/**
 * \name 状态和控制寄存器位定义(R/W)
 * @{
 */

#define AMHW_FSL_TPM_SC_MASK       (0x1FFu)           /**< \brief 状态和控制掩码 */
#define AMHW_FSL_TPM_SC_DMA_EN     (1UL << 8)         /**< \brief DMA使能（溢出） */
#define AMHW_FSL_TPM_SC_TO_FLAG    (1UL << 7)         /**< \brief 定时器溢出标志 */
#define AMHW_FSL_TPM_SC_TOI_EN     (1UL << 6)         /**< \brief 定时器溢出中断使能 */
#define AMHW_FSL_TPM_SC_CPWM_MODE  (1UL << 5)         /**< \brief TPM 计数模式 */
#define AMHW_FSL_TPM_SC_CM_SEL(s)  (((s) & 0x3) << 3) /**< \brief 时钟模式选择 */
#define AMHW_FSL_TPM_SC_PS_SEL(s)  (((s) & 0x7) << 0) /**< \brief 预分频因子选择 */

/** @} */

/**
 * \brief TPM时钟模式选择
 * \note 该位域被保护，仅当计数器被禁止时才可写入
 */
typedef enum amhw_fsl_tpm_clock_mode {
    AMHW_FSL_TPM_CLK_SRC_NONE     = 0x0,         /**< \brief 计数模块禁能 */
    AMHW_FSL_TPM_CLK_SRC_MODULE   = 0x1,         /**< \brief 选择模块时钟并使能 */
    AMHW_FSL_TPM_CLK_SRC_EXTERNAL = 0x2,         /**< \brief 外部时钟 */
} amhw_fsl_tpm_clock_mode_t;

/**
 * \brief TPM计数模式
 */
typedef enum amhw_fsl_tpm_counting_mode {
    AMHW_FSL_TPM_COUNTING_UP      = 0x0,         /**< \brief 向上计数，边沿模式 */
    AMHW_FSL_TPM_COUNTING_UP_DOWN = 0x1,         /**< \brief 向上-向下计数，中心对齐 */
} amhw_fsl_tpm_counting_mode_t;

/**
 * \brief TPM 预分频因子选择
 * \note 该位域被保护，仅当计数器被禁止时才可写入
 */
typedef enum amhw_fsl_tpm_prescale {
    AMHW_FSL_TPM_DIVIDED_BY_1   = 0x0,              /**< \brief 预分频因子为1 */
    AMHW_FSL_TPM_DIVIDED_BY_2   = 0x1,              /**< \brief 预分频因子为2 */
    AMHW_FSL_TPM_DIVIDED_BY_4   = 0x2,              /**< \brief 预分频因子为4 */
    AMHW_FSL_TPM_DIVIDED_BY_8   = 0x3,              /**< \brief 预分频因子为8 */
    AMHW_FSL_TPM_DIVIDED_BY_16  = 0x4,              /**< \brief 预分频因子为16 */
    AMHW_FSL_TPM_DIVIDED_BY_32  = 0x5,              /**< \brief 预分频因子为32 */
    AMHW_FSL_TPM_DIVIDED_BY_64  = 0x6,              /**< \brief 预分频因子为64 */
    AMHW_FSL_TPM_DIVIDED_BY_128 = 0x7,              /**< \brief 预分频因子为128 */
} amhw_fsl_tpm_prescale_t;

/**
 * \brief 状态和控制设定
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] flags    : AMHW_FSL_TPM_SC_* 宏值或多个
 *                       AMHW_FSL_TPM_SC_*宏的 或（OR）值(#AMHW_FSL_TPM_SC_DMA_EN)
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_ctrl_set (amhw_fsl_tpm_t *p_hw_tpm, uint32_t flags)
{
    p_hw_tpm->stat_ctl |= (flags & AMHW_FSL_TPM_SC_MASK);
}

/**
 * \brief 状态和控制清除
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] flags    : AMHW_FSL_TPM_SC_* 宏值或多个
 *                       AMHW_FSL_TPM_SC_*宏的 或（OR）值(#AMHW_FSL_TPM_SC_DMA_EN)
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_ctrl_clear (amhw_fsl_tpm_t *p_hw_tpm, uint32_t flags)
{
    p_hw_tpm->stat_ctl &= ~(flags & AMHW_FSL_TPM_SC_MASK);
}

/**
 * \brief 时钟模式
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] mode     : AMHW_FSL_TPM_SC_* 宏值或多个
 *                       AMHW_FSL_TPM_SC_*宏的 或（OR）值(#AMHW_FSL_TPM_SC_DMA_EN)
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_clock_mode (amhw_fsl_tpm_t            *p_hw_tpm,
                               amhw_fsl_tpm_clock_mode_t  mode)
{
    AM_BITS_SET(p_hw_tpm->stat_ctl, 3, 2, mode);
}

/**
 * \brief 预分频设置
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] prescale : 预分频值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_prescale_set (amhw_fsl_tpm_t          *p_hw_tpm,
                                amhw_fsl_tpm_prescale_t  prescale)
{
    AM_BITS_SET(p_hw_tpm->stat_ctl, 0, 3, prescale);
}

/**
 * \brief 预分频获取
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \return 预分频值，枚举类型amhw_tpm_prescale_t成员
 */
am_static_inline
amhw_fsl_tpm_prescale_t amhw_tpm_prescale_get (amhw_fsl_tpm_t *p_hw_tpm)
{
    return (amhw_fsl_tpm_prescale_t)AM_BITS_GET(p_hw_tpm->stat_ctl, 0, 3);
}

/**
 * \brief 状态和控制获取
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \return 状态和控制，参考AMHW_FSL_TPM_SC_#宏
 */
am_static_inline
uint32_t amhw_fsl_tpm_ctrl_get (amhw_fsl_tpm_t *p_hw_tpm)
{
    return p_hw_tpm->stat_ctl;
}

/**
 * \name 计数寄存器定义
 * @{
 */

#define AMHW_FSL_TPM_CNT_MASK       (0xFFFFu)           /**< \brief 计数值掩码 */

/** @} */

/**
 * \brief 清零计数值，写入任何值即可清零
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_count_clear (amhw_fsl_tpm_t *p_hw_tpm)
{
    p_hw_tpm->counter = 0x00;
}

/**
 * \brief 获取计数器值
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \return 计数值
 */
am_static_inline
uint16_t amhw_fsl_tpm_count_get (amhw_fsl_tpm_t *p_hw_tpm)
{
    return p_hw_tpm->counter;
}

/**
 * \name 模数寄存器定义(R/W)
 * @{
 */

#define AMHWV_TPM_MOD_MASK       (0xFFFFu)           /**< \brief 模数掩码 */

/** @} */

/**
 * \brief 设定模数值
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] value    : 需要设定的值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_mod_set (amhw_fsl_tpm_t *p_hw_tpm, uint16_t value)
{
    p_hw_tpm->modulo = value;
}

/**
 * \brief 获取模数值
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \return 模数值
 */
am_static_inline
uint16_t amhw_fsl_tpm_mod_get (amhw_fsl_tpm_t *p_hw_tpm)
{
    return p_hw_tpm->modulo;
}

/**
 * \name 通道状态和控制(R/W)
 * @{
 */

#define AMHW_FSL_TPM_CnSC_MASK      (0xFFu)             /**< \brief 模数掩码 */
#define AMHW_FSL_TPM_CHSC_FLAG      (1UL << 7)          /**< \brief 通道标志  -1:通道事件已发生 */
#define AMHW_FSL_TPM_CHSC_IE        (1UL << 6)          /**< \brief 通道中断使能 */
#define AMHW_FSL_TPM_CHSC_MS(s)     (((s) & 0x3) << 4)  /**< \brief 通道模式选择 */
#define AMHW_FSL_TPM_CHSC_EL_SEL(s) (((s) & 0x3) << 2)  /**< \brief 边沿或电平选择 */
#define AMHW_FSL_TPM_CHSC_MODE_MASK (0xFu << 2)
#define AMHW_FSL_TPM_CHSC_DMA_EN    (1UL << 0)          /**< \brief DMA传输使能（通道） */

/** @} */

/**
 * \brief 通道配置设定
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_TPM_CH(n)
 * \param[in] flags    : AMHW_FSL_TPM_CHSC_* 宏值或多个
 *                       AMHW_FSL_TPM_CHSC_*宏的 或（OR）值(#AMHW_FSL_TPM_CHSC_DMA_EN)
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_ch_sc_set (amhw_fsl_tpm_t *p_hw_tpm,
                              uint8_t          chan,
                              uint32_t         flags)
{
    p_hw_tpm->chan_ctl[chan].stat_ctl |= flags & AMHW_FSL_TPM_CnSC_MASK;
}

/**
 * \brief 通道配置清零
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_TPM_CH(n)
 * \param[in] flags    : AMHW_FSL_TPM_CHSC_* 宏值或多个
 *                       AMHW_FSL_TPM_CHSC_*宏的 或（OR）值(#AMHW_FSL_TPM_CHSC_DMA_EN)
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_ch_sc_clear (amhw_fsl_tpm_t *p_hw_tpm,
                                uint8_t          chan,
                                uint32_t         flags)
{
    p_hw_tpm->chan_ctl[chan].stat_ctl &= ~(flags & AMHW_FSL_TPM_CnSC_MASK);
}

/**
 * \brief 通道模式设置
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_TPM_CH(n)
 * \param[in] cnt_mode : 计数模式，向下计数或者向上-向下计数
 * \param[in] mode     : 输入捕获、输出比较模式、PWM等模式配置，参考配置表格
 *
 * \return 无
 * \note 参考配置表格
 */
am_static_inline
void amhw_fsl_tpm_ch_mode (amhw_fsl_tpm_t              *p_hw_tpm,
                            uint8_t                       chan,
                            amhw_fsl_tpm_counting_mode_t cnt_mode,
                            uint32_t                      mode)
{
    if ((uint8_t)cnt_mode) {
        amhw_fsl_tpm_ctrl_set(p_hw_tpm, AMHW_FSL_TPM_SC_CPWM_MODE);
    } else {
        amhw_fsl_tpm_ctrl_clear(p_hw_tpm, AMHW_FSL_TPM_SC_CPWM_MODE);
    }
    amhw_fsl_tpm_ch_sc_clear(p_hw_tpm, chan, AMHW_FSL_TPM_CHSC_MODE_MASK);
    amhw_fsl_tpm_ch_sc_set(p_hw_tpm, chan, mode);
}

/**
 * \brief 通道状态，判断通道是否发生事件
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_TPM_CH(n)
 *
 * \retval  AM_TRUE  : 通道事件已发生
 * \retval  AM_FALSE : 通道事件未发生
 */
am_static_inline
am_bool_t amhw_fsl_tpm_ch_even_flag_chk (amhw_fsl_tpm_t *p_hw_tpm, uint8_t chan)
{
    return (am_bool_t)(p_hw_tpm->chan_ctl[chan].stat_ctl & AMHW_FSL_TPM_CHSC_FLAG);
}

/**
 * \name 通道计数寄存器定义(R/W)
 * @{
 */

#define AMHW_FSL_TPM_CH_VALUE_MASK       (0xFFFFu)         /**< \brief 计数器值掩码 */

/** @} */

/**
 * \brief 通道计数器设定
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_FSL_TPM_CH(n)
 * \param[in] value    : 计数值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_ch_value_set (amhw_fsl_tpm_t *p_hw_tpm,
                                 uint8_t          chan,
                                 uint16_t         value)
{
    p_hw_tpm->chan_ctl[chan].value = value;
}

/**
 * \brief 通道计数器获取
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] chan     : 通道ID，使用宏#AMHW_MRT_CH(n)
 *
 * \return 通道计数值
 */
am_static_inline
uint16_t amhw_fsl_tpm_ch_value_get (amhw_fsl_tpm_t *p_hw_tpm,
                                     uint8_t          chan)
{
    return p_hw_tpm->chan_ctl[chan].value;
}

/**
 * \name 状态寄存器定义（写1清零）
 * @{
 */

#define AMHW_FSL_TPM_STATUS            (0x13Fu)        /**< \brief 捕获比较状态掩码 */
#define AMHW_FSL_TPM_STAT_OV_FLAG      (1UL << 8)      /**< \brief 溢出标志 */
#define AMHW_FSL_TPM_STAT_CH5_FLAG     (1UL << 5)      /**< \brief CH5通道事件标志 */
#define AMHW_FSL_TPM_STAT_CH4_FLAG     (1UL << 4)      /**< \brief CH4通道事件标志 */
#define AMHW_FSL_TPM_STAT_CH3_FLAG     (1UL << 3)      /**< \brief CH3通道事件标志 */
#define AMHW_FSL_TPM_STAT_CH2_FLAG     (1UL << 2)      /**< \brief CH2通道事件标志 */
#define AMHW_FSL_TPM_STAT_CH1_FLAG     (1UL << 1)      /**< \brief CH1通道事件标志 */
#define AMHW_FSL_TPM_STAT_CH0_FLAG     (1UL << 0)      /**< \brief CH0通道事件标志 */

#define AMHW_FSL_TPM_STAT_FLAG(n)      (1UL << n)      /** \brief CHn通道事件标志 */

/** @} */

/**
 * \brief TPM状态标志获取
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \return TPM状态标志,参见 AMHW_FSL_TPM_STAT_* 宏定义
 */
am_static_inline
uint32_t amhw_fsl_tpm_stat_flag_get (amhw_fsl_tpm_t *p_hw_tpm)
{
    return p_hw_tpm->status;
}

/**
 * \brief TPM状态标志清除
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] flags    : AMHW_FSL_TPM_STAT_* 宏值或多个
 *                       AMHW_FSL_TPM_STAT_*宏的 或（OR）值(#AMHW_FSL_TPM_STAT_CH0_FLAG)
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_stat_flag_clear (amhw_fsl_tpm_t *p_hw_tpm, uint32_t flags)
{
    p_hw_tpm->status = flags;
}

/**
 * \name 配置寄存器定义（R/W）
 * @{
 */

#define AMHW_FSL_TPM_CFG_MASK       (0xF0703E0u)         /**< \brief 捕获比较状态掩码 */
#define AMHW_FSL_TPM_CFG_TRG_SEL(s) (((s) & 0xf) << 24)  /**< \brief 触发选择 */
#define AMHW_FSL_TPM_CFG_CROT       (1UL << 18)          /**< \brief 触发时计数器重载 */
#define AMHW_FSL_TPM_CFG_CSOO       (1UL << 17)          /**< \brief 溢出时计数器停止 */
#define AMHW_FSL_TPM_CFG_CSOT       (1UL << 16)          /**< \brief 触发时计数器开始 */
#define AMHW_FSL_TPM_CFG_GTBEEN     (1UL << 9)           /**< \brief 全局定时器使能 */
#define AMHW_FSL_TPM_CFG_DGBMODE(s) (((s) & 0x3) << 6)   /**< \brief 调试模式 */
#define AMHW_FSL_TPM_CFG_DOZEEN     (1UL << 5)           /**< \brief 睡眠模式下计数器停止 */

/** @} */

/**
 * \brief TPM 触发源选择
 */
typedef enum amhw_fsl_tpm_trigger_src {
    AMHW_FSL_TPM_TRG_SRC_EXT_PIN     = 0x0,         /**< \brief 外部引脚输入 */
    AMHW_FSL_TPM_TRG_SRC_CMP0_OUT    = 0x1,         /**< \brief CMP0 输出 */
    AMHW_FSL_TPM_TRG_SRC_PIT_TRG0    = 0x4,         /**< \brief PIT0 */
    AMHW_FSL_TPM_TRG_SRC_PIT_TRG1    = 0x5,         /**< \brief PIT1 */
    AMHW_FSL_TPM_TRG_SRC_TPM0_OV     = 0x8,         /**< \brief TPM0 溢出 */
    AMHW_FSL_TPM_TRG_SRC_TPM1_OV     = 0x9,         /**< \brief TPM1 溢出 */
    AMHW_FSL_TPM_TRG_SRC_TPM2_OV     = 0xA,         /**< \brief TPM2 溢出 */
    AMHW_FSL_TPM_TRG_SRC_RTC_ALARM   = 0xC,         /**< \brief RTC 警报 */
    AMHW_FSL_TPM_TRG_SRC_RTC_SEC     = 0xD,         /**< \brief RTC 秒 */
    AMHW_FSL_TPM_TRG_SRC_LPTMR_TRG   = 0xE,         /**< \brief LPTMR */
} amhw_fsl_tpm_trigger_src_t;

/**
 * \brief TPM调试模式
 */
typedef enum amhw_fsl_tpm_dbg_mode {
    AMHW_FSL_TPM_DBG_PAUSED    = 0x0,      /**< \brief 调试模式下暂停，触发源和输入捕获被忽略 */
    AMHW_FSL_TPM_DBG_CONTINUES = 0x3,      /**< \brief 调试模式下继续运行 */
} amhw_fsl_tpm_dbg_mode_t;

/**
 * \brief TPM配置设定
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] flags    : AMHW_FSL_TPM_CFG_* 宏值或多个
 *                       AMHW_FSL_TPM_CFG_*宏的 或（OR）值(#AMHW_FSL_TPM_CFG_DOZEEN)
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_cfg_set (amhw_fsl_tpm_t *p_hw_tpm, uint32_t flags)
{
    p_hw_tpm->status |= flags & AMHW_FSL_TPM_CFG_MASK;
}

/**
 * \brief TPM配置设定
 *
 * \param[in] p_hw_tpm : 指向TPM寄存器块的指针
 * \param[in] flags    : AMHW_FSL_TPM_CFG_* 宏值或多个
 *                       AMHW_FSL_TPM_CFG_*宏的 或（OR）值(#AMHW_FSL_TPM_CFG_DOZEEN)
 * \return 无
 */
am_static_inline
void amhw_fsl_tpm_cfg_clear (amhw_fsl_tpm_t *p_hw_tpm, uint32_t flags)
{
    p_hw_tpm->status &= ~(flags & AMHW_FSL_TPM_CFG_MASK);
}


/* 使用匿名联合体段结束 */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)

  /* 允许匿名联合体使能 */
#elif defined(__GNUC__)

  /* 默认使用匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

/**
 * @} amhw_fsl_if_tpm
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_TPM_H */

/* end of file */

