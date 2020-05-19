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
 * \brief MiniPort-LED 配置文件
 * \sa am_hwconf_miniport_led.c
 *
 * 它可以单独使用，也可以和 MiniPort-595 联合使用，以节省控制引脚。
 *
 * 当前支持的用法有：
 * 1. MiniPort-LED                : am_miniport_led_inst_init();
 * 2. MiniPort-LED + MiniPort-595 : am_miniport_led_595_inst_init();
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-25  tee, first implementation.
 * \endinternal
 */

#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_led_hc595.h"
#include "am_led_gpio.h"
#include "am_hc595_gpio.h"
#include "am_hc595_spi.h"
#include "zml165_pin.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_led
 * \copydoc am_hwconf_miniport_led.c
 * @{
 */

/*******************************************************************************
   MiniPort-LED 单独使用
*******************************************************************************/

/* 定义GPIO LED 实例 */
static am_led_gpio_dev_t  __g_miniport_led;

static const int __g_miniport_led_pins[] = {
    PIOB_7, PIOB_6, PIOA_7, PIOA_5, PIOA_11, PIOA_6, PIOA_4, PIOA_0
};

/* 定义GPIO按键实例信息 */
static const am_led_gpio_info_t __g_miniport_led_info = {
    {
        2,                            /* 起始编号2 */
        9                             /* 结束编号9，共计8个LED */
    },
    __g_miniport_led_pins,
    AM_TRUE
};

int am_miniport_led_inst_init (void)
{
    return am_led_gpio_init(&__g_miniport_led, &__g_miniport_led_info);
}

/*******************************************************************************
   MiniPort-LED 和 MiniPort-595联合使用
*******************************************************************************/

static am_led_hc595_dev_t  __g_miniport_led_595;

static uint8_t __g_miniport_led_595_buf[1];

/* 定义GPIO按键实例信息 */
static const am_led_hc595_info_t __g_miniport_led_595_info = {
    {
        2,                            /* 起始编号2 */
        9                             /* 结束编号9，共计8个LED */
    },
    1,
    __g_miniport_led_595_buf,
    AM_TRUE
};

int am_miniport_led_595_inst_init (void)
{

    return am_led_hc595_init(&__g_miniport_led_595,
                             &__g_miniport_led_595_info,
                              am_miniport_595_inst_init());
}

/**
 * @}
 */

/* end of file */
