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
 * \brief WWDG操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-13  sdy, first implementation
 * \endinternal
 */

#ifndef __AMHW_HK32F103RBT6_WWDG_H
#define __AMHW_HK32F103RBT6_WWDG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/*
 * \brief 匿名结构体段的开始
 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_hk32f103rbt6_if_wwdg
 * \copydoc amhw_hk32f103rbt6_wwdg.h
 * @{
 */

/**
 * \brief WWDG寄存器块结构体
 */
typedef struct amhw_hk32f103rbt6_wwdg {
    __IO uint32_t cr;    /**< \brief 控制寄存器 */
    __IO uint32_t cfr;   /**< \brief 配置寄存器 */
    __IO uint32_t sr;    /**< \brief 状态寄存器 */
} amhw_hk32f103rbt6_wwdg_t;

#ifndef AMHW_HK32F103RBT6_WWDG
#define AMHW_HK32F103RBT6_WWDG  ((amhw_hk32f103rbt6_wwdg_t *)0x40002C00UL) /**< \brief 外设基地址 */
#endif

/**
 * \brief 写计数值
 *
 * \param[in] p_hw_wwdg : 指向WWDG寄存器块的指针
 * \param[in] count     : 值
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_wwdg_counter_set (amhw_hk32f103rbt6_wwdg_t *p_hw_wwdg,
                                   uint8_t             count)
{
    p_hw_wwdg->cr = (p_hw_wwdg->cr & ~0x7ful) |
                    (count & 0x7ful);
}

/**
 * \brief 使能窗口看门狗
 *
 * \param[in] p_hw_wwdg : 指向WWDG寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_wwdg_enable (amhw_hk32f103rbt6_wwdg_t *p_hw_wwdg)
{
    p_hw_wwdg->cr |= (1ul << 7);
}

/**
 * \brief 写窗口值
 *
 * \param[in] p_hw_wwdg : 指向WWDG寄存器块的指针
 * \param[in] key       : 值
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_wwdg_winvalue_set (amhw_hk32f103rbt6_wwdg_t    *p_hw_wwdg,
                                 uint8_t             key)
{
    p_hw_wwdg->cfr = (p_hw_wwdg->cfr & ~0x7ful) |
                     (key & 0x7ful);
}

/**
 * \brief 写时基值
 *
 * \param[in] p_hw_wwdg : 指向WWDG寄存器块的指针
 * \param[in] val       : 值
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_wwdg_timerbase_set (amhw_hk32f103rbt6_wwdg_t    *p_hw_wwdg,
                                  uint8_t             val)
{
    p_hw_wwdg->cfr = (p_hw_wwdg->cfr & ~(0x3ul << 7)) |
                     ((val & 0x3ul) << 7);
}

/**
 * \brief 提前唤醒中断
 *
 * \param[in] p_hw_wwdg : 指向WWDG寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_wwdg_ewi_set (amhw_hk32f103rbt6_wwdg_t *p_hw_wwdg)
{
    p_hw_wwdg->cfr |= (1ul << 9);
}

/**
 * \brief 提前唤醒中断标志获取
 *
 * \param[in] p_hw_wwdg : 指向WWDG寄存器块的指针
 *
 * \return 无
 */
am_static_inline
am_bool_t amhw_hk32f103rbt6_wwdg_ewif_read (amhw_hk32f103rbt6_wwdg_t *p_hw_wwdg)
{
    return (am_bool_t)((p_hw_wwdg->sr & 1ul) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 提前唤醒中断标志清除
 *
 * \param[in] p_hw_wwdg : 指向WWDG寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_wwdg_ewif_clear (amhw_hk32f103rbt6_wwdg_t *p_hw_wwdg)
{
    p_hw_wwdg->sr &= ~1ul;
}

/**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __AMHW_HK32F103RBT6_WWDG_H */

#endif

/* end of file */
