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
 * \brief bootloader 例程，本demo是作为bootloader。
 *
 * - 操作步骤：
 *   参考AMmetal-AM217-Core-bootloader操作手册
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-18  yrh, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l373_core_boot_kft_bootloader
 * \copydoc demo_mm32l373_core_boot_kft_bootloader.c
 */
#include "ametal.h"
#include "am_led.h"
#include "mm32l373_pin.h"
#include "am_boot.h"
#include "am_boot_kft.h"
#include "am_mm32l373_inst_init.h"
#include "am_bootconf_mm32l373_kft.h"

/** \brief 固件升级引脚 */
#define __ENABLE_PIN          (PIOC_7)
#define DEBUG_PRINT             0

void demo_mm32l373_core_boot_kft_bootloader_entry (void)
{
#if DEBUG_PRINT
    am_uart_handle_t uart;
    uart = am_mm32l373_uart1_inst_init();
    am_debug_init(uart, 115200);
#endif

    am_boot_flash_handle_t flash_handle = am_mm32l373_boot_flash_inst_init();
    am_gpio_pin_cfg(__ENABLE_PIN, AM_GPIO_INPUT | AM_GPIO_PULLUP);

    am_mm32l373_std_boot_inst_init(flash_handle);

    if (am_gpio_get(__ENABLE_PIN) && am_boot_app_is_ready()) {

        am_gpio_pin_cfg(__ENABLE_PIN, AM_GPIO_INPUT | AM_GPIO_FLOAT);
#if DEBUG_PRINT
        am_kprintf("bootloader : go to application...\r\n");
#endif

        if(AM_OK != am_boot_go_application()){

#if DEBUG_PRINT
            am_kprintf("bootloader : go application fail!\r\n");
#endif
            am_led_on(0);
            while(1);
        }
    }

    am_mm32l373_boot_kft_inst_init(flash_handle);
#if DEBUG_PRINT
    am_kprintf("bootloader : running......\r\n");
    am_kprintf("bootloader : start command pump\r\n");
#endif
    while(1) {
        am_boot_kft_command_pump();
    }
}
