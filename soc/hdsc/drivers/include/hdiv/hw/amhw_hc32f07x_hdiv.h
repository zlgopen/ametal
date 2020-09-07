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
 * \brief HDIV Peripheral Access Layer
 *
 *   The Miscellaneous System Control Module (FTFC) contains CPU configuration
 *   registers and on-chip memory controller registers.
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-04  zp, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_HC32F07X_HDIV_H
#define __AMHW_HC32F07X_HDIV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_hc32f07x_if_hdiv
 * \copydoc amhw_hc32f07x_hdiv.h
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

/** @} */

/*
 * \brief the structure of hidv register
 */
typedef struct amhw_hc32f07x_hdiv {
    __IO uint32_t divdend;   /**< \brief 被除数寄存器           offset : 0x00 */
    __IO uint32_t divisor;   /**< \brief 除数寄存器             offset : 0x04 */
    __IO uint32_t quotient;  /**< \brief 商寄存器              offset : 0x08 */
    __IO uint32_t remainder; /**< \brief 余数寄存器           offset : 0x0c */
    __IO uint32_t sign;      /**< \brief 符号寄存器                offset : 0x10 */
    __IO uint32_t stat;      /**< \brief 状态寄存器                offset : 0x14 */
} amhw_hc32f07x_hdiv_t;



typedef struct amhw_hc32f07x_hdiv_unsigned {
    uint32_t undividend;
    uint32_t undivisor;
    uint32_t quotient;
    uint32_t remainder;	
} amhw_hc32f07x_hdiv_unsigned_t;

typedef struct amhw_hc32f07x_hdiv_signed {
    int32_t dividend;
    int32_t divisor;
    int32_t quotient;
    int32_t remainder;	
} amhw_hc32f07x_hdiv_signed_t;

/**
 * \brief hdiv 有符号除法运算设置
 *
 * \param[in] p_hw_hdiv : 指向hdiv寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_hdiv_signed_set (amhw_hc32f07x_hdiv_t *p_hw_hdiv)
{
    p_hw_hdiv->sign |= (0x1ul << 0);
}

/**
 * \brief hdiv 无符号除法运算设置
 *
 * \param[in] p_hw_hdiv : 指向hdiv寄存器的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_hdiv_unsigned_set (amhw_hc32f07x_hdiv_t *p_hw_hdiv)
{
    p_hw_hdiv->sign &= ~(0x1ul << 0);
}

/**
 * \brief hdiv 除数为零警告标志获取
 *
 * \param[in] p_hw_hdiv : 指向hdiv寄存器的指针
 *
 * \return    AM_TRUE  : 除数为零
 *            AM_FALSE : 除数不为零
 */
am_static_inline
am_bool_t amhw_hc32f07x_hdiv_zerostat_get (amhw_hc32f07x_hdiv_t *p_hw_hdiv)
{
    return (am_bool_t)((p_hw_hdiv->stat >> 1)&0x01);
}

/**
 * \brief hdiv 运算结束标志获取
 *
 * \param[in] p_hw_hdiv : 指向hdiv寄存器的指针
 *
 * \return    AM_TRUE  : 运算结束
 *            AM_FALSE : 运算进行中
 */
am_static_inline
am_bool_t amhw_hc32f07x_hdiv_endstat_get (amhw_hc32f07x_hdiv_t *p_hw_hdiv)
{
    return (am_bool_t)((p_hw_hdiv->stat >> 0)&0x01);
}

/**
* \brief    HDIV 无符号除法
*
* \param[in] p_hw_hdiv : 指向hdiv寄存器的指针
* \param[in] p_divd    ：参数结构体
*
* \retval  AM_OK       : 运算成功
* \retval  -AM_EINVAL  : 参数无效
*/
int am_hc32f07x_hdiv_unsigned(amhw_hc32f07x_hdiv_t *p_hw_hdiv, 
	                            amhw_hc32f07x_hdiv_unsigned_t *p_undivd);

/**
* \brief    HDIV 有符号除法
*
* \param[in] p_hw_hdiv : 指向hdiv寄存器的指针
* \param[in] p_divd    ：参数结构体
*
* \retval  AM_OK       : 运算成功
* \retval  -AM_EINVAL  : 参数无效
*/
int am_hc32f07x_hdiv_signed(amhw_hc32f07x_hdiv_t *p_hw_hdiv, 
	                            amhw_hc32f07x_hdiv_signed_t *p_divd);

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

#endif /* __AMHW_HC32F07X_HDIV_H */

/* end of file */
