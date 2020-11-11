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
 * 该文件被包含于 \sa zsl42x_pin.h. 其定义了GPIO平台引脚的各个功能。
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-21  zcb, first implementation
 * \endinternal
 */

#ifndef __AM_ZSL42x_GPIO_UTIL_H
#define __AM_ZSL42x_GPIO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup am_zsl42x_if_gpio_util
 * \copydoc am_zsl42x_gpio_util.h
 * @{
 */

/**
 * \brief 平台宏定义
 *
 * bit[31:12] 平台定义
 *
 */

/**
 * \name 控制位域
 * @{
 */
#define AM_ZSL42x_GPIO_OUTRES_RATE                  AM_BIT(13)     /**< \brief 管脚响应速率 */
#define AM_ZSL42x_GPIO_MODE                         AM_BIT(14)     /**< \brief 管脚模式 */
#define AM_ZSL42x_GPIO_REMAP                        AM_BIT(15)     /**< \brief 管脚复用功能使能 */

#define AM_ZSL42x_GPIO_OUTRESRATEBITS_START         18        /**< \brief 输出响应速率码起始位 */
#define AM_ZSL42x_GPIO_OUTRESRATEBITS_LEN           2         /**< \brief 输出响应速率码长度 */

/** \brief 置响应速率码 */
#define AM_ZSL42x_GPIO_OUTRES_RATE_CODE(a)          (AM_SBF((a), AM_ZSL42x_GPIO_OUTRESRATEBITS_START) |  \
                                                      AM_ZSL42x_GPIO_OUTRES_RATE)

/** \brief 获取响应速率码 */
#define AM_ZSL42x_GPIO_OUTRES_RATE_GET(data)        AM_BITS_GET((data), AM_ZSL42x_GPIO_OUTRESRATEBITS_START, \
                                                                          AM_ZSL42x_GPIO_OUTRESRATEBITS_LEN)

#define AM_ZSL42x_GPIO_MODEBITS_START               20        /**< \brief 模式码起始位 */
#define AM_ZSL42x_GPIO_MODEBITS_LEN                 4         /**< \brief 模式码长度 */

/** \brief 置模式码 */
#define AM_ZSL42x_GPIO_MODE_CODE(a)                 (AM_SBF((a), AM_ZSL42x_GPIO_MODEBITS_START) |  \
                                                      AM_ZSL42x_GPIO_MODE)

/** \brief 获取模式码 */
#define AM_ZSL42x_GPIO_MODE_GET(data)               AM_BITS_GET((data), AM_ZSL42x_GPIO_MODEBITS_START, \
                                                                          AM_ZSL42x_GPIO_MODEBITS_LEN)

#define AM_ZSL42x_GPIO_REMAP_PERIPHERAL_START       24  /**< \brief 重映像外设码起始位 */
#define AM_ZSL42x_GPIO_REMAP_PERIPHERAL_LEN         4   /**< \brief 重映像外设码长度 */

/** \brief 获取重映像外设码 */
#define AM_ZSL42x_GPIO_REMAP_PERIPHERAL_GET(data)   AM_BITS_GET((data), AM_ZSL42x_GPIO_REMAP_PERIPHERAL_START, \
                                                                          AM_ZSL42x_GPIO_REMAP_PERIPHERAL_LEN)

#define AM_ZSL42x_GPIO_REMAP_MODE_START             28  /**< \brief 重映像模式码起始位 */
#define AM_ZSL42x_GPIO_REMAP_MODE_LEN               3   /**< \brief 重映像模式码长度 */

/** \brief 获取重映像模式码 */
#define AM_ZSL42x_GPIO_REMAP_MODE_GET(data)         AM_BITS_GET((data), AM_ZSL42x_GPIO_REMAP_MODE_START, \
                                                                          AM_ZSL42x_GPIO_REMAP_MODE_LEN)

/** \brief 获取重映像码设置  (PERIPHERAL,MODE) */
#define AM_ZSL42x_GPIO_REMAP_CODE(a,b)              (AM_SBF((AM_SBF(b, AM_ZSL42x_GPIO_REMAP_PERIPHERAL_LEN) | a), \
                                                                         AM_ZSL42x_GPIO_REMAP_PERIPHERAL_START) |   \
                                                                         AM_ZSL42x_GPIO_REMAP)
/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42x_GPIO_UTIL_H */

/* end of file */
