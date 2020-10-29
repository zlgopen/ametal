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
 * \brief lpsoftimer timer implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 20-02-13  win, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_softimer.h"

#include "am_zsl42x_lpsoftimer.h"
#include "am_zsl42x_lpsoftimer_if.h"

#include "am_zsl42x.h"
#include "hw/amhw_zsl42x_lptim.h"

#include <math.h>

/**
 * 每毫秒对应的ticks(百万倍)
 */
static uint64_t __g_ticks_per_ms;   /* unit: 1e-6 扩大了10^6倍 */

/**
 * lptmr全局设备信息指针
 */
static am_zsl42x_lptmr_lpsoftimer_devinfo_t *__gp_devinfo;

/**
 * lpsoftimer滴答值结构
 */
static struct lpsoftimer_ticks {
    uint32_t global_ticks;      /* 全局的ticks */
} volatile __g_lpsoftimer = {
    0,
};

/**
 * lpsoftimer避免回绕定时值
 */
#define __LPSOFTIMER_AVOID_WRAP_TICK        0xFFFFFFF0

/**
 * \brief 定时器中断请求函数
 */
static void __timer_irq_handler (void *p_arg);

/**
 * \brief 获取当前计数值
 */
static uint16_t __lptimer_ticks_get (void)
{
    return amhw_zsl42x_lptim_cnt16_count_get(__gp_devinfo->p_hw_lptmr1) & 0xFFFF;
}

/**
 * \brief 设置匹配值
 * \note (val - 1)以修正tick，若传入0，则修正到0xFFFF
 */
static void __lptimer_match_ticks_set (uint16_t val)
{

    /* 禁止定时器产生中断 */
    amhw_zsl42x_lptim_int_disable(__gp_devinfo->p_hw_lptmr0);

    /* 关闭计数器(测试所得) */
    amhw_zsl42x_lptim_disable(__gp_devinfo->p_hw_lptmr0);

    /* 等待同步完成 */
    while(amhw_zsl42x_lptim_write_into_arr_stat(__gp_devinfo->p_hw_lptmr0) ==
          AM_FALSE);

    /*
     * 设置重载寄存器
     * 定时时钟周期为0xFFFF-ARR+0x0001
     */
    amhw_zsl42x_lptim_arr_count_set(__gp_devinfo->p_hw_lptmr0,
                                  0XFFFF - val + 1);

    /* 清除标志位 */
    amhw_zsl42x_lptim_int_flag_clr(__gp_devinfo->p_hw_lptmr0);

    /* 使能定时器LPTIM允许计数 */
    amhw_zsl42x_lptim_enable(__gp_devinfo->p_hw_lptmr0);

    /* 中断使能 */
    amhw_zsl42x_lptim_int_enable(__gp_devinfo->p_hw_lptmr0);
}

/**
 * \brief 更新当前实际计数值
 * \retval 距上一次更新花了多少ticks
 */
static uint16_t __lpsoftimer_current_ticks_update (void)
{
    uint16_t lost_ticks;
    uint32_t cpu_key;

    cpu_key = am_int_cpu_lock();

    /* 溢出自动重载 */
    lost_ticks = __lptimer_ticks_get() - (uint16_t)__g_lpsoftimer.global_ticks;

    /* 此处绝不会越界（回绕） */

    /* 全局计时器更新 */
    __g_lpsoftimer.global_ticks += lost_ticks;

    am_int_cpu_unlock(cpu_key);

    return lost_ticks;
}

/**
 * \brief 获取当前实际计数值
 * \note 16位硬件定时器需要经过换算，32位硬件定时器可以直接获取CNT
 */
static uint32_t __lpsoftimer_current_ticks_get (void)
{
    __lpsoftimer_current_ticks_update();
    return __g_lpsoftimer.global_ticks;
}

/**
 * \brief lptimer初始化
 */
