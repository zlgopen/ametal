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
 * \brief bootloader 例程，本demo是作为单区的应用程序（无法在应用程序中升级）。
 *
 * - 操作步骤:
 *   1.eclipse工程打开demo_stm32f103rbt6_core_application.ld文件，打开:
 *
 *       FLASH (rx)  : ORIGIN = 0x0800AC00, LENGTH = 85K     // kft
 *
 *     的配置, 屏蔽其他flash配置。
 *
 *     keil工程打开demo_stm32f103rbt6_core_application.sct文件，打开上位机的配置，屏蔽其他配置
 *
 *   2.编译工程
 *   3.工程出的bin文件作为固件
 *
 * - 例程现象：
 *   1. LED灯闪烁，串口循环打印。
 *   
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-18  yrh, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_led.h"
#include "am_delay.h"

void demo_stm32f103rbt6_core_kft_application_entry (void)
{
    AM_DBG_INFO("application : stm32f103rbt6_core kft application start up successful!\r\n");

    while(1) {
        AM_DBG_INFO("application : stm32f103rbt6_core kft application running\r\n");
        am_led_toggle(0);
        am_mdelay(1000);
    }
}

/* end of file */
