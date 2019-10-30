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
 * \brief LPMODE Peripheral Access Layer
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-15  zp, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_ZLG118_LPMODE_H
#define __AMHW_ZLG118_LPMODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zlg118_if_lpmode
 * \copydoc amhw_zlg118_lpmode.h
 * @{
 */

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

#if   defined ( __CC_ARM )
  #define __ASM            __asm         /*!< asm keyword for ARM Compiler */
  #define __INLINE         __inline      /*!< inline keyword for ARM Compiler */
  #define __STATIC_INLINE  static __inline
#endif
#if defined ( __GNUC__ )
  #define __ASM            __asm         /*!< asm keyword for GNU Compiler */
  #define __INLINE         inline        /*!< inline keyword for GNU Compiler */
  #define __STATIC_INLINE  static inline
#endif

#include "core_cmInstr.h"

/** @} */

/**
 * \brief LPMODE - Register Layout Typedef
 */
typedef struct amhw_zlg118_lpmode {
    __IO  uint32_t cr;       /**< \brief 系统控制寄存器  */
} amhw_zlg118_lpmode_t;

#define LPMODECTRL ((amhw_zlg118_lpmode_t *)0xE000ED10)

/**
 * \brief 中断挂起事件控制
 *
 * \param[in] stata : AM_TRUE  ：每次新的中断挂起都会产生一个事件
 *                    AM_FALSE ：每次新的中断挂起都不会产生一个事件
 *
 * \return none
 */
am_static_inline
void amhw_zlg118_lpmode_sevonpend (am_bool_t stata)
{
    LPMODECTRL->cr = (LPMODECTRL->cr & (~(1ul << 4))) |
                     (stata << 4);
}

/**
 * \brief 深度睡眠模式控制
 *
 * \param[in] stata : AM_TRUE  ：执行 WFI进入深度休眠，进入Deepsleep模式
 *                    AM_FALSE ：执行 WFI进入休眠，进入sleep/Idle模式
 *
 * \return none
 */
am_static_inline
void amhw_zlg118_lpmode_sleepdeep (am_bool_t stata)
{
    LPMODECTRL->cr = (LPMODECTRL->cr & (~(1ul << 2))) |
                     (stata << 2);
}

/**
 * \brief 自动进入睡眠模式控制
 *
 * \param[in] stata : AM_TRUE  ：当退出异常处理并返回程序线程时，自动进入休眠模式
 *                    AM_FALSE ：执行 WFI指令，进入休眠模式
 *
 * \return none
 */
am_static_inline
void amhw_zlg118_lpmode_sleeponexit (am_bool_t stata)
{
    LPMODECTRL->cr = (LPMODECTRL->cr & (~(1ul << 1))) |
                     (stata << 1);
}

/**
 * \brief WFI指令函数
 *
 * \param[in] none
 *
 * \return none
 */
am_static_inline
void amhw_zlg118_lpmode_wfi (void)
{
#ifdef __CC_ARM
    __ASM("WFI");
#endif

#ifdef __GNUC__
    __WFI();
#endif
}

/**
 * \brief WFE指令函数
 *
 * \param[in] none
 *
 * \return none
 */
am_static_inline
void amhw_zlg118_lpmode_wfe (void)
{
#ifdef __CC_ARM
    __ASM("WFE");
#endif

#ifdef __GNUC__
    __WFE();
#endif
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

#endif /* __AMHW_ZLG118_LPMODE_H */

/* end of file */
