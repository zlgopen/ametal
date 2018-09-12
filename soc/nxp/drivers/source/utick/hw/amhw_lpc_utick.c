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
 * \brief UTICK(Micro-tick timer) implementation for operating hardware.
 * 
 * -Ultra simple timer.
 * -Write once to start.
 * -Interrupt or software polling
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */
 
#include "hw/amhw_lpc_utick.h"
 
/*******************************************************************************
* Public functions
*******************************************************************************/

void amhw_lpc_utick_tick_set (amhw_lpc_utick_t *p_hw_utick, 
                              uint32_t      tick_value,
                              am_bool_t        repeat)
{
    if (repeat == AM_TRUE) {
        tick_value |= AMHW_LPC_UTICK_CTRL_REPEAT;
    }

    p_hw_utick->ctrl = tick_value;
}

/* end of file */
