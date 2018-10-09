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
 * \brief AM_KL26 芯片特性相关头文件定义
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-13  mkr, first implementation.
 * \endinternal
 */

#ifndef __AM_KL26_H
#define __AM_KL26_H

#ifdef __cplusplus
extern "C" {
#endif


/** \brief 芯片引脚定义 */
#include "../kl26/kl26_pin.h"

/** \brief 芯片中断编号定义 */
#include "../kl26/kl26_inum.h"

/** \brief 芯片外设寄存器基址定义 */
#include "../kl26/kl26_regbase.h"

/** \brief 芯片外设映射 */
#include "../kl26/kl26_periph_map.h"

/** \brief 时钟ID定义 */
#include "../kl26/kl26_clk.h"

/** \brief DMA通道及参数定义 */
#include "../kl26/kl26_dmamux_chan.h"

/**
 * \name Cortex-M0+ 核处理器和外设配置
 * @{
 */
#ifndef AM_CORTEX_M0
#define AM_CORTEX_M0
#endif

#ifdef AM_CORTEX_M0

#define __MPU_PRESENT           0  /**< \brief M0+核无MPU                  */
#define __NVIC_PRIO_BITS        2  /**< \brief M0+核为2                    */
#define __Vendor_SysTickConfig  0  /**< \brief 设置为1 可以配置SysTick     */
#define __VTOR_PRESENT          1  /**< \brief 如果为1，可配置中断向量表   */

#include "core_cm0plus.h"

#endif
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_KL26_H */

/* end of file */
