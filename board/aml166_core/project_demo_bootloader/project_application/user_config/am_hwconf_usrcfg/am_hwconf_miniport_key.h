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
 * \brief MiniPort-Key 配置文件
 *
 * 单独使用 Miniport-KEY 时使用该文件提供的实例初始化函数
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_MINIPORT_KEY_H
#define __AM_HWCONF_MINIPORT_KEY_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief MiniPort-KEY 实例初始化
 *
 * \retval  AW_OK  初始化成功
 * \retval   < 0   初始化失败，请检C文件中各配置项是否合法
 */
int am_miniport_key_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
