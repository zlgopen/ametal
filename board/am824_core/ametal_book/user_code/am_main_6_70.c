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
 * \brief 程序清单6.70
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
    am_pcf85063_handle_t pcf85063_handle = am_microport_rtc_inst_init();
    uint8_t              data            = 0x00;
    
    am_pcf85063_ram_write (pcf85063_handle,  0x55);  // 写入数据0x55
    am_pcf85063_ram_read (pcf85063_handle,  &data);  // 读取数据
    
    if (data != 0x55) {   // 若数据不为0x55，表面读写出错，则点亮LED0
        am_led_on(0);
    }
    while(1) {
    }
}


/* end of file */

