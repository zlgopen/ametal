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
 * \brief RAM操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-08
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_RAM_H
#define __AMHW_ZSL42x_RAM_H

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
#elif defined(__GNUC__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_zsl42x_if_ram
 * \copydoc amhw_zsl42x_ram.h
 * @{
 */

/**
 * \brief RAM寄存器块结构体
 */
typedef struct amhw_zsl42x_ram {
    __IO uint32_t cr;       /* ram配置寄存器 */
    __I  uint32_t erraddr;  /* ram出错地址寄存器 */
    __I  uint32_t ifr;      /* ram出错中断标志寄存器 */
    __IO uint32_t iclr;     /* ram出错中断标志清除寄存器 */
} amhw_zsl42x_ram_t;

/**
 * \brief 使能出错报警中断
 *
 * \param[in]  p_hw_ram  : 指向RAM寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_ram_ie_enable (amhw_zsl42x_ram_t *p_hw_ram)
{
    p_hw_ram->cr |= 0x2ul;
}

/**
 * \brief 禁能出错报警中断
 *
 * \param[in]  p_hw_ram  : 指向RAM寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_ram_ie_disable (amhw_zsl42x_ram_t *p_hw_ram)
{
    p_hw_ram->cr &= ~0x2ul;
}

/**
 * \brief 获取出错地址
 *
 * \param[in]  p_hw_ram  : 指向RAM寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
uint16_t amhw_zsl42x_ram_erraddr_get (amhw_zsl42x_ram_t *p_hw_ram)
{
    return (p_hw_ram->erraddr & 0x7f);
}

/**
 * \brief 获取出错标志
 *
 * \param[in]  p_hw_ram  : 指向RAM寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zsl42x_ram_err_get (amhw_zsl42x_ram_t *p_hw_ram)
{
    return (p_hw_ram->ifr & 0x01);
}

/**
 * \brief 清除出错标志
 *
 * \param[in]  p_hw_ram  : 指向RAM寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_ram_err_clear (amhw_zsl42x_ram_t *p_hw_ram)
{
    p_hw_ram->iclr &= ~0x01;
}

/**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__GNUC__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __AMHW_ZSL42x_RAM_H */

#endif

/* end of file */
