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
 * \brief LSM6DSL 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  wk, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_LSM6DSL_H
#define __AM_HWCONF_LSM6DSL_H

#include "ametal.h"
#include "am_lsm6dsl.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 实例初始化 */
am_lsm6dsl_handle_t am_lsm6dsl_inst_init (void);    
    
#ifdef __cplusplus
}
#endif

#endif

/* end of file */
