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
 * \brief 定时器TIM接口
 *
 * \internal
 * \par History
 * - 1.01 20-04-23 zcb, Modify the CCR register declaration and
 *                      read and write according to the channel number
 * - 1.00 19-09-10  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32_TIM_H
#define __AMHW_HC32_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_hc32_if_tim
 * \copydoc amhw_hc32_tim.h
 * @{
 */

/**
  * \brief 定时器寄存器块结构体
  */
typedef struct amhw_hc32_tim {
    __IO uint32_t arr;            /**< \brief Timer重载寄存器/周期 */
    __IO uint32_t cnt;            /**< \brief Timer16位模式计数寄存器 */
    __IO uint32_t cnt32;          /**< \brief Timer32位模式计数寄存器 */

         union mxcr {
             __IO uint32_t m0cr;  /**< \brief Timer模式0控制寄存器 */
             __IO uint32_t m1cr;  /**< \brief Timer模式1控制寄存器 */
             __IO uint32_t m23cr; /**< \brief Timer模式23控制寄存器 */
         }mxcr_t;                 /**< \brief Timer模式0控制寄存器（按不同模式进行描述）*/

    __IO uint32_t ifr;            /**< \brief Timer中断标志 */
    __IO uint32_t iclr;           /**< \brief Timer中断清除寄存器 */
    __IO uint32_t mscr;           /**< \brief 主从模式控制 */
    __IO uint32_t fltr;           /**< \brief 滤波控制 */
    __IO uint32_t adtr;           /**< \brief ADC触发控制 */
    __IO uint32_t crchx[3];       /**< \brief 比较单元0、1、2控制寄存器 */
    __IO uint32_t dtr;            /**< \brief 死区寄存器 */
    __IO uint32_t rcr;            /**< \brief 重复计数寄存器 */
    __IO uint32_t arrdm;          /**< \brief Timer重载寄存器/周期映射地址 */

    __IO uint32_t ccr[6];         /**< \brief 比较捕获寄存器 */
} amhw_hc32_tim_t;

/**
 * \brief 定时器类型
 */
typedef enum amhw_hc32_tim_type {
    AMHW_HC32_TIM_TYPE_TIM0 = 0,  /**< \brief TIM0 */
    AMHW_HC32_TIM_TYPE_TIM1,      /**< \brief TIM1 */
    AMHW_HC32_TIM_TYPE_TIM2,      /**< \brief TIM2 */
    AMHW_HC32_TIM_TYPE_TIM3,      /**< \brief TIM3 */
} amhw_hc32_tim_type_t;

/**
 * \brief 门控开启控制
 */
typedef enum amhw_hc32_tim_gate {
    AMHW_HC32_TIM_GATE_DISABLE = 0,
    AMHW_HC32_TIM_GATE_ENABLE,
} amhw_hc32_tim_gate_t;

/*
 * 定时器通道号定义
 *
 * 定时器012只有2个通道，分别是CH0A和CH0B(在其他地方描述可能为通道0、1)
 * 定时器3共有6个通道，分别是CH0A和CH0B、CH1A和CH1B、CH2A和CH2B
 *
 * CHxA、CHxB在某些情况下作为一组相关联的通道
 */
#define HC32_TIM_CH0A    0
#define HC32_TIM_CH0B    1
#define HC32_TIM_CH1A    2
#define HC32_TIM_CH1B    3
#define HC32_TIM_CH2A    4
#define HC32_TIM_CH2B    5

/**
 * \brief ARR-16位模式重载寄存器         得到当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return ARR重载寄存器的值
 */
am_static_inline
uint16_t amhw_hc32_tim_arr_count_get (amhw_hc32_tim_t *p_hw_tim)
{
    return p_hw_tim->arr;
}

/**
 * \brief ARR-16位模式重载寄存器       设置当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] value    : 设置的计数器的值
 *
 *\ return 无
 */
am_static_inline
void amhw_hc32_tim_arr_count_set (amhw_hc32_tim_t *p_hw_tim, uint16_t value)
{
    p_hw_tim->arr = value;
}

/**
 * \brief CNT-16位模式计数寄存器         得到当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return CNT计数寄存器的值
 */
am_static_inline
uint16_t amhw_hc32_tim_cnt16_count_get (amhw_hc32_tim_t *p_hw_tim)
{
    return p_hw_tim->cnt;
}

/**
 * \brief CNT-16位模式计数寄存器       设置当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] value    : 设置的计数器的值
 *
 *\ return 无
 */
am_static_inline
void amhw_hc32_tim_cnt16_count_set (amhw_hc32_tim_t *p_hw_tim, uint16_t value)
{
    p_hw_tim->cnt = value;
}

/**
 * \brief CNT32-32位模式计数寄存器         得到当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 仅在模式0 32位定时器自由计数模式下有效，其他模式禁止写该寄存器
 *
 * \return CNT32计数寄存器的值
 */
am_static_inline
uint32_t amhw_hc32_tim_cnt32_count_get (amhw_hc32_tim_t *p_hw_tim)
{
    return p_hw_tim->cnt32;
}

/**
 * \brief CNT32-32位模式计数寄存器       设置当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] value    : 设置的计数器的值
 *
 * \note 仅在模式0 32位定时器自由计数模式下有效，其他模式禁止写该寄存器
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_cnt32_count_set (amhw_hc32_tim_t *p_hw_tim, uint32_t value)
{
    p_hw_tim->cnt32 = value;
}

/**
 * \brief 定时器模式枚举
 */
typedef enum {
    AMHW_HC32_TIM_MODE_TIMING = 0,        /* 定时器模式 */
    AMHW_HC32_TIM_MODE_PWC,               /* PWC模式 */
    AMHW_HC32_TIM_MODE_ZIGZAG,            /* 锯齿波模式 */
    AMHW_HC32_TIM_MODE_TRIANGULAR,        /* 三角波模式 */
}amhw_hc32_tim_mode_t;

/**
 * \brief MODE 工作模式  获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 定时器不同模式的选择控制位固定，再根据联合体结构的特性，只对某一寄存器操作即可。
 *
 * \return 设置的工作模式（amhw_hc32_tim_mode_t枚举量）
 */
am_static_inline
amhw_hc32_tim_mode_t amhw_hc32_tim_mode_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_mode_t)((p_hw_tim->mxcr_t.m0cr >> 12) & 0x3ul);
}

/**
 * \brief CMODE 工作模式  设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] mode     : 设置的工作模式（amhw_hc32_tim_mode_t枚举量）
 *
 * \note 定时器不同模式的选择控制位固定，再根据联合体结构的特性，只对某一寄存器操作即可。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode_set (amhw_hc32_tim_t     *p_hw_tim,
                               amhw_hc32_tim_mode_t mode)
{
    p_hw_tim->mxcr_t.m0cr = (p_hw_tim->mxcr_t.m0cr & (~(0x3ul << 12))) |
                            ((mode & 0x3ul) << 12);
}

/**
 * \brief CTEN 定时器使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 定时器不同模式的选择控制位固定，再根据联合体结构的特性，只对某一寄存器操作即可。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_disable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr &= ~(0x1ul << 0);
}

/**
 * \brief CTEN 定时器使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 定时器不同模式的选择控制位固定，再根据联合体结构的特性，只对某一寄存器操作即可。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_enable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr |= (0x1ul << 0);
}

/**
 * \brief 内部时钟分频选择
 */
typedef enum {
    AMHW_HC32_TIM_CLK_DIV1 = 0,    /* 1分频 */
    AMHW_HC32_TIM_CLK_DIV2,        /* 2分频 */
    AMHW_HC32_TIM_CLK_DIV4,        /* 4分频 */
    AMHW_HC32_TIM_CLK_DIV8,        /* 8分频 */
    AMHW_HC32_TIM_CLK_DIV16,       /* 16分频 */
    AMHW_HC32_TIM_CLK_DIV32,       /* 32分频 */
    AMHW_HC32_TIM_CLK_DIV64,       /* 64分频 */
    AMHW_HC32_TIM_CLK_DIV256,      /* 256分频 */
}amhw_hc32_tim_clkdiv_t;

/**
 * \brief PRS 内部时钟分频选择   获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 设置的分频系数（amhw_hc32_tim_clkdiv_t枚举量）
 */
am_static_inline
amhw_hc32_tim_clkdiv_t
    amhw_hc32_tim_mode_clkdiv_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_clkdiv_t)((p_hw_tim->mxcr_t.m0cr >> 4) & 0x7ul);
}

/**
 * \brief PRS 内部时钟分频选择   设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] clkdiv   : 设置的分频系数（amhw_hc32_tim_clkdiv_t枚举量）
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode_clkdiv_set (amhw_hc32_tim_t       *p_hw_tim,
                                      amhw_hc32_tim_clkdiv_t clkdiv)
{
    p_hw_tim->mxcr_t.m0cr = (p_hw_tim->mxcr_t.m0cr & (~(0x7ul << 4))) |
                            ((clkdiv & 0x7ul) << 4);
}

/**
 * \brief 计数时钟选择
 */
typedef enum {
    AMHW_HC32_TIM_CLK_SRC_TCLK = 0,   /* 内部计数时钟TCLK */
    AMHW_HC32_TIM_CLK_SRC_ETR,        /* 外部计数时钟ETR */
}amhw_hc32_tim_clk_src_t;
/**
 * \brief CT 计数时钟选择   获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 设置的时钟源（amhw_hc32_tim_clk_src_t枚举量）
 */
am_static_inline
amhw_hc32_tim_clk_src_t
    amhw_hc32_tim_mode_clk_src_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_clk_src_t)((p_hw_tim->mxcr_t.m0cr >> 2) & 0x1ul);
}

/**
 * \brief CT 计数时钟选择   设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] src      : 设置的时钟源（amhw_hc32_tim_clk_src_t枚举量）
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode_clk_src_set(amhw_hc32_tim_t        *p_hw_tim,
                                      amhw_hc32_tim_clk_src_t src)
{
    p_hw_tim->mxcr_t.m0cr = (p_hw_tim->mxcr_t.m0cr & (~(0x1ul << 2))) |
                            ((src & 0x1ul) << 2);
}

/******************************************************************************/
/***************************Timer模式0控制寄存器     相关控制函数**************************/
/******************************************************************************/

