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
 * \brief 系统堆适配，选择合适的内存作为系统堆
 *
 * 初始化后，即可使用 am_mem.h 文件中的相关接口
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-17  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_BSP_SYSTEM_HEAP_H
#define __AM_BSP_SYSTEM_HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief 适配器初始化
 */
void am_bsp_system_heap_init (void *, void *);

#ifdef __cplusplus
}
#endif

#endif /* __AM_BSP_SYSTEM_HEAP_H */

/* end of file */
