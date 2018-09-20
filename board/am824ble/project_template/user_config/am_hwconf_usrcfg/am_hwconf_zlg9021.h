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
 * \brief ZLG9021 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-28  pea, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_ZLG9021_H
#define __AM_HWCONF_ZLG9021_H

#include "ametal.h"
#include "am_zlg9021.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief  ZLG9021 实例初始化，获得 ZLG9021 标准服务操作句柄
 *
 * \return  ZLG9021 标准服务操作句柄
 */
am_zlg9021_handle_t am_zlg9021_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_ZLG9021_H */

/* end of file */
