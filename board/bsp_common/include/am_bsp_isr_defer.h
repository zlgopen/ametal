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
 * \brief ISR DEFER 板级支持
 *
 * 为避免在中断中处理过于繁重耗时的事务，影响系统实时性，往往将中断中要处理的事务推迟
 * 延迟任务中处理，详见 am_isr_defer.h
 *
 * \internal
 * \par modification history
 * - 1.00 12-10-23  tee, first implementation
 * \endinternal
 */

#ifndef __AM_BSP_ISR_DEFER_H
#define __AM_BSP_ISR_DEFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_isr_defer.h"

/**
 * \brief 定义中断延迟模块使用的优先级数目为32
 *
 * 只需要使用该宏定义一次，job的有效优先级即为：0 ~ 31
 *
 */
#define AM_BSP_ISR_DEFER_PRIORITY_NUM     32

/**
 * \brief ISR DEFER 板级初始化，将中断延迟任务放在 PENDSV中 处理。
 */
void am_bsp_isr_defer_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_BSP_NEWLIB_H */

/* end of file */
