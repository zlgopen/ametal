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
 * \brief 按键用户配置文件（GPIO 驱动）
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_KEY_GPIO_H
#define __AM_HWCONF_KEY_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \brief 按键实例初始化（GPIO 驱动）
 *
 * \param 无
 *
 * \retval AW_OK 初始化成功
 * \retval  < 0  初始化失败，请检查 am_hwconf_key_gpio.c 文件中各配置项是否合法
 */
int am_key_gpio_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
