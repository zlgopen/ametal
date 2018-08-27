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
 * \brief 低功耗定时器定时器（LPTMR）操作接口
 *
 * 1. 16位定时计数器或带有比较功能的脉冲计数器：
 *    -可选的中断可以从任何低功耗模式产生异步唤醒
 *    -硬件触发输出
 *    -计数器支持自由运行模式或在比较时复位
 * 2. 预分频器/故障过滤器的可配置的时钟源;
 * 3. 配置脉冲计数器的输入源：
 *    -上升沿/下降沿
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_LPTMR_H
#define __AMHW_FSL_LPTMR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_fsl_if_lptmr
 * \copydoc amhw_fsl_lptmr.h
 * @{
 */


/**
 * \brief LPTMR 寄存器块结构体
 */
typedef struct amhw_fsl_lptmr {
    __IO uint32_t ctl_stat;         /**< \brief 模块控制寄存器 */
    __IO uint32_t prescale;         /**< \brief 预分频寄存器 */
    __IO uint32_t compare;          /**< \brief 比较寄存器 */
    __IO uint32_t counter;          /**< \brief 计数寄存器 */
} amhw_fsl_lptmr_t;


/**
 * \name 状态和控制寄存器位定义(R/W)
 * @{
 */

#define AMHW_FSL_LPTMR_CSR_MASK    (0xFFU)
#define AMHW_FSL_LPTMR_CSR_TCF     (1UL << 7)         /**< \brief 定时器比较标志 */
#define AMHW_FSL_LPTMR_CSR_TIE     (1UL << 6)         /**< \brief 定时器中断使能 */
#define AMHW_FSL_LPTMR_CSR_TPS(s)  (((s) & 0x3) << 4) /**< \brief 定时器引脚选择 */
#define AMHW_FSL_LPTMR_CSR_TPP     (1UL << 3)         /**< \brief 定时器引脚极性 */
#define AMHW_FSL_LPTMR_CSR_TFC     (1UL << 2)         /**< \brief 定时器自由运行数值 */
#define AMHW_FSL_LPTMR_CSR_TMS     (1UL << 1)         /**< \brief 定时器极性选择 */
#define AMHW_FSL_LPTMR_CSR_TEN     (1UL << 0)         /**< \brief 定时器使能 */

/** @} */

/**
 * \brief 控制寄存器设定
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \param[in] flags      : AMHW_FSL_LPTMR_CSR_* 宏值或多个
 *                         AMHW_FSL_LPTMR_CSR_*宏的 或（OR）值(#AMHW_FSL_LPTMR_CSR_TEN)
 * \return 无
 */
am_static_inline
void amhw_fsl_lptmr_ctl_set (amhw_fsl_lptmr_t *p_hw_lptmr, uint32_t flags)
{
    p_hw_lptmr->ctl_stat |= flags;
}

/**
 * \brief 控制寄存器设定
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \param[in] flags      : AMHW_FSL_LPTMR_CSR_* 宏值或多个
 *                         AMHW_FSL_LPTMR_CSR_*宏的 或（OR）值(#AMHW_FSL_LPTMR_CSR_TEN)
 * \return 无
 */
am_static_inline
void amhw_fsl_lptmr_ctl_clear (amhw_fsl_lptmr_t *p_hw_lptmr, uint32_t flags)
{
    p_hw_lptmr->ctl_stat &= ~flags;
}

/**
 * \brief 控制寄存器设定
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 *
 * \retval AM_TRUE  : 比较标志已置位
 * \retval AM_FALSE : 比较标志未置位
 * \note 判断时不要用if(amhw_fsl_lptmr_flag_check()==AM_TRUE)，
 *               应该用if(amhw_fsl_lptmr_flag_check())
 */
am_static_inline
am_bool_t amhw_fsl_lptmr_flag_check (amhw_fsl_lptmr_t *p_hw_lptmr)
{
    return (am_bool_t)(p_hw_lptmr->ctl_stat & AMHW_FSL_LPTMR_CSR_TCF);
}

/**
 * \name 预分频寄存器位定义(R/W)
 * @{
 */

#define AMHW_FSL_LPTMR_PSR_TCF(s)  (((s) & 0xf) << 3) /**< \brief 预分频值选择 */
#define AMHW_FSL_LPTMR_PSR_PBYP    (1UL << 2)         /**< \brief 预分频器旁路 */
#define AMHW_FSL_LPTMR_PSR_PCS(s)  (((s) & 0x3) << 0) /**< \brief LPTMR时钟源选择 */

/** @} */

/**
 * \brief LPTMR 预分频因子选择
 * \note 该位域被保护，仅当定时器被禁止时才可修改
 */
typedef enum amhw_fsl_lptmr_ps {
    AMHW_FSL_LPTMR_DIVIDED_BY_2     = 0x0,    /**< \brief 预分频因子为2 */
    AMHW_FSL_LPTMR_DIVIDED_BY_4     = 0x1,    /**< \brief 预分频因子为4 */
    AMHW_FSL_LPTMR_DIVIDED_BY_8     = 0x2,    /**< \brief 预分频因子为8 */
    AMHW_FSL_LPTMR_DIVIDED_BY_16    = 0x3,    /**< \brief 预分频因子为16 */
    AMHW_FSL_LPTMR_DIVIDED_BY_32    = 0x4,    /**< \brief 预分频因子为32 */
    AMHW_FSL_LPTMR_DIVIDED_BY_64    = 0x5,    /**< \brief 预分频因子为64 */
    AMHW_FSL_LPTMR_DIVIDED_BY_128   = 0x6,    /**< \brief 预分频因子为128 */
    AMHW_FSL_LPTMR_DIVIDED_BY_256   = 0x7,    /**< \brief 预分频因子为256 */
    AMHW_FSL_LPTMR_DIVIDED_BY_512   = 0x8,    /**< \brief 预分频因子为512 */
    AMHW_FSL_LPTMR_DIVIDED_BY_1024  = 0x9,    /**< \brief 预分频因子为1024 */
    AMHW_FSL_LPTMR_DIVIDED_BY_2048  = 0xA,    /**< \brief 预分频因子为2048 */
    AMHW_FSL_LPTMR_DIVIDED_BY_4096  = 0xB,    /**< \brief 预分频因子为4096 */
    AMHW_FSL_LPTMR_DIVIDED_BY_8192  = 0xC,    /**< \brief 预分频因子为8192 */
    AMHW_FSL_LPTMR_DIVIDED_BY_16384 = 0xD,    /**< \brief 预分频因子为16384 */
    AMHW_FSL_LPTMR_DIVIDED_BY_32768 = 0xE,    /**< \brief 预分频因子为32768 */
    AMHW_FSL_LPTMR_DIVIDED_BY_65536 = 0xF,    /**< \brief 预分频因子为65536 */
} amhw_fsl_lptmr_ps_t;

/**
 * \brief LPTMR时钟源选择
 */
typedef enum amhw_fsl_lptmr_clock_src
{
    AMHW_FSL_LPTMR_CLOCK_SRC_MCGIRCLK = 0x0,    /**< \brief MCG 输出时钟  */
    AMHW_FSL_LPTMR_CLOCK_SRC_LPOCLK,            /**< \brief LPO 时钟 */
    AMHW_FSL_LPTMR_CLOCK_SRC_ER32KCLK,          /**< \brief ER32K 时钟 */
    AMHW_FSL_LPTMR_CLOCK_SRC_OSCERCLK,          /**< \brief OSCERCLK 时钟 */
} amhw_fsl_lptmr_clock_src_t;

/**
 * \brief 获取预分频寄存器内容
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \return 预分频寄存器内容
 * \note 该寄存器位定义：[6:3]分频值选择；[2]旁路使能；[1:0]时钟源选择
 */
am_static_inline
uint32_t amhw_fsl_lptmr_ps_reg_get (amhw_fsl_lptmr_t *p_hw_lptmr)
{
    return p_hw_lptmr->prescale;
}

/**
 * \brief 预分频因子设定
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \param[in] val        : 需要写入的值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_lptmr_prescaler_set (amhw_fsl_lptmr_t    *p_hw_lptmr,
                                   amhw_fsl_lptmr_ps_t  val)
{
    AM_BITS_SET(p_hw_lptmr->prescale, 3, 4, val);
}

/**
 * \brief 预分频过滤器设定
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \param[in] b_flag     : 使能或禁能
 *                          -AM_TRUE 分频器被旁路，分频器无效
 *                          -AM_FALSE 使能分频器，分频器有效
 * \return 无
 */
am_static_inline
void amhw_fsl_lptmr_prescaler_filter_set (amhw_fsl_lptmr_t *p_hw_lptmr,
                                          am_bool_t             b_flag)
{
    if (b_flag) {
        p_hw_lptmr->prescale |= AMHW_FSL_LPTMR_PSR_PBYP;
    } else {
        p_hw_lptmr->prescale &= ~AMHW_FSL_LPTMR_PSR_PBYP;
    }
}

/**
 * \brief 过滤时钟设定
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \param[in] clk        : LPTMR时钟源选择，枚举类型amhw_fsl_lptmr_clock_src_t成员
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_lptmr_clock_set (amhw_fsl_lptmr_t           *p_hw_lptmr,
                               amhw_fsl_lptmr_clock_src_t  clk)
{
    AM_BITS_SET(p_hw_lptmr->prescale, 0, 2, clk);
}


/**
 * \brief 时钟源获取
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \param[in] clk        : LPTMR时钟源选择，枚举类型amhw_fsl_lptmr_clock_src_t成员
 *
 * \return 无
 */
am_static_inline
uint32_t amhw_fsl_lptmr_clock_get (amhw_fsl_lptmr_t           *p_hw_lptmr)
{
    return AM_BITS_GET(p_hw_lptmr->prescale, 0, 2);
}

/**
 * \name 比较寄存器位定义(R/W)
 * @{
 */

#define AMHW_FSL_LPTMR_CMR_MASK     (0xFFFFu)    /**< \brief 预分频值选择 */

/** @} */

/**
 * \brief 比较器设定
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \param[in] val        : 需要写入的值
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_lptmr_compare_set (amhw_fsl_lptmr_t *p_hw_lptmr,
                                 uint32_t          val)
{
    p_hw_lptmr->compare = val;
}

/**
 * \brief 比较器获取
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \return 比较器值
 */
am_static_inline
uint16_t amhw_fsl_lptmr_compare_get (amhw_fsl_lptmr_t *p_hw_lptmr)
{
    return p_hw_lptmr->compare;
}

/**
 * \name 计数值寄存器位定义(R/W)
 * @{
 */

#define AMHW_FSL_LPTMR_CNR_MASK  (0xFFFFu)      /**< \brief 预分频值选择 */

/** @} */

/**
 * \brief 定时器获取
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \return 定时器值
 */
am_static_inline
uint16_t amhw_fsl_lptmr_counter_get (amhw_fsl_lptmr_t *p_hw_lptmr)
{
    /* 读值之前先写一个垃圾值才可以读 */
    p_hw_lptmr->counter = 0;

    return p_hw_lptmr->counter;
}

/**
 * @} amhw_fsl_if_lptmr
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_LPTMR_H */

/* end of file */

