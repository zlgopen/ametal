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
 * \brief Standard counter/timers interfaces for operating hardware.
 *
 * - 32-bit counter/timers, each with a programmable 32-bit prescaler.
 * - Counter or timer operation.
 * - Four 32-bit capture channels that can take a snapshot of the timer value 
 *   when an input signal transitions. A capture event may also optionally
 *   generate an interrupt.
 * - The timer and prescaler may be configured to be cleared on a designated 
 *   capture event. This feature permits easy pulse-width measurement by clearing 
 *   the timer on the leading edge of an input pulse and capturing the timer value 
 *   on the trailing edge.
 * - Four 32-bit match registers that allow:
 *       - Continuous operation with optional interrupt generation on match
 *       - Stop timer on match with optional interrupt generation
 *       - Reset timer on match with optional interrupt generation
 * - Four external outputs corresponding to match registers with the following 
 *   capabilities:
 *       - Set LOW on match
 *       - Set HIGH on match
 *       - Toggle on match
 *       - Do nothing on match
 * - for each timer, up to three match outputs can be used as single edge controlled 
 *   PWM outputs.
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_TIMER_H
#define __AMHW_LPC_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/** 
 * \addtogroup amhw_lpc_if_timer  
 * \copydoc amhw_lpc_timer.h
 * @{
 */
    
/**
 * \brief  Standard Timer  register block structure
 */
typedef struct amhw_lpc_timer { 
    __IO uint32_t ir;             /**< \brief Interrupt Register        */   
    __IO uint32_t tcr;            /**< \brief Timer Control Register    */
    __IO uint32_t tc;             /**< \brief Timer Counter Register    */
    __IO uint32_t pr;             /**< \brief Prescale Register         */
    __IO uint32_t pc;             /**< \brief Prescale Counter Register */
    __IO uint32_t mcr;            /**< \brief Match Control Register    */
    __IO uint32_t mr[4];          /**< \brief Match Register            */
    __IO uint32_t ccr;            /**< \brief Capture Control Register  */
    __IO uint32_t cr[4];          /**< \brief Capture Register          */
    __IO uint32_t emr;            /**< \brief External Match Register   */
    __I  uint32_t reserved0[12];  /**< \brief reserved                  */
    __IO uint32_t ctcr;           /**< \brief Count Control Register    */
    __IO uint32_t pwmc;           /**< \brief PWM Control Register      */
} amhw_lpc_timer_t;

/**
 * \brief Determine if a match interrupt is pending
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : Match interrupt channel(0 ~ 3) to check
 *
 * \retval  AM_TRUE  : the interrupt is pending,
 * \retval  AM_FALSE : no pending
 *
 */
am_static_inline 
am_bool_t amhw_lpc_timer_mat_pending_get (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    return (am_bool_t)((p_hw_timer->ir & (1 << ch)) != 0);
}

/**
 * \brief Determine if a capture interrupt is pending
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : Capture interrupt channel(0 ~ 3) to check
 *
 * \retval  AM_TRUE  : the interrupt is pending,
 * \retval  AM_FALSE : non pending
 */
am_static_inline  
am_bool_t amhw_lpc_timer_cap_pending_get (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    return (am_bool_t)((p_hw_timer->ir & (1 << (ch + 4))) != 0);
}

/**
 * \brief Clears a match interrupt pending
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : Match interrupt pending channel(0 ~ 3) to clear
 *
 * \return    None
 */
am_static_inline
void amhw_lpc_timer_mat_pending_clr (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->ir = (1 << ch);
}

/**
 * \brief Clears a capture interrupt pending
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : Capture interrupt channel(0 ~ 3) to clear
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_timer_cap_pending_clr (amhw_lpc_timer_t *p_hw_timer, int8_t ch)
{
    p_hw_timer->ir = (0x10 << ch);
}

/**
 * \brief  Enable the timer (start counting)
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \return    None
 */
am_static_inline 
void amhw_lpc_timer_enable (amhw_lpc_timer_t *p_hw_timer)
{
    p_hw_timer->tcr = (p_hw_timer->tcr & 0x03) | (1 << 0);
}

/**
 * \brief  Disable the timer (stop counting)
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \return      None
 */
am_static_inline 
void amhw_lpc_timer_disable (amhw_lpc_timer_t *p_hw_timer)
{
    p_hw_timer->tcr = (p_hw_timer->tcr & 0x03) & ~(1 << 0);
}

/**
 * \brief  Get the current timer count
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \return    Current timer count value
 */
am_static_inline 
uint32_t amhw_lpc_timer_count_get (amhw_lpc_timer_t *p_hw_timer)
{
    return p_hw_timer->tc;
}

/**
 * \brief Set the current timer count
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] value      : Setting count value.
 * \return None
 */
am_static_inline 
void amhw_lpc_timer_count_set (amhw_lpc_timer_t *p_hw_timer, uint32_t value)
{
    p_hw_timer->tc = value;
}

/**
 * \brief Get the prescaler value
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \return    Current timer prescale count value
 */
am_static_inline 
uint32_t amhw_lpc_timer_prescale_get (amhw_lpc_timer_t *p_hw_timer)
{
    return p_hw_timer->pr;
}

/**
 * \brief Sets the prescaler value
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] prescale   : Prescale value to set
 *
 * \return    None
 */
am_static_inline  
void amhw_lpc_timer_prescale_set (amhw_lpc_timer_t *p_hw_timer, uint32_t prescale)
{
    p_hw_timer->pr = prescale;
}

/**
 * \brief Get the current Prescale count
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \return    Current timer prescale count value
 */
am_static_inline 
uint32_t amhw_lpc_timer_prescale_count_get (amhw_lpc_timer_t *p_hw_timer)
{
    return p_hw_timer->pc;
}

/**
 * \brief Sets a timer match value
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : Match channel(0 ~ 3) value to set
 * \param[in] val        : Match value for the selected match channel
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_timer_mat_val_set (amhw_lpc_timer_t *p_hw_timer, uint8_t ch, uint32_t val)
{
    p_hw_timer->mr[ch] = val;
}

/**
 * \brief Get capture value from a capture channel
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : capture channel(0 ~ 3) value to get
 *
 * \return    The selected capture channel value
 */
am_static_inline  
uint32_t amhw_lpc_timer_cap_val_get (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    return p_hw_timer->cr[ch];
}

/**
 * \brief  Resets the timer counts and prescale counts to 0
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \return      None
 */
void amhw_lpc_timer_reset (amhw_lpc_timer_t *p_hw_timer);

/**
 * \brief  Enables a match interrupt when the count equal to the match value.
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : match channel(0 ~ 3) to enable interrupt
 *
 * \return      None
 */
am_static_inline 
void amhw_lpc_timer_mat_int_enable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->mcr = (p_hw_timer->mcr & (0x0FFF)) | (1 << (ch * 3));
}