/**
 * \brief UIE 中断禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_int_update_disable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr &= ~(0x1ul << 10);
}

/**
 * \brief UIE 中断使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_int_update_enable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr |= (0x1ul << 10);
}

/**
 * \brief GATEP 端口GATE极性控制   高电平
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_gate_high (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr &= ~(0x1ul << 9);
}

/**
 * \brief GATEP 端口GATE极性控制   低电平
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_gate_low (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr |= (0x1ul << 9);
}

/**
 * \brief GATE 定时器门控禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_gate_disable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr &= ~(0x1ul << 8);
}

/**
 * \brief GATE 定时器门控使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_gate_enable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr |= (0x1ul << 8);
}

/**
 * \brief TOGEN 模式0下翻转输出禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 1：翻转输出使能
 *       0：翻转输出关闭CHA,CHB输出为低电平
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_toggle_disable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr &= ~(0x1ul << 3);
}

/**
 * \brief TOGEN 模式0下翻转输出使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 1：翻转输出使能
 *       0：翻转输出关闭CHA,CHB输出为低电平
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_toggle_enable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m0cr |= (0x1ul << 3);
}

/**
 * \brief 模式选择32定时/16定时模式选择
 *
 * \note 32位自由计数  ： 0 - 0xffffffff计数范围，向上计数，仅存在模式0中，不可设定重载值。
 *       16位重载计数  ： 重载设定值 - 0xffff 计数范围。可设定重载值。
 *
 *       注意：如果使用32位自由计数，计数周期固定为 0 - 0xffffffff ， 不可更改。
 *
 */
typedef enum {
    AMHW_HC32_TIM_MODE0_COUNTER_32 = 0,   /* 32位自由计数  */
    AMHW_HC32_TIM_MODE0_COUNTER_16,       /* 16位重载计数 */
}amhw_hc32_tim_mode0_counter_t;
/**
 * \brief MD 模式选择32定时/16定时模式选择   获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 设置的32定时/16定时模式（amhw_hc32_tim_mode0_counter_t枚举量）
 */
am_static_inline
amhw_hc32_tim_mode0_counter_t amhw_hc32_tim_mode0_counter_get (amhw_hc32_tim_t  *p_hw_tim)
{
    return (amhw_hc32_tim_mode0_counter_t)((p_hw_tim->mxcr_t.m0cr >> 1) & 0x1ul);
}

/**
 * \brief MD 模式选择32定时/16定时模式选择   设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] src      : 设置的32定时/16定时模式（amhw_hc32_tim_mode0_counter_t枚举量）
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_counter_set(amhw_hc32_tim_t              *p_hw_tim,
                                       amhw_hc32_tim_mode0_counter_t src)
{
    p_hw_tim->mxcr_t.m0cr = (p_hw_tim->mxcr_t.m0cr & (~(0x1ul << 1))) |
                            ((src & 0x1ul) << 1);
}

/**
 * \brief UIF 溢出中断 标志获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return AM_TRUE  ： 标志有效
 *         AM_FALSE ： 标志无效
 */
am_static_inline
am_bool_t amhw_hc32_tim_mode0_int_update_flag_check(amhw_hc32_tim_t *p_hw_tim)
{
    return (p_hw_tim->ifr & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief UIF 溢出中断 标志清除
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_int_update_flag_clr(amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->iclr &= ~(0x1ul << 0);
}

/**
 * \brief MOE 翻转输出
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：翻转端口为输入状态
 *       1：翻转端口为输出状态
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_toggle_output (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->dtr |= (0x1ul << 12);
}

/**
 * \brief MOE 翻转输入
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：翻转端口为输入状态
 *       1：翻转端口为输出状态
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode0_toggle_input (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->dtr |= (0x1ul << 12);
}


/******************************************************************************/
/***************************Timer模式1控制寄存器     相关控制函数**************************/
/******************************************************************************/
/**
 * \brief Oneshot 单次触发模式选择   单次触发
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 1：完成一次脉冲测量自动结束， 再次测量需要重新使能CTEN
 *       0：循环测量
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_trigger_single (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m1cr |= (0x1ul << 14);
}

/**
 * \brief Oneshot 单次触发模式选择  循环触发
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 1：完成一次脉冲测量自动结束， 再次测量需要重新使能CTEN
 *       0：循环测量
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_trigger_cyclic (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m1cr &= ~(0x1ul << 14);
}

/**
 * \brief UIE 溢出中断禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_int_update_disable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m1cr &= ~(0x1ul << 10);
}

/**
 * \brief UIE 溢出中断使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_int_update_enable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m1cr |= (0x1ul << 10);
}

/**
 * \brief CIEA 脉冲宽度测量完成中断禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_int_edge_finish_disable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->crchx[0] &= ~(0x1ul << 10);
}

/**
 * \brief CIEA 脉冲宽度测量完成中断使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_int_edge_finish_enable (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->crchx[0] |= (0x1ul << 10);
}

/**
 * \brief 脉冲宽度测量边沿选择
 */
typedef enum {
    AMHW_HC32_TIM_PULSE_CYCLE_RISING,    /* 上沿-上沿周期 */
    AMHW_HC32_TIM_PULSE_WIDTH_LOW,       /* 低电平宽度 */
    AMHW_HC32_TIM_PULSE_WIDTH_HIGH,      /* 高电平宽度 */
    AMHW_HC32_TIM_PULSE_CYCLE_FALLING,   /* 下沿-下沿周期 */
}amhw_hc32_tim_measure_edge_t;
/**
 * \brief Edg2nd Edg1st 脉冲宽度测量边沿选择  获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 脉冲宽度测量边沿选择(amhw_hc32_tim_measure_edge_t枚举量)
 */
am_static_inline
amhw_hc32_tim_measure_edge_t
    amhw_hc32_tim_mode1_measure_edge_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_measure_edge_t)((p_hw_tim->mxcr_t.m1cr >> 8) & 0x3ul);
}

/**
 * \brief Edg2nd Edg1st 脉冲宽度测量边沿选择  设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] choice   : 脉冲宽度测量边沿选择(amhw_hc32_tim_measure_edge_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_measure_edge_set(
    amhw_hc32_tim_t             *p_hw_tim,
    amhw_hc32_tim_measure_edge_t choice)
{
    p_hw_tim->mxcr_t.m1cr =(p_hw_tim->mxcr_t.m1cr & (~(0x3ul << 8))) |
                           ((choice & 0x3ul) << 8);
}

/**
 * \brief UIF 溢出中断 标志获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return AM_TRUE  ： 标志有效
 *         AM_FALSE ： 标志无效
 */
am_static_inline
am_bool_t amhw_hc32_tim_mode1_int_update_flag_check(
    amhw_hc32_tim_t *p_hw_tim)
{
    return (p_hw_tim->ifr & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief UIF 溢出中断 标志清除
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_int_update_flag_clr(amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->iclr &= ~(0x1ul << 0);
}

/**
 * \brief UIF 脉冲宽度测量中断   标志获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return AM_TRUE  ： 标志有效
 *         AM_FALSE ： 标志无效
 */
am_static_inline
am_bool_t amhw_hc32_tim_mode1_int_edge_flag_check(amhw_hc32_tim_t *p_hw_tim)
{
    return ((p_hw_tim->ifr >> 2) & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief UIF 脉冲宽度测量中断   标志清除
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_int_edge_flag_clr (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->iclr &= ~(0x1ul << 2);
}

/**
 * \brief CCR0A 脉冲宽度测量结果
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return  脉冲宽度测量结果
 */
am_static_inline
uint16_t amhw_hc32_tim_mode1_measure_data_get (amhw_hc32_tim_t *p_hw_tim)
{
    return p_hw_tim->ccr[0] & 0xfffful;
}

/**
 * \brief IB0S CH0B输入选择      CH0B
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：CH0B
 *       1：内部触发TS选择信号
 *
 * \note 当PWM互补输出时自动选择GATE端口作为CH0B的输入
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_ch0b_in_ch0b (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr &= ~(0x1ul << 12);
}

/**
 * \brief IB0S CH0B输入选择     内部触发TS选择信号
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：CH0B
 *       1：内部触发TS选择信号
 *
 * \note 当PWM互补输出时自动选择GATE端口作为CH0B的输入
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_ch0b_in_ts (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr |= (0x1ul << 12);
}

/**
 * \brief IA0S  CH0A输入选择      CH0A
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：CH0A
 *       1：CH0A CH1A CH2A XOR(TIM3)    、          CH0A ETR GATE XOR(TIM0)
 *
 * \note 设置为1后端口的任意一个端口变化将导致输入变化
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_ch0a_in_ch0a (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr &= ~(0x1ul << 11);
}

/**
 * \brief IA0S  CH0A输入选择     内部触发TS选择信号
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：CH0A
 *       1：CH0A CH1A CH2A XOR(TIM3)    、          CH0A ETR GATE XOR(TIM0)
 *
 * \note 设置为1后端口的任意一个端口变化将导致输入变化
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_ch0a_in_other (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr |= (0x1ul << 11);
}

/**
 * \brief 触发选择
 */
typedef enum {
    AMHW_HC32_TIM_TS_SRC_ETR_ETFP = 0,    /* 端口ETR的滤波相位选择后的信号ETFP */
    AMHW_HC32_TIM_TS_SRC_ITR0,            /* 内部互联信号 ITR0 */
    AMHW_HC32_TIM_TS_SRC_ITR1,            /* 内部互联信号 ITR1 */
    AMHW_HC32_TIM_TS_SRC_ITR2,            /* 内部互联信号 ITR2 */
    AMHW_HC32_TIM_TS_SRC_ITR3,            /* 内部互联信号 ITR3 */
    AMHW_HC32_TIM_TS_SRC_CH0A_EDGE,       /* 端口CH0A的边沿信号 */
    AMHW_HC32_TIM_TS_SRC_CH0A_IAFP,       /* 端口CH0A的滤波相位选择后的信号IAFP */
    AMHW_HC32_TIM_TS_SRC_CH0B_IBFP,       /* 端口CH0B的滤波相位选择后的信号IBFP */
}amhw_hc32_tim_ts_src_t;
/**
 * \brief TS 触发选择  获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 触发选择(amhw_hc32_tim_ts_src_t枚举量)
 */
am_static_inline
amhw_hc32_tim_ts_src_t
    amhw_hc32_tim_mode1_ts_src_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_ts_src_t)((p_hw_tim->mscr >> 5) & 0x7ul);
}

/**
 * \brief TS 触发选择  获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] choice   : 触发选择(amhw_hc32_tim_ts_src_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_ts_src_set(amhw_hc32_tim_t       *p_hw_tim,
                                      amhw_hc32_tim_ts_src_t choice)
{
    p_hw_tim->mscr =(p_hw_tim->mscr & (~(0x7ul << 5))) |
                    ((choice & 0x7ul) << 5);
}

/**
 * \brief ETP  ETR输入相位选择      同相位
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_etr_in_phase_same (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->fltr &= ~(0x1ul << 31);
}

/**
 * \brief ETP  ETR输入相位选择         反向输入
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_etr_in_phase_reversed (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->fltr |= (0x1ul << 31);
}

/**
 * \brief 滤波控制（捕获）
 */
typedef enum {
    AMHW_HC32_TIM_FILTER_NO = 0,          /* 滤波无效 */
    AMHW_HC32_TIM_FILTER_PCLKDIV1_3 = 4,  /* pclk 3个连续有效 */
    AMHW_HC32_TIM_FILTER_PCLKDIV4_3,      /* pclk/4 3个连续有效 */
    AMHW_HC32_TIM_FILTER_PCLKDIV16_3,     /* pclk/16 3个连续有效 */
    AMHW_HC32_TIM_FILTER_PCLKDIV64_3,     /* pclk/64 3个连续有效 */
}amhw_hc32_tim_filter_t;

/**
 * \brief FLTET  ETR滤波控制    获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 滤波控制(amhw_hc32_tim_filter_t枚举量)
 */
am_static_inline
amhw_hc32_tim_filter_t
    amhw_hc32_tim_mode1_etr_fliter_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_filter_t)(amhw_hc32_tim_filter_t)((p_hw_tim->fltr >> 28) & 0x7ul);
}

