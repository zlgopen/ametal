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
 *    1. eclipse工程打开demo_aml19x_core_application.ld文件，可设置应用程序代码起始地址，如下：
 *           FLASH (rx)  : ORIGIN = 0x00006800, LENGTH = 115K //应用程序代码起始地址不应超过0x6A00
 *
 *      keil工程打开demo_aml19x_core_application.sct文件，根据提示修改boot程序大小，如下：
 *          #define     ALL_START    0X00006800    //不建议超过0x6A00
 *          #define     ALL_SIZE     0X0001CC00    //根据实际情况设置
 *
 *      默认flash分配情况如下：
 *      区域划分:    |----------|-------------------|
 *      名称:        bootloader     application
 *      大小:           25.5K           114k
 *
 *   1.编译工程
 *   2.固件处理，参考下面note提示
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
 *         bin_chek_sum.exe  xxx.bin（编译后的bin文件名称） xxx.bin（目标输出文件，名字任意取）
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

void demo_hc32l19x_core_single_application_entry (void)
{
    AM_DBG_INFO("application : aml19x_core single application start up successful!\r\n");

    while(1) {
        AM_DBG_INFO("application : aml19x_core single application running\r\n");
        am_led_toggle(0);
        am_mdelay(1000);
    }
}

/* end of file */
