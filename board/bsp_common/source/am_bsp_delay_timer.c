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
 * \brief 基于通用定时器实现的延时函数
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-12  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_timer.h"
#include "am_bsp_delay_timer.h"

/*******************************************************************************
  Local macro define
*******************************************************************************/

/* 低于400us时，使用while(i--)的形式进行延时 */
#define __DELAY_DEC_TIME_US         500

/*******************************************************************************
  Local struct type define
*******************************************************************************/

/**
 * \brief 延时定时器相关信息定义
 */
static struct __delay_timer {
    am_timer_handle_t handle;           /**< \brief 延时定时器操作句柄        */
    uint8_t           chan;             /**< \brief 延时定时器通道            */
    uint32_t          freq;             /**< \brief 延时定时器频率            */
    uint32_t          max_ticks;        /**< \brief 延时定时器最大计数值      */
    uint32_t          cali_ticks;       /**< \brief 延时定时器被调用的次数    */
    uint32_t          count_get_ticks;  /**< \brief count get 所消耗的 tick   */
    uint32_t          dec_factor;       /**< \brief 延时因子                  */
    uint32_t          dec_cali_factor1; /**< \brief 递减法延时修正值1         */
    uint32_t          dec_cali_factor2; /**< \brief 递减法延时修正值2         */
    uint32_t          dec_cali_us;      /**< \brief 递减法延时修正值          */
} __g_delay_timer = {NULL, 0, 0, 0, 0, 0, 0, 0};

/*******************************************************************************
  Local Functions
*******************************************************************************/

/* 获取定时器当前计数值 */
static uint32_t __timer_count_get (void)
{
    uint32_t ticks;

    am_timer_count_get(__g_delay_timer.handle,
                       __g_delay_timer.chan,
                       &ticks);

    return ticks;
}

/******************************************************************************/

/* 计算一段程序段所消耗的 ticks */
static uint32_t __time_ticks_cal (uint32_t start_ticks, uint32_t end_ticks)
{
    uint32_t ticks;

    if (end_ticks >= start_ticks) {
        ticks = end_ticks - start_ticks;
    } else {
        ticks = end_ticks + (__g_delay_timer.max_ticks - start_ticks);
    }

    if (ticks >= __g_delay_timer.count_get_ticks) {
        ticks -= __g_delay_timer.count_get_ticks;
    } else {
        ticks = 0;                                     /* 不应该出现这种情况  */
    }

    return ticks;
}

/******************************************************************************/

/* 计算am_timer_count_get() 所消耗的时间 */
static void __delay_count_get_ticks_cal (void)
{
    uint32_t           start_ticks;
    uint32_t           cur_ticks;

    __g_delay_timer.count_get_ticks = 0;

    start_ticks = __timer_count_get();
    cur_ticks   = __timer_count_get();

    __g_delay_timer.count_get_ticks = __time_ticks_cal(start_ticks, cur_ticks);
}

/******************************************************************************/
/* 计算基于 Ticks 延时的固有偏差 */
static void __delay_cali_ticks_cal (void)
{
    uint32_t           start_ticks;
    uint32_t           cur_ticks;

    __g_delay_timer.cali_ticks = 0xFFFFFFFF;            /* set to max         */

    /* 
     * 粗略评估延时误差, 
     * 对于ARMCC，使用微库时，64位数的计算非常耗时，将导致误差增大 
     */
    start_ticks = __timer_count_get();                  /* 修正记录起始位置处 */
    am_udelay(__DELAY_DEC_TIME_US + 1);
    cur_ticks   = __timer_count_get();                  /* 修正记录结束位置处 */

    /* 已经记了多少ticks, 作为延时误差修正值 */
    __g_delay_timer.cali_ticks = __time_ticks_cal(start_ticks, cur_ticks);
}

/******************************************************************************/

/* while(i--), 作为 us 级别延时的核心函数，不可优化，不可被内联处理 */
static void __delay_by_factor (uint32_t dec_factor) __attribute__ ((noinline));
 
static void __delay_by_factor (uint32_t dec_factor)
{
    volatile uint32_t factor = dec_factor;

    /*
     * 增加 ISB 指令，清空流水线，避免分支预测对延时造成影响，进而提高延时精度
     */
    while(factor--) {
        __asm volatile ("isb");
    }
}

