/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief FTFC Peripheral Access Layer
 *
 *   The Miscellaneous System Control Module (FTFC) contains CPU configuration
 *   registers and on-chip memory controller registers.
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-17  YRZ, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_ZSN700_FLASH_H
#define __AMHW_ZSN700_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zsn700_if_flash
 * \copydoc amhw_zsn700_flash.h
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
 * \brief the structure of  FLASH register
 */
typedef struct amhw_zsn700_flash {
    __IO uint32_t tnvs;      /**< \brief Tnvs时间参数寄存器,          offset : 0x00 */
    __IO uint32_t tpgs;      /**< \brief Tpgs时间参数寄存器,          offset : 0x04 */
    __IO uint32_t tprog;     /**< \brief Tprog时间参数寄存器,         offset : 0x08 */
    __IO uint32_t tserase;   /**< \brief Tserase时间参数寄存器,       offset : 0x0c */
    __IO uint32_t tmerase;   /**< \brief Tmerase时间参数寄存器,       offset : 0x10 */
    __IO uint32_t tprcv;     /**< \brief Tprcv时间参数寄存器,         offset : 0x14 */
    __IO uint32_t tsrcv;     /**< \brief Tsrcv时间参数寄存器,         offset : 0x18 */
    __IO uint32_t tmrcv;     /**< \brief Tmrcv时间参数寄存器,         offset : 0x1c */
    __IO uint32_t cr;        /**< \brief 控制寄存器,                  offset : 0x20 */
    __IO uint32_t ifr;       /**< \brief 中断标志寄存器,              offset : 0x24 */
    __IO uint32_t iclr;      /**< \brief 中断标志清除寄存器,          offset : 0x28 */
    __IO uint32_t bypass;    /**< \brief Bypass序列寄存器,            offset : 0x2c */
    __IO uint32_t slock0;    /**< \brief Sector0-127擦写保护寄存器,   offset : 0x30 */
    __IO uint32_t slock1;    /**< \brief Sector128-255擦写保护寄存器, offset : 0x34 */
    __I  uint32_t reserve0;  /**< \brief 保留位                     , offset : 0x38 */
    __I  uint32_t reserve1;  /**< \brief 保留位                     , offset : 0x3c */
    __IO uint32_t slock2;    /**< \brief Sector256-383擦写保护寄存器, offset : 0x40 */
    __IO uint32_t slock3;    /**< \brief Sector384-512擦写保护寄存器, offset : 0x44 */
} amhw_zsn700_flash_t;

