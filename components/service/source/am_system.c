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
 * \brief AMetal System implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_system.h"
#include "am_int.h"

/*******************************************************************************
  Global Variables
*******************************************************************************/ 
volatile static unsigned long __g_systick;
volatile static unsigned long __g_clkrate;

/*******************************************************************************
  Public Functions
*******************************************************************************/
int am_system_module_init (unsigned long clk_rate)
{
    __g_systick = 0;
    __g_clkrate = clk_rate;

    return 0;
}

/******************************************************************************/
int am_system_module_tick (void)
{
    int key = am_int_cpu_lock();
    __g_systick++;
    am_int_cpu_unlock(key);

    return 0;
}

/******************************************************************************/
am_tick_t am_sys_tick_get (void)
{
    return __g_systick;
}

/******************************************************************************/
unsigned long am_sys_clkrate_get (void)
{
    return __g_clkrate;
}

/******************************************************************************/
am_tick_t am_ms_to_ticks (unsigned int ms)
{
    uint64_t    t = ms;
    am_tick_t   tick;

    t *= am_sys_clkrate_get();
    t = (t + 1000 -1)/1000;
    tick = (am_tick_t)t;

    return tick;
}

/******************************************************************************/
unsigned int am_ticks_to_ms (am_tick_t ticks)
{
    uint64_t   t;

    t = ticks;
    t *= 1000;
    t /= am_sys_clkrate_get();
    return (unsigned int)t;
}

/* end of file */
