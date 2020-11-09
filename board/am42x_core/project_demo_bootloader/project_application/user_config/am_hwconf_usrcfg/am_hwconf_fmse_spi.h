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
 * \brief 安全芯片FMSE A03 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-03  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_FMSE_SPI_H
#define __AM_HWCONF_FMSE_SPI_H

#include "ametal.h"
#include "am_fmse_a03.h"
#include "am_fmse_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 安全芯片FMSE A03 设备实例化
 */
am_fmse_handle_t am_fmse_spi_inst_init (void);

/**
 * \brief 安全芯片FMSE A03 实例解初始化
 */
am_err_t am_fmse_spi_inst_deinit (am_fmse_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
