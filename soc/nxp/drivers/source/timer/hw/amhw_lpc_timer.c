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
 * \brief Standard counter/timers implementation for operating hardware.
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
 
#include "hw/amhw_lpc_timer.h"
 
/*******************************************************************************
* Public functions
*******************************************************************************/

void amhw_lpc_timer_mat_pin_ctrl (amhw_lpc_timer_t              *p_hw_timer, 
                                  uint8_t                    ch,
                                  uint8_t                    initial_state,
                                  amhw_lpc_timer_mat_pin_state_t match_state)
{
    
    /* Clear bits corresponding to selected match register */
    uint32_t   mask = (1 << ch) | (0x03 << (4 + (ch * 2)));
    
    /* Set new configuration for selected match register */
    p_hw_timer->emr = ((p_hw_timer->emr & 0xFFF) & ~mask) |
                      (((uint32_t)initial_state) << ch)   |
                      (((uint32_t)match_state)   << (4 + (ch * 2)));
}

/**
 * \brief Resets the timer counter and pre-scale counts to 0 
 */
void amhw_lpc_timer_reset (amhw_lpc_timer_t *p_hw_timer)
{
    uint32_t reg;
    
    reg = p_hw_timer->tcr;
    
    /* Disable timer, set terminal count to non-0 */
    p_hw_timer->tcr = 0;
    p_hw_timer->tc  = 1;

    /* Reset timer counter */
    p_hw_timer->tcr = ((uint32_t)(1 << 1));

    /* Wait for terminal count to clear */
    while (p_hw_timer->tc != 0) {}

    /* Restore timer state */
    p_hw_timer->tcr = reg;
}

/* end of file */
