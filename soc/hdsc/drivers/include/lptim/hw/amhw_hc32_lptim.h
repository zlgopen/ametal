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
 * \brief 低功耗定时器LPTIM接口
 *
 * \internal
 * \par History
 * - 1.00 19-09-27  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32_LPTIM_H
#define __AMHW_HC32_LPTIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_hc32_if_lptim
 * \copydoc amhw_hc32_lptim.h
 * @{
 */

/**
  * \brief 定时器寄存器块结构体
  */
typedef struct amhw_hc32_lptim {
    __I  uint32_t cnt;         /**< \brief 计数值只读寄存器 */
    __IO uint32_t arr;         /**< \brief 重载寄存器 */
    __IO uint32_t reserve;     /**< \brief 保留位 */
    __IO uint32_t cr;          /**< \brief 控制寄存器 */
    __I  uint32_t ifr;         /**< \brief 中断标志寄存器 */
    __IO uint32_t iclr;        /**< \brief 中断清除寄存器 */
} amhw_hc32_lptim_t;

/**
 * \brief 得到当前计数器的值
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 当前计数器的值
 */
am_static_inline
uint16_t amhw_hc32_lptim_cnt16_count_get (amhw_hc32_lptim_t *p_hw_lptim)
{
    return p_hw_lptim->cnt;
}

/**
 * \brief 设置重载寄存器的值
 *
 * \param[in] p_hw_lptim :    指向LPTIM定时器寄存器块的指针
 * \param[in] value      :    设置的重载寄存器的值
 *
 * \note  写 ARR前需要读取 CR.WT_FLAG，当且尽当 WT_FLAG为1时，写入时才能写入数据。
 *        写ARR寄存器后 WT_FLAG会变低。
 *
 *        注：在定时器启动时或者重载时这个值会装载到定时器计数器中。定时器计数器值不可软件更改。
 *
 *\ return 无
 */
am_static_inline
void amhw_hc32_lptim_arr_count_set (amhw_hc32_lptim_t *p_hw_lptim,
                                      uint16_t value)
{
    p_hw_lptim->arr = value;
}

/**
 * \brief 设置重载寄存器的值
 *
 * \param[in] p_hw_lptim :    指向LPTIM定时器寄存器块的指针
 * \param[in] value      :    设置的重载寄存器的值
 *
 * \note  写 ARR前需要读取 CR.WT_FLAG，当且尽当 WT_FLAG为1时，写入时才能写入数据。
 *        写ARR寄存器后 WT_FLAG会变低。
 *
 *        注：在定时器启动时或者重载时这个值会装载到定时器计数器中。定时器计数器值不可软件更改。
 *
 *\ return 无
 */
am_static_inline
uint16_t amhw_hc32_lptim_arr_count_get (amhw_hc32_lptim_t *p_hw_lptim)
{
    return p_hw_lptim->arr;
}

/**
 * \brief 时钟分频选择（在定时器运行时不可以更改）
 */
typedef enum {
    AMHW_HC32_LPTIM_CLK_DIV1 = 0,    /* 1分频 */
    AMHW_HC32_LPTIM_CLK_DIV2,        /* 2分频 */
    AMHW_HC32_LPTIM_CLK_DIV4,        /* 4分频 */
    AMHW_HC32_LPTIM_CLK_DIV8,        /* 8分频 */
    AMHW_HC32_LPTIM_CLK_DIV16,       /* 16分频 */
    AMHW_HC32_LPTIM_CLK_DIV32,       /* 32分频 */
    AMHW_HC32_LPTIM_CLK_DIV64,       /* 64分频 */
    AMHW_HC32_LPTIM_CLK_DIV256,      /* 256分频 */
}amhw_hc32_lptim_clk_div_t;

/**
 * \brief PRS 内部时钟分频选择   获取
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 设置的分频系数（amhw_hc32_lptim_clk_div_t枚举量）
 */
am_static_inline
amhw_hc32_lptim_clk_div_t
amhw_hc32_lptim_clk_div_get (amhw_hc32_lptim_t *p_hw_lptim)
{
    return (amhw_hc32_lptim_clk_div_t)((p_hw_lptim->cr >> 11) & 0x7ul);
}

/**
 * \brief PRS 内部时钟分频选择   设置
 *
 * \param[in] p_hw_lptim : 指向TIM定时器寄存器块的指针
 * \param[in] clkdiv   : 设置的分频系数（amhw_hc32_lptim_clkdiv_t枚举量）
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_clk_div_set (amhw_hc32_lptim_t        *p_hw_lptim,
                                    amhw_hc32_lptim_clk_div_t clkdiv)
{
    p_hw_lptim->cr = (p_hw_lptim->cr & (~(0x7ul << 11))) |
                     ((clkdiv & 0x7ul) << 11);
}

/**
 * \brief IE 中断禁能
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_int_disable (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr &= ~(0x1ul << 10);
}

/**
 * \brief IE 中断使能
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_int_enable (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr |= (0x1ul << 10);
}

/**
 * \brief GATEP 端口GATE极性控制   高电平
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_gate_high (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr &= ~(0x1ul << 9);
}

/**
 * \brief GATEP 端口GATE极性控制   低电平
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_gate_low (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr |= (0x1ul << 9);
}

/**
 * \brief GATE 定时器门控禁能
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_gate_disable (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr &= ~(0x1ul << 8);
}

/**
 * \brief GATE 定时器门控使能
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_gate_enable (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr |= (0x1ul << 8);
}

/**
 * \brief WT_FLAG     WT,写同步标志
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return AM_TRUE  : 同步完成，可更改ARR
 *         AM_FALSE : 正在同步， 写ARR无效
 */
am_static_inline
am_bool_t amhw_hc32_lptim_write_into_arr_stat (amhw_hc32_lptim_t *p_hw_lptim)
{
   return ((p_hw_lptim->cr >> 7) & 0x1ul) ? AM_TRUE : AM_FALSE;
}

typedef enum {
    AMHW_HC32_LPTIM_CLK_SRC_PCLK = 0ul,    /* LPTIM时钟为  PCLK */
    AMHW_HC32_LPTIM_CLK_SRC_XTL  = 2ul,    /* LPTIM时钟为  外部低速时钟 */
    AMHW_HC32_LPTIM_CLK_SRC_RCL  = 3ul,    /* LPTIM时钟为  内部低速时钟 */
}amhw_hc32_lptim_clk_src_t;

/**
 * \brief LPTIM 时钟选择
 *
 * \param[in] p_hw_lptim : 指向LPTIM寄存器结构体的指针
 * \param[in] clk_src    : amhw_hc32_lptim_clk_src_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32_lptim_clk_src_sel (amhw_hc32_lptim_t          *p_hw_lptim,
                                    amhw_hc32_lptim_clk_src_t   clk_src)
{
    p_hw_lptim->cr = (p_hw_lptim->cr & (~(0x3ul << 4))) | (clk_src << 4);
}

/**
 * \brief LPTIM 时钟获取
 *
 * \param[in] p_hw_lptim : 指向UALPTIMRT寄存器结构体的指针
 *
 * \return amhw_hc32_lptim_clk_src_t枚举量
 */
am_static_inline
amhw_hc32_lptim_clk_src_t
amhw_hc32_lptim_clk_src_get (amhw_hc32_lptim_t *p_hw_lptim)
{
    return (amhw_hc32_lptim_clk_src_t)((p_hw_lptim->cr >> 4) & 0x3ul);
}

/**
 * \brief TOG 输出禁能            同时输出0
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \note 0： TOG,TOGN 同时输出0
 *       1： TOG,TOGN 输出相位相反的信号。可供 buzzer 使用。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_tog_disable (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr &= ~(0x1ul << 3);
}

/**
 * \brief TOG 输出使能            输出相位相反的信号
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \note 0： TOG,TOGN 同时输出0
 *       1： TOG,TOGN 输出相位相反的信号。可供 buzzer 使用。
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_tog_enable (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr |= (0x1ul << 3);
}

typedef enum {
    AMHW_HC32_LPTIM_FUNCTION_TIMER   = 0,    /* 定时器 */
    AMHW_HC32_LPTIM_FUNCTION_COUNTER = 1,    /* 计数器 */
}amhw_hc32_lptim_function_t;