/**
 * \brief FLTET  ETR滤波控制     设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] choice   : 滤波控制(amhw_hc32_tim_filter_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_etr_fliter_set(amhw_hc32_tim_t       *p_hw_tim,
                                          amhw_hc32_tim_filter_t choice)
{
    p_hw_tim->fltr =(p_hw_tim->fltr & (~(0x7ul << 28))) |
                    ((choice & 0x7ul) << 28);
}

/**
 * \brief FLTB0  CHB滤波控制    获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 滤波控制(amhw_hc32_tim_filter_t枚举量)
 */
am_static_inline
amhw_hc32_tim_filter_t
    amhw_hc32_tim_mode1_chb_fliter_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_filter_t)((p_hw_tim->fltr >> 4) & 0x7ul);
}

/**
 * \brief FLTB0  CHB滤波控制     设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] choice   : 滤波控制(amhw_hc32_tim_filter_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_chb_fliter_set(amhw_hc32_tim_t       *p_hw_tim,
                                          amhw_hc32_tim_filter_t choice)
{
    p_hw_tim->fltr =(p_hw_tim->fltr & (~(0x7ul << 4))) |
                    ((choice & 0x7ul) << 4);
}

/**
 * \brief FLTA0  CHA滤波控制    获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 滤波控制(amhw_hc32_tim_filter_t枚举量)
 */
am_static_inline
amhw_hc32_tim_filter_t
    amhw_hc32_tim_mode1_cha_fliter_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_filter_t)((p_hw_tim->fltr >> 0) & 0x7ul);
}

/**
 * \brief FLTA0  CHA滤波控制    获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] choice   : 滤波控制(amhw_hc32_tim_filter_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode1_cha_fliter_set(amhw_hc32_tim_t       *p_hw_tim,
                                          amhw_hc32_tim_filter_t choice)
{
    p_hw_tim->fltr =(p_hw_tim->fltr & (~(0x7ul << 0))) |
                    ((choice & 0x7ul) << 0);
}




/******************************************************************************/
/***************************Timer模式23控制寄存器     相关控制函数*************************/
/******************************************************************************/
/**
 * \brief DIR  向上计数
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 计数方向，只有在锯齿波模式下可以写。其他模式下只读，写无效.
 *       从其他模式切换到中心对齐模式DIR自动清0。软件事件更新和从模式外部触发复位模式DIR自动清零。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_counter_up (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr &= ~(0x1ul << 27);
}

/**
 * \brief DIR  向下计数
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 计数方向，只有在锯齿波模式下可以写。其他模式下只读，写无效.
 *       从其他模式切换到中心对齐模式DIR自动清0。软件事件更新和从模式外部触发复位模式DIR自动清零。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_counter_down (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1ul << 27);
}

/**
 * \brief BG  软件刹车
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 自动清零，写0无效
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_soft_brake (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1ul << 26);
}

/**
 * \brief UG  软件更新
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 自动清零，写0无效
 *       初始化计数器并更新缓存寄存器到相应寄存器（缓存使能），预除频计数器也会被清零
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_soft_update (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1ul << 25);
}

/**
 * \brief TG  软件触发
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 自动清零，写0无效
 *       需要在触发模式SMS=2且mode=2/3下都可以触发。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_soft_trigger (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1ul << 24);
}

/**
 * \brief 中心对齐A比较中断模式
 */
typedef enum {
    AMHW_HC32_TIM_CISA_INT_MODE_NO = 0,  /* 无中断 */
    AMHW_HC32_TIM_CISA_INT_MODE_RISING,  /* 上沿中断 */
    AMHW_HC32_TIM_CISA_INT_MODE_FALLING, /* 下沿中断 */
    AMHW_HC32_TIM_CISA_INT_MODE_BOTH,    /* 上下沿都中断 */
}amhw_hc32_tim_int_mode_t;

/**
 * \brief CIS 中心对齐A比较中断模式
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] mode     : amhw_hc32_tim_int_mode_t枚举量
 *
 * \note 中心对齐A比较中断模式(B比较中断单独控制在CRx寄存器中CISB)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_int_mode_set(amhw_hc32_tim_t         *p_hw_tim,
                                         amhw_hc32_tim_int_mode_t mode )
{
    p_hw_tim->mxcr_t.m23cr = (p_hw_tim->mxcr_t.m23cr & (~(0x3ul << 21))) |
                             ((mode & 0x3ul) << 21);
}

#define AMHW_HC32_TIM_INT_BIE     (0x1UL << 20)  /* 刹车中断使能 */
#define AMHW_HC32_TIM_INT_TIE     (0x1UL << 19)  /* 触发中断使能 */
#define AMHW_HC32_TIM_INT_UIE     (0x1UL << 10)  /* 更新中断使能 */
#define AMHW_HC32_TIM_INT_ALL     0xFF           /* 所有中断 */

/**
 * \brief 上述宏定义使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : AMHW_HC32_TIM_INT_BIE   或
 *                       AMHW_HC32_TIM_INT_TIE   或
 *                       AMHW_HC32_TIM_INT_UIE   或
 *                       AMHW_HC32_TIM_INT_ALL
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_int_enable (amhw_hc32_tim_t *p_hw_tim,
                                        uint32_t           type)
{
    if(0xFF == type) {
        p_hw_tim->mxcr_t.m23cr |= AMHW_HC32_TIM_INT_BIE;
        p_hw_tim->mxcr_t.m23cr |= AMHW_HC32_TIM_INT_TIE;
        p_hw_tim->mxcr_t.m23cr |= AMHW_HC32_TIM_INT_UIE;
    }
    else {
        p_hw_tim->mxcr_t.m23cr |= type;
    }
}

/**
 * \brief 上述宏定义禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : AMHW_HC32_TIM_INT_BIE   或
 *                       AMHW_HC32_TIM_INT_TIE   或
 *                       AMHW_HC32_TIM_INT_UIE   或
 *                       AMHW_HC32_TIM_INT_ALL
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_int_disable (amhw_hc32_tim_t  *p_hw_tim,
                                     uint32_t           type)
{
    if(0xFF == type) {
        p_hw_tim->mxcr_t.m23cr &= ~AMHW_HC32_TIM_INT_BIE;
        p_hw_tim->mxcr_t.m23cr &= ~AMHW_HC32_TIM_INT_TIE;
        p_hw_tim->mxcr_t.m23cr &= ~AMHW_HC32_TIM_INT_UIE;
    }
    else {
        p_hw_tim->mxcr_t.m23cr &= ~type;
    }
}

#define AMHW_HC32_TIM_OCCE    (0x1UL << 23)  /* OCREF清除使能使能 */
#define AMHW_HC32_TIM_TDE     (0x1UL << 18)  /* 触发DMA使能 */
#define AMHW_HC32_TIM_UDE     (0x1UL << 11)  /* 更新DMA使能 */
#define AMHW_HC32_TIM_BUFPEN  (0x1UL << 7)   /* 重载缓存使能 */
#define AMHW_HC32_TIM_PWM2S   (0x1UL << 3)   /* OCREFA单点比较使能 */
#define AMHW_HC32_TIM_COMP    (0x1UL << 1)   /* 互补PWM使能 */
#define AMHW_HC32_TIM_ALL     0XFF           /* 所有使能（定时器使能除外）*/

