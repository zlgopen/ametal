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
 * \brief 调试串口用户配置文件
 * \sa am_hwconf_debug_uart.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-20  pea, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_uart.h"
#include "am_vdebug.h"
#include "am_hc32l13x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_debug_uart
 * \copydoc am_hwconf_debug_uart.c
 * @{
 */

/**
 * \brief 使用的调试串口，默认 UART1
 * 1 - UART1; 2 - UART2; 3 - UART3
 */
#define __DEBUG_UART            1

/** \brief 配置调试串口输出的波特率 */
#define __DEBUG_BAUDRATE        115200
am_uart_handle_t uart_handle = NULL;

/** \brief 调试串口实例初始化 */
am_uart_handle_t am_debug_uart_inst_init (void)
{

#if (__DEBUG_UART == 0)

    uart_handle = am_hc32_uart0_inst_init();

    /* 调试初始化 */
    am_debug_init(uart_handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 0) */

#if (__DEBUG_UART == 1)

    uart_handle = am_hc32_uart1_inst_init();
    /* 调试初始化 */
    am_debug_init(uart_handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 1) */

#if (__DEBUG_UART == 2)

    handle = am_hc32_uart2_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 2) */

#if (__DEBUG_UART == 3)

    handle = am_hc32_uart3_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 3) */

    return uart_handle;
}

/**
 * @}
 */

/* end of file */
