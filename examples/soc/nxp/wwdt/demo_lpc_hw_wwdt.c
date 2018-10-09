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
 * \brief WWDT 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 屏幕打印看门狗时钟频率；
 *   2. 前 5 次报警中断发生时，会执行喂狗操作,串口打印当前喂狗次数；
 *   3. 第 6 次报警中断发生时，不执行喂狗操作，会发生看门狗复位；
 *   4. 复位后，LED0 亮 500ms，串口打印 "Watchdog reset occurred!"。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. WDT 时钟，0.6MHz，64 分频，时钟频率 9.375KHz，WDT 计数器固定 4 分频时钟，
 *       当 WDT 时钟频率大于 400K 时，WDT 窗口功能不能正常使用。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_wwdt.c src_lpc_hw_wwdt
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-14  hgo, del amhw_lpc_wwdt_option_clr()
 * - 1.00 15-07-22  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_wwdt
 * \copydoc demo_lpc_hw_wwdt.c
 */

/** [src_lpc_hw_wwdt] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_arm_nvic.h"
#include "am_board.h"
#include "hw/amhw_lpc_wwdt.h"

/** \brief 看门狗报警次数统计 */
am_local volatile uint8_t __g_wdt_update_count;

/**
 * \brief 看门狗定时器，中断处理函数
 *
 * \param[in] p_arg 入口参数
 *
 * \return 无
 */
am_local void __wdt_irq_handler (void *p_arg)
{

    amhw_lpc_wwdt_t *handle = (amhw_lpc_wwdt_t *)p_arg;
    /* 看门狗定时器出现报警状态 */
    if (amhw_lpc_wwdt_status_get(handle) & AMHW_LPC_WWDT_WDMOD_WDINT) {

        /* 打印喂狗时当前的倒计数数值 */
        AM_DBG_INFO("The current countis %d .\r\n",
                    amhw_lpc_wwdt_currentcount_get(handle));

        amhw_lpc_wwdt_status_clr(handle,
                                 AMHW_LPC_WWDT_WDMOD_WDINT);

        /* 在超时前 5 次报警中断发生， 执行喂狗程序，然后允许看门狗复位 */
        if (__g_wdt_update_count < 5) {
            amhw_lpc_wwdt_feed(handle);
            __g_wdt_update_count++;
            AM_DBG_INFO("The %d times feed .\r\n", __g_wdt_update_count);
        }
    }
}


void demo_lpc_hw_wwdt_entry (amhw_lpc_wwdt_t *p_hw_wwdt,
                             int              inum,
                             uint32_t         wdt_freq)

{
          /* 检测看门狗是否复位 */
    if (amhw_lpc_wwdt_status_get(p_hw_wwdt) & AMHW_LPC_WWDT_WDMOD_WDTOF) {

        /* 打开 LED0 */
        am_led_on(LED0);

        /* 打印看门狗复位信息 */
        AM_DBG_INFO("Watchdog reset occurred! \r\n");

        amhw_lpc_wwdt_status_clr(p_hw_wwdt, AMHW_LPC_WWDT_WDMOD_WDTOF);
    }

    /* WWDT 初始化 */
    amhw_lpc_wwdt_init(p_hw_wwdt);

    /**
     * 设置看门狗时间持续大约 2s
     * 设置看门狗报警时间为 512 ticks
     * 设置看门狗窗口定时器时间为 1s
     */
    amhw_lpc_wwdt_timeout_set(p_hw_wwdt, wdt_freq * 2);
    amhw_lpc_wwdt_warning_set(p_hw_wwdt, 512);
    amhw_lpc_wwdt_window_set(p_hw_wwdt, wdt_freq * 1);

    /* 看门狗时间发生后，复位 MCU */
    amhw_lpc_wwdt_option_set(p_hw_wwdt, AMHW_LPC_WWDT_WDMOD_WDRESET);

    /* 清除报警和超时中断 */
    amhw_lpc_wwdt_status_clr(p_hw_wwdt,
                             AMHW_LPC_WWDT_WDMOD_WDTOF |
                             AMHW_LPC_WWDT_WDMOD_WDINT);

    /* 注册看门狗中断函数 */
    am_int_connect(inum, __wdt_irq_handler, (void *)p_hw_wwdt);

    /* 清除已经挂起的看门狗中断 */
    amhw_arm_nvic_pending_clr(inum);

    /* 打开看门狗中断 */
    am_int_enable(inum);

    /* 启动看门狗定时器 */
    amhw_lpc_wwdt_start(p_hw_wwdt);

    while (1) {
      ; /* void */
    }
}

/** [src_lpc_hw_wwdt] */

/* end of file */