/**
 * \brief 上述宏定义使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : AMHW_HC32_TIM_OCCE  或
 *                       AMHW_HC32_TIM_TDE   或
 *                       AMHW_HC32_TIM_UDE   或
 *                       AMHW_HC32_TIM_BUFPEN   或
 *                       AMHW_HC32_TIM_PWM2S   或
 *                       AMHW_HC32_TIM_COMP   或
 *                       AMHW_HC32_TIM_ALL
 *
 *
 * \note 1： OCREF_CLR信号可以清除OCREF输出
 *       0： OCREF输出不受OCREF_CLR影响
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_enable (amhw_hc32_tim_t *p_hw_tim,
                                    uint32_t           type)
{
    if(0xFF == type) {
        p_hw_tim->mxcr_t.m23cr |= AMHW_HC32_TIM_OCCE;
        p_hw_tim->mxcr_t.m23cr |= AMHW_HC32_TIM_TDE;
        p_hw_tim->mxcr_t.m23cr |= AMHW_HC32_TIM_UDE;
    }
    else {
        p_hw_tim->mxcr_t.m23cr |= type;
    }
}

/**
 * \brief 上述宏定义禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : AMHW_HC32_TIM_OCCE  或
 *                       AMHW_HC32_TIM_TDE   或
 *                       AMHW_HC32_TIM_UDE   或
 *                       AMHW_HC32_TIM_BUFPEN   或
 *                       AMHW_HC32_TIM_PWM2S   或
 *                       AMHW_HC32_TIM_COMP   或
 *                       AMHW_HC32_TIM_ALL
 *
 * \note 1： OCREF_CLR信号可以清除OCREF输出
 *       0： OCREF输出不受OCREF_CLR影响
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_disable (amhw_hc32_tim_t  *p_hw_tim,
                                     uint32_t           type)
{
    if(0xFF == type) {
        p_hw_tim->mxcr_t.m23cr &= ~AMHW_HC32_TIM_OCCE;
        p_hw_tim->mxcr_t.m23cr &= ~AMHW_HC32_TIM_TDE;
        p_hw_tim->mxcr_t.m23cr &= ~AMHW_HC32_TIM_UDE;
        p_hw_tim->mxcr_t.m23cr &= ~AMHW_HC32_TIM_BUFPEN;
        p_hw_tim->mxcr_t.m23cr &= ~AMHW_HC32_TIM_PWM2S;
        p_hw_tim->mxcr_t.m23cr &= ~AMHW_HC32_TIM_COMP;
    }
    else {
        p_hw_tim->mxcr_t.m23cr &= ~type;
    }
}

/**
 * \brief URS 更新源    0
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：上溢出/下溢出/软件更新UG/从模式复位
 *       1：上溢出/下溢出
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_uodate_src_0 (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr &= ~(0x1UL << 17);
}

/**
 * \brief URS 更新源    1
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：上溢出/下溢出/软件更新UG/从模式复位
 *       1：上溢出/下溢出
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_uodate_src_1 (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1UL << 17);
}

/**
 * \brief OCCS  OCREF清除源选择    0
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：电压比较器VC输出， VC选择在VCx_OUTCFG寄存器设置
 *       1：ETR端口滤波相位选择后的信号
 *
 *       当OCCE有效时， OC_clr可以清除OCREF的比较输出信号为零，（当OCMx>1时有效） ，
 *       下一个uev事件后继续比较输出
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_occref_src_0 (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr &= ~(0x1UL << 16);
}

/**
 * \brief OCCS  OCREF清除源选择    1
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：上溢出/下溢出/软件更新UG/从模式复位
 *       1：上溢出/下溢出
 *
 *       当OCCE有效时， OC_clr可以清除OCREF的比较输出信号为零，（当OCMx>1时有效） ，
 *       下一个uev事件后继续比较输出
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_occref_src_1 (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1UL << 16);
}

/**
 * \brief CSG  GATE 在PWM互补模式下捕获/比较选择     捕获
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 只有在PWM互补输出时有效,使用CCR0B作为GATE的比较或捕获通道
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_gate_set_cap (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1UL << 15);
}

/**
 * \brief CSG  GATE 在PWM互补模式下捕获/比较选择     比较
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 只有在PWM互补输出时有效,使用CCR0B作为GATE的比较或捕获通道
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_gate_set_cmp (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr &= ~(0x1UL << 15);
}

/**
 * \brief CFG  GATE作为捕获输入时， 下沿捕获有效控制     获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 只有在PWM互补输出时有效
 *
 * \return AM_TRUE :下沿捕获有效
 *         AM_FALSE:下沿捕获无效
 */
am_static_inline
am_bool_t amhw_hc32_tim_mode23_cfg_get (amhw_hc32_tim_t  *p_hw_tim)
{
    return ((p_hw_tim->mxcr_t.m23cr >> 9) & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief CFG  GATE作为捕获输入时， 下沿捕获有效控制     有效
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 只有在PWM互补输出时有效
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cfg_enable (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1UL << 9);
}

/**
 * \brief CFG  GATE作为捕获输入时， 下沿捕获有效控制    无效
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 只有在PWM互补输出时有效
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cfg_disable (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr &= ~(0x1UL << 9);
}

/**
 * \brief CFG  GATE作为捕获输入时， 上沿捕获有效控制     获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 只有在PWM互补输出时有效
 *
 * \return AM_TRUE :上沿捕获有效
 *         AM_FALSE:上沿捕获无效
 */
am_static_inline
am_bool_t amhw_hc32_tim_mode23_crg_get (amhw_hc32_tim_t  *p_hw_tim)
{
    return ((p_hw_tim->mxcr_t.m23cr >> 8) & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief CFG  GATE作为捕获输入时， 上沿捕获有效控制     有效
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 只有在PWM互补输出时有效
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_crg_enable (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1UL << 8);
}

/**
 * \brief CFG  GATE作为捕获输入时， 上沿捕获有效控制    无效
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 只有在PWM互补输出时有效
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_crg_disable (amhw_hc32_tim_t  *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr &= ~(0x1UL << 8);
}

/**
 * \brief Oneshot 单次触发模式选择   单次触发
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 1：发生事件更新后定时器停止。
 *       0：循环计数
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_trigger_single (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr |= (0x1ul << 14);
}

/**
 * \brief Oneshot 单次触发模式选择  循环触发
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 1：发生事件更新后定时器停止。
 *       0：循环计数
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_trigger_cyclic (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mxcr_t.m23cr &= ~(0x1ul << 14);
}


#define AMHW_HC32_TIM_INT_FLAG_TRIGGER   15    /* 触发中断标志 */
#define AMHW_HC32_TIM_INT_FLAG_BRAKE     14    /* 刹车中断标志 */
#define AMHW_HC32_TIM_INT_FLAG_CB2E      13    /* 通道CB2E捕获数据丢失标志 */
#define AMHW_HC32_TIM_INT_FLAG_CB1E      12    /* 通道CB1E捕获数据丢失标志 */
#define AMHW_HC32_TIM_INT_FLAG_CB0E      11    /* 通道CB0E捕获数据丢失标志 */
#define AMHW_HC32_TIM_INT_FLAG_CA2E      10    /* 通道CA2E捕获数据丢失标志 */
#define AMHW_HC32_TIM_INT_FLAG_CA1E      9     /* 通道CA1E捕获数据丢失标志 */
#define AMHW_HC32_TIM_INT_FLAG_CA0E      8     /* 通道CA0E捕获数据丢失标志 */
#define AMHW_HC32_TIM_INT_FLAG_CB2F      7     /* 通道CH2B发生捕获/比较匹配标志 */
#define AMHW_HC32_TIM_INT_FLAG_CB1F      6     /* 通道CH1B发生捕获/比较匹配标志 */
#define AMHW_HC32_TIM_INT_FLAG_CB0F      5     /* 通道CH0B发生捕获/比较匹配标志 */
#define AMHW_HC32_TIM_INT_FLAG_CA2F      4     /* 通道CH2A发生捕获/比较匹配标志 */
#define AMHW_HC32_TIM_INT_FLAG_CA1F      3     /* 通道CH1A发生捕获/比较匹配标志 */
#define AMHW_HC32_TIM_INT_FLAG_CA0F      2     /* 通道CH0A发生捕获/比较匹配标志 */
#define AMHW_HC32_TIM_INT_FLAG_UPDATE    0     /* 事件更新中断标志 */
#define AMHW_HC32_TIM_INT_FLAG_ALL     0xffff
/**
 * \brief 模式23下  标志获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] flag     : 传入对应宏定义
 *                       AMHW_HC32_TIM_INT_FLAG_TRIGGER
 *                       .
 *                       .
 *                       .
 *                       AMHW_HC32_TIM_INT_FLAG_UPDATE
 *
 * \return AM_TRUE  ： 标志有效
 *         AM_FALSE ： 标志无效
 */
am_static_inline
am_bool_t amhw_hc32_tim_mode23_int_flag_check(amhw_hc32_tim_t *p_hw_tim,
                                                uint32_t           flag)
{
    return (p_hw_tim->ifr  & (0x1ul << flag)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 模式23下   标志清除
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] flag     : 传入对应宏定义
 *                       AMHW_HC32_TIM_INT_FLAG_TRIGGER
 *                       .
 *                       .
 *                       .
 *                       AMHW_HC32_TIM_INT_FLAG_UPDATE
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_int_flag_clr (amhw_hc32_tim_t *p_hw_tim,
                                          uint32_t           flag)
{
    if(flag == 0xffff) {
        p_hw_tim->iclr = 0;
    } else {
        p_hw_tim->iclr &= ~(0x1ul << flag);
    }
}

/**
 * \brief IB0S CH0B输入选择      CH0B
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：CH0B
 *       1：内部触发TS选择信号
 *
 * \note 当PWM互补输出时自动选择GATE端口作为CH0B的输入
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_ch0b_in_ch0b (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr &= ~(0x1ul << 12);
}

/**
 * \brief IB0S CH0B输入选择     内部触发TS选择信号
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：CH0B
 *       1：内部触发TS选择信号
 *
 * \note 当PWM互补输出时自动选择GATE端口作为CH0B的输入
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_ch0b_in_ts (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr |= (0x1ul << 12);
}

/**
 * \brief IA0S  CH0A输入选择      CH0A
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：CH0A
 *       1：CH0A CH1A CH2A XOR(TIM3)    、          CH0A ETR GATE XOR(TIM0)
 *
 * \note 设置为1后端口的任意一个端口变化将导致输入变化
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_ch0a_in_ch0a (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr &= ~(0x1ul << 11);
}

/**
 * \brief IA0S  CH0A输入选择     内部触发TS选择信号
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note 0：CH0A
 *       1：CH0A CH1A CH2A XOR(TIM3)    、          CH0A ETR GATE XOR(TIM0)
 *
 * \note 设置为1后端口的任意一个端口变化将导致输入变化
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_ch0a_in_other (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr |= (0x1ul << 11);
}

/**
 * \brief 从模式功能选择
 */
typedef enum {
    AMHW_HC32_TIM_SLAVER_MODE_INCLK = 0,  /* 使用内部时钟 */
    AMHW_HC32_TIM_SLAVER_MODE_RESET,      /* 复位功能 */
    AMHW_HC32_TIM_SLAVER_MODE_TRIGGER,    /* 触发模式 */
    AMHW_HC32_TIM_SLAVER_MODE_OUTCLK,     /* 外部时钟模式 */
    AMHW_HC32_TIM_SLAVER_MODE_COUNTER1,   /* 正交编码计数模式1 */
    AMHW_HC32_TIM_SLAVER_MODE_COUNTER2,   /* 正交编码计数模式2 */
    AMHW_HC32_TIM_SLAVER_MODE_COUNTER3,   /* 正交编码计数模式3 */
    AMHW_HC32_TIM_SLAVER_MODE_GATE,       /* 门控功能 */
}amhw_hc32_tim_slaver_mode_t;

/**
 * \brief SMS 从模式功能选择  获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 从模式功能选择(amhw_hc32_tim_slaver_mode_t枚举量)
 */
am_static_inline
amhw_hc32_tim_slaver_mode_t
    amhw_hc32_tim_mode23_slaver_mode_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_slaver_mode_t)((p_hw_tim->mscr >> 8) & 0x7ul);
}

/**
 * \brief SMS 从模式功能选择  设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] choice   : 从模式功能选择(amhw_hc32_tim_slaver_mode_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_slaver_mode_set(amhw_hc32_tim_t         *p_hw_tim,
                                            amhw_hc32_tim_slaver_mode_t choice)
{
    p_hw_tim->mscr =(p_hw_tim->mscr & (~(0x7ul << 8))) |
                    ((choice & 0x7ul) << 8);
}

/**
 * \brief TS 触发选择  获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 触发选择(amhw_hc32_tim_ts_src_t枚举量)
 */
am_static_inline
amhw_hc32_tim_ts_src_t
    amhw_hc32_tim_mode23_ts_src_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_ts_src_t)((p_hw_tim->mscr >> 5) & 0x7ul);
}