/**
 * \brief TNVS 参数
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : HCLK频率（单位MHz）
 *
 * \note 计算公式： data = 8*HCLK， HCLK的单位为MHz
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_tnvs_set(amhw_zsn700_flash_t *p_hw_flash,
                                uint8_t              data)
{
    p_hw_flash->tnvs = (p_hw_flash->tnvs & (~0x1fful)) |
                       ((uint16_t)(data * 8) & (0x1fful));
}

/**
 * \brief TPGS 参数
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : HCLK频率（单位MHz）
 *
 * \note 计算公式： data = 5.75*HCLK， HCLK的单位为MHz。
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_tpgs_set(amhw_zsn700_flash_t *p_hw_flash,
                                uint8_t              data)
{
    p_hw_flash->tpgs = (p_hw_flash->tpgs & (~0x1fful)) |
                       ((uint16_t)(data * 5.75) & (0x1fful));
}

/**
 * \brief TPROG 参数
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : HCLK频率（单位MHz）
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_tprog_set(amhw_zsn700_flash_t *p_hw_flash,
                                 uint8_t              data)
{
    p_hw_flash->tprog = (p_hw_flash->tprog & (~0x1fful)) |
                        ((uint16_t)(data * 6.75) & (0x1fful));
}

/**
 * \brief TSERASE 参数
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : HCLK频率（单位MHz）
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_tserase_set(amhw_zsn700_flash_t *p_hw_flash,
                                   uint8_t              data)
{
    p_hw_flash->tserase = (p_hw_flash->tserase & (~0x3fffful)) |
                          ((uint32_t)(data * 4500) & (0x3fffful));
}

/**
 * \brief TMERASE 参数
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : HCLK频率（单位MHz）
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_tmerase_set(amhw_zsn700_flash_t *p_hw_flash,
                                   uint8_t              data)
{
    p_hw_flash->tmerase = (p_hw_flash->tmerase & (~0x1ffffful)) |
                          ((uint32_t)(data * 35000) & (0x1ffffful));
}

/**
 * \brief TPRCV 参数
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : HCLK频率（单位MHz）
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_tprcv_set(amhw_zsn700_flash_t *p_hw_flash,
                                 uint8_t              data)
{
    p_hw_flash->tprcv = (p_hw_flash->tprcv & (~0xffful)) |
                        ((uint16_t)(data * 6) & (0xffful));
}

/**
 * \brief TSRCV 参数
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : HCLK频率（单位MHz）
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_tsrcv_set(amhw_zsn700_flash_t *p_hw_flash,
                                 uint8_t              data)
{
    p_hw_flash->tsrcv = (p_hw_flash->tsrcv & (~0xffful)) |
                        ((uint16_t)(data * 60) & (0xffful));
}

/**
 * \brief TMRCV 参数
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : HCLK频率（单位MHz）
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_tmrcv_set(amhw_zsn700_flash_t *p_hw_flash,
                                 uint8_t              data)
{
    p_hw_flash->tmrcv = (p_hw_flash->tmrcv & (~0x3ffful)) |
                        ((uint16_t)(data * 250) & (0x3ffful));
}

/**
 * \brief DPSTB_EN 当系统进入deepsleep模式， FLASH低功耗模式禁能
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_deepsleep_lp_disable(amhw_zsn700_flash_t *p_hw_flash)
{
    p_hw_flash->cr &= ~(0x1ul << 9) ;
}

/**
 * \brief DPSTB_EN 当系统进入deepsleep模式， FLASH低功耗模式使能
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_deepsleep_lp_enable(amhw_zsn700_flash_t *p_hw_flash)
{
    p_hw_flash->cr |= (0x1ul << 9) ;
}


/**
 * \brief BUSY 空闲/忙标志位
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return AM_TRUE :  忙状态
 *        AM_FALSE:  空闲状态
 *
 */
am_static_inline
am_bool_t amhw_zsn700_flash_busy_check(amhw_zsn700_flash_t *p_hw_flash)
{
    return (p_hw_flash->cr & (0x1ul << 4)) ? AM_TRUE : AM_FALSE;
}


/**
 * \brief 读FLASH周期
 */
typedef enum {
    AMHW_ZSN700_FLASH_READ_WAITTIME_1 = 0,  /**< \brief  0~24MHz: 00/11: 1个周期 */
    AMHW_ZSN700_FLASH_READ_WAITTIME_2 = 1,  /**< \brief 24~48MHz:    01: 2个周期 */
    AMHW_ZSN700_FLASH_READ_WAITTIME_3 = 2,  /**< \brief 48~72MHz:    10: 3个周期 */
}amhw_zsn700_flash_read_waittime;

/**
 * \brief WAIT 读FLASH周期
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : 待写入参数
 *                          amhw_zsn700_flash_read_waittime枚举类型中的一个
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_read_waittime_set(
        amhw_zsn700_flash_t                *p_hw_flash,
        amhw_zsn700_flash_read_waittime     waittime)
{
    p_hw_flash->cr = (p_hw_flash->cr & (~(0x3ul << 2))) |
                     ((waittime & 0x3ul) << 2);
}

/**
 * \brief FLASH操作选择
 */
typedef enum {
    AMHW_ZSN700_FLASH_READ         = 0u,     /**< \brief 读 */
    AMHW_ZSN700_FLASH_WRITE        = 1u ,    /**< \brief 写 */
    AMHW_ZSN700_FLASH_SECTOR_ERASE = 2u,     /**< \brief 页擦除  */
    AMHW_ZSN700_FLASH_CHIP_ERASE   = 3u,     /**< \brief 全片擦除  */
}amhw_zsn700_flash_opt;

