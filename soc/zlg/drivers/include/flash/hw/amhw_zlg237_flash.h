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
 * \brief FTFC Peripheral Access Layer
 *
 *   The Miscellaneous System Control Module (FTFC) contains CPU configuration
 *   registers and on-chip memory controller registers.
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-17  ari, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_ZLG237_FLASH_H
#define __AMHW_ZLG237_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zlg237_if_flash
 * \copydoc amhw_zlg237_flash.h
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
typedef struct amhw_zlg237_flash {
    __IO uint32_t acr;      /**< \brief access control register offset : 0x00 */
    __IO uint32_t keyr;     /**< \brief key register            offset : 0x04 */
    __IO uint32_t optkeyr;  /**< \brief OPTkey register         offset : 0x08 */
    __IO uint32_t sr;       /**< \brief status register         offset : 0x0c */
    __IO uint32_t cr;       /**< \brief control register        offset : 0x10 */
    __IO uint32_t ar;       /**< \brief address register        offset : 0x14 */
    __IO uint32_t reserved; /**< \brief reserved register       offset : 0x18 */
    __IO uint32_t obr;      /**< \brief option byte register    offset : 0x1c */
    __IO uint32_t wrpr;     /**< \brief write protect register  offset : 0x20 */
    __IO uint32_t ecr;
} amhw_zlg237_flash_t;

/**
 * \brief Check if prefetch buffer is on
 *
 * \param[in] p_hw_flash  : The pointer to the structure of FLASH register
 *
 * \retval AM_TRUE : prefetch buffer is on
 *         AM_FALSE: prefetch buffer is off
 */
