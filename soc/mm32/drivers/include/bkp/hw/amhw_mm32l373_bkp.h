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
 * \brief 备份寄存器接口
 *
 * \internal
 * \par History
 * - 1.00 17-08-23  lqy, first implementation
 * \endinternal
 */

#ifndef __AMHW_MM32L373_BKP_H
#define __AMHW_MM32L373_BKP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_common.h"

/**
 * \addtogroup amhw_mm32l373_if_bkp
 * \copydoc amhw_mm32l373_bkp.h
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
/**
 * @}
 */

/** \brief BKP_RTCCR register bit mask */
#define  AMHW_MM32L373_BKP_RTCCR_CAL_SET  (0x007F) /**< Calibration value */
#define  AMHW_MM32L373_BKP_RTCCR_CCO_SET  (0x0080) /**< Calibration Clock Output */
#define  AMHW_MM32L373_BKP_RTCCR_ASOE_SET (0x0100) /**< Alarm or Second Output Enable */
#define  AMHW_MM32L373_BKP_RTCCR_ASOS_SET (0x0200) /**< Alarm or Second Output Selection */

/** \brief BKP_CR register bit mask */
#define  AMHW_MM32L373_BKP_CR_TPE_SET     (0x01)   /**< TAMPER pin enable */
#define  AMHW_MM32L373_BKP_CR_TPAL_SET    (0x02)   /**< TAMPER pin active level */

/** \brief CSR register bit mask */
#define  AMHW_MM32L373_BKP_CSR_CTE_SET    (0x0001) /**< Clear Tamper event */
#define  AMHW_MM32L373_BKP_CSR_CTI_SET    (0x0002) /**< Clear Tamper Interrupt */
#define  AMHW_MM32L373_BKP_CSR_TPIE_SET   (0x0004) /**< TAMPER Pin interrupt enable */
#define  AMHW_MM32L373_BKP_CSR_TEF_SET    (0x0100) /**< Tamper Event Flag */
#define  AMHW_MM32L373_BKP_CSR_TIF_SET    (0x0200) /**< Tamper Interrupt Flag */

/**
  * \brief 备份寄存器块结构体
  */
typedef struct amhw_mm32l373_bkp {
    __I  uint32_t reserve0;  /**< \brief 保留 */
    __IO uint32_t dr[10];    /**< \brief 备份区域数据寄存器 */
    __IO uint16_t rtc_cr;    /**< \brief RTC 控制寄存器 */
    __I  uint16_t reserve1;  /**< \brief 保留 */
    __IO uint16_t cr;        /**< \brief BKP 控制寄存器 */
    __I  uint16_t reserve2;  /**< \brief 保留 */
    __IO uint16_t csr;       /**< \brief 时钟监控状态寄存器 */
} amhw_mm32l373_bkp_t;

/**
 * \brief Clears Tamper Pin Event pending flag.
 *
 * \param[in] p_hw_bkp Pointer to amhw_mm32l373_bkp_t Structure
 * \param[in] flag     see AMHW_BKP_CSR_CTE_SET or AMHW_BKP_CSR_CTI_SET
 *
 * \return None
 */
am_static_inline
void amhw_mm32l373_bkp_clear_flag (amhw_mm32l373_bkp_t *p_hw_bkp, uint16_t flag)
{

    /* Set CTE bit to clear Tamper Pin Event flag */
    p_hw_bkp->csr |= flag;
}

/**
 * \brief 写备份区域数据寄存器
 *
 * \param[in] index    备份区域数据寄存器索引，值为 0 ~ 9
 * \param[in] value    写入备份区或数据寄存器的值，值为 0 ~ 65536
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_bkp_dr_write (amhw_mm32l373_bkp_t *p_hw_bkp,
                               uint8_t            index,
                               uint16_t           value)
{
    p_hw_bkp->dr[index] = value & 0x0000FFFF;
}

/**
 * \brief 读备份区域数据寄存器
 *
 * \param[in] index    备份区域数据寄存器索引，值为 0 ~ 9
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
uint16_t amhw_mm32l373_bkp_dr_read (amhw_mm32l373_bkp_t *p_hw_bkp, uint8_t index)
{
    return p_hw_bkp->dr[index] & 0x0000FFFF;
}

/**
 * \brief RTC 时钟校准设置
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 * \param[in] index    校准值
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_bkp_rtccr_cal_set (amhw_mm32l373_bkp_t *p_hw_bkp, uint8_t cal)
{
    p_hw_bkp->rtc_cr = ((p_hw_bkp->rtc_cr & (~AMHW_MM32L373_BKP_RTCCR_CAL_SET)) |
                        (cal & AMHW_MM32L373_BKP_RTCCR_CAL_SET));
}

/**
 * \brief RTC 时钟校准获取
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
uint8_t amhw_mm32l373_bkp_rtccr_cal_get (amhw_mm32l373_bkp_t *p_hw_bkp)
{
    return (uint8_t)(p_hw_bkp->rtc_cr & AMHW_MM32L373_BKP_RTCCR_CAL_SET);
}

/**
 * \brief RTC 时钟校准输出使能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_bkp_rtccr_cco_enable (amhw_mm32l373_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr |= AMHW_MM32L373_BKP_RTCCR_CCO_SET;
}

/**
 * \brief RTC 时钟校准输出失能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_bkp_rtccr_cco_disable (amhw_mm32l373_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr &= ~AMHW_MM32L373_BKP_RTCCR_CCO_SET;
}

/**
 * \brief RTC 时钟校准输出获取
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
am_bool_t amhw_mm32l373_bkp_rtccr_cco_get (amhw_mm32l373_bkp_t *p_hw_bkp)
{
    return (p_hw_bkp->rtc_cr & AMHW_MM32L373_BKP_RTCCR_CCO_SET) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief RTC 允许输出闹钟或秒脉冲使能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_bkp_rtccr_asoe_enable (amhw_mm32l373_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr |= AMHW_MM32L373_BKP_RTCCR_ASOE_SET;
}

/**
 * \brief RTC 允许输出闹钟或秒脉冲失能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_bkp_rtccr_asoe_disable (amhw_mm32l373_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr &= ~AMHW_MM32L373_BKP_RTCCR_ASOE_SET;
}

/**
 * \brief RTC 允许输出闹钟或秒脉冲获取
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
am_bool_t amhw_mm32l373_bkp_rtccr_asoe_get (amhw_mm32l373_bkp_t *p_hw_bkp)
{
    return (p_hw_bkp->rtc_cr & AMHW_MM32L373_BKP_RTCCR_ASOE_SET) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief RTC 输出秒脉冲
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_bkp_rtccr_asos_second (amhw_mm32l373_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr |= AMHW_MM32L373_BKP_RTCCR_ASOS_SET;
}

/**
 * \brief RTC 输出闹钟脉冲
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_bkp_rtccr_asos_alarm (amhw_mm32l373_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr &= ~AMHW_MM32L373_BKP_RTCCR_ASOS_SET;
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
/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_MM32_BKP_H */

/* end of file */
