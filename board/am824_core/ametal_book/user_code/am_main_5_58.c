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
 * \brief 程序清单5.58
 *
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_pwm.h"
#include "am_lpc82x_inst_init.h"

int am_main(void)
{
    am_pwm_handle_t  pwm_handle = am_lpc82x_sct0_pwm_inst_init ();

    am_pwm_config(pwm_handle, 1, 500000, 1000000);    // PWM通道1对应PIO0_24
    am_pwm_enable(pwm_handle, 1);                     // 使能PWM0 
    while(1) {
    }
}



/* end of file */

