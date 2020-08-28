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
 * \brief bootloader 双区例程(既可以本地升级也可以在应用程序中升级)。
 *
 * - 操作步骤（本地升级）：
 *   1. eclipse工程打开demo_amf07x_core_bootloader.ld文件，可根据具体的boot程序大小设置参数，如下：
 *          FLASH  (rx) : ORIGIN = 0x00000000, LENGTH = 0X6600  //长度最大为26.5k
 *
 *      keil工程打开demo_amf07x_core_bootloader.sct文件，根据提示修改boot程序大小，如下：
 *          #define     ALL_SIZE      0X00006600     //只可更改此参数
 *
 *      除上面工程链接文件之外，需要更改/user_config/../am_bootconf_hc32f07x_double.c文件内的信息结构体设置
 *
 *      默认flash分配情况如下：
 *      区域划分:    |------------|-------|-----------------|-------------------|
 *      名称:        bootloader    flag      application        save_flie
 *      大小:         25.5k        0.5k          51k             51k
 *
 *   2. 连接开发板和上位机串口助手，波特率设置为115200。（例程使用串口1）
 *   
 *   3. 给开发板下载本工程编译的程序，复位开发板运行。
 *
 *   4. 串口助手打印5秒倒计时，在5秒内通过串口助手发送任意数据给开发板，开发板准备接收固件。
 *
 *   5. 将处理好的固件通过串口助手发送给开发板（如果在发送的过程出错，将重新接收固件，上位机重新发送）。
 *
 *   6. 接收成功，程序跳转到应用程序执行，串口打印出应用程序信息，LED灯闪烁。
 *
 * - 操作步骤（应用中升级）：
 *
 *   应用中升级的前提条件是已经通过本地升级的方式将双区应用程序固件下载到了开发板中，并成功运行。
 *
 *   1. 发送升级命令：0x5a 0xa6 0x11 0x66 0x0d（命令是以16进制的形式发送，注意设置串口助手）
 *   
 *   2. 等待串口助手打印已经准备接收固件信息后，发送固件。
 *
 *   3. 固件发送成功，开发板重启，5秒延时后进去新的应用程序（延时时间内不要发送任何数据）
 *
 * \note
 *    固件处理：
 *
 *    打开在ametal/tools/bootloader/固件校验/ 文件夹，目录下有一个bin_chek_sum.bin的执行文件，
 *    将测试的应用工程编译出的bin文件拷贝到和bin_chek_sum.bin同一个目录下，
 *    在该目录下打开cmd，在命令行中依次输入：
 *         bin_chek_sum.exe  xxx.bin（编译后的bin文件名称） xxx.bin（目标输出文件，名字任意取）
 *
 *    执行命令后会生成一个由用户自己取名的目标输出文件，这个文件就是需要发送的固件。
 *    
 *　    
 * \internal
 * \par Modification History
 * - 1.00 20-8-27  xjy, first implementation
 * \endinternal
 */

#include "am_uart.h"
#include "am_vdebug.h"
#include "am_boot_firmware.h"
#include "am_boot_flash.h"
#include "demo_boot_entries.h"
#include "am_bootconf_hc32f07x.h"
#include "am_hc32f07x_inst_init.h"
#include "am_boot_enter_check_uart.h"
#include "am_boot_firmware_recv_uart.h"

void demo_hc32f07x_core_double_bootloader_uart_entry (void)
{
    am_uart_handle_t           uart_handle;
    am_boot_firmware_handle_t  firmware_handle;

    /* bootloader 标准接口初始化  */
    am_hc32_boot_double_inst_init();

    firmware_handle = am_hc32_boot_double_firmware_flash_inst_init();

    uart_handle = am_hc32_uart0_inst_init();
    am_debug_init(uart_handle, 115200);

    demo_std_double_bootloader_entry(
        am_boot_firwa_recv_uart_init(firmware_handle, uart_handle),
        am_boot_enter_check_uart_init(uart_handle),
        am_hc32_boot_msg_flag_inst_init());
}

/* end of file */
