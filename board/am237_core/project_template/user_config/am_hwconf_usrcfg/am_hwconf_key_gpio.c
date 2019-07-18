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
 * \brief GPIO 按键用户配置文件
 * \sa am_hwconf_key_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */
#include "zlg237_pin.h"
#include "ametal.h"
#include "am_zlg237_inst_init.h"
#include "am_key_gpio.h"
#include "am_input.h"

/**
 * \addtogroup am_if_src_hwconf_key_gpio
 * \copydoc am_hwconf_key_gpio.c
 * @{
 */

static const int __g_key_pins[]  = {PIOC_7};
static const int __g_key_codes[] = {KEY_KP0};

/** \brief 设备信息 */
static const am_key_gpio_info_t __g_key_gpio_info = {
    __g_key_pins,
    __g_key_codes,
    AM_NELEMENTS(__g_key_pins),
    AM_TRUE,
    10
};

/** \brief 设备实例 */
static am_key_gpio_t __g_key_gpio;

/** \brief 实例初始化函数 */
int am_key_gpio_inst_init (void)
{
    return am_key_gpio_init(&__g_key_gpio, &__g_key_gpio_info);
}

/**
 * @}
 */

/* end of file */