/******************************************************************************/
static uint32_t  __delay_by_factor_ticks_cal (uint32_t dec_factor)
{
    uint32_t  start_ticks;
    uint32_t  cur_ticks;

    start_ticks = __timer_count_get();
    __delay_by_factor(dec_factor);
    cur_ticks   = __timer_count_get();
    
    return __time_ticks_cal(start_ticks, cur_ticks);
}

/******************************************************************************/
/* 计算递减法延时因子 */
static void __delay_by_fator_cal (void)
{
    uint32_t           ticks, ticks1, ticks2;
    uint32_t           delay_us;
    uint32_t           dec_factor = 100;

    while (1) {

        ticks1 = __delay_by_factor_ticks_cal(dec_factor);

        /* ticks 超过100 us 对应的ticks 才计算，使计算误差 < 0.01us */
        if (ticks1 > __g_delay_timer.freq / 10000) {

            ticks2 = __delay_by_factor_ticks_cal(dec_factor * 2);

            /* 增加dec_factor所耗费的ticks即为延时dec_factor所对应的 ticks */
            ticks = ticks2 - ticks1;

            /*
             * ticks 对应的us数: us = ticks / (__g_delay_timer.freq / 1000000)
             *               即：us = 1000000 * ticks / __g_delay_timer.freq
             *
             * 1us 对应的延时因子 ：dec_factor / us
             *
             * 256us （便于作移位运算）对应的延时因子： 256 * dec_factor / us
             *
             * 即：256 * dec_factor / (1000000 * ticks / __g_delay_timer.freq)
             *
             * 即：256 * dec_factor * __g_delay_timer.freq / (1000000 * ticks)
             *
             * 避免溢出：
             *
             * 即：4 * dec_factor * __g_delay_timer.freq / (15625 * ticks)
             *     4 * dec_factor * (__g_delay_timer.freq / 15625) / ticks
             */
            __g_delay_timer.dec_factor = 4 * dec_factor * \
                                         (__g_delay_timer.freq / 15625) / ticks;

            break;
        }
        dec_factor *= 2;
    }

    /*
      * 计算 dec_cali_us
      */
     __g_delay_timer.dec_cali_us = 0xFFFFFFFF;           /* set to max */

    ticks1 = __timer_count_get();
    am_udelay(1);
    ticks2 = __timer_count_get();

    ticks1 = __time_ticks_cal(ticks1, ticks2);

    /* ticks1 对应的us, 四舍五入 */
    __g_delay_timer.dec_cali_us = \
            (ticks1 * 1000 + (__g_delay_timer.freq / 2000)) / \
                                                (__g_delay_timer.freq / 1000);
    /*
      * 计算 delay 函数固有延时1
      */
     __g_delay_timer.dec_cali_factor1 = 0xFFFFFFFF;          /* set to max */

    delay_us = __g_delay_timer.dec_cali_us + 1;

    ticks1 = __timer_count_get();
    am_udelay(delay_us);
    ticks2 = __timer_count_get();

    ticks1 = __time_ticks_cal(ticks1, ticks2);

    /*
     * 将 ticks1 转换为对应的 延时因子
     *
     * 延时 ticks 对应的延时因子为： dec_factor
     *
     * ticks 对应的延时因子即为： ticks1 * dec_factor / ticks
     *
     * 也可选择将 ticks 转换为 us：
     * (ticks * 1000 + (__g_delay_timer.freq / 2000))
     *                                        / (__g_delay_timer.freq / 1000);
     */
    __g_delay_timer.dec_cali_factor1 = (ticks1 * dec_factor + ticks / 2) / ticks;

    /*
     * 计算 delay 函数固有延时2
     */
    __g_delay_timer.dec_cali_factor2 = 0xFFFFFFFF;          /* set to max */

    /* 必须时延时数超过 dec_cali_factor1 */
    delay_us = (__g_delay_timer.dec_cali_factor1 + 1) * 256 /   \
                                     __g_delay_timer.dec_factor + 1;

    ticks1 = __timer_count_get();
    am_udelay(delay_us);
    ticks2 = __timer_count_get();

    ticks1 = __time_ticks_cal(ticks1, ticks2);

    /* 将 ticks1 转换为对应的 延时因子 */
    __g_delay_timer.dec_cali_factor2 = (ticks1 * dec_factor + ticks / 2) / ticks;
}

/******************************************************************************/

