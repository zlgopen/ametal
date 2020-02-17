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
 * 该文件被包含于 \sa mm32l073_pin.h. 其定义了GPIO平台引脚的各个功能。
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-07  nwt, first implementation
 * \endinternal
 */

#ifndef __AM_MM32L073_GPIO_UTIL_H
#define __AM_MM32L073_GPIO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup am_mm32l073_if_gpio_util
 * \copydoc am_mm32l073_gpio_util.h
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
#define AM_MM32L073_GPIO_DIR                   AM_BIT(12) /**< \brief 管脚方向 */
#define AM_MM32L073_GPIO_OUTRES_RATE           AM_BIT(13) /**< \brief 管脚响应速率 */
#define AM_MM32L073_GPIO_MODE                  AM_BIT(14) /**< \brief 管脚模式 */
#define AM_MM32L073_GPIO_FUNEN                 AM_BIT(15) /**< \brief 管脚复用功能使能 */

/** \brief 方向位偏移 */
#define AM_MM32L073_GPIO_DIRBIT_START          16         /**< \brief 方向码起始位 */
#define AM_MM32L073_GPIO_DIRBIT_LEN            2          /**< \brief 方向码长度 */

/** \brief 置方向码 */
#define AM_MM32L073_GPIO_DIR_CODE(a)           (AM_SBF((a), AM_MM32L073_GPIO_DIRBIT_START) |  \
                                                AM_MM32L073_GPIO_DIR)

/** \brief 获取方向码 */
#define AM_MM32L073_GPIO_DIR_GET(data)         AM_BITS_GET((data), AM_MM32L073_GPIO_DIRBIT_START, \
                                                            AM_MM32L073_GPIO_DIRBIT_LEN)

#define AM_MM32L073_GPIO_OUTRESRATEBITS_START  18         /**< \brief 输出响应速率码起始位 */
#define AM_MM32L073_GPIO_OUTRESRATEBITS_LEN    2          /**< \brief 输出响应速率码长度 */

/** \brief 置响应速率码 */
#define AM_MM32L073_GPIO_OUTRES_RATE_CODE(a)   (AM_SBF((a), AM_MM32L073_GPIO_OUTRESRATEBITS_START) |  \
                                                AM_MM32L073_GPIO_OUTRES_RATE)

/** \brief 获取响应速率码 */
#define AM_MM32L073_GPIO_OUTRES_RATE_GET(data) AM_BITS_GET((data), AM_MM32L073_GPIO_OUTRESRATEBITS_START, \
                                                             AM_MM32L073_GPIO_OUTRESRATEBITS_LEN)

#define AM_MM32L073_GPIO_MODEBITS_START        20         /**< \brief 模式码起始位 */
#define AM_MM32L073_GPIO_MODEBITS_LEN          4          /**< \brief 模式码长度 */

/** \brief 置模式码 */
#define AM_MM32L073_GPIO_MODE_CODE(a)          (AM_SBF((a), AM_MM32L073_GPIO_MODEBITS_START) |  \
                                                AM_MM32L073_GPIO_MODE)

/** \brief 获取模式码 */
#define AM_MM32L073_GPIO_MODE_GET(data)         AM_BITS_GET((data), AM_MM32L073_GPIO_MODEBITS_START, \
                                                            AM_MM32L073_GPIO_MODEBITS_LEN)

#define AM_MM32L073_GPIO_FUNBITS_START          24        /**< \brief 功能码起始位 */
#define AM_MM32L073_GPIO_FUNBITS_LEN            4         /**< \brief 功能码长度 */

/** \brief 置功能码 */
#define AM_MM32L073_GPIO_FUNC_CODE(a)           (AM_SBF((a), AM_MM32L073_GPIO_FUNBITS_START) |  \
                                                 AM_MM32L073_GPIO_FUNEN)

/** \brief 获取功能码 */
#define AM_MM32L073_GPIO_FUNC_GET(data)         AM_BITS_GET((data), AM_MM32L073_GPIO_FUNBITS_START, \
                                                            AM_MM32L073_GPIO_FUNBITS_LEN)

/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MM32L073_GPIO_UTIL_H */

/* end of file */