/**
 * \brief TS 触发选择 设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] choice   : 触发选择(amhw_hc32_tim_ts_src_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_ts_src_set(amhw_hc32_tim_t       *p_hw_tim,
                                      amhw_hc32_tim_ts_src_t choice)
{
    p_hw_tim->mscr =(p_hw_tim->mscr & (~(0x7ul << 5))) |
                    ((choice & 0x7ul) << 5);
}

/**
 * \brief MSM 主从选择  从
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note  0:无延时
 *        1:延时使能，使主送计数器同时启动
 *
 *        使用触发模式时，从模式设置为0，主模式设置为1，可以使主从计数同时启动
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_slaver_enable(amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr &= ~(0x1ul << 4);
}

/**
 * \brief MSM 主从选择  主
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note  0:无延时
 *        1:延时使能，使主送计数器同时启动
 *
 *        使用触发模式时，从模式设置为0，主模式设置为1，可以使主从计数同时启动
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_master_enable(amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr |= (0x1ul << 4);
}

/**
 * \brief CCDS 比较模式下DMA比较触发选择    比较匹配触发DMA
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note  0 ： 比较匹配触发DMA
 *        1 ： 比较匹配不触发DMA，事件更新代替比较匹配触发DMA
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cmp_dma(amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr &= ~(0x1ul << 3);
}

/**
 * \brief CCDS 比较模式下DMA比较触发选择   比较匹配不触发DMA，事件更新代替比较匹配触发DMA
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note  0 ： 比较匹配触发DMA
 *        1 ： 比较匹配不触发DMA，事件更新代替比较匹配触发DMA
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cmp_update(amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->mscr |= (0x1ul << 3);
}

/**
 * \brief 主模式输出选择，用于内部互联，连接到其他定时器的ITRx
 */
typedef enum {
    AMHW_HC32_TIM_MASTER_OUT_UG = 0,    /* 软件更新 */
    AMHW_HC32_TIM_MASTER_OUT_CTEN,      /* 定时器使能 */
    AMHW_HC32_TIM_MASTER_OUT_UEV,       /* 定时器事件更新 */
    AMHW_HC32_TIM_MASTER_OUT_CMPSO,     /* 比较匹配选择输出 */
    AMHW_HC32_TIM_MASTER_OUT_OCREF0A,   /* 比较参数输出 */
    AMHW_HC32_TIM_MASTER_OUT_OCREF0B_1A,/* 比较参数输出(定时器012为0B，定时器3为1A) */
    AMHW_HC32_TIM_MASTER_OUT_OCREF0B_2A,/* 比较参数输出(定时器012为0B，定时器3为2A) */
    AMHW_HC32_TIM_MASTER_OUT_OCREF0B,   /* 比较参数输出 */
}amhw_hc32_tim_master_out_t;
/**
 * \brief MMS 主模式输出选择 获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 主模式输出选择(amhw_hc32_tim_master_out_t枚举量)
 */
am_static_inline
amhw_hc32_tim_master_out_t
    amhw_hc32_tim_mode23_master_out_get (amhw_hc32_tim_t *p_hw_tim)
{
    return (amhw_hc32_tim_master_out_t)((p_hw_tim->mscr >> 0) & 0x7ul);
}

/**
 * \brief MMS 主模式输出选择  设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] choice   : 主模式输出选择(amhw_hc32_tim_master_out_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_master_out_set(amhw_hc32_tim_t          *p_hw_tim,
                                           amhw_hc32_tim_master_out_t choice)
{
    p_hw_tim->mscr =(p_hw_tim->mscr & (~(0x7ul << 0))) |
                    ((choice & 0x7ul) << 0);
}

/**
 * \brief 相位控制类型枚举（对应在寄存器中的起始位）
 */
typedef enum {
    AMHW_HC32_TIM_PHASE_ETP   = 31,   /* ETR输入相位选择 */
    AMHW_HC32_TIM_PHASE_BKP   = 27,   /* 刹车BK输入相位选择 */
    AMHW_HC32_TIM_PHASE_CCPB2 = 23,   /* 比较功能：CH2B通道比较输出相位控制 */
    AMHW_HC32_TIM_PHASE_CCPA2 = 19,   /* 比较功能：CH2A通道比较输出相位控制 */
    AMHW_HC32_TIM_PHASE_CCPB1 = 15,   /* 比较功能：CH1B通道比较输出相位控制 */
    AMHW_HC32_TIM_PHASE_CCPA1 = 11,   /* 比较功能：CH1A通道比较输出相位控制 */
    AMHW_HC32_TIM_PHASE_CCPB0 = 7,    /* 该控制位在不同模式下具有不同功能
                                         * 比较功能：输出比较模式
                                         *        CCPBx比较输出CHBx端口极性控制
                                         *
                                         * 编码计数与从模式门控功能：输入相位控制
                                         *        CCPB0从模式门控，复位，外部触发，
                                         *        外部时钟使用CH0B端口输入极性控制
                                         */
    AMHW_HC32_TIM_PHASE_CCPA0 = 3,    /* 该控制位在不同模式下具有不同功能
                                         * 比较功能：输出比较模式
                                         *        CCPAx比较输出CHAx端口极性控制
                                         *
                                         * 编码计数与从模式门控功能：输入相位控制
                                         *        CCPA0从模式门控，复位，外部触发，
                                         *        外部时钟使用CH0A端口输入极性控制
                                         */
}amhw_hc32_tim_phase_type_t;

/**
 * \brief 同向
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : 对应的控制类型(amhw_hc32_tim_phase_type_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_phase_same (amhw_hc32_tim_t           *p_hw_tim,
                                        amhw_hc32_tim_phase_type_t type)
{
    p_hw_tim->fltr &= ~(0x1ul << type);
}

/**
 * \brief 反向
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : 对应的控制类型(amhw_hc32_tim_phase_type_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_phase_reversed (amhw_hc32_tim_t         *p_hw_tim,
                                            amhw_hc32_tim_phase_type_t type)
{
    p_hw_tim->fltr |= (0x1ul << type);
}

/**
 * \brief 滤波控制类型枚举（对应在寄存器中的起始位）
 */
typedef enum {
    AMHW_HC32_TIM_FLITER_TYPE_FLTET = 28,   /* ETR滤波控制 */
    AMHW_HC32_TIM_FLITER_TYPE_FLTBK = 24,   /* 刹车输入滤波控制 */
    AMHW_HC32_TIM_FLITER_TYPE_FLTB2 = 20,   /* CH2B输入通道滤波设置 */
    AMHW_HC32_TIM_FLITER_TYPE_FLTA2 = 16,   /* CH2A输入通道滤波设置 */
    AMHW_HC32_TIM_FLITER_TYPE_FLTB1 = 12,   /* CH1B输入通道滤波设置 */
    AMHW_HC32_TIM_FLITER_TYPE_FLTA1 = 8,    /* CH1A输入通道滤波设置 */
    AMHW_HC32_TIM_FLITER_TYPE_FLTB0 = 4,    /* CH0B输入通道滤波设置 */
    AMHW_HC32_TIM_FLITER_TYPE_FLTA0 = 0,    /* CH0A输入通道滤波设置 */
}amhw_hc32_tim_filter_type_t;

/**
 * \brief 滤波控制    获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : 对应的控制类型(amhw_hc32_tim_filter_type_t枚举量)
 *
 * \return 滤波控制(amhw_hc32_tim_filter_t枚举量)
 */
