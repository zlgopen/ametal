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
 * \brief HC32 芯片特性相关头文件定义
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-03  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HC32_H
#define __AM_HC32_H

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 芯片引脚定义 */
#ifdef HC32X3X
#include "../hc32/hc32x3x_pin.h" /**< \brief 适用于HC32F030、HC32L13X */
#else    
#include "../hc32/hc32_pin.h"    /**< \brief 适用于HC32L/F17X、HC32L/F19X */
#endif
    
/** \brief 芯片中断编号定义 */
#include "../hc32/hc32_inum.h"

/** \brief 芯片外设寄存器基址定义 */
#include "../hc32/hc32_regbase.h"

/** \brief 芯片外设映射 */
#include "../hc32/hc32_periph_map.h"

/** \brief 时钟ID定义 */
#include "../hc32/hc32_clk.h"

/** \brief DMA通道号定义 */
#include "../hc32/hc32_dma_chan.h"

/**
 * \name Cortex-M0+ 内核处理器和外设设置
 * @{
 */

#ifndef AM_CORTEX_M0

#define AM_CORTEX_M0             1

#endif

#if defined AM_CORTEX_M0

#define __MPU_PRESENT            0  /**< \brief M0+核无MPU          */
#define __NVIC_PRIO_BITS         2  /**< \brief M0+核为2            */
#define __Vendor_SysTickConfig   0  /**< \brief 设置为1 可以配置SysTick */

#include "core_cm0plus.h"

#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_H */

/* end of file */
