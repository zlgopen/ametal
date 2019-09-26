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
 * \brief 定时器实现标准定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 调试串口输出定时器的相关信息
 *   2. 定时周期到达(默认为 10Hz)，调试串口会输出 The timing frq is 10Hz;
 *
 * \par 源代码
 * \snippet demo_std_timer_timing.c src_std_timer_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-22  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_std_timer_timing
 * \copydoc demo_std_timer_timing.c
 */

/** [src_std_timer_timing] */
#include "ametal.h"
#include "am_timer.h"
#include "am_delay.h"
#include "am_common.h"
#include "am_vdebug.h"
#include "am_board.h"

/**
 * \brief 定时器回调函数
 */
static void __tim_timing_callback (void *p_arg)
{
    AM_DBG_INFO("The timing frq is 10Hz\r\n");
    am_led_toggle(LED0);
}

/**
 * \brief 例程入口
 */
void demo_std_timer_timing_entry (am_timer_handle_t handle,
                                  int               timing_chan)
{

    const am_timer_info_t *p_info  = am_timer_info_get(handle);

    uint32_t clkin;

    am_timer_clkin_freq_get(handle, &clkin);

    AM_DBG_INFO("The timer size is : %d-bit \r\n", p_info->counter_width);
    AM_DBG_INFO("The timer clk_frequency: %d Hz\r\n", clkin);

    /* 设置回调函数 */
    am_timer_callback_set(handle, timing_chan, __tim_timing_callback, NULL);

    /* 设置定时时间为 0.1s （即1000000 / 10 us） */
    am_timer_enable_us(handle, timing_chan, 1000000 / 10);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_timer_timing] */

/* end of file */
