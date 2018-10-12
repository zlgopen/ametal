/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief FM24C02 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-28  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_EP24CXX_H
#define __AM_HWCONF_EP24CXX_H

#include "am_nvram.h"
#include "am_ep24cxx.h"

/**
 * \brief FM24C02 实例初始化（字节使用芯片相关接口操作FM24C02）
 * \return FM24C02 芯片句柄，若为NULL，表明初始化失败
 */
am_ep24cxx_handle_t am_fm24c02_inst_init (void);

/**
 * \brief FM24C02 实例初始化 （用作标准的NVRAM器件）
 *
 * \retval  AW_OK  初始化成功
 * \retval   < 0   初始化失败，请检C文件中各配置项是否合法
 */
int am_fm24c02_nvram_inst_init (void);

#endif

/* end of file */
