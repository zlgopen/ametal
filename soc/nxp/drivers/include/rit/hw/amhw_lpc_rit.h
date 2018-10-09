/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site    http://www.zlg.cn/
* e-mail      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief RIT(Repetitive Interrupt Timer)  interfaces for operating hardware.
 *
 * - 48-bit counter running from the main clock.Counter can be free-running or be
 *   reset by a generated interrupt.
 * - 48-bit compare value.
 * - 48-bit compare mask. An interrupt is generated when the counter value equals
 *   the compare value, after masking. This allows for combinations not possible
 *   with a simple compare
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_RIT_H
#define __AMHW_LPC_RIT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup amhw_lpc_if_rit
 * \copydoc amhw_lpc_rit.h
 * @{
 */

/**
 * \brief RIT register block structure
 */
typedef struct amhw_lpc_rit {
    __IO uint32_t  compval;       /**< \brief 32-bit Compare register         */
    __IO uint32_t  mask;          /**< \brief 32-bit Mask register            */
    __IO uint32_t  ctrl;          /**< \brief Control register                */
    __IO uint32_t  counter;       /**< \brief 32-bit counter                  */
    __IO uint32_t  compval_h;     /**< \brief Compare register, upper 16-bits */
    __IO uint32_t  mask_h;        /**< \brief Mask register, upper 16-bits    */
    __I  uint32_t  reserved;      /**< \brief reserved,do not use             */
    __IO uint32_t  counter_h;     /**< \brief Counter register, upper 16-bits */
} amhw_lpc_rit_t;

/**
 * \name Macro defines for RIT register
 * @{
 */

/** \brief Set by hardware when the counter value equals the masked compare value */
#define AMHW_LPC_RIT_CTRL_INT    ((uint32_t) (1ul << 0))

/**
 * \brief Set timer enable clear to 0 when the counter value
 *        equals the masked compare value
 */
#define AMHW_LPC_RIT_CTRL_ENCLR  ((uint32_t) (1ul << 1))

/**
 * \brief Set timer enable on debug, The timer is halted when the
 *        processor is halted for debugging
 */
#define AMHW_LPC_RIT_CTRL_ENBR   ((uint32_t) (1ul << 2))

/** \brief Set timer enable */
#define AMHW_LPC_RIT_CTRL_TEN    ((uint32_t) (1ul << 3))

/** @} */

/**
 * \brief Enable RIT Timer
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return None
 */
am_static_inline
void amhw_lpc_rit_enable (amhw_lpc_rit_t *p_hw_rit)
{
    p_hw_rit->ctrl |= AMHW_LPC_RIT_CTRL_TEN;
}

/**
 * \brief Disable Timer
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return None
 */
am_static_inline
void amhw_lpc_rit_disable (amhw_lpc_rit_t *p_hw_rit)
{
    p_hw_rit->ctrl &= ~AMHW_LPC_RIT_CTRL_TEN;
}

/**
 * \brief Enable timer debug,The timer is halted when the processor is halted for debugging
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return None
 */
am_static_inline
void amhw_lpc_rit_debug_enable (amhw_lpc_rit_t *p_hw_rit)
{
    p_hw_rit->ctrl |= AMHW_LPC_RIT_CTRL_ENBR;
}

/**
 * \brief Disable timer debug,Debug has no effect on the timer operation
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return None
 */
am_static_inline
void amhw_lpc_rit_debug_disable (amhw_lpc_rit_t *p_hw_rit)
{
    p_hw_rit->ctrl &= ~AMHW_LPC_RIT_CTRL_ENBR;
}

/**
 * \brief Enable clear on compare match
 *
 *  The timer will be cleared to 0 whenever the counter value equals the masked
 *  compare value
 *
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return None
 */
am_static_inline
void amhw_lpc_rit_clear_enable (amhw_lpc_rit_t *p_hw_rit)
{
    p_hw_rit->ctrl |= AMHW_LPC_RIT_CTRL_ENCLR;
}

/**
 * \brief Disable clear on compare match
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return None
 */
am_static_inline
void amhw_lpc_rit_clear_disable (amhw_lpc_rit_t *p_hw_rit)
{
    p_hw_rit->ctrl &= ~AMHW_LPC_RIT_CTRL_ENCLR;
}

/**
 * \brief Check whether interrupt flag is set or not
 *
 *  This interrupt flag is set to 1 by hardware whenever the counter value
 *  equals the masked compare value
 *
 * \param[in] p_hw_rit : Pointer to RIT register block
 *
 * \retval    AM_TRUE  : interrupt flag set
 * \retval    AM_FALSE : interrupt flag not set
 */
am_static_inline
am_bool_t amhw_lpc_rit_int_flag_check (amhw_lpc_rit_t *p_hw_rit)
{
    return (am_bool_t)((p_hw_rit->ctrl & (AMHW_LPC_RIT_CTRL_INT)) != 0);
}

/**
 * \brief Clear the interrupt flag
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return None
 */
am_static_inline
void amhw_lpc_rit_int_flag_clr (amhw_lpc_rit_t *p_hw_rit)
{
    p_hw_rit->ctrl |= AMHW_LPC_RIT_CTRL_INT;
}

/**
 * \brief Set a compare value for the interrupt to time out (48-bits)
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \param[in] val      : value (in ticks) of the compare value to be set.
 *                       48-bits maximum
 *
 * \return None
 */
am_static_inline
void amhw_lpc_rit_compval_set (amhw_lpc_rit_t *p_hw_rit, uint64_t val)
{
    p_hw_rit->compval   = (uint32_t)(val & 0xFFFFFFFF);
    p_hw_rit->compval_h = (uint32_t)((val >> 32) & 0xFFFF);
}

/**
 * \brief Set the mask value (48-bits)
 *
 *  A one in bit n  causes the comparison of the bit n to be always AM_TRUE.
 *
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \param[in] val      : value  of the mask value to be set, 48-bits maximum
 *
 * \return None
 */
am_static_inline
void amhw_lpc_rit_mask_set (amhw_lpc_rit_t *p_hw_rit, uint64_t val)
{
    p_hw_rit->mask   = (uint32_t)(val & 0xFFFFFFFF);
    p_hw_rit->mask_h = (uint32_t)((val >> 32) & 0xFFFF);
}

/**
 * \brief Get the current RIT mask value
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return value of the mask(48-bits)
 */
am_static_inline
uint64_t amhw_lpc_rit_mask_get (amhw_lpc_rit_t *p_hw_rit)
{
    uint64_t mask_value;

    mask_value  = (uint64_t)p_hw_rit->mask;
    mask_value |= (((uint64_t)p_hw_rit->mask_h) << 32);

    return mask_value;
}

/**
 * \brief Get the current RIT compare value
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return value of the compare value(48-bits)
 */
am_static_inline
uint64_t amhw_lpc_rit_compval_get (amhw_lpc_rit_t *p_hw_rit)
{
    uint64_t compare_value;

    compare_value  = (uint64_t)p_hw_rit->compval;
    compare_value |= (((uint64_t)p_hw_rit->compval_h) << 32);

    return compare_value;
}

/**
 * \brief Get the current RIT Counter value
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \return the current timer counter value (48-bit)
 */
am_static_inline
uint64_t amhw_lpc_rit_count_get (amhw_lpc_rit_t *p_hw_rit)
{
    uint64_t counter_value;

    counter_value  = (uint64_t)p_hw_rit->counter;
    counter_value |= (((uint64_t)p_hw_rit->counter_h) << 32);

    return counter_value;
}

/**
 * \brief Set the Counter value
 * \param[in] p_hw_rit : Pointer to RIT register block
 * \param[in] value    : The counter value to set
 * \return None
 */
void amhw_lpc_rit_count_set(amhw_lpc_rit_t *p_hw_rit, uint64_t value);


/**
 * @} amhw_lpc_if_rit
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_RIT_H */

/* end of file */
