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
 * \brief RTC interfaces for operating hardware.
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-27  fft, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_RTC_H
#define __AMHW_LPC_RTC_H


#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_common.h"
#include "am_types.h"

/** 
 * \addtogroup amhw_lpc_if_rtc
 * \copydoc amhw_lpc_rtc.h
 * @{
 */

/**
 * \brief LPC5410X Real Time clock register block structure
 */
typedef struct amhw_lpc_rtc {                                                                 
    __IO uint32_t ctrl;   /**< \brief RTC control register                                */
    __IO uint32_t match;  /**< \brief PRTC match (alarm) register                         */
    __IO uint32_t count;  /**< \brief RTC counter register                                */
    __IO uint32_t wake;   /**< \brief RTC high-resolution/wake-up timer control register  */
} amhw_lpc_rtc_t;

/** 
 * \brief control register definitions 
 * @{
 */

/** \brief Apply reset to RTC                                     */
#define AMHW_LPC_RTC_CTRL_SWRESET      AM_BIT(0)

/** \brief Oscillator fail detect status (failed bit)              */
#define AMHW_LPC_RTC_CTRL_OFD          AM_BIT(1)    

/** \brief RTC 1 Hz timer alarm flag status (match) bit            */
#define AMHW_LPC_RTC_CTRL_ALARM1HZ     AM_BIT(2)  

/** \brief RTC 1 kHz timer wake-up flag status (timeout) bit       */
#define AMHW_LPC_RTC_CTRL_WAKE1KHZ     AM_BIT(3)   

/** \brief RTC 1 Hz timer alarm for Deep power-down enable bit     */
#define AMHW_LPC_RTC_CTRL_ALARMDPD_EN  AM_BIT(4)   

/** \brief RTC 1 kHz timer wake-up for Deep power-down enable bit */
#define AMHW_LPC_RTC_CTRL_WAKEDPD_EN   AM_BIT(5) 

/** \brief RTC 1 kHz clock enable bit                              */
#define AMHW_LPC_RTC_CTRL_RTC1KHZ_EN   AM_BIT(6)    

/** \brief RTC enable bit                                         */
#define AMHW_LPC_RTC_CTRL_RTC_EN       AM_BIT(7)

/** \brief RTC Control register Mask for reserved and status bits  */
#define AMHW_LPC_RTC_CTRL_MASK         ((uint32_t) 0xF1)  

/** @} */


/**
 * \brief RTC control configure
 *
 * \param[in] p_hw_rtc : The base address of RTC block
 * \param[in] flags    : And OR'ed value of AMHW_LPC_RTC_CTRL_*(#AMHW_LPC_RTC_CTRL_OFD) definitions
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_rtc_control (amhw_lpc_rtc_t *p_hw_rtc, uint32_t flags)
{
    p_hw_rtc->ctrl = flags;
}

/**
 * \brief Enable RTC options
 *
 * \param[in] p_hw_rtc : The base address of RTC block
 * \param[in] flags    : And OR'ed value of AMHW_LPC_RTC_CTRL_*(#AMHW_LPC_RTC_CTRL_ALARMDPD_EN) 
 *                       definitions to enable
 *
 * \return    None
 * \note You can enable multiple RTC options at once using this function
 *       by OR'ing them together. It is recommended to only use the
 *       AMHW_LPC_RTC_CTRL_ALARMDPD_EN, AMHW_LPC_RTC_CTRL_WAKEDPD_EN, AMHW_LPC_RTC_CTRL_RTC1KHZ_EN, and
 *       AMHW_LPC_RTC_CTRL_RTC_EN flags with this function.
 */
am_static_inline 
void amhw_lpc_rtc_ctl_set (amhw_lpc_rtc_t *p_hw_rtc, uint32_t flags)
{
    p_hw_rtc->ctrl = (p_hw_rtc->ctrl & AMHW_LPC_RTC_CTRL_MASK) | flags;
}

/**
 * \brief Disable RTC options
 *
 * \param[in] p_hw_rtc : The base address of RTC block
 * \param[in] flags    : And OR'ed value of  AMHW_LPC_RTC_CTRL_*(#AMHW_LPC_RTC_CTRL_ALARMDPD_EN)
 *                       definitions to disable
 *
 * \return    None
 * \note You can enable multiple RTC options at once using this function
 *       by OR'ing them together. It is recommended to only use the
 *       AMHW_LPC_RTC_CTRL_ALARMDPD_EN, AMHW_LPC_RTC_CTRL_WAKEDPD_EN, AMHW_LPC_RTC_CTRL_RTC1KHZ_EN, and
 *       AMHW_LPC_RTC_CTRL_RTC_EN flags with this function.
 */
