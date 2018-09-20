/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 系统滴答、软件定时器配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-20  pea, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_SYSTEM_TICK_SOFTIMER_H
#define __AM_HWCONF_SYSTEM_TICK_SOFTIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_timer.h"

/**
 * \brief 系统滴答实例初始化(不使用软件定时器)
 *
 * 当使用系统滴答时，默认使用 MRT 的通道 0 作为基础定时单元，因此已经将 MRT 初始化为
 * 定时器功能，由于 MRT 可以支持 4 路定时，因此可以使用本函数返回的 handle，以便使用
 * 其它 3 路定时通道
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_system_tick_inst_init (void);

/**
 * \brief 系统滴答实例初始化(使用软件定时器)
 *
 * 当使用系统滴答时，默认使用 MRT 的通道 0 作为基础定时单元，因此已经将 MRT 初始化为
 * 定时器功能，由于 MRT 可以支持 4 路定时，因此可以使用本函数返回的 handle，以便使用
 * 其它 3 路定时通道
 *
 * \param 无
 *
 * \return TIMER 标准服务句柄，若为 NULL，表明初始化失败
 */
am_timer_handle_t am_system_tick_softimer_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_SYSTEM_TICK_SOFTIMER_H */

/* end of file */
