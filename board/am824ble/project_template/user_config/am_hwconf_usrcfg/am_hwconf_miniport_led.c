/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief MiniPort-LED 用户配置文件
 * \sa am_hwconf_miniport_led.c
 *
 * MiniPort-LED 可以单独使用，也可以和 MiniPort-595 联合使用，以节省控制引脚
 *
 * 当前支持的用法有：
 * 1. MiniPort-LED                : am_miniport_led_inst_init();
 * 2. MiniPort-LED + MiniPort-595 : am_miniport_led_595_inst_init();
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-25  tee, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_led_gpio.h"
#include "am_led_hc595.h"
#include "am_hc595_spi.h"
#include "am_hc595_gpio.h"
#include "am_lpc82x.h"
#include "am_lpc82x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_led
 * \copydoc am_hwconf_miniport_led.c
 * @{
 */

/*******************************************************************************
  MiniPort-LED 单独使用
*******************************************************************************/

/** \brief MiniPort-LED 引脚 */
am_local am_const int __g_miniport_led_pins[8] = {
    PIO0_8,     /* LED0 引脚 */
    PIO0_9,     /* LED1 引脚 */
    PIO0_10,    /* LED2 引脚 */
    PIO0_11,    /* LED3 引脚 */
    PIO0_12,    /* LED4 引脚 */
    PIO0_13,    /* LED5 引脚 */
    PIO0_14,    /* LED6 引脚 */
    PIO0_15     /* LED7 引脚 */
};

/** \brief MiniPort-LED 设备信息 */
am_local am_const am_led_gpio_info_t __g_miniport_led_gpio_devinfo = {
    {
        0 + 2,                    /* 起始编号 2 */
        7 + 2                     /* 结束编号 9，共计 8 个 LED */
    },
    __g_miniport_led_pins,    /* LED 引脚 */
    AM_TRUE                   /* 低电平点亮 */
};

/** \brief MiniPort-LED 设备实例 */
am_local am_led_gpio_dev_t __g_miniport_led_gpio_dev;

/**
 * \brief MiniPort-LED 实例初始化（单独使用）
 */
int am_miniport_led_inst_init (void)
{
    return am_led_gpio_init(&__g_miniport_led_gpio_dev,
                            &__g_miniport_led_gpio_devinfo);
}

/*******************************************************************************
  MiniPort-LED 和 MiniPort-595 联合使用
*******************************************************************************/

/** \brief 数据缓存，大小与 HC595 的级连个数相同 */
am_local uint8_t __g_miniport_led_595_buf[1];

/** \brief MiniPort-595 + MiniPort-LED 设备信息 */
am_local am_const am_led_hc595_info_t __g_miniport_led_595_devinfo = {
    {
        0 + 2,                       /* 起始编号 2 */
        7 + 2                        /* 结束编号 9，共计 8 个 LED */
    },
    1,                           /* HC595 的级连个数  */
    __g_miniport_led_595_buf,    /* 数据缓存，大小与 HC595 的级连个数相同 */
    AM_TRUE                      /* 低电平点亮 */
};

/** \brief MiniPort-595 + MiniPort-LED 设备实例 */
am_local am_led_hc595_dev_t __g_miniport_led_595_dev;

/**
 * \brief MiniPort-595 + MiniPort-LED 实例初始化
 */
int am_miniport_led_595_inst_init (void)
{
    return am_led_hc595_init(&__g_miniport_led_595_dev,
                             &__g_miniport_led_595_devinfo,
                              am_miniport_595_inst_init());
}

/**
 * @}
 */

/* end of file */
