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
 * \brief SWDT操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-11
 * \endinternal
 */

#ifndef __AMHW_HC32F460_SWDT_H
#define __AMHW_HC32F460_SWDT_H

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
 * \addtogroup amhw_hc32f460_if_swdt
 * \copydoc amhw_hc32f460_swdt.h
 * @{
 */

/**
 * \brief SWDT 状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CNT                       :16;  /**< \brief 计数值*/
    __IO uint32_t UDF                       : 1;  /**< \brief 计数下溢标志*/
    __IO uint32_t REF                       : 1;  /**< \brief 刷新错误标志*/
    uint32_t RESERVED18                     :14;  /**< \brief 保留*/
} stc_swdt_sr_field_t;

/**
 * \brief SWDT 刷新寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t RF                        :16;  /**< \brief 刷新值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_swdt_rr_field_t;

/**
 * \brief SWDT寄存器块结构体
 */
typedef struct
{
    uint8_t RESERVED0[4];        /**< \brief 保留*/
    union
    {
        __IO uint32_t SR;        /**< \brief 状态寄存器*/
        stc_swdt_sr_field_t SR_f;
    };
    union
    {
        __IO uint32_t RR;        /**< \brief 刷新寄存器*/
        stc_swdt_rr_field_t RR_f;
    };
}amhw_hc32f460_swdt_t;


/**
 * \brief 获取计数器当前计数值
 *
 * \param[in] p_hw_swdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
uint16_t amhw_hc32f460_swdt_count_value_get (amhw_hc32f460_swdt_t  *p_hw_swdt)
{
    return p_hw_swdt->SR_f.CNT;
}

typedef enum {
    AMHW_HC32F460_SWDT_STATUS_REF   = 1 << 17,   /**< \brief 刷新错误标志 */
    AMHW_HC32F460_SWDT_STATUS_UDF   = 1 << 16,   /**< \brief 计数下溢标志 */
} amhw_hc32f460_swdt_status_flag_t;

/**
 * \brief 获取看门狗状态
 *
 * \param[in] p_hw_swdt : 指向WDT寄存器块的指针
 * \param[in] flag     : 状态标志
 *
 * \return 无
 */
am_static_inline
am_bool_t amhw_hc32f460_swdt_status_get (amhw_hc32f460_swdt_t             *p_hw_swdt,
                                         amhw_hc32f460_swdt_status_flag_t  flag)
{
    return (p_hw_swdt->SR & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除看门狗状态
 *
 * \param[in] p_hw_swdt : 指向WDT寄存器块的指针
 * \param[in] flag     : 状态标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_swdt_status_clr (amhw_hc32f460_swdt_t             *p_hw_swdt,
                                    amhw_hc32f460_swdt_status_flag_t  flag)
{
    p_hw_swdt->SR &= ~flag;
}

typedef enum {
    AMHW_HC32F460_SWDT_REFRESH_START = 0x0123, /**< \brief 刷新计数器要写入的第一个值 */
    AMHW_HC32F460_SWDT_REFRESH_END   = 0x3210, /**< \brief 刷新计数器要写入的第二个值 */
} amhw_hc32f460_swdt_refresh_set_t;

/**
 * \brief 刷新看门狗计数器
 *
 * \param[in] p_hw_swdt : 指向WDT寄存器块的指针
 * \param[in] flag      : 刷新计数器要写入的特定值（amhw_hc32f460_swdt_refresh_set_t）
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_swdt_refresh_set (amhw_hc32f460_swdt_t             *p_hw_swdt,
                                     amhw_hc32f460_swdt_refresh_set_t  flag)
{
    p_hw_swdt->RR_f.RF = flag;
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
#endif /* __AMHW_HC32F460_WDT_H */

#endif

/* end of file */
