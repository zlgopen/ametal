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
 * \brief RGB LED 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. RGB LED 颜色变化。
 *
 * \note
 *    串口打印温湿度信息，需要将 PIOB_11 引脚连接 PC 串口的 TXD，
 *    PIOB_12 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zsl42x_rgb_led.c src_rgb_led
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_rgb_led
 * \copydoc demo_zsl42x_rgb_led.c
 */

/** [src_rgb_led] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_zsl42x_inst_init.h"
#include "demo_am42x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zsl42x_core_rgb_led_entry (void)
{
    uint32_t duty, i = 0;
    uint8_t dir = 0;

    AM_DBG_INFO("demo am42x_core rgb led!\r\n");

    am_pwm_handle_t tim0_handle = am_zsl42x_tim0_pwm_inst_init();
    am_pwm_handle_t tim4_handle = am_zsl42x_tim4_pwm_inst_init();

    am_pwm_config(tim0_handle, 0, 50000, 500000);
    am_pwm_config(tim0_handle, 1, 50000, 500000);
    am_pwm_config(tim4_handle, 0, 50000, 500000);

    am_pwm_enable(tim0_handle, 0);
    am_pwm_enable(tim0_handle, 1);
    am_pwm_enable(tim4_handle, 0);

    while (1) {

        if (dir == 0) {
            i++;
            if (i >= 80) {
               dir = 1;
            }
        } else {
            i--;
            if (i < 2) {
               dir = 0;
            }
        }

        duty = 500000 * i / 100;
        am_pwm_config(tim0_handle, 0, duty, 500000);
        am_pwm_config(tim0_handle, 1, duty, 500000);
        am_pwm_config(tim4_handle, 0, duty, 500000);
        am_pwm_enable(tim0_handle, 0);
        am_pwm_enable(tim0_handle, 1);
        am_pwm_enable(tim4_handle, 0);
        am_mdelay(20);
    }
}
/** [src_std_led] */

/* end of file */
