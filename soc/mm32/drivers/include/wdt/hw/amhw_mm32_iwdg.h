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
 * \brief IWDG操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-13  sdy, first implementation
 * \endinternal
 */

#ifndef __AMHW_MM32_IWDG_H
#define __AMHW_MM32_IWDG_H

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
 * \addtogroup amhw_mm32_if_iwdg
 * \copydoc amhw_mm32_iwdg.h
 * @{
 */

/**
 * \brief IWDG寄存器块结构体
 */
typedef struct amhw_mm32_iwdg {
    __IO uint32_t kr;  /**< \brief 键寄存器 */
    __IO uint32_t pr;  /**< \brief 预分频寄存器 */
    __IO uint32_t rlr; /**< \brief 重装载寄存器 */
    __IO uint32_t sr;  /**< \brief 状态寄存器 */
} amhw_mm32_iwdg_t;

/**
 * \brief 写键值
 *
 * \param[in] p_hw_iwdg : 指向IWDG寄存器块的指针
 * \param[in] key       : 键值
 *
 *      - 0x5555 ：允许访问 IWDG_PR 和 IWDG_RLR 寄存器
 *      - 0xAAAA ：喂狗
 *      - 0xCCCC ：启动看门狗工作
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_iwdg_keyvalue_set (amhw_mm32_iwdg_t    *p_hw_iwdg,
                                 uint16_t            key)
{
    p_hw_iwdg->kr = key;
}

/**
 * \brief 设置预分频值
 *
 * \param[in] p_hw_iwdg : 指向IWDG寄存器块的指针
 * \param[in] div       : 分频值
 *
 *      - 0 ：预分频因子  = 4
 *      - 1 ：预分频因子  = 8
 *      - 2 ：预分频因子  = 16
 *      - 3 ：预分频因子  = 32
 *      - 4 ：预分频因子  = 64
 *      - 5 ：预分频因子  = 128
 *      - 6 ：预分频因子  = 256
 *      - 7 ：预分频因子  = 256
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_iwdg_div_set (amhw_mm32_iwdg_t    *p_hw_iwdg,
                            uint8_t             div)
{
    p_hw_iwdg->pr = (div & 0x7ul);
}

/**
 * \brief 设置重载装值
 *
 * \param[in] p_hw_iwdg : 指向IWDG寄存器块的指针
 * \param[in] val       : 重载值
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_iwdg_reload_set (amhw_mm32_iwdg_t    *p_hw_iwdg,
                               uint16_t            val)
{
    p_hw_iwdg->rlr = (val & 0xffful);
}

/**
 * \brief 看门狗状态更新
 *
 * \param[in] p_hw_iwdg : 指向IWDG寄存器块的指针
 *
 * \return 状态值（各个状态是或的关系）
 *
 *        - 0 ： 无状态
 *        - 1 ： 看门狗预分频更新
 *        - 2 ： 看门狗计数器重装载值更新
 */
am_static_inline
uint8_t amhw_mm32_iwdg_status_get (amhw_mm32_iwdg_t *p_hw_iwdg)
{
   return (uint8_t)(p_hw_iwdg->sr & 0x3);
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
#endif /* __AMHW_MM32_IWDG_H */

#endif

/* end of file */
