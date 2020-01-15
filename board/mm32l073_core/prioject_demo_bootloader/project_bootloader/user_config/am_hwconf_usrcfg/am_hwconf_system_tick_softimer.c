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
#include "ametal.h"
#include "am_mm32l073.h"
#include "am_timer.h"
#include "am_softimer.h"
#include "am_system.h"
#include "am_arm_nvic.h"
#include "am_arm_systick.h"
#include "am_mm32l073_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_system_tick_softimer
 * \copydoc am_hwconf_system_tick_softimer.c
 * @{
 */


struct __prescale_and_count_cal_ctx {
    uint32_t   desired_ticks;           /* 目标 ticks                         */
    uint32_t   count_max;               /* 定时器最大 count 值                */
    uint32_t   error;                   /* 当前预分频和计数值所对应的误差值   */
    uint32_t   count;                   /* 当前计算结果: count 值             */
    uint32_t   prescale;                /* 当前计算结果: 预分频值             */
    am_bool_t  update;                  /* 是否更新过预分频值                 */
};

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
    am_timer_handle_t handle  = am_mm32l073_systick_inst_init();

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


static void __error_check_with_new_prescale (
    struct __prescale_and_count_cal_ctx *p_ctx, uint32_t prescale_new)
{
    uint32_t count = p_ctx->desired_ticks / prescale_new;
    uint32_t error;

    /* 避免 count 值超过 count_max */
    if (count > p_ctx->count_max) {
        count = p_ctx->count_max;
    }

    error = p_ctx->desired_ticks - count * prescale_new;

    if (p_ctx->error > error) {        /* 发现更小误差的组合值 */
        p_ctx->error    = error;
        p_ctx->count    = count;
        p_ctx->prescale = prescale_new;
        p_ctx->update   = AM_TRUE;
    }
}


/******************************************************************************/
static int __prescale_and_count_cal (const am_timer_info_t *p_info,
                                     uint32_t               ticks,
                                     uint32_t              *p_prescale,
                                     uint32_t              *p_count)
{
    struct __prescale_and_count_cal_ctx  ctx;

    uint32_t prescale_max;
    uint32_t prescale_min;
    uint32_t prescale;

    uint32_t count_max;

    count_max = ( (uint32_t)1ull << (p_info->counter_width) ) - 1;

    /* 无需分频 */
    if (ticks <= count_max) {

         *p_prescale = 1;
         *p_count    = ticks;

         return AM_OK;
    }

    /* 需要分频，但不支持任何分频值 */
    if (p_info->prescaler == 0) {
        return -AM_ENOTSUP;
    }

    prescale_min = ticks / count_max;        /* 满足 Ticks 的最小分频  */

    ctx.count_max     = count_max;
    ctx.desired_ticks = ticks;
    ctx.error         = ticks;              /* 初始设置为最大误差      */
    ctx.count         = count_max;
    ctx.prescale      = 1;
    ctx.update        = AM_FALSE;

    if (p_info->features & AM_TIMER_SUPPORT_ALL_PRESCALE_1_TO_MAX) {

        prescale_max = p_info->prescaler;

        if (prescale_max < prescale_min) {   /* 无法满足 */
            return -AM_ENOTSUP;
        }

        /* 遍历所有分频值，寻找最佳分频值（误差最小）*/
        for (prescale = prescale_min; prescale <= prescale_max; prescale++) {
            __error_check_with_new_prescale(&ctx, prescale);
            if (ctx.error == 0) {
                break;
            }
        }

    } else {

        for (prescale = 1; prescale != 0; prescale <<= 1) {

            /* 支持该分频， 且满足最小值要求  */
            if ((prescale & p_info->prescaler) && (prescale >= prescale_min)) {
                __error_check_with_new_prescale(&ctx, prescale);
                if (ctx.error == 0) {
                    break;
                }
            }
        }
    }

    if (ctx.update) {

         *p_prescale = ctx.prescale;
         *p_count    = ctx.count;

        return AM_OK;
    }

    return -AM_ENOTSUP;
}


/******************************************************************************/
static int __timer_enable_us (am_timer_handle_t handle, uint8_t chan, uint32_t nus)
{
    uint32_t               clkin;
    uint32_t               prescale;
    uint32_t               ticks;
    uint32_t               count;
    const am_timer_info_t *p_info;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_info = am_timer_info_get(handle);

    am_timer_clkin_freq_get(handle, &clkin);

    /* 计算在输入频率下，应该记录的count 值 */
    ticks = nus *  (clkin / 1000000);

    if (__prescale_and_count_cal(p_info, ticks, &prescale, &count) != AM_OK) {
        return -AM_ENOTSUP;
    }

    am_timer_prescale_set(handle, chan, prescale);

    if (p_info->counter_width <= 32) {
        am_timer_enable(handle, chan, count);
    } else {
        am_timer_enable64(handle, chan, count);
    }

    return AM_OK;
}

/** \brief 系统滴答实例初始化(使用软件定时器) */
void am_system_tick_softimer_inst_init (void)
{
    am_timer_handle_t handle = am_mm32l073_systick_inst_init();

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
        __timer_enable_us(handle, 0, 1000000 / __SYSTEM_TICK_RATE);
    }
}

/**
 * @}
 */

/* end of file */
