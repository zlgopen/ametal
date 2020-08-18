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
 * \brief 通用服务（如 NVRAM、按键输入等）初始化
 *
 * \internal
 * \par modification history
 * - 1.00 18-10-16  tee, first implementation
 * \endinternal
 */

#ifndef __AM_SERVICE_INST_INIT_H
#define __AM_SERVICE_INST_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief NVRAM 标准服务初始化
 *
 * \note 使用 NVRAM 前，必须调用该函数，可通过将 am_prj_config.h 文件中的
 *       #AM_CFG_NVRAM_ENABLE 宏值修改为 1 实现，使系统启动时自动调用该函数
 */
void am_nvram_inst_init (void);

/**
 * \brief 事件输入管理器（包含按键输入）服务初始化
 */
void am_event_input_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SERVICE_INST_INIT_H */

/* end of file */
