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
#include "am_hc32f460_inst_init.h"


/**
 * \addtogroup am_if_src_hwconf_debug_uart
 * \copydoc am_hwconf_debug_uart.c
 * @{
 */

/**
 * \brief 使用的调试串口，默认 UART3
 * 1 - UART1; 2 - UART2; 3 - UART3;
 * 4 - UART4;
 */
#define __DEBUG_UART            3

/** \brief 配置调试串口输出的波特率 */
#define __DEBUG_BAUDRATE        115200

/** \brief 调试串口实例初始化 */
am_uart_handle_t am_debug_uart_inst_init (void)
{
    am_uart_handle_t handle = NULL;

#if (__DEBUG_UART == 1)

    handle = am_hc32f460_uart1_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 1) */

#if (__DEBUG_UART == 2)

    handle = am_hc32f460_uart2_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 2) */

#if (__DEBUG_UART == 3)

    handle = (am_uart_handle_t)am_hc32f460_uart3_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 3) */

#if (__DEBUG_UART == 4)

    handle = am_hc32f460_uart4_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 4) */

#if (__DEBUG_UART == 5)

    handle = am_hc32f460_uart5_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 5) */

#if (__DEBUG_UART == 6)

    handle = am_hc32f460_uart6_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 6) */

#if (__DEBUG_UART == 7)

    handle = am_hc32f460_uart7_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 7) */

#if (__DEBUG_UART == 8)

    handle = am_hc32f460_uart8_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);

#endif /* (__DEBUG_UART == 8) */

    return handle;
}

/**
 * @}
 */

/* end of file */
