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
 * \brief 调试串口配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-20  pea, first implementation
 * \endinternal
 */
#ifndef __AM_HWCONF_DEBUG_UART_H
#define __AM_HWCONF_DEBUG_UART_H

#include "ametal.h"
#include "am_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 调试串口实例初始化
 * \param 无
 * \return 调试串口使用的 UART 句柄
 */
am_uart_handle_t am_debug_uart_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_DEBUG_UART_H */

/* end of file */