/**
 * \brief 计数器/定时器功能选择
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 * \param[in] type       : amhw_hc32_lptim_function_t枚举量
 *
 * \note    定时器使用TCK_SEL选择的时钟进行计数
 *
 *          计数器使用外部输入的下降沿进行计数。采样时钟使用TCK_SEL选择的时钟，
 *          外部输入时钟要低于 1/2 采样时钟
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_function_sel (amhw_hc32_lptim_t         *p_hw_lptim,
                                     amhw_hc32_lptim_function_t type)
{
    p_hw_lptim->cr = (p_hw_lptim->cr & (~(0x1ul << 2))) |
                     ((type & 0x1ul) << 2);
}

typedef enum {
    AMHW_HC32_LPTIM_MODE_NORELOAD = 0,    /* 无重载 */
    AMHW_HC32_LPTIM_MODE_RELOAD   = 1,    /* 自动重装载 */
}amhw_hc32_lptim_mode_t;

/**
 * \brief 定时器工作模式选择
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 * \param[in] type       : amhw_hc32_lptim_mode_t枚举量
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_mode_sel (amhw_hc32_lptim_t     *p_hw_lptim,
                                 amhw_hc32_lptim_mode_t type)
{
    p_hw_lptim->cr = (p_hw_lptim->cr & (~(0x1ul << 1))) |
                     ((type & 0x1ul) << 1);
}

/**
 * \brief TR 定时器禁能
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_disable (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr &= ~(0x1ul << 0);
}

/**
 * \brief TR 定时器使能
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_lptim_enable (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->cr |= (0x1ul << 0);
}

/**
 * \brief 获取标志是否有效
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return AM_TRUE  : 标志有效
 *         AM_FALSE : 标志无效
 */
am_static_inline
am_bool_t amhw_hc32_lptim_int_flag_check (amhw_hc32_lptim_t *p_hw_lptim)
{
    return (p_hw_lptim->ifr & 0x1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 标志清除
 *
 * \param[in] p_hw_lptim : 指向LPTIM定时器寄存器块的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_lptim_int_flag_clr (amhw_hc32_lptim_t *p_hw_lptim)
{
    p_hw_lptim->iclr &= ~0x1ul;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32_LPTIM_H */

/* end of file */
