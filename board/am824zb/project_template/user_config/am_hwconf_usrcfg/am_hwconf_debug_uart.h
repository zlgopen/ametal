/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 调试串口用户配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-01-16  cml, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_DEBUG_UART_H
#define __AM_HWCONF_DEBUG_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_uart.h"

/**
 * \brief 调试串口实例初始化
 *
 * \param 无
 *
 * \return UART 标准服务句柄，若为 NULL，表明初始化失败
 */
am_uart_handle_t am_debug_uart_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_DEBUG_UART_H */

/* end of file */
