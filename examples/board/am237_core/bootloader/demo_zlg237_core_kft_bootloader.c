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
 * \brief bootloader 例程，本demo是作为bootloader，该demo需要配合kboot的上位机KinetiesFlashTools。
 *
 * - 操作步骤（本地升级）：
 *   1. eclipse工程打开demo_am237_core_bootloader.ld文件，打开:
 *
 *       FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 28K   // single
 *
 *      的配置, 屏蔽其他flash配置。
 *
 *      keil工程打开demo_am237_core_bootloader.sct文件，打开上位机的配置，屏蔽其他配置
 *
 *   2. 打开上位机KinetiesFlashTools : 在port set栏中选择uart;
 *   3. 下载本程序到开发板运行
 *   4. 点击上位机的connect，如果正确连接，会有打印信息提示
 *   5. 打开Update选项栏
 *   6. 选择升级的固件，并设置目标地址，也就是固件在目标板flash中存放的起始地址
 *   7. 点击update按键，查看打印信息是否升级成功
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-18  yrh, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_led.h"
#include "zlg237_pin.h"
#include "am_boot.h"
#include "am_boot_mem_flash.h"
#include "am_boot_kft.h"
#include "am_boot_enter_check_key.h"
#include "am_baudrate_detect.h"
#include "demo_boot_entries.h"
#include "am_zlg237_inst_init.h"
#include "am_bootconf_zlg237.h"

#define RAM_START_ADDR 0x20000000
#define RAM_SIZE       20 * 1024

void demo_zlg237_core_bootloader_kft_entry (void)
{
    am_uart_handle_t             uart_handle;
    am_boot_flash_handle_t       flash_handle;
    am_boot_mem_handle_t         memory_handle;
    am_boot_enter_check_handle_t enter_check_handle;
    am_baudrate_detect_handle_t  baudrate_detect_handle;

    flash_handle  = am_zlg237_boot_kft_flash_inst_init();
    memory_handle = am_boot_mem_flash_init(flash_handle);

    am_zlg237_boot_kft_inst_init();

    enter_check_handle = am_boot_enter_check_key_init(
        PIOC_7, AM_GPIO_INPUT | AM_GPIO_PULLUP);

    if (am_boot_enter_check(enter_check_handle) && am_boot_app_is_ready()) {
        am_boot_go_application();
    }

    baudrate_detect_handle = am_zlg237_baudrate_detect_inst_init();

    int ret;
    uint32_t baund;
    while(1) {
       ret = am_baudrate_get(baudrate_detect_handle, &baund);
       if (ret == AM_OK) {
           if(baund <= 7200 && baund > 3000) {
               baund = 4800;
           } else if(baund <=384000  && baund > 192000) {
               baund = 256000;
           }
           break;
       }
    }

    am_zlg237_baudrate_detect_inst_deinit(baudrate_detect_handle);
    /* 串口初始化应该在获取波特率之后 */
    uart_handle = am_zlg237_usart1_inst_init();

    demo_std_bootloader_kft_entry(flash_handle,
                                  memory_handle,
                                  baund,
                                  uart_handle,
                                  RAM_START_ADDR,
                                  RAM_SIZE);
}

/* end of file */