/**
 * \brief Disable a match interrupt when the count equal to the match value.
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : match channel(0 ~ 3) to enable interrupt
 *
 * \return      None
 */
am_static_inline 
void amhw_lpc_timer_mat_int_disable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->mcr = (p_hw_timer->mcr & (0x0FFF)) & (~(1 << (ch * 3)));
}

/**
 * \brief Enable reset the counter when the count equal to the match value
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : match channel(0 ~ 3) to enable reset
 *
 * \return      None
 */
am_static_inline  
void amhw_lpc_timer_mat_reset_enable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->mcr = (p_hw_timer->mcr & (0x0FFF)) | (1 << ((ch * 3) + 1));
}

/**
 * \brief Disable a match reset the counter when the count equal to 
 *         the match value
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : match channel(0 ~ 3) to disable reset
 *
 * \return    None
 */
am_static_inline  
void amhw_lpc_timer_mat_reset_disable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->mcr = (p_hw_timer->mcr & (0x0FFF)) & (~(1 << ((ch * 3) + 1)));
}

/**
 * \brief Enable stop the counter when the count equal to the match value
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : match channel(0 ~ 3) to enable stop
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_timer_mat_stop_enable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->mcr = (p_hw_timer->mcr & (0x0FFF)) | (1 << (((ch * 3) + 2)));
}

/**
 * \brief Disable stop the counter when the count equal to the match value
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : match channel(0 ~ 3) to disable stop
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_timer_mat_stop_disable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->mcr = (p_hw_timer->mcr & (0x0FFF)) | (~(1 << (((ch * 3) + 2))));
}

/**
 * \brief Enables capture on rising edge  
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : capture channel(0 ~ 3) to enable rising edge
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_timer_cap_rising_enable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->ccr = (p_hw_timer->ccr & 0x0FFF) | (1 << (ch * 3));
}

/**
 * \brief Disable capture on rising edge  
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : capture channel(0 ~ 3) to Disable rising edge
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_timer_cap_rising_disable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->ccr = (p_hw_timer->ccr & 0x0FFF) & (~(1 << (ch * 3)));
}


/**
 * \brief Enable capture on falling edge  
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : capture channel(0 ~ 3) to enable falling edge
 *
 * \return    None
 */