am_static_inline
amhw_hc32_tim_filter_t
    amhw_hc32_tim_mode23_fliter_get (amhw_hc32_tim_t            *p_hw_tim,
                                       amhw_hc32_tim_filter_type_t type)
{
    return (amhw_hc32_tim_filter_t)((p_hw_tim->fltr >> type) & 0x7ul);
}

/**
 * \brief 滤波控制     设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : 对应的控制类型(amhw_hc32_tim_filter_type_t枚举量)
 * \param[in] choice   : 滤波控制(amhw_hc32_tim_filter_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_fliter_set(amhw_hc32_tim_t            *p_hw_tim,
                                       amhw_hc32_tim_filter_type_t type,
                                       amhw_hc32_tim_filter_t      choice)
{
    p_hw_tim->fltr =(p_hw_tim->fltr & (~(0x7ul << type))) |
                    ((choice & 0x7ul) << type);
}

/**
 * \brief 比较控制(比较)
 *
 * \note PWM模式1
 *            单点比较：
 *                   上计数时CNT<CCRxy输出高，下计数时CNT>CCRxy输出为低电平
 *            双点比较：
 *                   1） 锯齿波上计数 CCRxA<CNT≤CCRxB输出为低电平
 *                   2） 锯齿波下计数 CCRxA<CNT≤CCRxB输出为高电平
 *                   3） 三角波上计数CNT<CCRxA输出高，下计数CNT>CCRxB为低电平
 *
 *        PWM模式2
 *            单点比较：
 *                   上计数时CNT<CCRxy输出低，下计数时CNT>CCRxy输出为高电平
 *            双点比较：
 *                   1） 锯齿波上计数 CCRxA<CNT≤CCRxB输出为高电平
 *                   2） 锯齿波下计数 CCRxA<CNT≤CCRxB输出为低电平
 *                   3） 三角波上计数CNT<CCRxA输出低，下计数CNT>CCRxB为高电平
 */
typedef enum {
    AMHW_HC32_TIM_COMPARE_0 = 0,       /* 强制为0 */
    AMHW_HC32_TIM_COMPARE_1,           /* 强制为1 */
    AMHW_HC32_TIM_COMPARE_FIT_0,       /* 比较匹配时强制为0 */
    AMHW_HC32_TIM_COMPARE_FIT_1,       /* 比较匹配时强制为1 */
    AMHW_HC32_TIM_COMPARE_FIT_TOGGLE,  /* 比较匹配时翻转 */
    AMHW_HC32_TIM_COMPARE_FIT_HIGH,    /* 比较匹配时输出一个计数周期的高电平 */
    AMHW_HC32_TIM_COMPARE_PWM1,        /* PWM模式1 */
    AMHW_HC32_TIM_COMPARE_PWM2,        /* PWM模式2 */
}amhw_hc32_tim_compare_t;

/**
 * \brief 比较控制类型枚举（对应在寄存器中的起始位）
 */
typedef enum {
    AMHW_HC32_TIM_COMPARE_TYPE_FLTB2 = 20,   /* CH2B通道比较控制 */
    AMHW_HC32_TIM_COMPARE_TYPE_FLTA2 = 16,   /* CH2A通道比较控制 */
    AMHW_HC32_TIM_COMPARE_TYPE_FLTB1 = 12,   /* CH1B通道比较控制 */
    AMHW_HC32_TIM_COMPARE_TYPE_FLTA1 = 8,    /* CH1A通道比较控制 */
    AMHW_HC32_TIM_COMPARE_TYPE_FLTB0 = 4,    /* CH0B通道比较控制 */
    AMHW_HC32_TIM_COMPARE_TYPE_FLTA0 = 0,    /* CH0A通道比较控制 */
}amhw_hc32_tim_compare_type_t;

/**
 * \brief 比较控制    获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : 对应的控制类型(amhw_hc32_tim_compare_type_t枚举量)
 *
 * \return 比较控制(amhw_hc32_tim_compare_t枚举量)
 */
am_static_inline
amhw_hc32_tim_compare_t
    amhw_hc32_tim_mode23_compare_get (amhw_hc32_tim_t             *p_hw_tim,
                                        amhw_hc32_tim_compare_type_t type)
{
    return (amhw_hc32_tim_compare_t)((p_hw_tim->fltr >> type) & 0x7ul);
}

/**
 * \brief 比较控制     设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : 对应的控制类型(amhw_hc32_tim_compare_type_t枚举量)
 * \param[in] choice   : 滤波控制(amhw_hc32_tim_compare_t枚举量)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_compare_set(amhw_hc32_tim_t             *p_hw_tim,
                                        amhw_hc32_tim_compare_type_t type,
                                        amhw_hc32_tim_compare_t      choice)
{
    p_hw_tim->fltr =(p_hw_tim->fltr & (~(0x7ul << type))) |
                    ((choice & 0x7ul) << type);
}

/* ADC触发控制寄存器（TIMx_ADTR）相关控制位宏定义 */
#define AMHW_HC32_TIM_ADC_TRIGGER_ADTE   (0x1UL << 7) /* 使能ADC触发全局控制 */
#define AMHW_HC32_TIM_ADC_TRIGGER_CMB2E  (0x1UL << 6) /* 通道2B比较匹配触发ADC使能 */
#define AMHW_HC32_TIM_ADC_TRIGGER_CMB1E  (0x1UL << 5) /* 通道1B比较匹配触发ADC使能 */
#define AMHW_HC32_TIM_ADC_TRIGGER_CMB0E  (0x1UL << 4) /* 通道0B比较匹配触发ADC使能 */
#define AMHW_HC32_TIM_ADC_TRIGGER_CMA2E  (0x1UL << 3) /* 通道2A比较匹配触发ADC使能 */
#define AMHW_HC32_TIM_ADC_TRIGGER_CMA1E  (0x1UL << 2) /* 通道1A比较匹配触发ADC使能 */
#define AMHW_HC32_TIM_ADC_TRIGGER_CMA0E  (0x1UL << 1) /* 通道0A比较匹配触发ADC使能 */
#define AMHW_HC32_TIM_ADC_TRIGGER_UEVE   (0x1UL << 0) /* 事件更新触发ADC使能 */
#define AMHW_HC32_TIM_ADC_TRIGGER_ALL    0xFF         /* 所有 */

/**
 * \brief 上述宏定义使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : AMHW_HC32_TIM_ADC_TRIGGER_ADTE
 *                       .
 *                       .
 *                       .
 *                       AMHW_HC32_TIM_ADC_TRIGGER_ALL
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_adc_trigger_enable (amhw_hc32_tim_t *p_hw_tim,
                                                uint32_t           type)
{
    if(0xFF == type) {
        p_hw_tim->adtr |= AMHW_HC32_TIM_ADC_TRIGGER_ADTE;
        p_hw_tim->adtr |= AMHW_HC32_TIM_ADC_TRIGGER_CMB2E;
        p_hw_tim->adtr |= AMHW_HC32_TIM_ADC_TRIGGER_CMB1E;
        p_hw_tim->adtr |= AMHW_HC32_TIM_ADC_TRIGGER_CMB0E;
        p_hw_tim->adtr |= AMHW_HC32_TIM_ADC_TRIGGER_CMA2E;
        p_hw_tim->adtr |= AMHW_HC32_TIM_ADC_TRIGGER_CMA1E;
        p_hw_tim->adtr |= AMHW_HC32_TIM_ADC_TRIGGER_CMA0E;
        p_hw_tim->adtr |= AMHW_HC32_TIM_ADC_TRIGGER_UEVE;
    }
    else {
        p_hw_tim->adtr |= type;
    }
}

/**
 * \brief 上述宏定义禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : AMHW_HC32_TIM_ADC_TRIGGER_ADTE
 *                       .
 *                       .
 *                       .
 *                       AMHW_HC32_TIM_ADC_TRIGGER_ALL
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_adc_trigger_disable (amhw_hc32_tim_t  *p_hw_tim,
                                                 uint32_t           type)
{
    if(0xFF == type) {
        p_hw_tim->adtr &= ~AMHW_HC32_TIM_ADC_TRIGGER_ADTE;
        p_hw_tim->adtr &= ~AMHW_HC32_TIM_ADC_TRIGGER_CMB2E;
        p_hw_tim->adtr &= ~AMHW_HC32_TIM_ADC_TRIGGER_CMB1E;
        p_hw_tim->adtr &= ~AMHW_HC32_TIM_ADC_TRIGGER_CMB0E;
        p_hw_tim->adtr &= ~AMHW_HC32_TIM_ADC_TRIGGER_CMA2E;
        p_hw_tim->adtr &= ~AMHW_HC32_TIM_ADC_TRIGGER_CMA1E;
        p_hw_tim->adtr &= ~AMHW_HC32_TIM_ADC_TRIGGER_CMA0E;
        p_hw_tim->adtr &= ~AMHW_HC32_TIM_ADC_TRIGGER_UEVE;
    }
    else {
        p_hw_tim->adtr &= ~type;
    }
}

/**
 * \brief CCGB   捕获比较B软件触发
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 * \note 硬件自动清零
 *       在比较模式下只产生中断
 *       在捕获模式下产生中断并且捕获计数器的值到捕获寄存器中
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_crchx_ccgb_enable (amhw_hc32_tim_t *p_hw_tim,
                                               uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 15);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 15);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 15);
            break;

        default:
            break;
    }
}

/**
 * \brief CCGA   捕获比较A软件触发
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \note 硬件自动清零
 *       在比较模式下只产生中断
 *       在捕获模式下产生中断并且捕获计数器的值到捕获寄存器中
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_crchx_ccga_enable (amhw_hc32_tim_t *p_hw_tim,
                                               uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 14);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 14);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 14);
            break;

        default:
            break;
    }
}

/**
 * \brief B通道比较匹配设置
 */
typedef enum {
    AMHW_HC32_TIM_CISB_TYPE_NO = 0,   /* 无匹配 */
    AMHW_HC32_TIM_CISB_TYPE_RISING,   /* 上升匹配 */
    AMHW_HC32_TIM_CISB_TYPE_FALLING,  /* 下降匹配 */
    AMHW_HC32_TIM_CISB_TYPE_BOTH,     /* 双匹配 */
}amhw_hc32_tim_cisb_type_t;

