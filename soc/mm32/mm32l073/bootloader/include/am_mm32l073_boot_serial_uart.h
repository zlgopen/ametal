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
 * \brief bootloader 串行数据处理标准设备实现（uart驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 18-12-11  yrh, first implementation.
 * \endinternal
 */

#ifndef __AM_BOOT_SERIAL_UART_H
#define __AM_BOOT_SERIAL_UART_H

#include "am_boot_serial_byte.h"
#include "am_boot_autobaud.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief uart驱动 serial数据的设备
 */
typedef struct am_zlg116_boot_serial_uart_dev {
    am_boot_serial_byte_serv_t    serial_serv;      /**< \brief 标准的 串行数据操作 服务    */
    //am_autobaud_handle_t          autobaud_handle;  /**< \brief 自动波特率检测的操作句柄  */
    am_uart_handle_t              uart_handle;      /**< \brief 标准的uart操作句柄  */
} am_zlg116_boot_serial_uart_dev_t;

am_boot_serial_handle_t am_zlg116_boot_serial_uart_init(am_boot_autobaud_handle_t autobaud_handle);

void am_zlg116_boot_serial_uart_deinit(void);


#ifdef __cplusplus
}
#endif

#endif /* __AM_BOOT_SERIAL_UART_H */

/* end of file */
