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
 * \brief 程序清单6.22
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-18  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_delay.h"
#include "am_led.h"
#include "am_mx25xx.h"
#include "app_test_mx25xx.h"
#include "am_hwconf_microport.h"

int am_main (void)
{
    am_mx25xx_handle_t mx25xx_handle = am_microport_flash_inst_init();
    if (app_test_mx25xx(mx25xx_handle) != AM_OK) {
        am_led_on(0);
        while(1);
    }
    while(1) {
        am_led_toggle(0);    am_mdelay(200);
    }
}

/* end of file */