/**
 * \brief OP FLASH操作选择
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] data        : 待写入参数
 *                          amhw_zsn700_flash_read_waittime枚举类型中的一个
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_opt_set(amhw_zsn700_flash_t   *p_hw_flash,
                               amhw_zsn700_flash_opt  opt)
{
    p_hw_flash->bypass = 0x5A5A;
    p_hw_flash->bypass = 0xA5A5;
    p_hw_flash->cr    &= ~0x3ul;

    p_hw_flash->bypass = 0x5A5A;
    p_hw_flash->bypass = 0xA5A5;
    p_hw_flash->cr    |= (opt & 0x3ul);
}

/**
 * \brief 获取OP FLASH操作选择
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return amhw_zsn700_flash_read_waittime枚举类型中的一个
 */
am_static_inline
uint8_t amhw_zsn700_flash_opt_get(amhw_zsn700_flash_t   *p_hw_flash)
{
    return (p_hw_flash->cr & 0x3ul);
}

/* 擦写被保护地址中断 */
#define AMHW_ZSN700_FLASH_INT_ERASE_P     (0x1ul << 6)

/* 擦写PC值中断 */
#define AMHW_ZSN700_FLASH_INT_ERASE_PC    (0x1ul << 5)

/* 擦写所有中断 */
#define AMHW_ZSN700_FLASH_INT_ALL         (0x3ul << 5)

/**
 * \brief IE FLASH中断禁能
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] type        : 中断类型 AMHW_ZSN700_FLASH_INT_ERASE_P   或
 *                                   AMHW_ZSN700_FLASH_INT_ERASE_PC  或
 *                                   AMHW_ZSN700_FLASH_INT_ALL
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_int_disable(amhw_zsn700_flash_t *p_hw_flash,
                                   uint32_t             type)
{
    p_hw_flash->cr &= ~type;
}

/**
 * \brief IE FLASH中断使能
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] type        : 中断类型 AMHW_ZSN700_FLASH_INT_ERASE_P   或
 *                                   AMHW_ZSN700_FLASH_INT_ERASE_PC  或
 *                                   AMHW_ZSN700_FLASH_INT_ALL
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_int_enable(amhw_zsn700_flash_t *p_hw_flash,
                                  uint32_t             type)
{
    p_hw_flash->cr |= type;
}

/* 擦写保护报警中断标志位 */
#define AMHW_ZSN700_FLASH_INT_ERASE_P_FLAG     (0x1ul << 1)

/* 擦写PC地址报警中断标志位 */
#define AMHW_ZSN700_FLASH_INT_ERASE_PC_FLAG    (0x1ul << 0)

/* 所有中断标志位 */
#define AMHW_ZSN700_FLASH_INT_ALL_FLAG         (0x3ul << 0)

/**
 * \brief IF FLASH中断标志获取
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] flag        : 中断标志 AMHW_ZSN700_FLASH_INT_ERASE_P_FLAG   或
 *                                   AMHW_ZSN700_FLASH_INT_ERASE_PC_FLAG
 *
 * return AM_TRUE :  有效
 *        AM_FALSE:  无效
 */
am_static_inline
am_bool_t amhw_zsn700_flash_int_flag_check(amhw_zsn700_flash_t *p_hw_flash,
                                           uint32_t             flag)
{

    return (p_hw_flash->ifr & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief ICLR FLASH中断标志清除
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] flag        : 中断标志 AMHW_ZSN700_FLASH_INT_ERASE_P_FLAG   或
 *                                   AMHW_ZSN700_FLASH_INT_ERASE_PC_FLAG
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_int_flag_clr(amhw_zsn700_flash_t *p_hw_flash,
                                    uint32_t             flag)
{
    p_hw_flash->iclr &= ~flag;
}

/**
 * \brief ICLR FLASH中断标志清除
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] value       : 待写入数据
 *
 * \note 在修改本模块寄存器之前，必须对 BYSEQ[15:0]寄存器写入 0x5a5a-0xa5a5序列。
 *       每次写入该Bypass序列后，只可以修改一次寄存器。
 *       如需再次修改寄存器，必须再次输入 0x5a5a-0xa5a5序列。
 *
 * return none
 */
am_static_inline
void amhw_zsn700_flash_key_set (amhw_zsn700_flash_t *p_hw_flash,
                                uint16_t             value)
{
    p_hw_flash->bypass = value;
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

#endif /* __AMHW_ZSN700_FLASH_H */

/* end of file */
