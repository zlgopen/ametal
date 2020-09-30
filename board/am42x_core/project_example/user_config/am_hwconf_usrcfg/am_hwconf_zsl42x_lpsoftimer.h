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
 * \brief zsl42x LPSOFTIMER 定时功能用户配置文件
 * \sa am_hwconf_zsl42x_lpsoftimer.h
 *
 * \note LPSOFTIMER 使用 LPTM0 和 LPTIM1 来实现，注意不可同时使用。
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-17 zcb, first implementation
 * - 1.01 20-09-23  yg, adaptation zsl42x.
 * \endinternal
 */

#ifndef __AM_HWCONF_ZSL42X_LPSOFTIMER_H
#define __AM_HWCONF_ZSL42X_LPSOFTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_zsl42x_lpsoftimer_if.h"

/**
 * \brief LPTMR低功耗定时器初始化
 */
void am_zsl42x_lpsoftimer_init (void);

/**
 * \brief LPTMR低功耗定时器去初始化
 */
void am_zsl42x_lpsoftimer_deinit (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_ZSL42X_LPSOFTIMER_H */

/* end of file */
