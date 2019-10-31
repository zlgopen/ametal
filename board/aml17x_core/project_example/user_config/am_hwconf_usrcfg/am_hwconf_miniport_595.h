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
 * \brief Miniport-595 用户配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_MINIPORT_595_H
#define __AM_HWCONF_MINIPORT_595_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_hc595.h"

/**
 * \brief MiniPort-595 实例初始化
 *
 * \retval  AW_OK  初始化成功
 * \retval   < 0   初始化失败，请检C文件中各配置项是否合法
 */
am_hc595_handle_t am_miniport_595_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
