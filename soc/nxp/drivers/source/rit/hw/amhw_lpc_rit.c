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
 * \brief RIT(Repetitive Interrupt Timer) implementation  for operating hardware.
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
 
#include "hw/amhw_lpc_rit.h"
 
 
/******************************************************************************
* Public functions
*******************************************************************************/

/**
 * \brief Set the Counter value
 */
void amhw_lpc_rit_count_set (amhw_lpc_rit_t *p_hw_rit, uint64_t value)
{
    if ((p_hw_rit->ctrl & AMHW_LPC_RIT_CTRL_TEN) != 0) { /* the timer is running */
        
         /* 
          * Software must stop the counter 
          * before reloading it with a new value 
          */
        amhw_lpc_rit_disable(p_hw_rit);
        
        p_hw_rit->counter   = (uint32_t)(value & 0xFFFFFFFF);
        p_hw_rit->counter_h = (uint32_t)((value >> 32) & 0xFFFF);
        
        amhw_lpc_rit_enable(p_hw_rit);
        
        return;
    }
    
    p_hw_rit->counter   = (uint32_t)(value & 0xFFFFFFFF);
    p_hw_rit->counter_h = (uint32_t)((value >> 32) & 0xFFFF);
}

/* end of file */
