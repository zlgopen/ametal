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
 * \brief GPIO 按键例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 按一次按键 LED0 灯熄灭，再按一次按键 LED0 灯亮，如此反复。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 按键需要将 J14 跳线帽的 KEY 和 PIO0_1 短接才能使用。
 *
 * \par 源代码
 * \snippet demo_lpc82x_std_gpio_key.c src_lpc82x_std_gpio_key
 *
 * \internal
 * \par Modification history
 * - 1.00 15-11-25  mem, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc82x_std_gpio_key
 * \copydoc demo_lpc82x_std_gpio_key.c
 */

/** [src_lpc82x_std_gpio_key] */
#include "ametal.h"
#include "am_board.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_vdebug.h"

void demo_lpc_std_gpio_key_entry(int pin)
{
    am_gpio_pin_cfg(pin, AM_GPIO_INPUT);

    AM_FOREVER {

        /* 等待按键按下 */
        while (am_gpio_get(pin) == AM_GPIO_LEVEL_HIGH);

        /* 延迟去抖 */
        am_mdelay(200);

        /* 等待按键释放 */
        while (am_gpio_get(pin) == AM_GPIO_LEVEL_LOW);

        am_led_toggle(LED0);
        AM_DBG_INFO("demo am845_core std gpio key is ok!\r\n");
    }
}

/** [src_lpc82x_std_gpio_key] */

/* end of file */