static void __lptimer_init (const am_zsl42x_lptmr_lpsoftimer_devinfo_t *p_info)
{
    uint32_t lptmr_clock = p_info->xtl_clk;
    uint32_t pre_reg     = 1;
    uint32_t pre_real    = 0;
    uint32_t temp        = 0;

    /* 计数/计时功能选择 */
    amhw_zsl42x_lptim_function_sel(p_info->p_hw_lptmr0,
                                   AMHW_ZSL42x_LPTIM_FUNCTION_TIMER);
    amhw_zsl42x_lptim_function_sel(p_info->p_hw_lptmr1,
                                   AMHW_ZSL42x_LPTIM_FUNCTION_TIMER);

    /* 不自动重载 */
    amhw_zsl42x_lptim_mode_sel(p_info->p_hw_lptmr0,
                               AMHW_ZSL42x_LPTIM_MODE_NORELOAD);
    amhw_zsl42x_lptim_mode_sel(p_info->p_hw_lptmr1,
                               AMHW_ZSL42x_LPTIM_MODE_RELOAD);

    /* 时钟源小于256kHz， 分频至1kHz */
    if (lptmr_clock <= 256000) {

        /* 分频至1kHz */
        temp = lptmr_clock / 1000;

        /* 计算分频值 2^n */
        for (pre_real = 2; pre_real < temp;) {
            pre_reg++;
            pre_real  = pre_real << 1;
        }
        /* lptmr_clock / pre_real 为1秒的tick */
        __g_ticks_per_ms = (uint64_t)(lptmr_clock / pre_real) * 1000000 / 1000;

    } else {
        pre_reg          =  AMHW_ZSL42x_LPTIM_CLK_DIV256;
        /* lptmr_clock / pre_real 为1秒的tick */
        __g_ticks_per_ms = (uint64_t)(lptmr_clock / 256) * 1000000 / 1000;
    }

    /* 设置预分频值 */
    amhw_zsl42x_lptim_clk_div_set(p_info->p_hw_lptmr0,
                                (amhw_zsl42x_lptim_clk_div_t)pre_reg);
    amhw_zsl42x_lptim_clk_div_set(p_info->p_hw_lptmr1,
                                (amhw_zsl42x_lptim_clk_div_t)pre_reg);

    /* 禁能门控  */
    amhw_zsl42x_lptim_gate_disable(p_info->p_hw_lptmr0);
    amhw_zsl42x_lptim_gate_disable(p_info->p_hw_lptmr1);

    /* TOG、TOGN信号输出设置  */
    amhw_zsl42x_lptim_tog_disable(p_info->p_hw_lptmr0);
    amhw_zsl42x_lptim_tog_disable(p_info->p_hw_lptmr1);

    /* 定时器中断 */
    amhw_zsl42x_lptim_int_enable(p_info->p_hw_lptmr0);
    amhw_zsl42x_lptim_int_enable(p_info->p_hw_lptmr1);

    /* 使能定时器LPTIM允许计数 */
    amhw_zsl42x_lptim_disable(p_info->p_hw_lptmr0);
    amhw_zsl42x_lptim_enable(p_info->p_hw_lptmr1);

    /* 等待同步完成 */
    while(amhw_zsl42x_lptim_write_into_arr_stat(__gp_devinfo->p_hw_lptmr1) ==
          AM_FALSE);

    /*
     * 设置重载寄存器
     * 定时时钟周期为0xFFFF-ARR+0x0001
     */
    amhw_zsl42x_lptim_arr_count_set(__gp_devinfo->p_hw_lptmr1, 1);

#if 0
    while(amhw_zsl42x_lptim_write_into_arr_stat(__gp_devinfo->p_hw_lptmr1 ) ==
          AM_FALSE);

    /* 使能定时器LPTIM允许计数 */
    amhw_zsl42x_lptim_disable(p_info->p_hw_lptmr0);
    amhw_zsl42x_lptim_enable(p_info->p_hw_lptmr1);
#endif

    /* 使能中断  */
    am_int_connect(p_info->lptmr0_inum, __timer_irq_handler, (void *)0);
    am_int_enable(p_info->lptmr0_inum);
}

/******************************************************************************/
int am_zsl42x_lptmr_lpsoftimer_init (
    const am_zsl42x_lptmr_lpsoftimer_devinfo_t *p_devinfo)
{
    /* 参数有效性检查 */
    if (NULL == p_devinfo) {
        return -AM_EINVAL;
    }

    /* 拷贝全局设备信息指针 */
    __gp_devinfo = (am_zsl42x_lptmr_lpsoftimer_devinfo_t *)p_devinfo;

    /* 平台初始化 */
    if (p_devinfo->pfn_plfm_init != NULL) {
        p_devinfo->pfn_plfm_init();
    }

    /* 中断频率分频至接近1000Hz以保证计数器的最大利用率 */
    __lptimer_init(p_devinfo);

    return 0;
}

/******************************************************************************/
void am_zsl42x_lptmr_lpsoftimer_deinit (void)
{
    /* 平台去初始化 */
    __gp_devinfo->pfn_plfm_deinit();
}

/******************************************************************************/
// adapter

float __lpsoftimer_temp_compensation(float period, float temp)
{
#if 1       //执行此段代码需要47.298us
    float k       = __gp_devinfo->temp_coef;         //  RTC_TEMP_COEFFICIENT
    float k_dev   = __gp_devinfo->temp_coef_sub;     //  RTC_TEMP_DEV_COEFFICIENT
    float t       = __gp_devinfo->temp_turnover;     //  RTC_TEMP_TURNOVER
    float t_dev   = __gp_devinfo->temp_turnover_sub; //  RTC_TEMP_DEV_TURNOVER
    float interim = 0.0;
    float ppm     = 0.0;

    if (k < 0.0) {
        ppm = (k - k_dev);
    } else {
        ppm = ( k + k_dev );
    }

    interim = (temp - (t - t_dev));
    ppm    *=  interim * interim;

    // Calculate the drift in time
    interim = (period * ppm) / 1000000;
    // Calculate the resulting time period
    interim += period;

    // Calculate the resulting period
    if (interim < 0.0) {
        return period;
    } else {
        return interim;
    }
#else
    return period;
#endif
}

