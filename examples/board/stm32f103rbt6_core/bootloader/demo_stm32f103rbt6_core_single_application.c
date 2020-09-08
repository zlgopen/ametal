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
 *       FLASH (rx)  : ORIGIN = 0x08007000, LENGTH = 100K  //single
 *
 *     的配置, 屏蔽其他flash配置。
 *
 *     keil工程打开demo_stm32f103rbt6_core_application.sct文件，打开单区的配置，屏蔽其他配置
 *
 *   2.编译工程
 *   3.固件处理，参考下面note提示
 *
 * - 例程现象：
 *   1. LED灯闪烁，串口循环打印。
 *   
 *
 * \note
 *    工程编译后会生成对应的工程bin文件，需要将bin文件进行处理后才能作为固件发送
 *
 *    固件处理：
 *
 *    打开在ametal/tools/bootloader/固件校验/ 文件夹，目录下有一个bin_chek_sum.bin的执行文件，
 *    将本工程编译出的bin文件拷贝到和bin_chek_sum.bin同一个目录下，
 *    在该目录下打开cmd，在命令行中依次输入：
 *         bin_chek_sum.bin  xxx.bin（编译后的bin文件名称） xxx.bin（目标输出文件，名字任意取）
 *
 *    执行命令后会生成一个由用户自己取名的目标输出文件，这个文件就是需要发送的固件。
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

void demo_stm32f103rbt6_core_single_application_entry (void)
{
    AM_DBG_INFO("application : stm32f103rbt6_core single application start up successful!\r\n");

    while(1) {
        AM_DBG_INFO("application : stm32f103rbt6_core single application running\r\n");
        am_led_toggle(0);
        am_mdelay(1000);
    }
}

/* end of file */
