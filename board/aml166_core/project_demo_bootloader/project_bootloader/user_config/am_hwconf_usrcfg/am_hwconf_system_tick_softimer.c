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
 * \brief 系统滴答、软件定时器配置文件
 * \sa am_hwconf_system_tick_softimer.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-20  pea, first implementation
 * \endinternal
 */
#include <am_aml166_inst_init.h>
#include "ametal.h"
#include "am_zlg116.h"
#include "am_timer.h"
#include "am_softimer.h"
#include "am_system.h"
#include "am_arm_nvic.h"
#include "am_arm_systick.h"

/**
 * \addtogroup am_if_src_hwconf_system_tick_softimer
 * \copydoc am_hwconf_system_tick_softimer.c
 * @{
 */

/**
 * \brief 设置系统滴答的频率，默认 1KHz
 *
 * 系统滴答的使用详见 am_system.h
 */
#define __SYSTEM_TICK_RATE      1000

/** \brief 定时器回调函数，用于产生系统滴答 */
am_local void __system_tick_callback (void *p_arg)
{
    am_system_module_tick();       /* 驱动系统滴答 */
}

/** \brief 系统滴答实例初始化(不使用软件定时器) */
void am_system_tick_inst_init (void)
{
    am_timer_handle_t handle  = am_zlg116_systick_inst_init();

    if (NULL != handle) {

        uint32_t clk_freq;

        am_timer_count_freq_get(handle, 0, &clk_freq);

        /* 将 SYSTICK 定时器的中断优先级设置为最低 */
        am_arm_nvic_priority_set(SysTick_IRQn, 0x03, 0x03);

        am_timer_callback_set(handle, 0, __system_tick_callback, NULL);

        am_system_module_init(__SYSTEM_TICK_RATE);

        am_timer_enable(handle, 0, clk_freq / __SYSTEM_TICK_RATE);
    }
}

/******************************************************************************/

/** \brief 定时器回调函数，用于产生系统滴答，驱动软件定时器等等 */
am_local void __system_tick_softimer_callback (void *p_arg)
{
    am_system_module_tick();       /* 驱动系统滴答 */
    am_softimer_module_tick();     /* 驱动软件定时器 */
}

/** \brief 系统滴答实例初始化(使用软件定时器) */
void am_system_tick_softimer_inst_init (void)
{
    am_timer_handle_t handle = am_zlg116_systick_inst_init();

    if (NULL != handle) {
 
        /* 初始化软件定时器 */
        am_softimer_module_init(__SYSTEM_TICK_RATE);

        /* 将 SYSTICK 定时器的中断优先级设置为最低 */
        am_arm_nvic_priority_set(SysTick_IRQn, 0x03, 0x03);

        am_timer_callback_set(handle, 0, __system_tick_softimer_callback, NULL);

        am_system_module_init(__SYSTEM_TICK_RATE);

        /*
         * 定时时间，1 / __SYSTEM_TICK_RATE，
         * 转换为 us 即为： 1000000 / __SYSTEM_TICK_RATE
         *
         * 在 am_system_tick_inst_init() 函数中，使用了 am_timer_enable()函数，
         * 注意他们之前的区别。
         */
        am_timer_enable_us(handle, 0, 1000000 / __SYSTEM_TICK_RATE);
    }
}

/**
 * @}
 */

/* end of file */
