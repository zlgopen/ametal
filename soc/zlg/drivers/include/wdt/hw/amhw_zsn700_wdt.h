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
 * \brief WDT操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10
 * \endinternal
 */

#ifndef __AMHW_ZSN700_WDT_H
#define __AMHW_ZSN700_WDT_H

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
 * \addtogroup amhw_zsn700_if_wdt
 * \copydoc amhw_zsn700_wdt.h
 * @{
 */

/**
 * \brief WDT寄存器块结构体
 */
typedef struct amhw_zsn700_wdt {
    __O  uint32_t rst;  /**< \brief 清除控制寄存器 */
    __IO uint32_t con;  /**< \brief 控制寄存器 */
} amhw_zsn700_wdt_t;


/**
 * \brief 看门狗启动/清零控制
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] count    : 值
 *
 * \return 无
 */
am_static_inline
void amhw_zsn700_wdt_rst_set (amhw_zsn700_wdt_t *p_hw_wdt,
                              uint8_t             count)
{
    p_hw_wdt->rst = count;
}

/**
 * \brief 看门狗中断标志获取
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 0：未发生WDT中断, 1：已发生WDT中断
 */
am_static_inline
uint8_t amhw_zsn700_rcc_int_get (amhw_zsn700_wdt_t *p_hw_wdt)
{
    return ((p_hw_wdt->con >> 7) & 1);
}

/**
 * \brief 看门狗溢出后动作配置
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] en       : 动作标志
 *
 * \return 无
 */
am_static_inline
void amhw_zsn700_wdt_en_set (amhw_zsn700_wdt_t *p_hw_wdt,
                             uint8_t            en)
{
    p_hw_wdt->con = (p_hw_wdt->con & ~(1 << 5)) | en;
}

/**
 * \brief 看门狗运行状态获取
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 0：WDT停止, 1：WDT正在运行
 */
am_static_inline
uint8_t amhw_zsn700_wdt_stat_get (amhw_zsn700_wdt_t *p_hw_wdt)
{
    return ((p_hw_wdt->con >> 4) & 1);
}

/**
 * \brief 看门狗溢出时间配置
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] time     : 溢出时间
 *
 * \return 无
 */
am_static_inline
void amhw_zsn700_wdt_wov_set (amhw_zsn700_wdt_t *p_hw_wdt,
                              uint8_t            time)
{
    p_hw_wdt->con = (p_hw_wdt->con & ~0xf) | time;
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
#endif /* __AMHW_ZSN700_WDT_H */

#endif

/* end of file */
