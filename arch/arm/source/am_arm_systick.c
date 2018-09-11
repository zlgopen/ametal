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
 * \brief SYSTICK 驱动，服务 Timer 标准接口
 *
 * \note 使用 SYSTICK 服务的 Timer 标准接口，就不能使用标准延时接口
 *       am_mdelay()和am_udelay()
 * \internal
 * \par Modification History
 * - 1.02 15-12-14  hgo, fix potential bug caused by incorrect p_arg on callback
 * - 1.00 15-09-22  win, first implementation.
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/
#include "am_arm_systick.h"
#include "hw/amhw_arm_systick.h"
#include "am_clk.h"

/*******************************************************************************
  函数声明
*******************************************************************************/
static const am_timer_info_t * __systick_info_get (void *p_drv);

static int __systick_clkin_freq_get (void *p_drv, uint32_t *p_freq);
static int __systick_prescale_set (void *p_drv, uint8_t chan, uint32_t prescale);
static int __systick_prescale_get (void *p_drv, uint8_t chan, uint32_t *p_prescale);
static int __systick_count_get (void *p_drv, uint8_t chan, void *p_count);
static int __systick_rollover_get (void *p_drv,uint8_t chan,void *p_rollover);
static int __systick_disable (void *p_drv, uint8_t chan);
static int __systick_enable (void *p_drv, uint8_t chan, void *p_count);

static int __systick_callback_set (void    *p_drv,
                                   uint8_t  chan,
                                   void   (*pfn_callback)(void *),
                                   void    *p_arg);

/*******************************************************************************
  全局变量
*******************************************************************************/

/* 定时器信息 */
static const am_timer_info_t __g_systick_info = {
    24,                                                   /* 24位定时器       */
    1,                                                    /* 单通道           */
    AM_TIMER_CAN_INTERRUPT      |                         /* 可以产生中断     */
    AM_TIMER_AUTO_RELOAD        |                         /* 支持自动重载     */
    AM_TIMER_INTERMEDIATE_COUNT,                          /* 当前计数器值可读 */
    0                                                     /* 不支持时钟预分频 */
};

/* SYSTICK 驱动函数  */
static const struct am_timer_drv_funcs __g_systick_drv_funcs = {
    __systick_info_get,
    __systick_clkin_freq_get,
    __systick_prescale_set,
    __systick_prescale_get,
    __systick_count_get,
    __systick_rollover_get,
    __systick_enable,
    __systick_disable,
    __systick_callback_set,
};

/*
 * 考虑到 SYSTICK 中断处理函数无法传递参数，使用一个全局变量指针，
 * 保存 SYSTICK 设备结构体地址
 */
static am_arm_systick_dev_t *__gp_systick_dev;

/*******************************************************************************
  中断处理
*******************************************************************************/

/**
 * \brief SYSTICK 中断处理函数
 *
 * \return 无
 */
void SysTick_Handler (void)
{
    if (__gp_systick_dev->pfn_callback != NULL) {
        __gp_systick_dev->pfn_callback(__gp_systick_dev->p_arg);
    }
}

/*******************************************************************************
  标准定时器驱动实现
*******************************************************************************/

/* 获取 SYSTICK 信息 */
static const am_timer_info_t * __systick_info_get (void *p_drv)
{
    return &__g_systick_info;
}

/******************************************************************************/
static int __systick_clkin_freq_get (void *p_drv, uint32_t *p_freq)
{
    am_arm_systick_dev_t *p_dev = (am_arm_systick_dev_t *)p_drv;

    if ((p_dev == NULL) || (p_freq == NULL)) {
        return -AM_EINVAL;
    }

    /* 获取当前系统时钟频率，systick 使用的是系统时钟 */
    *p_freq = am_clk_rate_get(p_dev->p_devinfo->clk_id);

    return AM_OK;
}