am_static_inline  
void amhw_lpc_timer_cap_falling_enable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->ccr = (p_hw_timer->ccr & 0x0FFF) | (1 << (ch * 3 + 1));
}

/**
 * \brief Disable capture on falling edge  
 * 
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : capture channel(0 ~ 3) to disable falling edge
 *
 * \return    None
 */
am_static_inline  
void amhw_lpc_timer_cap_falling_disable (amhw_lpc_timer_t *p_hw_timer, int8_t ch)
{
    p_hw_timer->ccr = (p_hw_timer->ccr & 0x0FFF) | (~(1 << (ch * 3 + 1)));
}

/**
 * \brief Enable interrupt on capture
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : capture channel(0 ~ 3) to enable interrupt
 *
 * \return    None
 */
am_static_inline  
void amhw_lpc_timer_cap_int_enable (amhw_lpc_timer_t *p_hw_timer, int8_t ch)
{
    p_hw_timer->ccr = (p_hw_timer->ccr & 0x0FFF) | (1 << (ch * 3 + 2));
}

/**
 * \brief Disable interrupt on capture
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] ch         : capture channel(0 ~ 3) to disable interrupt
 *
 * \return    None
 */
am_static_inline  
void amhw_lpc_timer_cap_int_disable (amhw_lpc_timer_t *p_hw_timer, uint8_t ch)
{
    p_hw_timer->ccr = (p_hw_timer->ccr & 0x0FFF) & (~(1 << (ch * 3 + 2)));
}

/**
 * \brief Standard timer match pin change state
 */
typedef enum  amhw_lpc_timer_mat_pin_state {
    AMHW_LPC_TIMER_MAT_PIN_DO_NOTHING = 0, /**< \brief does nothing on match pin  */
    AMHW_LPC_TIMER_MAT_PIN_CLR        = 1, /**< \brief clear match pin to low     */
    AMHW_LPC_TIMER_MAT_PIN_SET        = 2, /**< \brief set match pin to high      */
    AMHW_LPC_TIMER_MAT_PIN_TOGGLE     = 3  /**< \brief toggles match pin          */
} amhw_lpc_timer_mat_pin_state_t;

/**
 * \brief Set external match pin control
 *
 *  When the timer counter value equal to the match value.
 *  The output pin can set, clear, toggle or do nothing.
 *
 * \param[in] p_hw_timer    : Pointer to Timer register block
 * \param[in] ch            : Match channel pin to use (0 ~ 3)
 * \param[in] initial_state : Initial state of the pin, 1 for high, 0 for low
 * \param[in] match_state   : Selects the match state for the pin
 *
 * \return    None
 */
void amhw_lpc_timer_mat_pin_ctrl (amhw_lpc_timer_t              *p_hw_timer,
                                  uint8_t                    ch,
                                  uint8_t                    initial_state,
                                  amhw_lpc_timer_mat_pin_state_t match_state);

/**
 * \brief Standard timer clock and edge for count source
 */