static void __delay (uint64_t ticks)
{
    uint32_t   max_ticks            = __g_delay_timer.max_ticks;
    uint32_t   half_count_get_ticks = __g_delay_timer.count_get_ticks / 2;

    uint32_t   start_ticks;     /* 记录值 */
    uint32_t   cur_ticks;       /* 当前值 */
    uint32_t   cnt_ticks;       /* 计数值 */

    /* 记录当前ticks值 */
    start_ticks = __timer_count_get();

    /* 至此，可以认为已经消耗了 cali_ticks, 检查是否已经计时结束 */
    if (ticks <= __g_delay_timer.cali_ticks) {
        return;
    } else {
        ticks -= __g_delay_timer.cali_ticks;
    }

    while (1) {

        /* 获取当前ticks值 */
        cur_ticks = __timer_count_get();

        /* 已经记了多少ticks */
        if (cur_ticks >= start_ticks) {
            cnt_ticks = cur_ticks - start_ticks;
        } else {
            cnt_ticks = cur_ticks + (max_ticks - start_ticks);
        }

        /*
         * 检查是否已经计时结束,
         * 特别地，剩余时间不到 count 获取耗时的一半，也结束，减小误差
         */
        if (cnt_ticks >= ticks) {
            break;
        } else {
            ticks -= cnt_ticks;
            if (ticks <= half_count_get_ticks) {
                break;
            }
        }
        start_ticks = cur_ticks;
    }
}

/*******************************************************************************
  Public Functions
*******************************************************************************/

/**
 * \brief 延时函数初始化
 */
void am_bsp_delay_timer_init (am_timer_handle_t handle, uint8_t chan)
{
    const am_timer_info_t *p_info = NULL;

    int key;

    /* 参数有效性判断 */
    if (NULL == handle) {
        return;
    }

    /* 获取当前定时器信息 */
    p_info = am_timer_info_get(handle);

    /* 定时器特性判断 */
    if ((!(AM_TIMER_INTERMEDIATE_COUNT & p_info->features)) || /* 不支持读取当前计数值 */
        (AM_TIMER_CANNOT_DISABLE & p_info->features) ||        /* 不能被禁能 */
        (AM_TIMER_STOP_WHILE_READ & p_info->features) ||       /* 读取时会停止 */
        (!(AM_TIMER_AUTO_RELOAD & p_info->features))) {        /* 不支持自动重载 */
        return;
    }

    /* 定时器通道合理性判断 */
    if (chan >= p_info->chan_num) {
        return;
    }

    /* 对全局__g_delay_timer赋值 */
    __g_delay_timer.handle    = handle;
    __g_delay_timer.chan      = chan;

    am_timer_count_freq_get(handle, chan, &__g_delay_timer.freq);
    am_timer_rollover_get(handle, chan, &__g_delay_timer.max_ticks);

    /* 若无其它应用使用延时定时器，则设置重载值为最大值 */
    if (__g_delay_timer.max_ticks == 0) {
        __g_delay_timer.max_ticks = (1 << p_info->counter_width) - 1;
    }
    am_timer_enable(handle, chan, __g_delay_timer.max_ticks);

    /* 评估一些重要时间时，关闭中断 */
    key = am_int_cpu_lock();
    __delay_count_get_ticks_cal();             /* 计算count_get()需要的ticks  */
    am_int_cpu_unlock(key);

    key = am_int_cpu_lock();
    __delay_cali_ticks_cal();                  /* 计算基于ticks延时误差修正值 */
    am_int_cpu_unlock(key);

    key = am_int_cpu_lock();
    __delay_by_fator_cal();                    /* 计算递减法延时因子          */
    am_int_cpu_unlock(key);
}

/******************************************************************************/
void am_mdelay (uint32_t nms)
{
    __delay((uint64_t)nms * __g_delay_timer.freq / 1000);
}

/******************************************************************************/
void am_udelay (uint32_t nus)
{
    if (nus < __DELAY_DEC_TIME_US) {

        uint32_t factor;

        if (nus <= __g_delay_timer.dec_cali_us) {
            return;
        }

        factor = (__g_delay_timer.dec_factor * nus) >> 8;

        if (factor <= __g_delay_timer.dec_cali_factor1) {

            factor -= __g_delay_timer.dec_cali_factor1;

        } else if (factor <= __g_delay_timer.dec_cali_factor2){

            factor -= __g_delay_timer.dec_cali_factor2;

            __delay_by_factor(0);

        } else {

            factor -= __g_delay_timer.dec_cali_factor2;

            __delay_by_factor(factor);
        }

    } else {
        __delay((uint64_t)nus * __g_delay_timer.freq / 1000000);
    }
}

/* end of file */
