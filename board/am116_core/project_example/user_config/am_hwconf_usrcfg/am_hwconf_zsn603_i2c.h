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
 * \brief Miniport-595 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_HWCONF_ZSN603_I2C_H
#define __AM_HWCONF_ZSN603_I2C_H

#include "ametal.h"
#include "zsn603.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief ZSN603 (I2C 模式) 实例初始化
 *
 * \retval  handle  初始化成功
 * \retval  NULL    初始化失败
 */
zsn603_handle_t  am_zsn603_i2c_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
