/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief AM824ZB
 *
 * \internal
 * \par Modification history
 * - 1.00 14-12-01  tee, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_prj_config.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_lpc82x_clk.h"
#include "am_softimer.h"
#include "am_lpc82x_inst_init.h"
#include "am_lpc82x.h"
#include "am_system.h"
#include "am_buzzer.h"
#include "am_buzzer_pwm.h"
#include "am_event_input_key.h"
#include "am_event_category_input.h"
#include "am_hwconf_key_gpio.h"
#include "am_hwconf_led_gpio.h"
#include "am_arm_nvic.h"

#include "am_bsp_system_heap.h"
#include "am_bsp_isr_defer_pendsv.h"
#include "am_bsp_delay_timer.h"
#include "am_service_inst_init.h"

#ifdef __CC_ARM
#ifdef __MICROLIB
#include "am_bsp_microlib.h"
#else
#include "am_bsp_armlib.h"
#endif /* __MICROLIB */
#elif defined(__GNUC__)
#include "am_bsp_newlib.h"
#endif

/** \brief SRAM 信息,使用 ARMCC 时需要提供 SRAM结束地址 */
#ifdef __CC_ARM
#define SRAM_SIZE   8
#define SRAM_START  0x10000000
#define SRAM_END    (SRAM_START + SRAM_SIZE * 1024)
#endif /* __CC_ARM */

/*******************************************************************************
  全局变量
*******************************************************************************/

/**
 * \brief SCT0_PWM 的handle
 *
 * 当使用蜂鸣器时，默认将使用SCT的OUT1输出PWM波形，因此已经将SCT初始化为PWM功能，
 * 由于SCT可以输出六路PWM，因此可以使用该handle，以便使用其它5路PWM。
 */
am_pwm_handle_t g_sct0_pwm_handle;

/**
 * \brief MRT 的handle
 *
 * 当使用软件定时器时，默认将使用MRT作为基础定时单元，因此已经将MRT初始化为定时器
 * 功能，由于MRT可以支持4路定时，因此可以使用该 handle，以便使用其它3路 handle
 */
am_timer_handle_t g_mrt_handle;

/**
 * \brief CAT823 看门狗的handle
 *
 *  CAT823 WDI已连接  PIO0_22，注意该引脚最好不要使用，以防芯片意外复位
 */
am_wdt_handle_t   g_cat823_handle;

/*******************************************************************************
  板上LED相关信息
*******************************************************************************/

#if (AM_CFG_LED_ENABLE == 1)

#include "am_led.h"

#endif /* (AM_CFG_LED_ENABLE == 1) */

/*******************************************************************************
  板上无源蜂鸣器相关信息
*******************************************************************************/

#if (AM_CFG_BUZZER_ENABLE == 1)

#include "am_lpc_sct_pwm.h"

#define __BUZZER_PWM_PERIOD_NS \
            (1000000000 / AM_CFG_BUZZER_PWM_FREQ)

#define __BUZZER_PWM_DUTY_NS   \
            (10000000 * AM_CFG_BUZZER_PWM_DUTY / AM_CFG_BUZZER_PWM_FREQ)

#endif /* (AM_CFG_BUZZER_ENABLE == 1) */

/*******************************************************************************
  公共函数
*******************************************************************************/

#if (AM_CFG_SYSTEM_TICK_ENABLE == 1)

/* 系统定时器，用于产生系统滴答，驱动软件定时器等等 */
static void __system_timer_callback (void *p_arg)
{
    am_system_module_tick();       /* 驱动系统滴答 */

#if (AM_CFG_SOFTIMER_ENABLE == 1)
    am_softimer_module_tick();     /* 驱动软件定时器 */
#endif /* (AM_CFG_SOFTIMER_ENABLE == 1) */

}

#endif /* (AM_CFG_SYSTEM_TICK_ENABLE == 1) */

#if (AM_CFG_CAT823_WDT_ENABLE == 1)

am_softimer_t     g_softimer;

/* 喂狗操作 */
static void __cat823_wdt_feed (void *p_arg)
{
    am_wdt_feed(g_cat823_handle);
}

#endif

/**
 * \brief 板级初始化
 */
