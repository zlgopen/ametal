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
 * \brief Newlib 适配器
 *
 * \par 说明
 * 1. 建议使用 newlib-nano库，在链接器参数中增加参数：--specs=nano.specs
 * 2. 若需要使用printf打印浮点数，则需要增加参数： -u _printf_float
 * 3. 若需要使用scanf接收浮点数，则需要增加参数：  -u _scanf_float
 * 4. 若使用了printf，建议栈空间不小于 1K
 *
 * 在使用前，必须初始化适配器，若使用UART输出信息，则初始化函数的调用范例如下：
 *
 * \code
 * static int __console_send (void *p_cookie, void *p_buf, size_t cnt)
 * {
 *    return am_uart_poll_send(p_cookie, p_buf, cnt);
 * }
 *
 * static int __console_receive (void *p_cookie, void *p_buf, size_t cnt)
 * {
 *     return am_uart_poll_receive(p_cookie, p_buf, cnt);
 * }
 *
 * static const am_newlib_adapter_ops_t  __g_newlib_adapter_ops = {
 *     __console_send,
 *     __console_receive
 * };
 *
 * void am_board_newlib_init (am_uart_handle_t handle)
 * {
 *     am_newlib_adapter_init(&__g_newlib_adapter_ops, handle);
 * }
 * \endcode
 *
 * \internal
 * \par modification history:
 * - 1.00 17-07-24  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_NEWLIB_ADAPTER_H
#define __AM_NEWLIB_ADAPTER_H

#include "am_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_newlib_adapter
 * \copydoc am_newlib_adapter.h
 * @{
 */

/**
 * \brief 配置选项，根据实际硬件平台提供
 */
typedef struct am_newlib_adapter_ops {
    int (*pfn_console_send) (void *p_cookie, const void *p_buf, size_t cnt);
    int (*pfn_console_receive) (void *p_cookie, void *p_buf, size_t cnt);
} am_newlib_adapter_ops_t;

/**
 * \brief 适配器初始化
 *
 * \param[in] p_ops     : 驱动函数
 * \param[in] p_cookie  : 驱动函数参数
 *
 * \return AM_OK
 */
int am_newlib_adapter_init (const am_newlib_adapter_ops_t *p_ops,
                            void                          *p_cookie);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_NEWLIB_ADAPTER_H */

/* end of file */
