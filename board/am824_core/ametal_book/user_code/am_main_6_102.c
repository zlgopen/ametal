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
 * \brief 程序清单6.102
 *
 * \note 该历程需要用到第三方库，hash table
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-25  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_zlg600.h"
#include "am_hwconf_zlg600.h"
#include "am_led_dev.h"
#include "am_delay.h"
#include "string.h"

int am_main (void)
{
    int      ret;
    uint8_t  info[20];

	am_zlg600_handle_t 	zlg600_handle = am_zlg600_uart_inst_init();
    	ret = am_zlg600_info_get(zlg600_handle, &info[0]);
    	if (((ret != AM_OK) || (strcmp((char *)info, "ZLG600A V1.02"))) != 0 ) {
        am_led_on(0);
        while(1);
    	}
    	while (1) {
        am_led_toggle(0);		am_mdelay(100);
    }
}

/* end of file */
