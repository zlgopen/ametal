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
 * \brief bootloader 例程，本demo是作为应用程序。
 *
 * - 操作步骤：
 *   参考AMmetal-MM32L373-Core-bootloader操作手册
 *
 *
 * \par 源代码
 * \snippet demo_mm32l373_core_hw_adc_int.c src_mm32l373_core_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-18  yrh, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l373_core_boot_kft_application
 * \copydoc demo_mm32l373_core_boot_kft_application.c
 */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_led.h"
#include "am_delay.h"

void demo_mm32l373_core_boot_kft_application_entry (void)
{
    AM_DBG_INFO("application : mm32l373_core_boot_kft_application start up successful!\r\n");

    while(1) {
        am_led_toggle(0);
        am_mdelay(1000);
    }
}
