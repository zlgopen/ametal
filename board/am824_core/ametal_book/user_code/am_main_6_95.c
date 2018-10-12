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
 * \brief 程序清单6.95
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-19  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_hwconf_microport.h"
#include "am_led.h"

int am_main (void)
{
    am_ds1302_handle_t   ds1302_handle = am_microport_ds1302_inst_init();
    uint8_t               data[31];
    int     i;
    for (i = 0; i < 31; i++) {
        data[i] = i;
    }
    am_ds1302_ram_write (ds1302_handle, data, 31, 0);    // 从起始地址0开始写入31字节数据
    
    for (i = 0; i < 31; i++) {
        data[i] = 0;
    }
    am_ds1302_ram_read (ds1302_handle, data, 31, 0);    // 从起始地址0开始读出31字节数据
    for (i = 0; i < 31; i++) {
        if (data[i] != i) {
            am_led_on(0);
        }
    }
    while(1) {
    }
}


/* end of file */

