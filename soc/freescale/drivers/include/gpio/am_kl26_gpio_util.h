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
 * \brief GPIO平台辅助工具宏
 *
 * 该文件被包含于 \sa ks16xx_pin.h. 其定义了GPIO平台引脚的各个功能。
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-15  nwt, make some changes about Macro definition.
 * - 1.00 15-10-18  wangxiaojian, first implementation.
 * \endinternal
 */

#ifndef __AM_KL26_GPIO_UTIL_H
#define __AM_KL26_GPIO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup am_kl26_if_gpio_util
 * \copydoc am_kl26_gpio_util.h
 * @{
 */

/*
 * bit[11:6] 保留
 * bit[31:12] 用于平台定义
 *
 */

/**
 * \name PORT_PCRn 控制位域
 * @{
 */

#define AM_KL26_PORT_PULLUP               AM_BIT(12) /**< \brief 管脚选择内部上拉使能 */
#define AM_KL26_PORT_PULLEN               AM_BIT(13) /**< \brief 管脚内部上/下拉使能 */
#define AM_KL26_PORT_SLEWSLOW             AM_BIT(14) /**< \brief 管脚低转换率 */
#define AM_KL26_PORT_FILTEREN             AM_BIT(15) /**< \brief 无源输入滤波器使能 */
#define AM_KL26_PORT_DRIVERHIGH           AM_BIT(16) /**< \brief 管脚高驱动力 */
#define AM_KL26_PORT_FUNEN                AM_BIT(17) /**< \brief 管脚复用功能使能 */
#define AM_KL26_PORT_DIR                  AM_BIT(18) /**< \brief 管脚方向 */

/*******************************************************************************/

/** \brief 上/下拉位偏移 */
#define AM_KL26_GPIO_PULL_OFFSET          18

/** \brief 置上/下拉模式码 */
#define AM_KL26_GPIO_PULL_CODE(a)        (AM_SBF((a), AM_KL26_GPIO_PULL_OFFSET) | \
                                               AM_KL26_PORT_PULLUP)

/** \brief 获取上/下拉模式码 */
#define AM_KL26_GPIO_PULL_GET(data)       AM_BIT_GET((data), AM_KL26_GPIO_PULL_OFFSET)

/** \brief 上/下使能位偏移 */
#define AM_KL26_GPIO_PE_OFFSET            19

/** \brief 置上/下拉模式禁能码 */
#define AM_KL26_GPIO_PE_CODE(a)          (AM_SBF((a), AM_KL26_GPIO_PE_OFFSET) | \
                                               AM_KL26_PORT_PULLEN)

/** \brief 获取上/下拉模式禁能码 */
#define AM_KL26_GPIO_PE_GET(data)         AM_BIT_GET((data), AM_KL26_GPIO_PE_OFFSET)

/*******************************************************************************/

/** \brief 转换率位偏移 */
#define AM_KL26_GPIO_SRE_OFFSET           20

/** \brief 置转换率码 */
#define AM_KL26_GPIO_SRE_CODE(a)         (AM_SBF((a), AM_KL26_GPIO_SRE_OFFSET) | \
                                               AM_KL26_PORT_SLEWSLOW)

/** \brief 获取转换率码 */
#define AM_KL26_GPIO_SRE_GET(data)        AM_BIT_GET((data), AM_KL26_GPIO_SRE_OFFSET)

/*******************************************************************************/

/** \brief 无源滤波器位偏移 */
#define AM_KL26_GPIO_PFE_OFFSET           21

/** \brief 置无源滤波器码 */
#define AM_KL26_GPIO_PFE_CODE(a)         (AM_SBF((a), AM_KL26_GPIO_PFE_OFFSET) | \
                                               AM_KL26_PORT_FILTEREN)

/** \brief 获取无源滤波器码 */
#define AM_KL26_GPIO_PFE_GET(data)        AM_BIT_GET((data), AM_KL26_GPIO_PFE_OFFSET)

/*******************************************************************************/

/** \brief 驱动能力位偏移 */
#define AM_KL26_GPIO_DSE_OFFSET           22

/** \brief 置驱动能力码 */
#define AM_KL26_GPIO_DSE_CODE(a)         (AM_SBF((a), AM_KL26_GPIO_DSE_OFFSET) | \
                                               AM_KL26_PORT_DRIVERHIGH)

/** \brief 获取驱动能力码 */
#define AM_KL26_GPIO_DSE_GET(data)        AM_BIT_GET((data), AM_KL26_GPIO_DSE_OFFSET)

/*******************************************************************************/

#define AM_KL26_GPIO_FUNC_BITS_START      23  /**< \brief 管脚复用功能起始位 */
#define AM_KL26_GPIO_FUNC_BITS_LEN        3   /**< \brief 管脚复用功能码长度 */

/** \brief 置管脚复用功能码 */
#define AM_KL26_GPIO_FUNC_CODE(a)         (AM_SBF((a), AM_KL26_GPIO_FUNC_BITS_START) | \
                                                AM_KL26_PORT_FUNEN)

/** \brief 获取管脚复用功能码 */
#define AM_KL26_GPIO_FUNC_GET(data)       (AM_BITS_GET((data), AM_KL26_GPIO_FUNC_BITS_START, \
                                                AM_KL26_GPIO_FUNC_BITS_LEN))

/*******************************************************************************/

/** \brief 方向位偏移 */
#define AM_KL26_GPIO_DIRBIT_OFFSET        26

/** \brief 置方向码 */
#define AM_KL26_GPIO_DIR_CODE(data)       (AM_SBF((data), AM_KL26_GPIO_DIRBIT_OFFSET) | \
                                                AM_KL26_PORT_DIR)

/** \brief 获取方向码 */
#define AM_KL26_GPIO_DIR_GET(data)          AM_BIT_GET((data), AM_KL26_GPIO_DIRBIT_OFFSET)

/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_KL26_GPIO_UTIL_H */

/* end of file */
