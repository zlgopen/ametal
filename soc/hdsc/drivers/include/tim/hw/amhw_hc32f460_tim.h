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
 * - 1.00 
 * \endinternal
 */

#ifndef __AMHW_HC32F460_TIM_H
#define __AMHW_HC32F460_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

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

/** @} */
/**
 *******************************************************************************
 ** \brief Timer0 channel enumeration
 **
 ******************************************************************************/
typedef enum tim0_channel
{
    TIM0_CHANNEL_A = 0x00u,
    TIM0_CHANNEL_B = 0x01u
}tim0_channel_t;

/**
 *******************************************************************************
 ** \brief Timer0 Async Mode clock enumeration
 **
 ******************************************************************************/
typedef enum tim0_async_clock_src
{
    tim0_LRC    = 0x00u,
    tim0_XTAL32 = 0x01u
}tim0_async_clock_src_t;

/**
 *******************************************************************************
 ** \brief Timer0 Sync Mode clock enumeration
 **
 ******************************************************************************/
typedef enum tim0_sync_clock_src
{
    tim0_pclk1          = 0x00u,
    tim0_Inside_hard_trig = 0x01u
}tim0_sync_clock_src_t;

/**
 *******************************************************************************
 ** \brief Timer0 counter mode enumeration
 **
 ******************************************************************************/
typedef enum tim0_counter_mode
{
    tim0_sync  = 0x00u,
    tim0_async = 0x01u
}tim0_counter_mode_t;

/**
 *******************************************************************************
 ** \brief Timer0 trigger event mode enumeration
 **
 ******************************************************************************/
typedef enum tim0_function
{
    tim0_output_capare = 0x00u,
    tim0_input_captrue = 0x01u
}tim0_function_t;

/**
 *******************************************************************************
 ** \brief Timer0 clock division enumeration
 **
 ******************************************************************************/
typedef enum tim0_clock_div
{
    tim0_clk_div0 = 0u,
    tim0_clk_div2,
    tim0_clk_div4,
    tim0_clk_div8,
    tim0_clk_div16,
    tim0_clk_div32,
    tim0_clk_div64,
    tim0_clk_div128,
    tim0_clk_div256,
    tim0_clk_div512,
    tim0_clk_div1024
}tim0_clock_div_t;

typedef struct
{
    __IO uint32_t CNTA                      :16;
    uint32_t RESERVED16                     :16;
} stc_tmr0_cntar_field_t;

typedef struct
{
    __IO uint32_t CNTB                      :16;
    uint32_t RESERVED16                     :16;
} stc_tmr0_cntbr_field_t;

typedef struct
{
    __IO uint32_t CMPA                      :16;
    uint32_t RESERVED16                     :16;
} stc_tmr0_cmpar_field_t;

typedef struct
{
    __IO uint32_t CMPB                      :16;
    uint32_t RESERVED16                     :16;
} stc_tmr0_cmpbr_field_t;

typedef struct
{
    __IO uint32_t CSTA                      : 1;
    __IO uint32_t CAPMDA                    : 1;
    __IO uint32_t INTENA                    : 1;
    uint32_t RESERVED3                      : 1;
    __IO uint32_t CKDIVA                    : 4;
    __IO uint32_t SYNSA                     : 1;
    __IO uint32_t SYNCLKA                   : 1;
    __IO uint32_t ASYNCLKA                  : 1;
    uint32_t RESERVED11                     : 1;
    __IO uint32_t HSTAA                     : 1;
    __IO uint32_t HSTPA                     : 1;
    __IO uint32_t HCLEA                     : 1;
    __IO uint32_t HICPA                     : 1;
    __IO uint32_t CSTB                      : 1;
    __IO uint32_t CAPMDB                    : 1;
    __IO uint32_t INTENB                    : 1;
    uint32_t RESERVED19                     : 1;
    __IO uint32_t CKDIVB                    : 4;
    __IO uint32_t SYNSB                     : 1;
    __IO uint32_t SYNCLKB                   : 1;
    __IO uint32_t ASYNCLKB                  : 1;
    uint32_t RESERVED27                     : 1;
    __IO uint32_t HSTAB                     : 1;
    __IO uint32_t HSTPB                     : 1;
    __IO uint32_t HCLEB                     : 1;
    __IO uint32_t HICPB                     : 1;
} stc_tmr0_bconr_field_t;

