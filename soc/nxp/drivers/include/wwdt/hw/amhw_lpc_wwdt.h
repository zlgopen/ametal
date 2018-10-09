/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief  WWDT interfaces for operating hardware.
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-25  fft, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_WWDT_H
#define __AMHW_LPC_WWDT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
    
/** 
 * \addtogroup amhw_lpc_if_wwdt
 * \copydoc amhw_lpc_wwdt.h
 * @{
 */

/**
 * \brief Windowed Watchdog register block structure
 */
typedef struct amhw_lpc_wwdt {
    __IO uint32_t  mod;         /**< \brief Watchdog mode register              */
    __IO uint32_t  tc;          /**< \brief Watchdog timer constant register    */
    __O  uint32_t  feed;        /**< \brief Watchdog feed sequence register     */
    __I  uint32_t  tv;          /**< \brief Watchdog timer value register       */
    __I  uint32_t  reserved0;   /**< \brief reserved,do not use                 */
    __IO uint32_t  warnint;     /**< \brief Watchdog warning interrupt register */
    __IO uint32_t  window;      /**< \brief Watchdog timer window register      */
} amhw_lpc_wwdt_t;

/**
* \name Watchdog Mode register definitions
* @{
*/

#define AMHW_LPC_WWDT_WDMOD_BITMASK    ((uint32_t)0x3F)  /**< \brief Watchdog Mode Bitmask */      
#define AMHW_LPC_WWDT_WDMOD_WDEN       AM_BIT(0)         /**< \brief WWDT enable bit       */      
#define AMHW_LPC_WWDT_WDMOD_WDRESET    AM_BIT(1)         /**< \brief WWDT reset enable bit */      
#define AMHW_LPC_WWDT_WDMOD_WDTOF      AM_BIT(2)         /**< \brief WWDT time-out flag bit*/   

/** \brief WWDT warning interrupt flag bit */   
#define AMHW_LPC_WWDT_WDMOD_WDINT      AM_BIT(3)         
#define AMHW_LPC_WWDT_WDMOD_WDPROTECT  AM_BIT(4)         /**< \brief WWDT Protect flag bit */      
#define AMHW_LPC_WWDT_WDMOD_LOCK       AM_BIT(5)         /**< \brief WWDT lock bit         */

/** @} */
 
 
/**
 * \brief Initialize the Watchdog timer
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \return    None
 */
am_static_inline
void amhw_lpc_wwdt_init (amhw_lpc_wwdt_t *p_hw_wwdt)
{
    /* Disable watchdog */
    p_hw_wwdt->mod     = 0;
    p_hw_wwdt->tc      = 0xFF;
    p_hw_wwdt->warnint = 0x0;
    p_hw_wwdt->window  = 0xFFFFFF;
}

/**
 * \brief Set WDT time out constant value used for feed
 *
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \param[in] timeout   : WDT timeout in ticks, between 0xFF and 0xFFFFFF
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_wwdt_timeout_set (amhw_lpc_wwdt_t *p_hw_wwdt, uint32_t timeout)
{
    p_hw_wwdt->tc = timeout;
}

/**
 * \brief Feed watchdog timer
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \return    None
 * \note If this function isn't called, a watchdog timer warning will occur.
 *        After the warning, a time out will occur if a feed has happened.
 *        Note that if WWDT registers are modified in an interrupt then it is a good
 *        idea to prevent those interrupts when writing the feed sequence.
 */
am_static_inline 
void amhw_lpc_wwdt_feed (amhw_lpc_wwdt_t *p_hw_wwdt)
{
    p_hw_wwdt->feed = 0xAA;
    p_hw_wwdt->feed = 0x55;
}

/**
 * \brief Set WWDT warning interrupt
 *
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \param[in] timeout   : WDT warning in ticks, between 0 and 1023
 *
 * \return    None
 *
 * \note This is the number of ticks after the watchdog start that the
 *       warning interrupt will be generated.
 */
am_static_inline
void amhw_lpc_wwdt_warning_set (amhw_lpc_wwdt_t *p_hw_wwdt, uint32_t timeout)
{
    p_hw_wwdt->warnint = timeout;
}

/**
 * \brief Get WWDT warning interrupt
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \return    WWDT warning interrupt
 */
