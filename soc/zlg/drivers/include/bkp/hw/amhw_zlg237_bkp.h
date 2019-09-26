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
 * - 1.00 19-07-23  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG237_BKP_H
#define __AMHW_ZLG237_BKP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"

/**
 * \addtogroup amhw_zlg237_if_bkp
 * \copydoc amhw_zlg237_bkp.h
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
/* Calibration value */
#define  AMHW_ZLG237_BKP_RTCCR_CAL_SET  (0x007F)

/* Calibration Clock Output */
#define  AMHW_ZLG237_BKP_RTCCR_CCO_SET  (0x0080)

/* Alarm or Second Output Enable */
#define  AMHW_ZLG237_BKP_RTCCR_ASOE_SET (0x0100)

/* Alarm or Second Output Selection */
#define  AMHW_ZLG237_BKP_RTCCR_ASOS_SET (0x0200)

/** \brief BKP_CR register bit mask */
#define  AMHW_ZLG237_BKP_CR_TPE_SET     (0x01)   /**< TAMPER pin enable */
#define  AMHW_ZLG237_BKP_CR_TPAL_SET    (0x02)   /**< TAMPER pin active level*/

/** \brief CSR register bit mask */
#define  AMHW_ZLG237_BKP_CSR_CTE_SET   (0x0001) /* Clear Tamper event */
#define  AMHW_ZLG237_BKP_CSR_CTI_SET   (0x0002) /* Clear Tamper Interrupt */
#define  AMHW_ZLG237_BKP_CSR_TPIE_SET  (0x0004) /* TAMPER Pin interrupt enable*/
#define  AMHW_ZLG237_BKP_CSR_TEF_SET   (0x0100) /* Tamper Event Flag */
#define  AMHW_ZLG237_BKP_CSR_TIF_SET   (0x0200) /* Tamper Interrupt Flag */

/**
  * \brief 备份寄存器块结构体
  */
typedef struct amhw_zlg237_bkp {
    __I  uint32_t reserve0;  /**< \brief 保留 */
    __IO uint32_t dr[10];    /**< \brief 备份区域数据寄存器 */
    __IO uint16_t rtc_cr;    /**< \brief RTC 控制寄存器 */
    __I  uint16_t reserve1;  /**< \brief 保留 */
    __IO uint16_t cr;        /**< \brief BKP 控制寄存器 */
    __I  uint16_t reserve2;  /**< \brief 保留 */
    __IO uint16_t csr;       /**< \brief 时钟监控状态寄存器 */
    __I  uint16_t reserve3;  /**< \brief 保留 */
    __I  uint32_t reserve4;  /**< \brief 保留 */
    __IO uint32_t lse_cfg;   /**< \brief LSE晶振控制寄存器 */
} amhw_zlg237_bkp_t;

/**
 * \brief Clears Tamper Pin Event pending flag.
 *
 * \param[in] p_hw_bkp Pointer to amhw_zlg237_bkp_t Structure
 * \param[in] flag     see AMHW_ZLG237_BKP_CSR_CTE_SET or
 *                         AMHW_ZLG237_BKP_CSR_CTI_SET
 *
 * \return None
 */