/**
 * Lora timer 温度补偿
 */
uint32_t am_lpsoftimer_temp_compensation (uint32_t period, float temperature)
{
    return (uint32_t)__lpsoftimer_temp_compensation((float)period, temperature);
}

/**
 * \brief softimer Ms时间转化为系统tick时间
 */
uint32_t am_lpsoftimer_ms_to_tick (am_lpsoftimer_time_t ms, float temp)
{
    float tick = __gp_devinfo->timer_offset_coef *
                 __lpsoftimer_temp_compensation(ms, temp);

    return (uint32_t)floor(tick);
}

/**
 * \brief softimer 系统tick转化为Ms时间
 */
am_lpsoftimer_time_t am_lpsoftimer_tick_to_ms (uint32_t tick, float temp)
{
    float  k       = __gp_devinfo->temp_coef;
    float  k_dev   = __gp_devinfo->temp_coef_sub;
    float  t       = __gp_devinfo->temp_turnover;
    float  t_dev   = __gp_devinfo->temp_turnover_sub;
    float  interim = 0.0;
    float  ppm     = 0.0;
    double ms      = 0;

    if (k < 0.0) {
        ppm = (k - k_dev);
    } else {
        ppm = ( k + k_dev );
    }

    interim = (temp - (t - t_dev));
    ppm    *=  interim * interim;
    ms      =  tick / (ppm / 1000000 + 1);

    return (uint32_t)floor((2 - __gp_devinfo->timer_offset_coef) * ms);
}

uint32_t __fsl_lptmr_lpsoftimer_current_get (void)
{
    return ((uint64_t)__lpsoftimer_current_ticks_get() *
            (uint64_t)1000000) / __g_ticks_per_ms;
}

void __fsl_lptmr_lpsoftimer_timeout_set (uint32_t timeout)
{
    uint32_t remain_ticks;
    uint32_t cpu_key;

    __lpsoftimer_current_ticks_update();

    cpu_key = am_int_cpu_lock();

    /* 无符号自动回绕，未来减现在，即使溢出也能获得绝对值 */
    remain_ticks = ((uint64_t)timeout * __g_ticks_per_ms) / (uint64_t)1000000;
    if (remain_ticks <= ((uint16_t)__LPSOFTIMER_AVOID_WRAP_TICK)) {
        __lptimer_match_ticks_set(remain_ticks);
    } else {
        __lptimer_match_ticks_set((uint16_t)__LPSOFTIMER_AVOID_WRAP_TICK);
    }

    am_int_cpu_unlock(cpu_key);
}

void am_lpsoftimer_timeout_set (am_lpsoftimer_time_if_t timeout)
{
    __fsl_lptmr_lpsoftimer_timeout_set(timeout);
}

am_lpsoftimer_time_if_t am_lpsoftimer_current_get (void)
{
    return __fsl_lptmr_lpsoftimer_current_get();
}

/**
 * \brief 关闭CPU锁
 */
uint32_t am_lpsoftimer_cpu_lock (void)
{
    return am_int_cpu_lock();
}

/**
 * \brief 打开CPU锁
 */
void am_lpsoftimer_cpu_unlock (uint32_t lock)
{
    am_int_cpu_unlock(lock);
}

/******************************************************************************/

/**
 * \brief LPTIMER0 中断服务
 */
static void __lptimer0_irq_handler (void)
{
    /* 更新全局tick */
    __lpsoftimer_current_ticks_update();

    /* 回调软件定时器中断服务函数 */
    am_lpsoftimer_isr();

    /* 更新全局tick */
    __lpsoftimer_current_ticks_update();

    /* 清除标志位 */
    amhw_zsl42x_lptim_int_flag_clr(__gp_devinfo->p_hw_lptmr0);
}

/**
 * \brief LPTIMER1 中断服务
 */
static void __lptimer1_irq_handler (void)
{
    /* 更新全局tick */
    __lpsoftimer_current_ticks_update();

    /* 清除标志位 */
    amhw_zsl42x_lptim_int_flag_clr(__gp_devinfo->p_hw_lptmr1);
}

/**
 * \brief 每毫秒对应 1.024个tick => 每个tick 0.976563ms
 *        证实输入需要tick需要-1才能达到想要的计数
 *        输入0时则下一个tick会中断
 *       (定时器使能、两个值相等且计数值增加时，TCF触发)
 */
static void __timer_irq_handler (void *p_arg)
{
    (void)p_arg;

    if (amhw_zsl42x_lptim_int_flag_check(__gp_devinfo->p_hw_lptmr0)) {
        __lptimer0_irq_handler();
    }

    if (amhw_zsl42x_lptim_int_flag_check(__gp_devinfo->p_hw_lptmr1)) {
        __lptimer1_irq_handler();
    }
}

/* end of file */
