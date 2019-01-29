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
 * \brief LPC84X 模板工程
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 19-01-20  bzq, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_led.h"

/**
 * \brief AMetal 应用程序入口
 */
void am_main (void) 
{
    AM_DBG_INFO("Start up successful!\r\n");
    /* 例程入口 */

    AM_FOREVER {
        am_led_toggle(LED0);

        am_mdelay(500);
    }
}

/* end of file */