am_static_inline 
uint32_t amhw_lpc_wwdt_warning_get (amhw_lpc_wwdt_t *p_hw_wwdt)
{
    return p_hw_wwdt->warnint;
}

/**
 * \brief Set WWDT window time
 *
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \param[in] timeout   : WWDT time out in ticks, between WWDT_TICKS_MIN and WWDT_TICKS_MAX
 *
 * \return    None
 *
 * \note The watchdog timer must be fed between the time out from the amhw_lpc_wwdt_timeout_set()
 *       function and this function, with this function defining the last tick before the
 *       watchdog window interrupt occurs.
 */
am_static_inline 
void amhw_lpc_wwdt_window_set (amhw_lpc_wwdt_t *p_hw_wwdt, uint32_t timeout)
{
    p_hw_wwdt->window = timeout;
}

/**
 * \brief Get WWDT window time
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \return    WWDT window time
 */
am_static_inline 
uint32_t amhw_lpc_wwdt_window_get (amhw_lpc_wwdt_t *p_hw_wwdt)
{
    return p_hw_wwdt->window;
}

/**
 * \brief Enable watchdog timer options
 *
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \param[in] options   : An or'ed set of options of values
 *                        AMHW_LPC_WWDT_WDMOD_WDEN, AMHW_LPC_WWDT_WDMOD_WDRESET, and AMHW_LPC_WWDT_WDMOD_WDPROTECT
 * \return    None
 *
 * \note You can enable more than one option at once (eg, AMHW_LPC_WWDT_WDMOD_WDRESET |
 *       AMHW_LPC_WWDT_WDMOD_WDPROTECT), but use the AMHW_LPC_WWDT_WDMOD_WDEN after all other options
 *       are set (or unset) with no other options. If AMHW_LPC_WWDT_WDMOD_LOCK is used, it cannot
 *       be unset.
 */
am_static_inline 
void amhw_lpc_wwdt_option_set (amhw_lpc_wwdt_t *p_hw_wwdt, uint32_t options)
{
    p_hw_wwdt->mod = (p_hw_wwdt->mod & AMHW_LPC_WWDT_WDMOD_BITMASK) | options;
}

/**
 * \brief Enable WWDT activity
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \return    None
 */
am_static_inline 
void amhw_lpc_wwdt_start (amhw_lpc_wwdt_t *p_hw_wwdt)
{
    amhw_lpc_wwdt_option_set(p_hw_wwdt, AMHW_LPC_WWDT_WDMOD_WDEN);
    amhw_lpc_wwdt_feed(p_hw_wwdt);
}

/**
 * \brief Read WWDT status flag
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \return    Watchdog status, an Or'ed value of WWDT_WDMOD_*
 */
am_static_inline 
uint32_t amhw_lpc_wwdt_status_get (amhw_lpc_wwdt_t *p_hw_wwdt)
{
    return p_hw_wwdt->mod;
}

/**
 * \brief Clear WWDT status flags
 *
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \param[in] flags     : The flags to clear
 *
 * \return  None
 *
 * \note You can only clear flag AMHW_LPC_WWDT_WDMOD_WDINT, AMHW_LPC_WWDT_WDMOD_WDTOF or the 
 *       Or'ed value of two flags
 */
am_static_inline
void amhw_lpc_wwdt_status_clr (amhw_lpc_wwdt_t *p_hw_wwdt, uint32_t flags)
{
    if (flags & AMHW_LPC_WWDT_WDMOD_WDTOF) {
        p_hw_wwdt->mod &= (~AMHW_LPC_WWDT_WDMOD_WDTOF) & 0x3F;
    }
    
    /* Interrupt flag is cleared by writing a 1 */
    if (flags & AMHW_LPC_WWDT_WDMOD_WDINT) {
        p_hw_wwdt->mod = (p_hw_wwdt->mod & 0x3F) | AMHW_LPC_WWDT_WDMOD_WDINT;
    }
}

/**
 * \brief Get the current value of WWDT
 * \param[in] p_hw_wwdt : The base of WatchDog Timer peripheral on the chip
 * \return    current value of WWDT
 */
am_static_inline 
uint32_t amhw_lpc_wwdt_currentcount_get (amhw_lpc_wwdt_t *p_hw_wwdt)
{
    return p_hw_wwdt->tv;
}

/**
 * @} amhw_lpc_if_wwdt
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_WWDT_H */

/* end of file */
