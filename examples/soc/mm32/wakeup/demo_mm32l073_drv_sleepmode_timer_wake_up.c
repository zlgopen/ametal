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
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 睡眠模式例程，使用定时器周期唤醒，通过驱动层接口实现
 *
 * - 实现现象
 *   1. 串口输出"enter sleep!"，系统时钟源切换为 LSI，启动定时器，
 *      进入睡眠模式后，J-Link 调试断开，此时用户可测量睡眠模式的功耗；
 *   2. 等待定时时间到，MCU 被唤醒，串口输出"wake_up!"，然后重新进入
 *      睡眠模式。
 *
 * \par 源代码
 * \snippet demo_mm32l073_drv_sleepmode_timer_wake_up.c src_mm32l073_drv_sleepmode_timer_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 18-05-16  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_drv_sleepmode_timer_wake_up
 * \copydoc demo_mm32l073_drv_sleepmode_timer_wake_up.c
 */

/** [src_mm32l073_drv_sleepmode_timer_wake_up] */
#include "ametal.h"
#include "am_timer.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_mm32l073.h"
#include "am_mm32l073_pwr.h"
#include "am_mm32l073_clk.h"
#include "hw/amhw_mm32_uart.h"

/**
 * \brief 定时器回调函数
 */
am_local void __tim_timing_callback (void *p_arg)
{
    ; /* VOID */
}

/**
 * \brief 系统时钟源切换为 LSI
 */
am_local void __sysclk_switch_to_lsi (void)
{

    /* 停机模式使能 LSI 作为系统时钟 */
    amhw_mm32l073_rcc_lsi_enable();

    /* 等待 LSI 时钟就绪 */
    while (amhw_mm32l073_rcc_lsirdy_read() == AM_FALSE);

    /* 切换时钟 */
    amhw_mm32l073_rcc_sys_clk_set(AMHW_MM32L073_SYSCLK_LSI);

    /* 失能 PLL */
    amhw_mm32l073_rcc_pll_disable();
}

/**
 * \brief 系统时钟源切换为 PLL
 */
am_local void __sysclk_switch_to_pll (void)
{
    amhw_mm32l073_rcc_pll_enable();
    while (amhw_mm32l073_rcc_pllrdy_read() == AM_FALSE);

    /* 系统时钟选为 PLL */
    amhw_mm32l073_rcc_sys_clk_set(AMHW_MM32L073_SYSCLK_PLL);

    /* 在正常模式下禁能 LSI 作为系统时钟 */
    amhw_mm32l073_rcc_lsi_disable();
}

/**
 * \brief 例程入口
 */
void demo_mm32l073_drv_sleepmode_timer_wake_up_entry (am_timer_handle_t timer_handle,
                                                    uint32_t          timer_clk_rate)
{
    am_timer_callback_set(timer_handle, 0, __tim_timing_callback, NULL);

    /* 唤醒配置 */
    am_mm32l073_wake_up_cfg(AM_MM32L073_PWR_MODE_SLEEP, NULL, NULL);

    while (1) {

        AM_DBG_INFO("enter sleep!\r\n");

        /* 等待串口发送完成 */
        while((MM32L073_UART1->csr & AMHW_MM32_UART_TX_COMPLETE_FALG) == AM_FALSE);

        /* 将系统时钟源切换为 LSI */
        __sysclk_switch_to_lsi();

        /* 设置定时中断周期为 1S，并启动定时器 */
        am_timer_enable(timer_handle, 0, timer_clk_rate * 1);

        /* 进入睡眠模式 */
        am_mm32l073_pwr_mode_into(AM_MM32L073_PWR_MODE_SLEEP);

        /* 关闭定时器 */
        am_timer_disable(timer_handle, 0);

        /* 将系统时钟源切换为 PLL */
        __sysclk_switch_to_pll();

        AM_DBG_INFO("wake_up!\r\n");

        am_mdelay(10);
    }
}
/** [src_mm32l073_drv_sleepmode_timer_wake_up] */

/* end of file */