am_static_inline 
void amhw_lpc_rtc_ctl_clr (amhw_lpc_rtc_t *p_hw_rtc, uint32_t flags)
{
    p_hw_rtc->ctrl = (p_hw_rtc->ctrl & AMHW_LPC_RTC_CTRL_MASK) & ~flags;
}

/**
 * \brief Enables the RTC
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return    None
 *
 * \note You can also use amhw_lpc_rtc_ctl_set() with the
 *       AMHW_LPC_RTC_CTRL_RTC_EN flag to enable the RTC.
 */
am_static_inline 
void amhw_lpc_rtc_enable (amhw_lpc_rtc_t *p_hw_rtc)
{
    amhw_lpc_rtc_ctl_set(p_hw_rtc, AMHW_LPC_RTC_CTRL_RTC_EN);
}

/**
 * \brief Disables the RTC
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return    None
 *
 * \note You can also use amhw_lpc_rtc_ctl_clr() with the
 *       AMHW_LPC_RTC_CTRL_RTC_EN flag to enable the RTC.
 */
am_static_inline 
void amhw_lpc_rtc_disable (amhw_lpc_rtc_t *p_hw_rtc)
{
    amhw_lpc_rtc_ctl_clr(p_hw_rtc, AMHW_LPC_RTC_CTRL_RTC_EN);
}

/**
 * \brief Reset RTC
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return    None
 *
 * \note The RTC state will be returned to it's default.
 */
am_static_inline 
void amhw_lpc_rtc_reset (amhw_lpc_rtc_t *p_hw_rtc)
{
    amhw_lpc_rtc_ctl_set(p_hw_rtc, AMHW_LPC_RTC_CTRL_SWRESET);
    amhw_lpc_rtc_ctl_clr(p_hw_rtc, AMHW_LPC_RTC_CTRL_SWRESET);
}

/**
 * \brief Check the Oscillator fail detect status
 * \param[in] p_hw_rtc : The base address of RTC block
 *
 * \retval AM_TRUE  : RTC oscillator fail detected
 * \retval AM_FALSE : The RTC oscillator is running properly
 */
am_static_inline 
am_bool_t amhw_lpc_rtc_ofd_check (amhw_lpc_rtc_t *p_hw_rtc) 
{
    return (am_bool_t)((p_hw_rtc->ctrl & AMHW_LPC_RTC_CTRL_OFD) != 0);
}

/**
 * \brief Clear the Oscillator fail detect status
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return None
 */
am_static_inline 
void amhw_lpc_rtc_ofd_clr (amhw_lpc_rtc_t *p_hw_rtc)
{
    amhw_lpc_rtc_ctl_set(p_hw_rtc, AMHW_LPC_RTC_CTRL_OFD);
}

/**
 * \brief Enables the RTC 1KHz high resolution timer
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return    None
 * \note You can also use amhw_lpc_rtc_ctl_set() with the
 *       AMHW_LPC_RTC_CTRL_RTC1KHZ_EN flag to enable the high resolution
 *       timer.
 */
am_static_inline 
void amhw_lpc_rtc_enable_1kHz (amhw_lpc_rtc_t *p_hw_rtc)
{
    amhw_lpc_rtc_ctl_set(p_hw_rtc, AMHW_LPC_RTC_CTRL_RTC1KHZ_EN);
}

/**
 * \brief Disables the RTC 1KHz high resolution timer
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return    None
 *
 * \note You can also use amhw_lpc_rtc_ctl_clr() with the
 *       AMHW_LPC_RTC_CTRL_RTC1KHZ_EN flag to disable the high resolution
 *       timer.
 */
am_static_inline
void amhw_lpc_rtc_disable_1kHz (amhw_lpc_rtc_t *p_hw_rtc)
{
    amhw_lpc_rtc_ctl_clr(p_hw_rtc, AMHW_LPC_RTC_CTRL_RTC1KHZ_EN);
}

/**
 * \brief Enables selected RTC wakeup events
 *
 * \param[in] p_hw_rtc : The base address of RTC block
 * \param[in] ints     : Wakeup events to enable
 *
 * \return    None
 * \note Select either one or both (OR'ed) AMHW_LPC_RTC_CTRL_ALARMDPD_EN
 *       and AMHW_LPC_RTC_CTRL_WAKEDPD_EN values to enabled. You can also
 *       use amhw_lpc_rtc_ctl_set() with the flags to enable
 *       the events.
 */
