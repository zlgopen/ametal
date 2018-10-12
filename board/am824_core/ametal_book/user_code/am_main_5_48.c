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
 * \brief 程序清单5.48
 *
 * \note 该历程需要用到miniport拓展板
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "led.h"
#include "buzzer.h"
#include "keyn.h"
#include "am_delay.h"
int am_main (void)
{
    uint8_t key_return;

    led_init();
    keyn_init();
    buzzer_init();
    while(1) {
        key_return = keyn_scan();
        if (key_return == 0) {
            buzzer_beep(100);
        } else if (key_return == 0x80) {
            led_toggle(0);
        }
        am_mdelay(10);
    }
}



/* end of file */