am_static_inline
void amhw_zlg237_bkp_clear_flag (amhw_zlg237_bkp_t *p_hw_bkp, uint16_t flag)
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
void amhw_zlg237_bkp_dr_write (amhw_zlg237_bkp_t *p_hw_bkp,
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
uint16_t amhw_zlg237_bkp_dr_read (amhw_zlg237_bkp_t *p_hw_bkp, uint8_t index)
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
void amhw_zlg237_bkp_rtccr_cal_set (amhw_zlg237_bkp_t *p_hw_bkp, uint8_t cal)
{
    p_hw_bkp->rtc_cr = ((p_hw_bkp->rtc_cr & (~AMHW_ZLG237_BKP_RTCCR_CAL_SET)) |
                        (cal & AMHW_ZLG237_BKP_RTCCR_CAL_SET));
}

/**
 * \brief RTC 时钟校准获取
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
uint8_t amhw_zlg237_bkp_rtccr_cal_get (amhw_zlg237_bkp_t *p_hw_bkp)
{
    return (uint8_t)(p_hw_bkp->rtc_cr & AMHW_ZLG237_BKP_RTCCR_CAL_SET);
}

/**
 * \brief RTC 时钟校准输出使能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_bkp_rtccr_cco_enable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr |= AMHW_ZLG237_BKP_RTCCR_CCO_SET;
}

/**
 * \brief RTC 时钟校准输出失能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_bkp_rtccr_cco_disable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr &= ~AMHW_ZLG237_BKP_RTCCR_CCO_SET;
}

/**
 * \brief RTC 时钟校准输出获取
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
am_bool_t amhw_zlg237_bkp_rtccr_cco_get (amhw_zlg237_bkp_t *p_hw_bkp)
{
    return (p_hw_bkp->rtc_cr & AMHW_ZLG237_BKP_RTCCR_CCO_SET) ?
           AM_TRUE : AM_FALSE;
}

/**
 * \brief RTC 允许输出闹钟或秒脉冲使能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_bkp_rtccr_asoe_enable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr |= AMHW_ZLG237_BKP_RTCCR_ASOE_SET;
}

/**
 * \brief RTC 允许输出闹钟或秒脉冲失能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_bkp_rtccr_asoe_disable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr &= ~AMHW_ZLG237_BKP_RTCCR_ASOE_SET;
}

/**
 * \brief RTC 允许输出闹钟或秒脉冲获取
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
am_bool_t amhw_zlg237_bkp_rtccr_asoe_get (amhw_zlg237_bkp_t *p_hw_bkp)
{
    return (p_hw_bkp->rtc_cr & AMHW_ZLG237_BKP_RTCCR_ASOE_SET) ?
           AM_TRUE : AM_FALSE;
}

/**
 * \brief RTC 输出秒脉冲
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_bkp_rtccr_asos_second (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr |= AMHW_ZLG237_BKP_RTCCR_ASOS_SET;
}

/**
 * \brief RTC 输出闹钟脉冲
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_bkp_rtccr_asos_alarm (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->rtc_cr &= ~AMHW_ZLG237_BKP_RTCCR_ASOS_SET;
}

/**
 * \brief LSE 晶振IO驱动能力配置
 */
typedef enum amhw_zlg237_bkp_iop {
    AMHW_ZLG237_BKP_LSECFG_IOP_0 = 0,  /*档位0  最低档位驱动能力 */
    AMHW_ZLG237_BKP_LSECFG_IOP_1,      /*档位1 */
    AMHW_ZLG237_BKP_LSECFG_IOP_2,      /*档位2 */
    AMHW_ZLG237_BKP_LSECFG_IOP_3,      /*档位3  最高档位驱动能力 */
} amhw_zlg237_bkp_iop_t;
/**
 * \brief IOP LSE 晶振 IO驱动能力配置
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 * \param[in] flag     传入参数应为  amhw_zlg237_bkp_iop_t 枚举中的变量
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
void amhw_zlg237_bkp_lsecfg_iop_set (amhw_zlg237_bkp_t     *p_hw_bkp,
                                     amhw_zlg237_bkp_iop_t  flag)
{
    p_hw_bkp->lse_cfg = (p_hw_bkp->lse_cfg & (~(0x3u << 0))) | flag;
}

/**
 * \brief IOP_MON 获取IO驱动能力当前档位
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 * \param[in] flag     传入参数应为  amhw_zlg237_bkp_iop_t 枚举中的变量
 *
 * \note : 当AUTO_IOP比特设置为1时，IOP[1:0]的设置值不会立即生效；LSE的驱动档位
 *         会由2’b11逐渐降为IOP[1:0]的设置值。在此期间，可通过读取IOP_MON[1:0]
 *         获得当前LSE的驱动档位值。
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
amhw_zlg237_bkp_iop_t amhw_zlg237_bkp_lsecfg_iop_get(
    amhw_zlg237_bkp_t *p_hw_bkp)
{
    return (amhw_zlg237_bkp_iop_t)(p_hw_bkp->lse_cfg & (0x3u << 3));
}

/**
 * \brief NFBYP LSE片内模拟滤噪电器bypass使能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return none
 */
am_static_inline
void amhw_zlg237_bkp_lsecfg_nfbyp_enable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->lse_cfg |= (1u << 6);
}

/**
 * \brief NFBYP LSE片内模拟滤噪电器bypass禁能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return none
 */
am_static_inline
void amhw_zlg237_bkp_lsecfg_nfbyp_disable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->lse_cfg &= ~(1u << 6);
}

/**
 * \brief AUTO_IOP IOP设置立即生效使能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \note :    0 : IOP[1:0]的LSE驱动档位设置值立即生效。
 *            1 : LSE驱动档位将由2’b11逐渐降为IOP[1:0]的设置值。
 *
 * \return none
 */
am_static_inline
void amhw_zlg237_bkp_lsecfg_autoiop_enable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->lse_cfg |= (1u << 7);
}

/**
 * \brief AUTO_IOP IOP设置立即生效禁能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \note :    0 : IOP[1:0]的LSE驱动档位设置值立即生效。
 *            1 : LSE驱动档位将由2’b11逐渐降为IOP[1:0]的设置值。
 *
 * \return none
 */
am_static_inline
void amhw_zlg237_bkp_lsecfg_autoiop_disable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->lse_cfg &= ~(1u << 7);
}

/**
 * \brief AGC 自动增益幅度设置
 */
typedef enum amhw_zlg237_bkp_agc {
    AMHW_ZLG237_BKP_LSECFG_AGC_0 = 0,  /*档位0  最低档 */
    AMHW_ZLG237_BKP_LSECFG_AGC_1,      /*档位1 */
    AMHW_ZLG237_BKP_LSECFG_AGC_2,      /*档位2 */
    AMHW_ZLG237_BKP_LSECFG_AGC_3,      /*档位3  最高档 */
} amhw_zlg237_bkp_agc_t;
/**
 * \brief SEL_AGC AGC 自动增益幅度设置
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 * \param[in] flag     传入参数应为  amhw_zlg237_bkp_agc_t 枚举中的变量
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
void amhw_zlg237_bkp_lsecfg_agc_set (amhw_zlg237_bkp_t     *p_hw_bkp,
                                     amhw_zlg237_bkp_agc_t  flag)
{
    p_hw_bkp->lse_cfg = (p_hw_bkp->lse_cfg & (~(0x3u << 8))) | flag;
}

/**
 * \brief AGC_EN LSE晶振IO内部自动增益电路使能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return none
 */
am_static_inline
void amhw_zlg237_bkp_lsecfg_agc_enable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->lse_cfg |= (1u << 15);
}

/**
 * \brief AGC_EN LSE晶振IO内部自动增益电路禁能
 *
 * \param[in] p_hw_bkp 指向系统配置寄存器块的指针
 *
 * \return none
 */
am_static_inline
void amhw_zlg237_bkp_lsecfg_agc_disable (amhw_zlg237_bkp_t *p_hw_bkp)
{
    p_hw_bkp->lse_cfg &= ~(1u << 15);
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

#endif /* __AMHW_ZLG237_BKP_H */

/* end of file */