am_static_inline 
void amhw_lpc_rtc_enable_wakeup (amhw_lpc_rtc_t *p_hw_rtc, uint32_t ints)
{
    amhw_lpc_rtc_ctl_set(p_hw_rtc, ints);
}

/**
 * \brief Disables selected RTC wakeup events
 *
 * \param[in] p_hw_rtc : The base address of RTC block
 * \param[in] ints     : Wakeup events to disable
 *
 * \return    None
 * \note Select either one or both (OR'ed) AMHW_LPC_RTC_CTRL_ALARMDPD_EN
 *       and AMHW_LPC_RTC_CTRL_WAKEDPD_EN values to disabled. You can also
 *       use amhw_lpc_rtc_ctl_clr() with the flags to disable
 *       the events.
 */
am_static_inline 
void amhw_lpc_rtc_disable_wakeup (amhw_lpc_rtc_t *p_hw_rtc, uint32_t ints)
{
    amhw_lpc_rtc_ctl_clr(p_hw_rtc, ints);
}

/**
 * \brief Clears latched RTC statuses
 *
 * \param[in] p_hw_rtc : The base address of RTC block
 * \param[in] sts_mask : OR'ed status bits to clear
 *
 * \return    None
 * \note Use and OR'ed stsMask value of AMHW_LPC_RTC_CTRL_OFD, AMHW_LPC_RTC_CTRL_ALARM1HZ, 
 *       and AMHW_LPC_RTC_CTRL_WAKE1KHZ to clear specific RTC states.
 */
am_static_inline 
void amhw_lpc_rtc_stat_clr (amhw_lpc_rtc_t *p_hw_rtc, uint32_t sts_mask)
{
    p_hw_rtc->ctrl = (p_hw_rtc->ctrl & AMHW_LPC_RTC_CTRL_MASK) | sts_mask;
}

/**
 * \brief Return RTC control/status register
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return    The current RTC control/status register
 *
 * \note Mask the return value with a RTC_CTRL_* definitions to determine
 *       which bits are set. For example, mask the return value with
 *       AMHW_LPC_RTC_CTRL_ALARM1HZ to determine if the alarm interrupt is pending.
 */
am_static_inline
uint32_t amhw_lpc_rtc_stat_get (amhw_lpc_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->ctrl;
}

/**
 * \brief Set RTC match value for alarm status/interrupt
 *
 * \param[in] p_hw_rtc : The base address of RTC block
 * \param[in] count    : Alarm event time
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_rtc_alarm_set (amhw_lpc_rtc_t *p_hw_rtc, uint32_t count)
{
    p_hw_rtc->match = count;
}

/**
 * \brief Return the RTC match value used for alarm status/interrupt
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return    Alarm event time
 */
am_static_inline 
uint32_t amhw_lpc_rtc_alarm_get (amhw_lpc_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->match;
}

/**
 * \brief Set RTC match count for 1 second timer count
 *
 * \param[in] p_hw_rtc : The base address of RTC block
 * \param[in] count    : Initial count to set
 *
 * \return    None
 * \note Only write to this register when the AMHW_LPC_RTC_CTRL_RTC_EN bit in
 *       the ctrl Register is 0. The counter increments one second
 *       after the AMHW_LPC_RTC_CTRL_RTC_EN bit is set.
 */
am_static_inline 
void amhw_lpc_rtc_count_set (amhw_lpc_rtc_t *p_hw_rtc, uint32_t count)
{
    p_hw_rtc->count = count;
}

/**
 * \brief Get current RTC 1 second timer count
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return    current RTC 1 second timer count
 */
am_static_inline 
uint32_t amhw_lpc_rtc_count_get (amhw_lpc_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->count;
}

/**
 * \brief Set RTC wake count countdown value (in mS ticks)
 *
 * \param[in] p_hw_rtc : The base address of RTC block
 * \param[in] count    : wakeup time in milliSeconds
 *
 * \return    None
 * \note A write pre-loads a start count value into the wake-up
 *       timer and initializes a count-down sequence.
 */
am_static_inline 
void amhw_lpc_rtc_setwake (amhw_lpc_rtc_t *p_hw_rtc, uint16_t count)
{
    p_hw_rtc->wake = count;
}

/**
 * \brief Get RTC wake count countdown value
 * \param[in] p_hw_rtc : The base address of RTC block
 * \return    current RTC wake count countdown value (in mS)
 */
am_static_inline 
uint16_t amhw_lpc_rtc_getwake (amhw_lpc_rtc_t *p_hw_rtc)
{
    return p_hw_rtc->wake;
}

/**
 * @} amhw_lpc_if_rtc
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_RTC_H */

/* end of file */
