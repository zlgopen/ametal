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
 * \sa am_hwconf_key_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_input.h"
#include "am_key_gpio.h"
#include "am_lpc82x.h"

/**
 * \addtogroup am_if_src_hwconf_key_gpio
 * \copydoc am_hwconf_key_gpio.c
 * @{
 */

/** \brief 按键引脚号 */
am_local am_const int __g_key_pins[] = {
    PIO0_1    /* KEY/RES 按键引脚 */
};

/** \brief 按键编码 */
am_local am_const int __g_key_codes[] = {
    KEY_KP0    /* KEY/RES 按键编码 */
};

/** \brief 按键设备信息 */
am_local am_const am_key_gpio_info_t __g_key_gpio_devinfo = {
    __g_key_pins,               /* 按键引脚号 */
    __g_key_codes,              /* 各个按键对应的编码（上报） */
    AM_NELEMENTS(__g_key_pins), /* 按键数目 */
    AM_TRUE,                    /* 是否低电平激活（按下为低电平） */
    10                          /* 按键扫描时间间隔，一般为 10ms */
};

/** \brief 按键设备实例 */
am_local am_key_gpio_t __g_key_gpio_dev;

/**
 * \brief 按键实例初始化（GPIO 驱动）
 */
int am_key_gpio_inst_init (void)
{
    return am_key_gpio_init(&__g_key_gpio_dev, &__g_key_gpio_devinfo);
}

/**
 * @}
 */

/* end of file */
