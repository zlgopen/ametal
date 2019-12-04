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
 * \brief FM175xx 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */
#ifndef __AM_HWCONF_FM175XX_H
#define __AM_HWCONF_FM175XX_H

#include "ametal.h"
#include "am_fm175xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief fm175xx实例初始化
 * \return fm175xx服务句柄，若为NULL，表明初始化失败
 */
am_fm175xx_handle_t am_fm175xx_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif
