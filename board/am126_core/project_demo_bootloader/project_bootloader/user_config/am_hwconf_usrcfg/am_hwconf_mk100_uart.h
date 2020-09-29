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
 * \brief MK100配置文件
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-20  mxy, first implementation.
 * \endinternal
 */
#ifndef __AM_HWCONF_MK100_UART_H
#define __AM_HWCONF_MK100_UART_H

#include "ametal.h"
#include "am_mk100.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief MK100 实例初始化
 * \retval MK100 句柄值，若为 NULL，表明初始化失败
 */
am_mk100_handle_t am_mk100_uart2_inst_init (void);

/**
 * \brief MK100 实例解初始化
 * \param[in] handle : 通过 am_mk100_uart2_inst_init() 函数获得的MK100句柄
 * \retval 无
 */
void am_mk100_uart2_inst_deinit(am_mk100_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