/**
 * \brief CISB   B通道比较匹配设置    获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return amhw_hc32_tim_cisb_type_t枚举量
 */
am_static_inline
amhw_hc32_tim_cisb_type_t
    amhw_hc32_tim_mode23_cisb_get (amhw_hc32_tim_t *p_hw_tim,
                                     uint8_t            chan)
{
    uint8_t get_value;
    switch(chan / 2) {
        case 0:
            get_value = (p_hw_tim->crchx[0] >> 12) & 0x3ul;
            break;

        case 1:
            get_value = (p_hw_tim->crchx[1] >> 12) & 0x3ul;
            break;

        case 2:
            get_value = (p_hw_tim->crchx[2] >> 12) & 0x3ul;
            break;

        default:
            break;
    }
    return (amhw_hc32_tim_cisb_type_t)get_value;
}

/**
 * \brief CISB   B通道比较匹配设置     设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 * \param[in] choice   : amhw_hc32_tim_cisb_type_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cisb_set(amhw_hc32_tim_t           *p_hw_tim,
                                     uint8_t                      chan,
                                     amhw_hc32_tim_cisb_type_t  choice)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] = (p_hw_tim->crchx[0] & (~(0x3ul << 12))) |
                                 ((choice & 0x3ul) << 12);
            break;

        case 1:
            p_hw_tim->crchx[1] = (p_hw_tim->crchx[1] & (~(0x3ul << 12))) |
                                 ((choice & 0x3ul) << 12);
            break;

        case 2:
            p_hw_tim->crchx[2] = (p_hw_tim->crchx[2] & (~(0x3ul << 12))) |
                                 ((choice & 0x3ul) << 12);
            break;

        default:
            break;
    }
}

/**
 * \brief CDEB   B捕获比较触发DMA禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cdeb_dma_disable (amhw_hc32_tim_t *p_hw_tim,
                                             uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 11);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 11);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 11);
            break;

        default:
            break;
    }
}

/**
 * \brief CDEB   B捕获比较触发DMA使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cdeb_dma_enable (amhw_hc32_tim_t *p_hw_tim,
                                             uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 11);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 11);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 11);
            break;

        default:
            break;
    }
}

/**
 * \brief CDEA   A捕获比较触发DMA禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cdea_dma_disable (amhw_hc32_tim_t *p_hw_tim,
                                             uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 10);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 10);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 10);
            break;

        default:
            break;
    }
}

/**
 * \brief CDEA   A捕获比较触发DMA使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cdea_dma_enable (amhw_hc32_tim_t *p_hw_tim,
                                             uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 10);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 10);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 10);
            break;

        default:
            break;
    }
}

/**
 * \brief CIEB   B捕获比较触发中断禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cieb_int_disable (amhw_hc32_tim_t *p_hw_tim,
                                              uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 9);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 9);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 9);
            break;

        default:
            break;
    }
}

/**
 * \brief CIEB   B捕获比较触发中断使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cieb_int_enable (amhw_hc32_tim_t *p_hw_tim,
                                             uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 9);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 9);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 9);
            break;

        default:
            break;
    }
}

/**
 * \brief B捕获比较触发中断使能状态获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : HC32_TIM_CH0B
 *                       HC32_TIM_CH1B
 *                       HC32_TIM_CH2B
 *
 * \return  AM_TRUE  ：使能有效
 *          AM_FALSE ：使能无效
 *
 */
am_static_inline
am_bool_t amhw_hc32_tim_mode23_cieb_int_get (amhw_hc32_tim_t *p_hw_tim,
                                               uint8_t            chan)
{
	am_bool_t result = AM_FALSE;

    switch(chan / 2) {
        case 0:
        	result = (p_hw_tim->crchx[0] & (0x1ul << 9)) ? AM_TRUE : AM_FALSE;
            break;

        case 1:
        	result = (p_hw_tim->crchx[1] & (0x1ul << 9)) ? AM_TRUE : AM_FALSE;
            break;

        case 2:
        	result = (p_hw_tim->crchx[2] & (0x1ul << 9)) ? AM_TRUE : AM_FALSE;
            break;

        default:
            break;
    }

    return result;
}

/**
 * \brief CIEA   A捕获比较触发中断禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_ciea_int_disable (amhw_hc32_tim_t *p_hw_tim,
                                              uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 8);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 8);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 8);
            break;

        default:
            break;
    }
}

/**
 * \brief CIEA   A捕获比较触发中断使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_ciea_int_enable (amhw_hc32_tim_t *p_hw_tim,
                                             uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 8);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 8);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 8);
            break;

        default:
            break;
    }
}

/**
 * \brief A捕获比较触发中断使能状态获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : HC32_TIM_CH0A 或
 *                       HC32_TIM_CH1A 或
 *                       HC32_TIM_CH2A 或
 *
 * \return  AM_TRUE  ：使能有效
 *          AM_FALSE ：使能无效
 *
 */
am_static_inline
am_bool_t amhw_hc32_tim_mode23_ciea_int_get (amhw_hc32_tim_t *p_hw_tim,
                                               uint8_t            chan)
{
	am_bool_t result = AM_FALSE;

    switch(chan / 2) {
        case 0:
        	result = (p_hw_tim->crchx[0] & (0x1ul << 8)) ? AM_TRUE : AM_FALSE;
            break;

        case 1:
        	result = (p_hw_tim->crchx[1] & (0x1ul << 8)) ? AM_TRUE : AM_FALSE;
            break;

        case 2:
        	result = (p_hw_tim->crchx[2] & (0x1ul << 8)) ? AM_TRUE : AM_FALSE;
            break;

        default:
            break;
    }

    return result;
}

/**
 * \brief BUFEB   比较功能： B比较缓存使能控制     禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_buffb_disable (amhw_hc32_tim_t *p_hw_tim,
                                           uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 7);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 7);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 7);
            break;

        default:
            break;
    }
}

/**
 * \brief BUFEB   比较功能： B比较缓存使能控制     使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_buffb_enable (amhw_hc32_tim_t *p_hw_tim,
                                          uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 7);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 7);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 7);
            break;

        default:
            break;
    }
}

/**
 * \brief BUFEA   比较功能：A比较缓存使能控制     禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_buffa_disable (amhw_hc32_tim_t *p_hw_tim,
                                           uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 6);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 6);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 6);
            break;

        default:
            break;
    }
}

/**
 * \brief BUFEA   比较功能： A比较缓存使能控制     使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_buffa_enable (amhw_hc32_tim_t *p_hw_tim,
                                          uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 6);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 6);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 6);
            break;

        default:
            break;
    }
}

/**
 * \brief CSB   B通道捕获/比较功能选择      比较模式
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_csb_compare (amhw_hc32_tim_t *p_hw_tim,
                                         uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 5);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 5);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 5);
            break;

        default:
            break;
    }
}

/**
 * \brief CSB   B通道捕获/比较功能选择      捕获模式
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_csb_cap (amhw_hc32_tim_t *p_hw_tim,
                                     uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 5);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 5);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 5);
            break;

        default:
            break;
    }
}

/**
 * \brief CSA   A通道捕获/比较功能选择      比较模式
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_csa_compare (amhw_hc32_tim_t *p_hw_tim,
                                         uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 4);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 4);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 4);
            break;

        default:
            break;
    }
}

/**
 * \brief CSA   A通道捕获/比较功能选择      捕获模式
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_csa_cap (amhw_hc32_tim_t *p_hw_tim,
                                     uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 4);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 4);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 4);
            break;

        default:
            break;
    }
}

/**
 * \brief CFB   B通道下降沿捕获禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cfb_disable (amhw_hc32_tim_t *p_hw_tim,
                                         uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 3);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 3);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 3);
            break;

        default:
            break;
    }
}

/**
 * \brief CFB   B通道下降沿捕获使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cfb_enable (amhw_hc32_tim_t *p_hw_tim,
                                        uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 3);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 3);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 3);
            break;

        default:
            break;
    }
}

/**
 * \brief CRB   B通道上升沿捕获禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_crb_disable (amhw_hc32_tim_t *p_hw_tim,
                                         uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 2);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 2);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 2);
            break;

        default:
            break;
    }
}

/**
 * \brief CRB   B通道上升沿捕获使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_crb_enable (amhw_hc32_tim_t *p_hw_tim,
                                        uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 2);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 2);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 2);
            break;

        default:
            break;
    }
}

/**
 * \brief CFA   A通道下降沿捕获禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cfa_disable (amhw_hc32_tim_t *p_hw_tim,
                                         uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 1);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 1);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 1);
            break;

        default:
            break;
    }
}

/**
 * \brief CFA   A通道下降沿捕获使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cfa_enable (amhw_hc32_tim_t *p_hw_tim,
                                        uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 1);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 1);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 1);
            break;

        default:
            break;
    }
}

/**
 * \brief CRA   A通道上升沿捕获禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cra_disable (amhw_hc32_tim_t *p_hw_tim,
                                         uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] &= ~(0x1ul << 0);
            break;

        case 1:
            p_hw_tim->crchx[1] &= ~(0x1ul << 0);
            break;

        case 2:
            p_hw_tim->crchx[2] &= ~(0x1ul << 0);
            break;

        default:
            break;
    }
}

/**
 * \brief CRA   A通道上升沿捕获使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_cra_enable (amhw_hc32_tim_t *p_hw_tim,
                                        uint8_t            chan)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] |= (0x1ul << 0);
            break;

        case 1:
            p_hw_tim->crchx[1] |= (0x1ul << 0);
            break;

        case 2:
            p_hw_tim->crchx[2] |= (0x1ul << 0);
            break;

        default:
            break;
    }
}

/**
 * \brief 比较功能输出刹车电平控制
 */
typedef enum {
    AMHW_HC32_TIM_CMP_BRAKEOUT_Z = 0,      /* 高阻输出 */
    AMHW_HC32_TIM_CMP_BRAKEOUT_NO,         /* 对输出无影响 */
    AMHW_HC32_TIM_CMP_BRAKEOUT_LOW,        /* 强制输出低电平 */
    AMHW_HC32_TIM_CMP_BRAKEOUT_HIGH,       /* 强制输出高电平 */
}amhw_hc32_tim_cmp_brakeout_t;

