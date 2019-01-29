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
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 间隔 0.5s，LED0 闪烁 5 次；
 *   2. 之后，LED0 以 0.2s 的间隔一直闪烁。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_gpio.c src_lpc824_hw_gpio
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_gpio
 * \copydoc demo_lpc824_hw_gpio.c
 */

/** [src_lpc824_hw_gpio] */
#include "ametal.h"
#include "am_delay.h"
#include "hw/amhw_lpc84x_gpio.h"


void demo_lpc845_hw_gpio_entry (amhw_lpc84x_gpio_t *p_hw_gpio,
                                int                 pin)
{
    uint8_t i = 0;
  
    /* 间隔 0.5s，LED0 闪烁 5 次 */
    for (i = 0; i < 5; i++) {
        amhw_lpc84x_gpio_pin_out_low(p_hw_gpio, pin);
        am_mdelay(500);

        amhw_lpc84x_gpio_pin_out_high(p_hw_gpio, pin);
        am_mdelay(500);
    }

    AM_FOREVER {

        /* LED0 以 0.2s 的间隔一直闪烁 */
        amhw_lpc84x_gpio_pin_out_tog(p_hw_gpio, pin);
        am_mdelay(200);
    }
}

/** [src_lpc824_hw_gpio] */

/* end of file */