void am_board_init (void)
{
    am_uart_handle_t dbg_handle = NULL;

#ifdef  __GNUC__
    extern char __heap_start__;            /* Defined by the linker */
    extern char __heap_end__;              /* Defined by the linker */

    static char *heap_start = &__heap_start__;
    static char *heap_end   = &__heap_end__;

#elif defined(__CC_ARM)

    extern int Image$$RW_IRAM1$$ZI$$Limit; /* Defined by the linker */

    int *heap_start = (int *)&Image$$RW_IRAM1$$ZI$$Limit;
    int *heap_end   = (int *)SRAM_END;
#endif

    /* 系统堆栈初始化 */
    am_bsp_system_heap_init((void *)heap_start, (void *)heap_end);

/*
 * 若使能延时函数的使用，则执行初始化，默认实现为使用 Systick 定时器
 */
#if (AM_CFG_DELAY_ENABLE == 1)
    am_bsp_delay_timer_init(am_arm_systick_inst_init(), 0);
#endif /* (AM_CFG_DELAY_ENABLE == 1) */

/**
 * \brief 如果为1，则初始化led的相关功能，板上默认有两个LED
 *
 * ID: 0 --- PIO0.8 （需要短接跳线帽 J9）
 * ID: 1 --- PIO0.9 （需要短接跳线帽 J10）
 */
#if (AM_CFG_LED_ENABLE == 1)
    am_led_gpio_inst_init();
#endif /* (AM_CFG_LED_ENABLE == 1) */

#ifdef AM_VDEBUG
#if (AM_CFG_DEBUG_ENABLE == 1)

    dbg_handle = am_debug_uart_inst_init();

#endif /* (AM_CFG_DEBUG_ENABLE == 1) */
#endif /* AM_VDEBUG */

#if (AM_CFG_STDLIB_ENABLE == 1)
    #ifdef __CC_ARM
        #ifdef __MICROLIB

            /* 初始化Micro LIB */
            am_bsp_microlib_init(dbg_handle);
        #else

            /* 初始化ARM LIB */
            am_bsp_armlib_init(dbg_handle);
        #endif /* __MICROLIB */

    #elif defined(__GNUC__)

        /* 初始化 NEW LIB 库 */
        am_bsp_newlib_init(dbg_handle);
    #endif
#endif /* (AM_CFG_STDLIB_ENABLE == 2) */

#if (AM_CFG_SYSTEM_TICK_ENABLE == 1)

    /* 使能一个定时器作为系统TICK */
    {

        g_mrt_handle = am_lpc82x_mrt_inst_init();

        /* 将软件定时器的中断优先级设置为最低 */
        am_arm_nvic_priority_set(INUM_MRT, 0x03, 0x03);

        am_timer_callback_set(g_mrt_handle,
                              0,
                              __system_timer_callback,
                              NULL);

        am_system_module_init(AM_CFG_SYSTEM_TICK_RATE);

        am_timer_enable_us(g_mrt_handle,
                           0,
                           1000000 / AM_CFG_SYSTEM_TICK_RATE);
    }


#if (AM_CFG_SOFTIMER_ENABLE == 1)
    am_softimer_module_init(AM_CFG_SYSTEM_TICK_RATE);
#endif /* (AM_CFG_SOFTIMER_ENABLE == 1) */

#endif /* (AM_CFG_SYSTEM_TICK_ENABLE == 1) */

    g_cat823_handle = am_cat823_inst_init();

#if (AM_CFG_CAT823_WDT_ENABLE == 1)

    /* CAT823看门狗芯片只支持喂狗超时时间设置为1120ms */
    am_wdt_enable(g_cat823_handle, 1120);

    am_softimer_init(&g_softimer, __cat823_wdt_feed, NULL);
    am_softimer_start(&g_softimer, AM_CFG_CAT823_FEED_TIME_MS);

#endif

    /**
     * \brief 如果为1，则初始化蜂鸣器的相关功能，板上默认有一个蜂鸣器
     *
     * 默认使用SCT_OU1 （PIO0_2）输出PWM （需要短接跳线帽 J7）
     */
#if (AM_CFG_BUZZER_ENABLE == 1)

    g_sct0_pwm_handle = am_lpc82x_sct0_pwm_inst_init();

    am_buzzer_pwm_init(g_sct0_pwm_handle,
                       1,                    /* 使用通道1, SCT_OUT1, PIO0_2 */
                      __BUZZER_PWM_DUTY_NS,
                      __BUZZER_PWM_PERIOD_NS);

#endif /* (AM_CFG_BUZZER_ENABLE == 1) */

#if (AM_CFG_KEY_ENABLE == 1) || (AM_CFG_KEY_GPIO_ENABLE == 1)
    am_event_input_inst_init();                   /* 事件输入管理器服务初始化 */
#endif /* (AM_CFG_KEY_ENABLE == 1) || (AM_CFG_KEY_GPIO_ENABLE == 1) */

#if (AM_CFG_KEY_GPIO_ENABLE == 1)
    am_key_gpio_inst_init();
#endif /* (AM_CFG_KEY_GPIO_ENABLE == 1) */

    /* 其它内容待添加 */

}

/* end of file */
