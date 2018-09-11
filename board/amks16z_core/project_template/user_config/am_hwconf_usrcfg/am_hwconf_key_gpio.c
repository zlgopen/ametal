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
 * \brief GPIO按键 配置文件
 * \sa am_hwconf_key_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_kl26_inst_init.h"
#include "am_key_gpio.h"
#include "kl26_pin.h"
#include "am_input.h"

/**
 * \addtogroup am_if_src_hwconf_key_gpio
 * \copydoc am_hwconf_key_gpio.c
 * @{
 */

/* 定义GPIO按键实例 */
static am_key_gpio_t __g_key_gpio;

static const int __g_key_pins[] = {PIOE_24};
static const int __g_key_codes[] = {KEY_KP0};

/* 定义GPIO按键实例信息 */
static const am_key_gpio_info_t __g_key_gpio_info = {
    __g_key_pins,
    __g_key_codes,
    AM_NELEMENTS(__g_key_pins),
    AM_TRUE,
    10
};

int am_key_gpio_inst_init (void)
{
    return am_key_gpio_init(&__g_key_gpio, &__g_key_gpio_info);
}

/**
 * @}
 */

/* end of file */
