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
 * \brief 多频率定时器（MRT）例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 以 10Hz 的频率闪烁；
 *   2. PIO0_17 引脚的输出电平状态以 8KHz 的频率变换，产生 4KHz 的波形。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 由于 MRT 默认作为系统1滴答使用，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_SOFTIMER_ENABLE、 AM_CFG_SYSTEM_TICK_ENABLE 以及
 *       AM_CFG_KEY_GPIO_ENABLE 定义为 0，不使用软件定时器、系统嘀嗒、板载独立按键。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_mrt.c src_lpc824_hw_mrt
 *
 * \internal
 * \par History
 *
 * - 1.01 15-12-02  mem ,modified
 * - 1.00 15-07-15  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_mrt
 * \copydoc demo_lpc824_hw_mrt.c
 */

/** [src_lpc824_hw_mrt] */
#include "ametal.h"
#include "am_led.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x_clk.h"
#include "hw/amhw_lpc_mrt.h"
#include "hw/amhw_lpc82x_clk.h"

/**
 * \brief MRT 中断服务函数
  */
am_local void __mrt_irq_handler (void *p_arg)
{
    amhw_lpc_mrt_t *p_hw_mrt = (amhw_lpc_mrt_t *)p_arg;
    /* 通道 0 中断处理 */
    if (amhw_lpc_mrt_chan_pending_chk(p_hw_mrt, AMHW_LPC_MRT_CH(0))
                                                                  == AM_TRUE) {
        am_gpio_toggle(PIO0_17);

        /* 清中断挂起标志 */
        amhw_lpc_mrt_int_pending_clr(p_hw_mrt, AMHW_LPC_MRT_CH(0));
    }

    /* 通道 1 中断处理 */
    if (amhw_lpc_mrt_chan_pending_chk(p_hw_mrt, AMHW_LPC_MRT_CH(1))
                                                                  == AM_TRUE) {
        am_led_toggle(LED0);

        /* 如果需要再次产生中断，one-shot 模式应该再次加载中断值 */
        amhw_lpc_mrt_intval_set(p_hw_mrt,
                                AMHW_LPC_MRT_CH(1),
                                amhw_lpc82x_clk_system_clkrate_get() / 10,
                                AM_TRUE);

        /* 清中断挂起标志 */
        amhw_lpc_mrt_int_pending_clr(p_hw_mrt, AMHW_LPC_MRT_CH(1));
    }
}

/**
 * \brief 例程入口
 */
void demo_lpc824_hw_mrt_entry (amhw_lpc_mrt_t *p_hw_mrt, int inum)
{
    uint32_t ticks = 0;

    /* 禁能所有通道的中断 */
    amhw_lpc_mrt_int_disable(p_hw_mrt, AMHW_LPC_MRT_CH(0));
    amhw_lpc_mrt_int_disable(p_hw_mrt, AMHW_LPC_MRT_CH(1));
    amhw_lpc_mrt_int_disable(p_hw_mrt, AMHW_LPC_MRT_CH(2));
    amhw_lpc_mrt_int_disable(p_hw_mrt, AMHW_LPC_MRT_CH(3));

    am_int_connect(inum, __mrt_irq_handler, (void *)p_hw_mrt);
    am_int_enable(inum);

    /**
     * 通道 0 为 AMHW_LPC_MRT_MODE_REPEAT 模式，中断频率为 8KHz，
     * 最大中断值为 0x7FFFFFFF，
     * 在 24MHz，最大时间为 0x7FFFFFFF / 24 us = 89478485 us，最小频率为：2 Hz。
     */
    amhw_lpc_mrt_mode_set(p_hw_mrt,
                          AMHW_LPC_MRT_CH(0),
                          AMHW_LPC_MRT_MODE_REPEAT);
    amhw_lpc_mrt_intval_set(p_hw_mrt,
                            AMHW_LPC_MRT_CH(0),
                            amhw_lpc82x_clk_system_clkrate_get() / 8000,
                            AM_TRUE);
    amhw_lpc_mrt_int_enable(p_hw_mrt, AMHW_LPC_MRT_CH(0));

    /* 通道 1 为 AMHW_LPC_MRT_MODE_ONESHOT 模式，中断频率 10Hz */
    amhw_lpc_mrt_mode_set(p_hw_mrt,
                          AMHW_LPC_MRT_CH(1),
                          AMHW_LPC_MRT_MODE_ONESHOT);
    amhw_lpc_mrt_intval_set(p_hw_mrt,
                           AMHW_LPC_MRT_CH(1),
                           amhw_lpc82x_clk_system_clkrate_get() / 10,
                           AM_TRUE);
    amhw_lpc_mrt_int_enable(p_hw_mrt, AMHW_LPC_MRT_CH(1));

    /* 通道 2 为 AMHW_LPC_MRT_MODE_ONESHOT 模式，延时 100us */
    amhw_lpc_mrt_mode_set(p_hw_mrt,
                          AMHW_LPC_MRT_CH(2),
                          AMHW_LPC_MRT_MODE_ONESHOT);

    ticks = 100 * (amhw_lpc82x_clk_system_clkrate_get() / 1000000);

    amhw_lpc_mrt_intval_set(p_hw_mrt,
                            AMHW_LPC_MRT_CH(2),
                            ticks,
                            AM_TRUE);

    /* 等待 */
    while((amhw_lpc_mrt_chan_pending_chk(p_hw_mrt, AMHW_LPC_MRT_CH(2))
                                                                == AM_FALSE));

    /* 清通道 2 中断标志 */
    amhw_lpc_mrt_int_pending_clr(p_hw_mrt, AMHW_LPC_MRT_CH(2));

    /*
     * 通道 3 为 AMHW_LPC_MRT_MODE_ONESHOT_STALL 模式延时 100ns。
     * one-stall 模式适用于非常短暂的延时。当总线写操作停止时，多频率定时器请求
     * 延时，延时完成后再进行写操作。不需要中断或查询状态标志。
     */
    amhw_lpc_mrt_mode_set(p_hw_mrt,
                          AMHW_LPC_MRT_CH(3),
                          AMHW_LPC_MRT_MODE_ONESHOT_STALL);

    /* 延时 100 个系统时钟，当延时完成，写操作开始进行 */
    amhw_lpc_mrt_intval_set(p_hw_mrt,
                            AMHW_LPC_MRT_CH(3),
                            100,
                            AM_TRUE);

    AM_FOREVER {
        AM_DBG_INFO("idle channel :%d \r\n",
                    amhw_lpc_mrt_idle_channel_get(p_hw_mrt));
        am_mdelay(1000);
    }
}
/** [src_lpc824_hw_mrt] */

/* end of file */