typedef enum amhw_lpc_timer_src {
    AMHW_LPC_TIMER_SRC_RISING_PCLK  = 0, /**< \brief Timer ticks on PCLK rising edge     */
    AMHW_LPC_TIMER_SRC_RISING_CAP   = 1, /**< \brief Timer ticks on capture rising edge  */
    AMHW_LPC_TIMER_SRC_FALLING_CAP  = 2, /**< \brief Timer ticks on capture falling edge */
    AMHW_LPC_TIMER_SRC_BOTH_CAP     = 3  /**< \brief Timer ticks on capture both edges   */
} amhw_lpc_timer_src_t;

/**
 * \brief Sets timer count source and edge
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] timer_src  : Timer clock source and edge
 * \param[in] cap_num    : Capture number pin to use (0 ~ 3)
 *
 * \return    None
 *
 * \note  If timer_src selected a capture pin, select the specific CAPn pin with the ch 
 *        value will take effect.
 */
am_static_inline  
void amhw_lpc_timer_src_set (amhw_lpc_timer_t     *p_hw_timer, 
                             amhw_lpc_timer_src_t  timer_src,
                             uint8_t           cap_num)
{
    p_hw_timer->ctcr = (p_hw_timer->ctcr &  0xF0ul)  |
                       ((uint32_t)timer_src)         |
                       ((uint32_t)cap_num << 2);
}

/**
 * \brief The capture source for clear the timer and the prescaler.
 */
typedef enum  amhw_lpc_timer_cap_clrsrc {
    AMHW_LPC_TIMER_CAP_CLRSRC_CH0_RISING  = 0, /**< \brief channel 0 rising edge   */
    AMHW_LPC_TIMER_CAP_CLRSRC_CH0_FALLING = 1, /**< \brief channel 0 Falling edge  */
    AMHW_LPC_TIMER_CAP_CLRSRC_CH1_RISING  = 2, /**< \brief channel 1 rising edge   */
    AMHW_LPC_TIMER_CAP_CLRSRC_CH1_FALLING = 3, /**< \brief channel 1 Falling edge  */
    AMHW_LPC_TIMER_CAP_CLRSRC_CH2_RISING  = 4, /**< \brief channel 2 rising edge   */
    AMHW_LPC_TIMER_CAP_CLRSRC_CH2_FALLING = 5, /**< \brief channel 2 Falling edge  */ 
} amhw_lpc_timer_cap_clrsrc_t;

/**
 * \brief Enable the capture src clear the timer and the prescaler
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] clr_src    : The capture clear src edge
 *
 * \return    None
 */
am_static_inline
void amhw_lpc_timer_cap_clr_enable (amhw_lpc_timer_t            *p_hw_timer,
                                    amhw_lpc_timer_cap_clrsrc_t  clr_src)
{
    p_hw_timer->ctcr = (p_hw_timer->ctcr & 0x1F) | (clr_src << 5) | (1 <<  4);
}

/**
 * \brief  Disable the capture src clear the timer and the prescaler
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \return    None
 */
am_static_inline 
void amhw_lpc_timer_cap_clr_disable (amhw_lpc_timer_t *p_hw_timer)
{
    p_hw_timer->ctcr &= ~((uint32_t)(1 << 4));
}

/**
 * \brief Enable match channel for PWM mode
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] mat_num    : Match number(0 ~ 3) for PWM mode
 *
 * \return    None
 * 
 * \note  When enable the match channel for PWM mode. when the timer counter 
 *        value less than the match value. The output is low level.Other wise, 
 *        The Output is High level.
 */
am_static_inline 
void amhw_lpc_timer_mat_pwm_enable (amhw_lpc_timer_t *p_hw_timer, uint8_t mat_num)
{
    p_hw_timer->pwmc |=  (1 << mat_num);
}

/**
 * \brief Disable match channel for PWM mode
 *
 * \param[in] p_hw_timer : Pointer to Timer register block
 * \param[in] mat_num    : Match number(0 ~ 3) for PWM mode
 *
 * \return None
 */
am_static_inline 
void amhw_lpc_timer_mat_pwm_disable (amhw_lpc_timer_t *p_hw_timer, uint8_t mat_num)
{
    p_hw_timer->pwmc &=  ~(1 << mat_num);
}

/**
 * @} amhw_lpc_if_timer
 */

#ifdef __cplusplus
}
#endif


#endif /* __AMHW_LPC_TIMER_H */

/* end of file */
