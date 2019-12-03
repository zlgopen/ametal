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
 * \brief MiniPort-ZLG72128 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_MINIPORT_ZLG72128_H
#define __AM_HWCONF_MINIPORT_ZLG72128_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief MiniPort-ZLG72128 实例初始化
 *
 * \retval  AW_OK  初始化成功，可以使用通用数码管接口和按键接口操作ZLG72128
 * \retval   < 0   初始化失败，请检查C文件中各配置项是否合法
 */
int am_miniport_zlg72128_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
