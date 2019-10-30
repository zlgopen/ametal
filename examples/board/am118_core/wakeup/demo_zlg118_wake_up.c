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
 * \brief 低功耗模式（睡眠、深度睡眠）例程，通过驱动层接口实现
 *
 * - 实现现象
 *   1.串口提示当前正在进行的模式测试，首先进入睡眠模式。
 *   2.按下 KEY/RES键（PIOA_7）唤醒 MCU,并进入深度睡眠模式测试。
 *   3.按下 KEY/RES键（PIOA_7）唤醒 MCU,LED闪烁，MCU处于正常工作模式。
 *
 * \note
 *   1.测试本例程之前应将 am_prj_config.h 中的宏 AM_CFG_SYSTEM_TICK_ENABLE、
 *     AM_CFG_SOFTIMER_ENABLE 和   AM_CFG_KEY_GPIO_ENABLE 设置为 0。
 *   2.如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zlg118_drv_lpmode_wake_up.c src_zlg118_drv_lpmode_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-15  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_drv_lpmode_wake_up
 * \copydoc demo_zlg118_drv_lpmode_wake_up.c
 */

/** [src_zlg118_drv_lpmode_wake_up] */
#include "ametal.h"
#include "am_board.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "zlg118_pin.h"
#include "am_zlg118_inst_init.h"
#include "demo_am118_core_entries.h"
#include "am_zlg118_lpmode.h"

/**
 * \brief 引脚中断服务函数
 */
static void __gpio_isr (void *p_arg)
{
    ;
}

/**
 * \brief 例程入口
 */
void demo_zlg118_drv_lpmode_wake_up_entry (void)
{
    AM_DBG_INFO("sleep mode test!\r\n");
    am_mdelay(100);

    /* 引脚中断配置 */
    am_gpio_pin_cfg(PIOA_7, PIOA_7_INPUT_PU);
    am_gpio_trigger_connect(PIOA_7, __gpio_isr, NULL);
    am_gpio_trigger_cfg(PIOA_7, AM_GPIO_TRIGGER_FALL);
    am_gpio_trigger_on(PIOA_7);

    /* 低功耗模式初始化 */
    am_zlg118_lpmode_init();

    /* 睡眠模式下系统时钟配置 */
    am_zlg118_lpmode_clk_change(AM_ZLG118_LPMODE_MODE_SLEEP);

    /* 进入睡眠模式，唤醒后不再进入 */
    am_zlg118_lpmode_sleep(AM_FALSE);

    AM_DBG_INFO("sleep mode, wake_up!\r\n");

    AM_DBG_INFO("deepsleep mode test!\r\n");

    /* 睡眠模式下系统时钟配置 */
    am_zlg118_lpmode_clk_change(AM_ZLG118_LPMODE_MODE_DEEPSLEEP);

    /* 进入深度睡眠模式，唤醒后不再进入 */
    am_zlg118_lpmode_deepsleep(AM_FALSE);

    AM_DBG_INFO("deepsleep mode, wake_up!\r\n\r\n");

    while (1) {

        /* 指示灯指示时钟是否恢复正常 */
        am_led_on(LED0);
        am_mdelay(500);
        am_led_off(LED0);
        am_mdelay(500);
    }
}
/** [src_zlg118_drv_lpmode_wake_up] */

/* end of file */