/**
 * \brief BKSB  B通道比较功能输出刹车电平控制     获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return amhw_hc32_tim_cmp_brakeout_t枚举量
 */
am_static_inline
amhw_hc32_tim_cmp_brakeout_t
    amhw_hc32_tim_mode23_bksb_get (amhw_hc32_tim_t *p_hw_tim,
                                     uint8_t            chan)
{
    uint8_t get_value;
    switch(chan / 2) {
        case 0:
            get_value = (p_hw_tim->crchx[0] >> 2) & 0x3ul;
            break;

        case 1:
            get_value = (p_hw_tim->crchx[1] >> 2) & 0x3ul;
            break;

        case 2:
            get_value = (p_hw_tim->crchx[2] >> 2) & 0x3ul;
            break;

        default:
            break;
    }
    return (amhw_hc32_tim_cmp_brakeout_t)get_value;
}

/**
 * \brief BKSB  B通道比较功能输出刹车电平控制     设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 * \param[in] choice   : amhw_hc32_tim_cmp_brakeout_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_bksb_set(amhw_hc32_tim_t             *p_hw_tim,
                                     uint8_t                        chan,
                                     amhw_hc32_tim_cmp_brakeout_t choice)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] = (p_hw_tim->crchx[0] & (~(0x3ul << 2))) |
                                 ((choice & 0x3ul) << 2);
            break;

        case 1:
            p_hw_tim->crchx[1] = (p_hw_tim->crchx[1] & (~(0x3ul << 2))) |
                                 ((choice & 0x3ul) << 2);
            break;

        case 2:
            p_hw_tim->crchx[2] = (p_hw_tim->crchx[2] & (~(0x3ul << 2))) |
                                 ((choice & 0x3ul) << 2);
            break;

        default:
            break;
    }
}

/**
 * \brief BKSA  A通道比较功能输出刹车电平控制     获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 *
 * \return amhw_hc32_tim_cmp_brakeout_t枚举量
 */
am_static_inline
amhw_hc32_tim_cmp_brakeout_t
    amhw_hc32_tim_mode23_bksa_get (amhw_hc32_tim_t *p_hw_tim,
                                     uint8_t            chan)
{
    uint8_t get_value;
    switch(chan / 2) {
        case 0:
            get_value = (p_hw_tim->crchx[0] >> 0) & 0x3ul;
            break;

        case 1:
            get_value = (p_hw_tim->crchx[1] >> 0) & 0x3ul;
            break;

        case 2:
            get_value = (p_hw_tim->crchx[2] >> 0) & 0x3ul;
            break;

        default:
            break;
    }
    return (amhw_hc32_tim_cmp_brakeout_t)get_value;
}

/**
 * \brief BKSA  A通道比较功能输出刹车电平控制     设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CH0A               或
 *                            HC32_TIM_CH0B               或
 *                            HC32_TIM_CH1A(TIM3特有通道)    或
 *                            HC32_TIM_CH1B(TIM3特有通道)    或
 *                            HC32_TIM_CH2A(TIM3特有通道)    或
 *                            HC32_TIM_CH2B(TIM3特有通道)    或
 * \param[in] choice   : amhw_hc32_tim_cmp_brakeout_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_bksa_set(amhw_hc32_tim_t             *p_hw_tim,
                                     uint8_t                        chan,
                                     amhw_hc32_tim_cmp_brakeout_t choice)
{
    switch(chan / 2) {
        case 0:
            p_hw_tim->crchx[0] = (p_hw_tim->crchx[0] & (~(0x3ul << 0))) |
                                 ((choice & 0x3ul) << 0);
            break;

        case 1:
            p_hw_tim->crchx[1] = (p_hw_tim->crchx[1] & (~(0x3ul << 0))) |
                                 ((choice & 0x3ul) << 0);
            break;

        case 2:
            p_hw_tim->crchx[2] = (p_hw_tim->crchx[2] & (~(0x3ul << 0))) |
                                 ((choice & 0x3ul) << 0);
            break;

        default:
            break;
    }
}

#define AMHW_HC32_TIM_DTR_VCE     (0x1UL << 14)  /* VC刹车使能 */
#define AMHW_HC32_TIM_DTR_SAFEEN  (0x1UL << 13)  /* Safety 刹车使能 */
#define AMHW_HC32_TIM_DTR_MOE     (0x1UL << 12)  /* PWM输出使能 */
#define AMHW_HC32_TIM_DTR_AOE     (0x1UL << 11)  /* PWM输出自动使能 */
#define AMHW_HC32_TIM_DTR_BKE     (0x1UL << 10)  /* 刹车使能 */
#define AMHW_HC32_TIM_DTR_DTEN    (0x1UL << 9)   /* 死区控制使能 */

/**
 * \brief 死区时间寄存器（TIMx_DTR）        禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : AMHW_HC32_TIM_DTR_VCE
 *                       .
 *                       .
 *                       .
 *                       AMHW_HC32_TIM_DTR_DTEN
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_dtr_disable (amhw_hc32_tim_t *p_hw_tim,
                                         uint16_t           type)
{
    p_hw_tim->dtr &= ~type;
}

/**
 * \brief 死区时间寄存器（TIMx_DTR）        使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : AMHW_HC32_TIM_DTR_VCE
 *                       .
 *                       .
 *                       .
 *                       AMHW_HC32_TIM_DTR_DTEN
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_dtr_enable (amhw_hc32_tim_t *p_hw_tim,
                                        uint16_t           type)
{
    p_hw_tim->dtr |= type;
}

/**
 * \brief bksel    刹车选择       TIM1/2使用本身的的刹车控制
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note TIM0/TIM3选择无效
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_bksel_mine (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->dtr &= ~(0x1ul << 8);
}

/**
 * \brief bksel    刹车选择      TIM1/2使用 TIM0的的刹车控制
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note TIM0/TIM3选择无效
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_bksel_tim0 (amhw_hc32_tim_t *p_hw_tim)
{
    p_hw_tim->dtr |= (0x1ul << 8);
}

/**
 * \brief DTR  死区时间   获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note  DTR[7]   = 0     T = DTR[6:0]+2             2-129      step=1
 *
 *        DTR[7:6] = 10    T = {DTR[5:0]+64}*2 +2     130-256    step=2
 *
 *        DTR[7:5] = 110   T = {DTR[4:0]+32}*8 +2     258-506    step=8
 *
 *        DTR[7:5] = 111   T = {DTR[4:0]+32}*16 +2    514-1010   step=16
 *
 * \return 设定数值
 */
am_static_inline
uint8_t amhw_hc32_tim_mode23_dtr_get (amhw_hc32_tim_t *p_hw_tim)
{
    return ((p_hw_tim->dtr >> 0) & 0xfful);
}

/**
 * \brief DTR  死区时间   设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] data     : 待设定数值(2-256、258-506、514-1010)
 *
 * \note  DTR[7]   = 0     T = DTR[6:0]+2             2-129      step=1
 *
 *        DTR[7:6] = 10    T = {DTR[5:0]+64}*2 +2     130-256    step=2
 *
 *        DTR[7:5] = 110   T = {DTR[4:0]+32}*8 +2     258-506    step=8
 *
 *        DTR[7:5] = 111   T = {DTR[4:0]+32}*16 +2    514-1010   step=16
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_dtr_set(amhw_hc32_tim_t *p_hw_tim,
                                    uint16_t           data)
{
	if(data <= 2) {
		data = 0;
    }else if(data <= 129) {
        data = data - 2;
    } else if(data <= 257) {
        if(data == 257) {
            data = 256;
        }
        data = (((data - 2) >> 1) - 64) | 0x80;
    } else if(data <= 513) {
        if((data <= 513) || (data > 506)) {
            data = 506;
        }
        data = (((data - 2) >> 3) - 32) | 0xC0;
    } else if(data <= 1010) {
        data = (((data - 2) >> 4) - 32) | 0xE0;
    } else {
        data = 0;
    }

    p_hw_tim->dtr = (p_hw_tim->dtr & (~(0xfful << 0))) |
                    ((data & 0xfful) << 0);
}

/**
 * \brief RCR  重复周期计数值   获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note  设置RCR+1个周期个上溢出下溢出后产生事件更新，计数器上溢出或下溢出时内部RCR_CNT减1，
 *        当计数到零后RCR_CNT重载RCR的值，并且产生事件更新UEV信号
 *
 * \return 设定数值
 */
am_static_inline
uint8_t amhw_hc32_tim_mode23_rcr_get (amhw_hc32_tim_t *p_hw_tim)
{
    return ((p_hw_tim->rcr >> 0) & 0xfful);
}

/**
 * \brief RCR  重复周期计数值   设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] data     : 待设定数值
 *
 * \note  设置RCR+1个周期个上溢出下溢出后产生事件更新，计数器上溢出或下溢出时内部RCR_CNT减1，
 *        当计数到零后RCR_CNT重载RCR的值，并且产生事件更新UEV信号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_rcr_set(amhw_hc32_tim_t *p_hw_tim,
                                    uint8_t            data)
{
    p_hw_tim->rcr = (p_hw_tim->rcr & (~(0xfful << 0))) |
                    ((data & 0xfful) << 0);
}

/**
 * \brief CCR比较捕获寄存器获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CHxx
 *
 * \return 设定数值
 */
am_static_inline
uint16_t amhw_hc32_tim_mode23_ccr_get (amhw_hc32_tim_t   *p_hw_tim,
                                       uint8_t            chan)
{
    return p_hw_tim->ccr[chan] & 0xfffful;
}

/**
 * \brief CCR比较捕获寄存器设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] chan     : 通道号  HC32_TIM_CHxx
 * \param[in] data     : 待设定数值
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_tim_mode23_ccr_set(amhw_hc32_tim_t   *p_hw_tim,
                                  uint8_t            chan,
                                  uint16_t           data)
{
    p_hw_tim->ccr[chan] = data;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32_TIM_H */

/* end of file */
