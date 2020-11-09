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
 * \brief LPMODE 驱动接口声明
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-18  nwt, first implementation
 * \endinternal
 */

#ifndef __AM_ZSL42x_LPMODE_H
#define __AM_ZSL42x_LPMODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "hw/amhw_zsl42x_lpmode.h"
#include "hw/amhw_zsl42x_rcc.h"

/**
 * \addtogroup am_zsl42x_if_lpmode
 * \copydoc am_zsl42x_lpmode.h
 * @{
 */

/**
 * \brief LPMODE 模式
 */
typedef enum am_zsl42x_lpmode_mode {
    AM_ZSL42x_LPMODE_MODE_RUN = 0,      /**< \brief 运行模式 */
    AM_ZSL42x_LPMODE_MODE_SLEEP,        /**< \brief 睡眠模式 */
    AM_ZSL42x_LPMODE_MODE_DEEPSLEEP,    /**< \brief 深度睡眠模式 */
} am_zsl42x_lpmode_mode_t;

/*
 * \brief LPMODE 设备结构体
 */
typedef struct am_zsl42x_lpmode_dev {

    /** \brief LPMODE 模式 */
    am_zsl42x_lpmode_mode_t lpmode_mode;

} am_zsl42x_lpmode_dev_t;

/** \brief LPMODE 设备操作句柄类型定义 */
typedef am_zsl42x_lpmode_dev_t *am_zsl42x_lpmode_handle_t;

/**
 * \brief LPMODE 初始化
 *
 * \return 返回 am_zsl42x_lpmode_handle_t 类型的句柄
 */
am_zsl42x_lpmode_handle_t am_zsl42x_lpmode_init (void);

/**
 * \brief 深度睡眠模式
 *
 * \param[in] stata  AM_TRUE  ： 当退出异常处理后，自动再次进入休眠；
 *                   AM_FALSE ： 唤醒后不再自动进入休眠
 *
 * \return 无
 */
void am_zsl42x_lpmode_deepsleep (am_bool_t stata);

/**
 * \brief 普通睡眠模式
 *
 * \param[in] stata  AM_TRUE  ： 当退出异常处理后，自动再次进入休眠；
 *                   AM_FALSE ： 唤醒后不再自动进入休眠
 *
 * \return 无
 */
void am_zsl42x_lpmode_sleep (am_bool_t stata);

/**
 * \brief LPMODE模式的系统时钟配置
 *
 * \param[in] mode LPMODE 模式定义
 *
 * \retval  AM_OK      模式切换有效
 * \retval  AM_ERROR   模式切换错误
 * \retval -AM_EINVAL  式参数错误
 */
int am_zsl42x_lpmode_clk_change (am_zsl42x_lpmode_mode_t mode);

/**
 * \brief 获取 LPMODE 模式
 *
 * \param[in] 无
 *
 * \return am_zsl42x_lpmode_mode_t 这一枚举类型的值
 */
am_zsl42x_lpmode_mode_t am_zsl42x_lpmode_mode_get (void);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42x_LPMODE_H */

/* end of file */
