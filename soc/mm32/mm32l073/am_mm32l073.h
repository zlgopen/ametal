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
 * \brief mm32l073 芯片特性相关头文件定义
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-07  nwt, first implementation
 * \endinternal
 */

#ifndef __AM_MM32L073_H
#define __AM_MM32L073_H

#ifdef __cplusplus
extern "C" {
#endif

#include "soc_cfg.h"

/** \brief 芯片引脚定义 */
#include "mm32l073_pin.h"

/** \brief 芯片中断编号定义 */
#include "mm32l073_inum.h"

/** \brief 芯片外设寄存器基址定义 */
#include "mm32l073_regbase.h"

/** \brief 芯片外设映射 */
#include "mm32l073_periph_map.h"

/** \brief 时钟ID定义 */
#include "mm32l073_clk.h"

/** \brief DMA通道号定义 */
#include "mm32l073_dma_chan.h"

/**
 * \name Cortex-M0 内核处理器和外设设置
 * @{
 */
#ifndef AM_CORTEX_M0
#define AM_CORTEX_M0             1
#endif

#if defined AM_CORTEX_M0

#define __MPU_PRESENT            0  /**< \brief M0核无MPU */
#define __NVIC_PRIO_BITS         2  /**< \brief M0核为2 */
#define __Vendor_SysTickConfig   1  /**< \brief 设置为1 可以配置SysTick */

#include "core_cm0.h"

#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MM32L073_H */

/* end of file */
