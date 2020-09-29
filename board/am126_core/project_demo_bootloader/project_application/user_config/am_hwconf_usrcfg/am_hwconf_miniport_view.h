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
 * \brief MiniPort-View 配置文件
 *
 * 它可以单独使用，也可以和 MiniPort-595 联合使用，以节省控制引脚。
 *
 * 当前支持的用法有：
 * 1. MiniPort-View                : am_miniport_view_inst_init();
 * 2. MiniPort-View + MiniPort-595 : am_miniport_view_595_inst_init();
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-25  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_MINIPORT_VIEW_H
#define __AM_HWCONF_MINIPORT_VIEW_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief MiniPort-View单独使用实例初始化
 *
 * \retval  AW_OK  初始化成功
 * \retval   < 0   初始化失败，请检C文件中各配置项是否合法
 */
int am_miniport_view_inst_init (void);

/**
 * \brief MiniPort-View 和  MiniPort-595 联合使用实例初始化
 *
 * \retval  AW_OK  初始化成功
 * \retval   < 0   初始化失败，请检C文件中各配置项是否合法
 */
int am_miniport_view_595_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
