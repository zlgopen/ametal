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
 * \brief RTC interfaces for operating hardware.
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-23  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG237_RTC_H
#define __AMHW_ZLG237_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_common.h"

/**
 * \addtogroup amhw_zlg237_if_rtc
 * \copydoc amhw_zlg237_rtc.h
 * @{
 */

/**
 * \brief RTC 寄存器结构体定义
 */
typedef struct amhw_zlg237_rtc {
    __IO uint16_t crh;         /**< \brief RTC 控制寄存器高位 */
    __I  uint16_t reserve0;    /**< \brief 保留位 */
    __IO uint16_t crl;         /**< \brief RTC 控制寄存器低位 */
    __I  uint16_t reserve1;    /**< \brief 保留位 */
    __IO uint16_t prlh;        /**< \brief RTC 预分频装载寄存器高位 */
    __I  uint16_t reserve2;    /**< \brief 保留位 */
    __IO uint16_t prll;        /**< \brief RTC 预分频装载寄存器低位 */
    __I  uint16_t reserve3;    /**< \brief 保留位 */
    __IO uint16_t divh;        /**< \brief RTC 预分频器分频因子寄存器高位 */
    __I  uint16_t reserve4;    /**< \brief 保留位 */
    __IO uint16_t divl;        /**< \brief RTC 预分频器分频因子寄存器低位 */
    __I  uint16_t reserve5;    /**< \brief 保留位 */
    __IO uint16_t cnth;        /**< \brief RTC 计数器寄存器高位 */
    __I  uint16_t reserve6;    /**< \brief 保留位 */
    __IO uint16_t cntl;        /**< \brief RTC 计数器寄存器低位 */
    __I  uint16_t reserve7;    /**< \brief 保留位 */
    __IO uint16_t alrh;        /**< \brief RTC 闹钟寄存器高位 */
    __I  uint16_t reserve8;    /**< \brief 保留位 */
    __IO uint16_t alrl;        /**< \brief RTC 闹钟寄存器低位 */
    __I  uint16_t reserve9;    /**< \brief 保留位 */
} amhw_zlg237_rtc_t;

/**
 * \brief RTC 控制寄存器高位中断允许枚举
 */
typedef enum {
    AMHW_ZLG237_RTC_SECIE = 0,
    AMHW_ZLG237_RTC_ALRIE = 1,
    AMHW_ZLG237_RTC_OWIE  = 2,
} amhw_zlg237_rtc_crh_int_permit;

/**
 * \brief RTC 控制寄存器低位状态标志枚举
 */
typedef enum {
    AMHW_ZLG237_RTC_SECF  = 0,
    AMHW_ZLG237_RTC_ALRF  = 1,
    AMHW_ZLG237_RTC_OWF   = 2,
    AMHW_ZLG237_RTC_RSF   = 3,
    AMHW_ZLG237_RTC_RTOFF = 5,
} amhw_zlg237_rtc_crl_status;

/**
 * \brief 中断允许函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] permit   RTC 控制寄存器高位中断允许枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_crh_allow_int (amhw_zlg237_rtc_t              *p_hw_rtc,
                                    amhw_zlg237_rtc_crh_int_permit  permit)
{
    p_hw_rtc->crh |= (1 << permit);
}

/**
 * \brief 中断禁止函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] permit   RTC 控制寄存器高位中断允许枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_crh_forbid_int (amhw_zlg237_rtc_t              *p_hw_rtc,
                                     amhw_zlg237_rtc_crh_int_permit  permit)
{
    p_hw_rtc->crh &= ~(1 << permit);
}

/**
 * \brief 读控制寄存器低位状态位
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] status    RTC 控制寄存器低位状态标志枚举
 *
 * \retval AM_TRUE  标志位置位
 * \retval AM_FALSE 标志位未置位
 */
am_static_inline
am_bool_t amhw_zlg237_rtc_crl_read_statu (amhw_zlg237_rtc_t          *p_hw_rtc,
                                          amhw_zlg237_rtc_crl_status  status)
{
    return (am_bool_t)((p_hw_rtc->crl & (1ul << status)) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief 控制寄存器低位状态位清除
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] status   RTC 控制寄存器低位状态标志枚举
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_clr_status_clear (amhw_zlg237_rtc_t          *p_hw_rtc,
                                       amhw_zlg237_rtc_crl_status  status)
{
    p_hw_rtc->crl &= ~(1 << status);
}

/**
 * \brief 进入 RTC 配置模式
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_crl_cnf_enter (amhw_zlg237_rtc_t *p_hw_rtc)
{
    p_hw_rtc->crl |= (1 << 4);
}

/**
 * \brief 退出 RTC 配置模式
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_crl_cnf_out (amhw_zlg237_rtc_t *p_hw_rtc)
{
    p_hw_rtc->crl &= ~(1 << 4);
}

/**
 * \brief RTC 预分频装载寄存器高位写函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] divh     预分频高位
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_prlh_div_write (amhw_zlg237_rtc_t *p_hw_rtc, uint16_t divh)
{
    p_hw_rtc->prlh = (divh & 0x000F);
}

/**
 * \brief RTC 预分频装载寄存器低位写函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] divl     预分频低位
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_prll_div_write (amhw_zlg237_rtc_t *p_hw_rtc, uint16_t divl)
{
    p_hw_rtc->prll = divl;
}

/**
 * \brief RTC 预分频装载寄存器高位读函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 *
 * \return RTC 预分频寄存器高位状态值
 */
am_static_inline
uint16_t amhw_zlg237_rtc_divh_div_read (amhw_zlg237_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->divh;
}

/**
 * \brief RTC 预分频装载寄存器低位读函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 *
 * \return RTC 预分频寄存器低位状态值
 */
am_static_inline
uint16_t amhw_zlg237_rtc_divl_div_read (amhw_zlg237_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->divl;
}

/**
 * \brief RTC 计数器寄存器高位设置函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] cnth     计数器高位
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_cnth_set (amhw_zlg237_rtc_t *p_hw_rtc, uint16_t cnth)
{
    p_hw_rtc->cnth = cnth;
}

/**
 * \brief RTC 计数器寄存器低位设置函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] cntl     计数器低位
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_cntl_set (amhw_zlg237_rtc_t *p_hw_rtc, uint16_t cntl)
{
    p_hw_rtc->cntl = cntl;
}

/**
 * \brief 获取 RTC 计数器寄存器高位计数值
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 *
 * \return 计数器高位数值
 */
am_static_inline
uint16_t amhw_zlg237_rtc_cnth_get (amhw_zlg237_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->cnth;
}

/**
 * \brief 获取 RTC 计数器寄存器高位计数值
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 *
 * \return 寄存器低位数值
 */
am_static_inline
uint16_t amhw_zlg237_rtc_cntl_get (amhw_zlg237_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->cntl;
}

/**
 * \brief RTC 闹钟寄存器高位写函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] alarmh   闹钟寄存器高位
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_alrh_set (amhw_zlg237_rtc_t *p_hw_rtc, uint16_t alarmh)
{
    p_hw_rtc->alrh = alarmh;
}

/**
 * \brief RTC 闹钟寄存器低位写函数
 *
 * \param[in] p_hw_rtc 指向 amhw_zlg237_rtc_t 结构的指针
 * \param[in] alarml   闹钟寄存器低位
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_rtc_alrl_set (amhw_zlg237_rtc_t *p_hw_rtc, uint16_t alarml)
{
    p_hw_rtc->alrl = alarml;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif/* __ZLG237_RTC_H */

/* end of file */
