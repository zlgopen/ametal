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
 * \brief HC32F460 芯片特性相关头文件定义
 *
 * \internal
 * \par Modification history
 * - 1.00 20-01-15  cds, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_H
#define __AM_HC32F460_H

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 芯片引脚定义 */
#include "hc32f460_pin.h"

/** \brief 芯片中断编号定义 */
#include "hc32f460_inum.h"

/** \brief 芯片外设寄存器基址定义 */
#include "hc32f460_regbase.h"

/** \brief 芯片外设映射 */
#include "hc32f460_periph_map.h"

/** \brief 时钟ID定义 */
#include "hc32f460_clk.h"

/**
 * \name Cortex-M4 内核处理器和外设设置
 * @{
 */
#ifndef AM_CORTEX_M4
#define AM_CORTEX_M4             1
#endif

#if defined AM_CORTEX_M4

#define __MPU_PRESENT            1  /**< \brief M4 核有 MPU */
#define __NVIC_PRIO_BITS         4  /**< \brief M4 核为 4 */
#define __Vendor_SysTickConfig   0  /**< \brief 设置为 1 可以配置 SysTick */

#include "core_cm4.h"

#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_H */

/* end of file */