am_static_inline
am_bool_t amhw_zlg237_flash_check_prebuff_on (amhw_zlg237_flash_t *p_hw_flash)
{
    return (p_hw_flash->acr >> 5 & 0x1u) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief Set prefetch buffer on
 *
 * \param[in] p_hw_flash  : The pointer to the structure of FLASH register
 * \param[in] flag        : AM_TRUE : prefetch buffer is on
 *                          AM_FALSE: prefetch buffer is off
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_set_prebuff_on (amhw_zlg237_flash_t      *p_hw_flash,
                                       am_bool_t              flag)
{
    p_hw_flash->acr = (p_hw_flash->acr & (~(1u << 4))) | (flag << 4);
}

/**
 * \brief Flash half cycle access enable
 *
 * \param[in] p_hw_flash  : The pointer to the structure of FLASH register
 * \param[in] flag        :AM_TRUE : Flash half cycle access enable
 *                         AM_FALSE: Flash half cycle access disable
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_half_cycle_enable (amhw_zlg237_flash_t  *p_hw_flash,
                                          am_bool_t             flag)
{
    p_hw_flash->acr = (p_hw_flash->acr & (~(1u << 3))) | (flag << 3);
}

/*
 * \brief latency set
 * \ @{
 */

#define AMHW_ZLG237_FLASH_LARENCY_O   0 /**< \brief 0 wait status */
#define AMHW_ZLG237_FLASH_LARENCY_1   1 /**< \brief 1 wait status */
#define AMHW_ZLG237_FLASH_LARENCY_2   2 /**< \brief 2 wait status */
#define AMHW_ZLG237_FLASH_LARENCY_3   3 /**< \brief 3 wait status */
#define AMHW_ZLG237_FLASH_LARENCY_7   4 /**< \brief 7 wait status */
#define AMHW_ZLG237_FLASH_LARENCY_9   5 /**< \brief 9 wait status */
#define AMHW_ZLG237_FLASH_LARENCY_19  6 /**< \brief 19 wait status */
#define AMHW_ZLG237_FLASH_LARENCY_39  7 /**< \brief 39 wait status */

/** @}*/

/**
 * \brief Flash latency set
 *
 * \param[in] p_hw_flash  : The pointer to the structure of FLASH register
 * \param[in] flag        : see the field of  latency set
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_latency_set (amhw_zlg237_flash_t    *p_hw_flash,
                                    uint32_t                flag)
{
    p_hw_flash->acr = (p_hw_flash->acr & (~(3u << 0))) | (flag << 0);
}

/**
 * \brief Flash key set
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 * \param[in] value      : [0 : 31]
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_key_set (amhw_zlg237_flash_t  *p_hw_flash,
                                uint32_t              value)
{
    p_hw_flash->keyr = value;
}

/**
 * \brief Flash OPTkey set
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 * \param[in] value      : [0 : 31]
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_optkey_set (amhw_zlg237_flash_t    *p_hw_flash,
                                   uint32_t             value)
{
    p_hw_flash->optkeyr = value;
}
/*
 * \brief FLASH status flag
 * \ @{
 */

#define AMHW_ZLG237_FLASH_EOP_FLAG       (1U << 5) /**< \brief end of operation flag */
#define AMHW_ZLG237_FLASH_WRPRTERR_FLAG  (1U << 4) /**< \brief write protect error flag */
#define AMHW_ZLG237_FLASH_PGERR_FLAG     (1U << 2) /**< \brief programming  flag */
#define AMHW_ZLG237_FLASH_BUSY_FLAG      (1U << 0) /**< \brief busy flag */
/** @}*/

/**
 * \brief Flash Status flag check
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 * \param[in] flag       : see the field of FLASH status flag
 *
 * return AM_TRUE or FLASE
 */
am_static_inline
am_bool_t amhw_zlg237_flash_status_check (amhw_zlg237_flash_t *p_hw_flash,
                                          uint32_t             flag)
{
    return (p_hw_flash->sr & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief Flash Status flag clear
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 * \param[in] flag       : see the field of FLASH status flag
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_status_flag_clr (amhw_zlg237_flash_t *p_hw_flash,
                                        uint32_t             flag)
{
    p_hw_flash->sr |= flag;
}

/*
 * \brief FLASH control register flag
 * \ @{
 */

#define AMHW_ZLG237_FLASH_EOP_INT_EN_MASK  (1U << 12) /**< \brief End of operation interrupt enable */
#define AMHW_ZLG237_FLASH_ERR_INT_EN_MASK  (1U << 10) /**< \brief error interrupt enable */
#define AMHW_ZLG237_FLASH_OPTWRE_EN_MASK   (1U << 9)  /**< \brief option byte write enable */
#define AMHW_ZLG237_FLASH_LOCK_MASK        (1U << 7)  /**< \brief lock */
#define AMHW_ZLG237_FLASH_START_MASK       (1U << 6)  /**< \brief start erase */
#define AMHW_ZLG237_FLASH_OPT_ERASE_MASK   (1U << 5)  /**< \brief option byte erase */
#define AMHW_ZLG237_FLASH_OPT_PROGRAM_MASK (1U << 4)  /**< \brief option byte programming */
#define AMHW_ZLG237_FLASH_MASS_ERASE_MASK  (1U << 2)  /**< \brief mass erase */
#define AMHW_ZLG237_FLASH_PAGE_ERASE_MASK  (1U << 1)  /**< \brief page erase */
#define AMHW_ZLG237_FLASH_PROGRAM_MASK     (1U << 0)  /**< \brief programming */

/** @}*/

/**
 * \brief Flash control register set
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 * \param[in] flag       : see the field of FLASH control register flag
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_cs_reg_set (amhw_zlg237_flash_t *p_hw_flash, uint32_t flag)
{

    p_hw_flash->cr |= flag;

}

/**
 * \brief Flash control register set
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 * \param[in] flag       : see the field of FLASH control register flag
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_cs_reg_clr (amhw_zlg237_flash_t *p_hw_flash, uint32_t flag)
{
    p_hw_flash->cr &= ~flag;
}

/**
 * \brief Flash control register get
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 *
 * return the value of Flash control register
 */
am_static_inline
uint32_t amhw_zlg237_flash_cs_reg_get (amhw_zlg237_flash_t *p_hw_flash)
{
    return p_hw_flash->cr;
}

/**
 * \brief Flash option byte data 0
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 *
 * return data0
 */
am_static_inline
uint8_t amhw_zlg237_flash_opt_data0_get (amhw_zlg237_flash_t *p_hw_flash)
{
    return ((p_hw_flash->obr >> 10) & 0xffu);
}

/**
 * \brief Flash option byte data 1
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 *
 * return data0
 */
am_static_inline
uint8_t amhw_zlg237_flash_opt_data1_get (amhw_zlg237_flash_t *p_hw_flash)
{
    return ((p_hw_flash->obr >> 18) & 0xffu);
}

/*
 * \brief FLASH control register flag
 * \ @{
 */

#define AMHW_ZLG237_FLASH_BOOT1_MASK      (1U << 6) /**< \brief BOOT1 */
#define AMHW_ZLG237_FLASH_RST_STDBY_MASK  (1U << 4) /**< \brief RST_STDBY */
#define AMHW_ZLG237_FLASH_RST_STOP_MASK   (1U << 3) /**< \brief RST_STOP */
#define AMHW_ZLG237_FLASH_WDG_SW_MASK     (1U << 2) /**< \brief WDG_SW */
#define AMHW_ZLG237_FLASH_RSPRT_MASK      (1U << 1) /**< \brief read protection level status */
#define AMHW_ZLG237_FLASH_OPT_ERR_MASK    (1U << 0) /**< \brief option byte error */

/** @}*/

/**
 * \brief Flash option byte status check
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 *
 * return AM_TRUE or FLASE
 */
am_static_inline
am_bool_t amhw_zlg237_flash_opt_staus_check (amhw_zlg237_flash_t *p_hw_flash,
                                             uint8_t              flag)
{
    return ((p_hw_flash->obr & flag) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief Flash address set
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 * \param[in] value      : [0 : 31]
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_address_set (amhw_zlg237_flash_t *p_hw_flash,
                                    uint32_t             value)
{
    p_hw_flash->ar = value;
}

/**
 * \brief Flash write protect  set
 *
 * \param[in] p_hw_flash : The pointer to the structure of FLASH register
 * \param[in] value      : [0 : 31]
 *
 * return none
 */
am_static_inline
void amhw_zlg237_flash_wrpr_set (amhw_zlg237_flash_t *p_hw_flash,
                                 uint32_t             value)
{
    p_hw_flash->wrpr = value;
}

#define AMHW_ZLG237_FLASH_NO_HALF_PAGE_ERASE   0
#define AMHW_ZLG237_FLASH_HALF_PAGE_ERASE      1

/**
 * \brief 设置是否半页擦除
 *
 * \param[in] value :
 *
 *         0 : 不使用half-page擦除
 *         1 : 使用half-page擦除；HPRE设置为1时，FLASH_CR.PER比特必须清零
 *
 * \return none
 */
am_static_inline
void amhw_zlg237_flash_ecr_haer_set(amhw_zlg237_flash_t  *p_hw_flash, uint8_t value)
{

    p_hw_flash->ecr = (p_hw_flash->ecr & (~(1ul << 0))) | ((value & 0x1ul) << 0);
}

#define AMHW_ZLG237_FLASH_NO_WORD_PROGRAM   0
#define AMHW_ZLG237_FLASH_WORD_PROGRAM      1

/**
 * \brief 设置是否使用word字编程
 *
 * \param[in] value :
 *
 *         0 : 不使用word字编程
 *         1 : 使用word字编程；WPG设置为1时，FLASH_CR.PG比特必须清零
 *
 * \return none
 */
am_static_inline
void amhw_zlg237_flash_ecr_wpg_set(amhw_zlg237_flash_t  *p_hw_flash, uint8_t value)
{

    p_hw_flash->ecr = ((p_hw_flash->ecr & (~(1ul << 3))) | ((value & 0x1ul) << 3));
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

#endif /* __AMHW_ZLG_FLASH_H */

/* end of file */