/******************************************************************************/
static int __systick_prescale_set (void *p_drv, uint8_t chan, uint32_t prescale)
{
    if (prescale == 1) {
        return AM_OK;
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
static int __systick_prescale_get (void *p_drv, uint8_t chan, uint32_t *p_prescale)
{
    am_arm_systick_dev_t *p_dev = (am_arm_systick_dev_t *)p_drv;

    if (p_dev == NULL || p_prescale == NULL || chan != 0) {
        return -AM_EINVAL;
    }

    /* 无分频器，分频值始终为1  */
    *p_prescale = 1;

    return AM_OK;
}

/******************************************************************************/
static int __systick_count_get (void *p_drv, uint8_t chan, void *p_count)
{
    am_arm_systick_dev_t *p_dev        = (am_arm_systick_dev_t *)p_drv;
    amhw_arm_systick_t   *p_hw_systick = NULL;

    if (p_dev == NULL || p_count == NULL || chan != 0) {
        return -AM_EINVAL;
    }

    p_hw_systick = (amhw_arm_systick_t *)p_dev->p_devinfo->systick_regbase;

    *((uint32_t *)p_count) = amhw_arm_systick_reload_val_get(p_hw_systick) - \
                             amhw_arm_systick_val_get(p_hw_systick);

    return AM_OK;
}

/******************************************************************************/
static int __systick_rollover_get (void     *p_drv,
                                   uint8_t   chan,
                                   void     *p_rollover)
{
    am_arm_systick_dev_t *p_dev        = (am_arm_systick_dev_t *)p_drv;
    amhw_arm_systick_t   *p_hw_systick = NULL;

    if (p_drv == NULL || p_rollover == NULL || chan != 0) {
        return -AM_EINVAL;
    }

    p_hw_systick = (amhw_arm_systick_t   *) p_dev->p_devinfo->systick_regbase;

    *((uint32_t *)p_rollover) = amhw_arm_systick_reload_val_get(p_hw_systick);

    return AM_OK;
}

/******************************************************************************/
static int __systick_disable (void *p_drv, uint8_t chan)
{
    am_arm_systick_dev_t *p_dev        = NULL;
    amhw_arm_systick_t   *p_hw_systick = NULL;

    if (p_drv == NULL || chan != 0) {
        return -AM_EINVAL;
    }

    p_dev        = (am_arm_systick_dev_t *)p_drv;
    p_hw_systick = (amhw_arm_systick_t   *)p_dev->p_devinfo->systick_regbase;

    amhw_arm_systick_int_disable(p_hw_systick);
    amhw_arm_systick_disable(p_hw_systick);

    return AM_OK;
}

/******************************************************************************/
static int __systick_enable (void     *p_drv,
                             uint8_t   chan,
                             void     *p_count)
{
    am_arm_systick_dev_t *p_dev        = NULL;
    amhw_arm_systick_t   *p_hw_systick = NULL;

    if ((p_drv == NULL) || (chan != 0) || (p_count == NULL)) {
        return -AM_EINVAL;
    }

    p_dev        = (am_arm_systick_dev_t *)p_drv;
    p_hw_systick = (amhw_arm_systick_t   *)p_dev->p_devinfo->systick_regbase;

    /* 设置重载定时寄存器中的数值 */
    amhw_arm_systick_reload_val_set(p_hw_systick, *((uint32_t *)p_count));

    /* 清除当前定时计数器中的数值 */
    amhw_arm_systick_val_set(p_hw_systick, 0x0);

    if (p_dev->pfn_callback == NULL) {
        amhw_arm_systick_int_disable(p_hw_systick);
    } else {
        amhw_arm_systick_int_enable(p_hw_systick);
    }

    /* 使能 SYSTICK */
    amhw_arm_systick_enable(p_hw_systick);

    return AM_OK;
}

/******************************************************************************/
static int __systick_callback_set (void    *p_drv,
                                   uint8_t  chan,
                                   void   (*pfn_callback)(void *),
                                   void    *p_arg)
{
    am_arm_systick_dev_t *p_dev        = NULL;
    amhw_arm_systick_t   *p_hw_systick = NULL;

    if (p_drv == NULL || chan != 0) {
        return -AM_EINVAL;
    }

    p_dev        = (am_arm_systick_dev_t *)p_drv;
    p_hw_systick = (amhw_arm_systick_t   *)p_dev->p_devinfo->systick_regbase;

    if (pfn_callback == NULL) {
        amhw_arm_systick_int_disable(p_hw_systick);
    } else {
        p_dev->p_arg        = p_arg;
        p_dev->pfn_callback = pfn_callback;
        
        amhw_arm_systick_int_enable(p_hw_systick);
    }

    return AM_OK;
}

/*******************************************************************************
  Public functions
*******************************************************************************/

am_timer_handle_t  am_arm_systick_init (am_arm_systick_dev_t           *p_dev,
                                        const am_arm_systick_devinfo_t *p_devinfo)
{
    amhw_arm_systick_t *p_hw_systick = NULL;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_hw_systick              = (amhw_arm_systick_t *)p_devinfo->systick_regbase;
    p_dev->p_devinfo          = p_devinfo;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_systick_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev;
    p_dev->pfn_callback       = NULL;
    p_dev->p_arg              = NULL;

    __gp_systick_dev          = p_dev;

    /* 设置定时器时钟频率 */
    if ((p_devinfo->clk_freq_src != AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM) &&
        (p_devinfo->clk_freq_src != AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM_HALF)) {
        return NULL;
    } else {
        amhw_arm_systick_config(p_hw_systick, p_devinfo->clk_freq_src);
    }

    return &(p_dev->timer_serv);
}

/******************************************************************************/
void am_arm_systick_deinit (am_timer_handle_t handle)
{
    am_arm_systick_dev_t *p_dev        = NULL;
    amhw_arm_systick_t   *p_hw_systick = NULL;

    if (handle == NULL) {
        return ;
    }

    p_dev = (am_arm_systick_dev_t *)handle;
    
    p_hw_systick = (amhw_arm_systick_t *)p_dev->p_devinfo->systick_regbase;

    p_dev->timer_serv.p_funcs = NULL;
    p_dev->timer_serv.p_drv   = NULL;
    p_dev->pfn_callback       = NULL;
    p_dev->p_arg              = NULL;

    amhw_arm_systick_int_disable(p_hw_systick);
    amhw_arm_systick_disable(p_hw_systick);
}
