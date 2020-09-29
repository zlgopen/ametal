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
 * \brief GS200 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-16  fzb, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_GS200_H
#define __AM_HWCONF_GS200_H

#include "ametal.h"
#include "am_gs200.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief  GS200 实例初始化，获得 GS200 标准服务操作句柄
 *
 * \return  GS200 标准服务操作句柄
 */
am_gs200_handle_t am_gs200_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_GS200_H */

/* end of file */
