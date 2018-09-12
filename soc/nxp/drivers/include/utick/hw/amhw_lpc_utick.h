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
 * \brief UTICK(Micro-tick timer) interfaces for operating hardware.
 * 
 * - Ultra simple timer.
 * - Write once to start.
 * - Interrupt or software polling
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_UTICK_H
#define __AMHW_LPC_UTICK_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/** 
 * \addtogroup amhw_lpc_if_utick
 * \copydoc amhw_lpc_utick.h
 * @{
 */
 
/**
 * \brief Micro Tick register block structure
 */
typedef struct amhw_lpc_utick {
    __IO uint32_t ctrl;        /**< \brief UTICK Control register */
    __IO uint32_t status;      /**< \brief UTICK Status register  */
} amhw_lpc_utick_t;
 
/**
 * \name Macro defines for UTICK register
 * @{
 */

/** \brief  UTICK repeat delay bit     */
#define AMHW_LPC_UTICK_CTRL_REPEAT           ((uint32_t)1UL << 31)

/** \brief  UTICK Delay Value Mask     */
#define AMHW_LPC_UTICK_CTRL_DELAY_MASK       ((uint32_t)0x7FFFFFFF)

/** \brief  UTICK Interrupt Status bit */
#define AMHW_LPC_UTICK_STATUS_INTR           ((uint32_t)1 << 0)

/** \brief  UTICK Active Status bit    */
#define AMHW_LPC_UTICK_STATUS_ACTIVE         ((uint32_t)1 << 1)

/** \brief  UTICK Status Register Mask */
#define AMHW_LPC_UTICK_STATUS_MASK           ((uint32_t)0x03)

/** @} */

/**
 * \brief Set UTICK ticks.
 *
 * The delay will be equal to DELAYVAL + 1 periods of the timer clock 
 * The minimum usable value is 1, for a delay of 2 timer clocks. A value of 0 stops 
 * the timer.
 *
 * \param[in] p_hw_utick : point to the base address of UTICK. eg AMHW_LPC_UTICK
 * \param[in] tick_value : Tick value, the maximum value is 0x7FFFFFFF(31-bits)
 * \param[in] repeat     : AM_TRUE --delay repeats continuously 
 *                         AM_FALSE--One-time delay.
 * \return  None
 */
void amhw_lpc_utick_tick_set(amhw_lpc_utick_t *p_hw_utick, 
                         uint32_t      tick_value, 
                         am_bool_t        repeat);

/**
 * \brief Read UTICK Value
 * \param[in] p_hw_utick : Pointer to UTICK register block
 * \return  Current tick value
 */
am_static_inline 
uint32_t amhw_lpc_utick_tick_get (amhw_lpc_utick_t *p_hw_utick)
{
    return (p_hw_utick->ctrl & AMHW_LPC_UTICK_CTRL_DELAY_MASK);
}

/**
 * \brief Disable UTICK timer
 * \param[in] p_hw_utick : Pointer to UTICK register block
 * \return  None
 */

am_static_inline 
void amhw_lpc_utick_disable (amhw_lpc_utick_t *p_hw_utick)
{
    p_hw_utick->ctrl = 0;
}

/**
 * \brief Check the interrupt flag of UTICK set or not.
 * \param[in] p_hw_utick : Pointer to UTICK register block
 * \retval  AM_TRUE  : interrupt flag set
 * \retval  AM_FALSE : not set
 */
am_static_inline 
am_bool_t amhw_lpc_utick_int_flag_check (amhw_lpc_utick_t *p_hw_utick)
{
    return (am_bool_t)((p_hw_utick->status & AMHW_LPC_UTICK_STATUS_INTR) != 0);
}

/**
 * \brief Clear UTICK Interrupt flag
 * \param[in] p_hw_utick : Pointer to UTICK register block
 * \return  None
 */
am_static_inline 
void amhw_lpc_utick_int_flag_clr (amhw_lpc_utick_t *p_hw_utick)
{
    p_hw_utick->status = AMHW_LPC_UTICK_STATUS_INTR;
}
 

/**
 * \brief Check the Active flag of UTICK set or not.
 * \param[in] p_hw_utick : Pointer to UTICK register block
 * \retval  AM_TRUE  : Active flag set
 * \retval  AM_FALSE : not set
 */
am_static_inline 
am_bool_t amhw_lpc_utick_active_flag_check (amhw_lpc_utick_t *p_hw_utick)
{
    return (am_bool_t)((p_hw_utick->status & AMHW_LPC_UTICK_STATUS_ACTIVE) != 0);
}
 
/**
 * @} amhw_lpc_if_utick
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_UTICK_H */

/* end of file */