typedef struct
{
    __IO uint32_t CMAF                      : 1;
    uint32_t RESERVED1                      :15;
    __IO uint32_t CMBF                      : 1;
    uint32_t RESERVED17                     :15;
} stc_tmr0_stflr_field_t;

typedef struct
{
    union
    {
        __IO uint32_t CNTAR;
        stc_tmr0_cntar_field_t CNTAR_f;
    };
    union
    {
        __IO uint32_t CNTBR;
        stc_tmr0_cntbr_field_t CNTBR_f;
    };
    union
    {
        __IO uint32_t CMPAR;
        stc_tmr0_cmpar_field_t CMPAR_f;
    };
    union
    {
        __IO uint32_t CMPBR;
        stc_tmr0_cmpbr_field_t CMPBR_f;
    };
    union
    {
        __IO uint32_t BCONR;
        stc_tmr0_bconr_field_t BCONR_f;
    };
    union
    {
        __IO uint32_t STFLR;
        stc_tmr0_stflr_field_t STFLR_f;
    };
}amhw_hc32f460_tim_t;

/**
 * \brief ARR-16位模式重载寄存器         得到当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return ARR重载寄存器的值
 */
am_static_inline
uint16_t amhw_hc32f460_tim_arr_count_get (amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan)
{
//    return p_hw_tim->arr;
    uint16_t arr = 0;

    if (TIM0_CHANNEL_A == chan)
    {
        arr = p_hw_tim->CMPAR;
    }
    else
    {
        arr = p_hw_tim->CMPBR;
    }
    return arr;
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
void amhw_hc32f460_tim_arr_count_set (amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan, uint16_t value)
{
//    p_hw_tim->arr = value;
    if (TIM0_CHANNEL_A == chan)
    {
        p_hw_tim->CMPAR = value;
    }
    else
    {
        p_hw_tim->CMPBR = value;
    }
}

/**
 * \brief CNT-16位模式计数寄存器         得到当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return CNT计数寄存器的值
 */
am_static_inline
uint16_t amhw_hc32f460_tim_cnt16_count_get (amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan)
{
//    return p_hw_tim->cnt;
    uint16_t cnt = 0;
    if (TIM0_CHANNEL_A == chan)
    {
        cnt = p_hw_tim->CNTAR;
    }
    else
    {
        cnt = p_hw_tim->CNTBR;
    }
    return cnt;
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
void amhw_hc32f460_tim_cnt16_count_set (amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan, uint16_t value)
{
//    p_hw_tim->cnt = value;
    if (TIM0_CHANNEL_A == chan)
    {
        p_hw_tim->CNTAR = value;
    }
    else
    {
        p_hw_tim->CNTBR = value;
    }
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
void amhw_hc32f460_tim_disable (amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan)
{
//    p_hw_tim->mxcr_t.m0cr &= ~(0x1ul << 0);
    if (TIM0_CHANNEL_A == chan)
    {
        p_hw_tim->BCONR_f.CSTA = 0;
    }
    else
    {
        p_hw_tim->BCONR_f.CSTB = 0;
    }
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
void amhw_hc32f460_tim_enable (amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan)
{
//    p_hw_tim->mxcr_t.m0cr |= (0x1ul << 0);
    if (TIM0_CHANNEL_A == chan)
    {
        p_hw_tim->BCONR_f.CSTA = 1;
    }
    else
    {
        p_hw_tim->BCONR_f.CSTB = 1;
    }
}

/**
 * \brief 内部时钟分频选择
 */
typedef enum {
    AMHW_HC32F460_TIM_CLK_DIV0 = 0,    /* 1分频 */
    AMHW_HC32F460_TIM_CLK_DIV2,        /* 2分频 */
    AMHW_HC32F460_TIM_CLK_DIV4,        /* 4分频 */
    AMHW_HC32F460_TIM_CLK_DIV8,        /* 8分频 */
    AMHW_HC32F460_TIM_CLK_DIV16,       /* 16分频 */
    AMHW_HC32F460_TIM_CLK_DIV32,       /* 32分频 */
    AMHW_HC32F460_TIM_CLK_DIV64,       /* 64分频 */
    AMHW_HC32F460_TIM_CLK_DIV128,      /* 128分频 */
    AMHW_HC32F460_TIM_CLK_DIV256,      /* 256分频 */
    AMHW_HC32F460_TIM_CLK_DIV512,      /* 512分频 */
    AMHW_HC32F460_TIM_CLK_DIV1024,     /* 1024分频 */
}amhw_hc32f460_tim_clkdiv_t;

/**
 * \brief PRS 内部时钟分频选择   获取
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 设置的分频系数（amhw_hc32_tim_clkdiv_t枚举量）
 */
am_static_inline
amhw_hc32f460_tim_clkdiv_t
    amhw_hc32f460_tim_mode_clkdiv_get (amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan)
{
    amhw_hc32f460_tim_clkdiv_t clkdiv;

    if (TIM0_CHANNEL_A == chan)
    {
        clkdiv = (amhw_hc32f460_tim_clkdiv_t)p_hw_tim->BCONR_f.CKDIVA;
    }
    else
    {
        clkdiv = (amhw_hc32f460_tim_clkdiv_t)p_hw_tim->BCONR_f.CKDIVB;
    }
    return clkdiv;
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
void amhw_hc32f460_tim_mode_clkdiv_set (amhw_hc32f460_tim_t       *p_hw_tim,
                                      uint8_t  chan, 
                                      amhw_hc32f460_tim_clkdiv_t clkdiv)
{
//    p_hw_tim->mxcr_t.m0cr = (p_hw_tim->mxcr_t.m0cr & (~(0x7ul << 4))) |
//                            ((clkdiv & 0x7ul) << 4);
    if (TIM0_CHANNEL_A == chan)
    {
        p_hw_tim->BCONR_f.CKDIVA = clkdiv;
    }
    else
    {
        p_hw_tim->BCONR_f.CKDIVB = clkdiv;
    }
}

/**
 * \brief UIE 中断禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_tim_int_disable (amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan)
{
//    p_hw_tim->mxcr_t.m0cr &= ~(0x1ul << 10);
    if (TIM0_CHANNEL_A == chan)
    {
        p_hw_tim->BCONR_f.INTENA = 0;
    }
    else
    {
        p_hw_tim->BCONR_f.INTENB = 0;
    }
}

/**
 * \brief UIE 中断使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_tim_int_enable (amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan)
{
//    p_hw_tim->mxcr_t.m0cr |= (0x1ul << 10);
    if (TIM0_CHANNEL_A == chan)
    {
        p_hw_tim->BCONR_f.INTENA = 1;
    }
    else
    {
        p_hw_tim->BCONR_f.INTENB = 1;
    }
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
am_bool_t amhw_hc32f460_tim_int_flag_check(amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan)
{
//    return (p_hw_tim->ifr & 0x1ul) ? AM_TRUE : AM_FALSE;
    uint8_t value = 0;
    if(TIM0_CHANNEL_A == chan)
    {
        value = p_hw_tim->STFLR_f.CMAF;
    }
    else
    {
        value = p_hw_tim->STFLR_f.CMBF;
    }
    return value ? AM_TRUE : AM_FALSE;
}

/**
 * \brief UIF 溢出中断 标志清除
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_tim_int_flag_clr(amhw_hc32f460_tim_t *p_hw_tim, uint8_t chan)
{
//    p_hw_tim->iclr &= ~(0x1ul << 0);
    if(TIM0_CHANNEL_A == chan)
    {
        p_hw_tim->STFLR_f.CMAF =0u;
        while(0u != p_hw_tim->STFLR_f.CMAF)
        {
            ;
        }
    }
    else
    {
        p_hw_tim->STFLR_f.CMBF = 0u;
        while(0u != p_hw_tim->STFLR_f.CMBF)
        {
            ;
        }
    }
}

/**
 * \brief 使用匿名联合体段结束
 * @{
 */

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
/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_TIM_H */

/* end of file */
