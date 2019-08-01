/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief bootloader 双区应用程序例程
 *
 *
 * \par 源代码
 * \snippet demo_std_double_application.c src_std_double_application
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-04 yrh, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_double_application
 * \copydoc demo_std_double_application.c
 */

/** [src_std_double_application] */
#include "am_boot_firmware_recv.h"
#include "am_boot_firmware.h"
#include "am_boot_enter_check.h"
#include "am_boot_msg_flag.h"
#include "am_delay.h"
#include "am_boot.h"
#include "am_vdebug.h"

void demo_std_double_application_entry(am_boot_firwa_recv_handle_t  firwa_recv_handle,
                                       am_boot_enter_check_handle_t check_handle,
                                       am_boot_msg_handle_t         msg_handle)
{
    int ret;
    /* 是否进入接收固件 */
    if(am_boot_enter_check(check_handle)) {
        /* 接收固件 */
        ret = am_boot_firmware_recv(firwa_recv_handle);
        if(ret != AM_OK) {
            am_boot_enter_check_reinit(check_handle);
            am_kprintf("app : firmware receive fail\r\n");
            return;
        } else {
            /* 固件接收成功后，设置消息，新的固件已经接收完成 */
            am_boot_msg_set(msg_handle, (void *)AM_BOOTLOADER_FLAG_UPDATE);
            /* 系统重启前进行小段的延时，防止某些寄存器没有执行完 */
            am_mdelay(200);
            /* 系统软件复位 */
            am_boot_reset();
        }
    }
}

/** [src_std_double_application] */

/* end of file */
