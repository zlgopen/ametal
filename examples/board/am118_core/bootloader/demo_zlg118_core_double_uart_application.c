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
 * \brief bootloader 例程，本demo是作为应用程序固件。
 *
 * - 操作步骤:
 *
 *   1. eclipse工程打开demo_am118_core_application.ld文件，可设置应用程序代码起始地址，如下：
 *           FLASH (rx)  : ORIGIN = 0x00006800, LENGTH = 115K //应用程序代码起始地址不应超过0x6A00
 *
 *      keil工程打开demo_am118_core_application.sct文件，根据提示修改boot程序大小，如下：
 *          #define     ALL_START    0X00006800    //不建议超过0x6A00
 *          #define     ALL_SIZE     0X0001CC00    //根据实际情况设置，建议本分区大小与应用区大小一致
 *
 *      除上面工程链接文件之外，需要更改/user_config/../am_appconf_double_zlg118.c文件内的信息结构体设置
 *
 *      默认flash分配情况如下：
 *      区域划分:    |------------|-------|-----------------|-------------------|
 *      名称:        bootloader    flag      application        save_flie
 *      大小:         25.5k        0.5k          115k             115k
 *
 *   2.编译工程
 *   3.固件处理，参考下面note提示
 *6
 * - 例程现象：
 *   1. LED灯闪烁，串口循环打印。
 *   2. 等待用户输入升级命令（在应用中升级固件）
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
 * - 1.00 20-8-21  xjy, first implementation
 * \endinternal
 */

#include "am_appconf_double_zlg118.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_boot_enter_check_uart_cmd.h"
#include "am_boot_firmware_recv_uart.h"
#include "am_boot_msg.h"
#include "am_boot_firmware.h"
#include "am_zlg118_inst_init.h"
#include "demo_boot_entries.h"

void demo_zlg118_core_double_application_entry (void)
{
    AM_DBG_INFO("application : am118_core double application start up successful!\r\n");
    am_mdelay(1);

    am_uart_handle_t             uart_handle;
    am_boot_firmware_handle_t    firmware_handle;
    am_boot_enter_check_handle_t check_handle;
    am_boot_firwa_recv_handle_t  firwa_recv_handle;
    am_boot_msg_handle_t         msg_handle;
    am_zlg118_boot_inst_init();
    firmware_handle = am_zlg118_boot_firmware_flash_inst_init();

    uart_handle = am_zlg118_uart0_inst_init();
    am_debug_init(uart_handle, 115200);
    firwa_recv_handle = am_boot_firwa_recv_uart_init(firmware_handle, uart_handle);
    check_handle = am_boot_enter_check_uart_cmd_init(uart_handle);
    msg_handle = am_zlg118_boot_msg_flag_inst_init();

    while(1) {
        /* 此demo需要循环调用 */
        demo_std_double_application_entry(
            firwa_recv_handle,
            check_handle,
            msg_handle);

        am_led_toggle(0);
        am_mdelay(1000);
        AM_DBG_INFO("application : am118_core double application running!\r\n");
    }
}

/* end of file */
