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
 * \brief 读卡器用户配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-02  htf, first implementation
 * \endinternal
 */
#ifndef __AM_HWCONF_ZSN700_READER_H
#define __AM_HWCONF_ZSN700_READER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \brief 读卡器实例初始化
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_zsn700_reader_handle_t am_zsn700_reader_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_ZSN700_READER_H */

/* end of file */
