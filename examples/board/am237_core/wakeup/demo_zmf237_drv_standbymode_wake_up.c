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
 * \brief 待机模式例程，通过驱动层接口实现
 *
 * - 操作步骤：
 *   1. 上电前将 WAKE_UP(PIOA_0) 连接到低电平；
 *   2. 等待 MCU 进入待机模式；
 *   3. 将 PIOA_0 连接到高电平，使 PIOA_0 上产生上升沿唤醒 MCU。
 *
 * - 实现现象
 *   1. 成功进入待机模式时，串口输出"enter standby!"，且 J-Link 调试断开；
 *      否则串口输出"WAKE_UP(PIOA_0) must be low!"；
 *   2. PIOA_0 产生上升沿后，MCU 复位运行。
 *
 * \note
 *    只有在 WAKE_UP(PIOA_0) 为低电平的时候才能进入待机模式
 *
 * \par 源代码
 * \snippet demo_zlg237_drv_standbymode_wake_up.c
 *               src_zlg237_drv_standbymode_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 19-07-25  zp, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg237_drv_standbymode_wake_up
 * \copydoc demo_zlg237_drv_standbymode_wake_up.c
 */
 
/** [src_zlg237_drv_standbymode_wake_up] */
#include "ametal.h"
#include "am_board.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "zlg237_pin.h"
#include "am_zlg237_pwr.h"
#include "am_zlg237_inst_init.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 闪烁 LED0
 */
am_local void __led_flash (void)
{
    while (1) {
        am_led_on(LED0);
        am_mdelay(500);
        am_led_off(LED0);
        am_mdelay(500);
    }
}

/**
 * \brief 例程入口
 */
void demo_zlg237_drv_standbymode_wake_up_entry (void)
{
    uint32_t i = 0;

    AM_DBG_INFO("low power test!\r\n");

    am_zlg237_pwr_inst_init();

    /* 唤醒配置 */
    am_zlg237_wake_up_cfg(AM_ZLG237_PWR_MODE_STANBY, NULL, NULL);

    for (i = 0; i < 5; i++) {
        am_mdelay(1000);
    }

    /* 进入待机模式 */
    if (am_zlg237_pwr_mode_into(AM_ZLG237_PWR_MODE_STANBY) != AM_OK) {

        /* 只有在 WAKE_UP(PIOA_0) 为低电平的时候才能进入待机模式 */
        AM_DBG_INFO("WAKE_UP(PIOA_0) must be low!\r\n");
        __led_flash();
    }

    /* 不应该执行到这里，因为待机模式唤醒之后芯片会直接复位 */
    AM_DBG_INFO("error!\r\n");

    __led_flash();
}

/** [src_zlg237_drv_standbymode_wake_up] */

/* end of file */
