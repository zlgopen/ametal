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
 * \brief TIM定时功能驱动层实现
 *
 * \note 当发生一个更新事件时，所有的寄存器都被更新，硬件同时（依据 URS 位）设
 * 置更新标志位（TIMx_SR寄存器中的 UIF 位）。重复计数器被重新加载为TIMx_RCR 寄存
 * 器的值,自动装载影子寄存器被重新置入预装载寄存器的值  预分频器的缓冲区被置入预
 * 装载寄存器的值（TIMx_PSC 寄存器的内容）
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-21  nwt, first implementation
 * \endinternal
 */
#include "am_int.h"
#include "am_clk.h"
#include "am_mm32_tim_timing.h"
#include "am_clk.h"

/*******************************************************************************
  定时器驱动函数声明
*******************************************************************************/

static const am_timer_info_t * __tim_timing_info_get (void *p_drv);

static int __tim_timing_clkin_freq_get (void *, uint32_t *);
static int __tim_timing_prescale_set (void *, uint8_t, uint32_t );
static int __tim_timing_prescale_get (void *, uint8_t, uint32_t *);
static int __tim_timing_count_get (void *, uint8_t, void *);
static int __tim_timing_rollover_get (void *, uint8_t, void *);
static int __tim_timing_disable (void *, uint8_t);
static int __tim_timing_enable (void *, uint8_t, void *);
static int __tim_timing_callback_set (void *, uint8_t, void (*)(void *), void *);

static void __tim_irq_handler (void *p_arg);

/*******************************************************************************
  全局变量
*******************************************************************************/

/* 定时器信息 */
static const am_timer_info_t __g_tim_timing_info = {
    16,                                      /* 16位定时器               */
    1,                                       /* 单通道                   */
    AM_TIMER_CAN_INTERRUPT      |            /* 可以产生中断             */
    AM_TIMER_AUTO_RELOAD        |            /* 支持自动重载             */
    AM_TIMER_INTERMEDIATE_COUNT |            /* 当前计数器值可读         */
    AM_TIMER_SUPPORT_ALL_PRESCALE_1_TO_MAX,  /* 预分频：1 ~ 65536 任意值 */
    65536                                    /* 预分频最大值：65536      */
};

/* TIM定时器驱动函数 */
static const struct am_timer_drv_funcs __g_tim_timing_drv_funcs = {
    __tim_timing_info_get,
    __tim_timing_clkin_freq_get,
    __tim_timing_prescale_set,
    __tim_timing_prescale_get,
    __tim_timing_count_get,
    __tim_timing_rollover_get,
    __tim_timing_enable,
    __tim_timing_disable,
    __tim_timing_callback_set,
};

/*******************************************************************************
  中断处理
*******************************************************************************/

void __tim_irq_handler (void *p_arg)
{
    am_mm32_tim_timing_dev_t *p_dev    = (am_mm32_tim_timing_dev_t *)p_arg;
    amhw_mm32_tim_t          *p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    /*
     * 判断是否是定时器溢出中断
     */
    if (amhw_mm32_tim_status_flg_get(p_hw_tim, AMHW_MM32_TIM_UIF) != 0 ) {

        if (p_dev->pfn_callback != NULL) {
            p_dev->pfn_callback(p_dev->p_arg);
        }

        /* 清除溢出标志 */
        amhw_mm32_tim_status_flg_clr(p_hw_tim, AMHW_MM32_TIM_UIF);
    }
}

/*******************************************************************************
  标准定时器驱动实现
*******************************************************************************/

static const am_timer_info_t * __tim_timing_info_get (void *p_drv)
{
    return &__g_tim_timing_info;
}

/******************************************************************************/
static int __tim_timing_clkin_freq_get (void *p_drv, uint32_t *p_freq)
{
    am_mm32_tim_timing_dev_t *p_dev = (am_mm32_tim_timing_dev_t *)p_drv;

    if ((p_dev == NULL) || (p_freq == NULL)) {
        return -AM_EINVAL;
    }

    /* 获取当前系统时钟频率，systick 使用的是系统时钟 */
    *p_freq =  am_clk_rate_get(p_dev->p_devinfo->clk_num);

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_prescale_set (void    *p_drv, 
                                      uint8_t  chan, 
                                      uint32_t prescale)
{
    am_mm32_tim_timing_dev_t *p_dev = (am_mm32_tim_timing_dev_t *)p_drv;
    amhw_mm32_tim_t          *p_hw_tim;

    if ((p_drv == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }

    p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    if ((prescale != 0) && (prescale <= 65536)) {
 
        amhw_mm32_tim_prescale_set(p_hw_tim, prescale - 1);  /* 设置分频值 */

        return AM_OK;
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
static int __tim_timing_prescale_get (void     *p_drv, 
                                      uint8_t   chan, 
                                      uint32_t *p_prescale)
{
    am_mm32_tim_timing_dev_t *p_dev = (am_mm32_tim_timing_dev_t *)p_drv;
    amhw_mm32_tim_t          *p_hw_tim;

    if ((p_dev == NULL) || (p_prescale == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 获取分频值 */
    *p_prescale = amhw_mm32_tim_prescale_get(p_hw_tim);

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_count_get (void *p_drv, uint8_t chan, void *p_count)
{
    am_mm32_tim_timing_dev_t *p_dev = (am_mm32_tim_timing_dev_t *)p_drv;
    amhw_mm32_tim_t          *p_hw_tim;

    if ((p_dev == NULL) || (p_count == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    *(uint32_t *)p_count = amhw_mm32_tim_count_get(p_hw_tim);

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_rollover_get (void *p_drv,uint8_t chan,void *p_rollover)
{
    am_mm32_tim_timing_dev_t *p_dev = (am_mm32_tim_timing_dev_t *)p_drv;
    amhw_mm32_tim_t          *p_hw_tim;

    if ((p_dev == NULL) || (p_rollover == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    *(uint32_t *)p_rollover = amhw_mm32_tim_arr_get(p_hw_tim);

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_disable (void *p_drv, uint8_t chan)
{
    am_mm32_tim_timing_dev_t *p_dev = (am_mm32_tim_timing_dev_t *)p_drv;
    amhw_mm32_tim_t          *p_hw_tim;
    
    if ((p_dev == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }
    
    p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 禁能更新中断 */
    amhw_mm32_tim_int_disable(p_hw_tim, AMHW_MM32_TIM_UIE);

    amhw_mm32_tim_disable(p_hw_tim);

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_enable (void *p_drv, uint8_t chan, void *p_count)
{
    am_mm32_tim_timing_dev_t *p_dev = (am_mm32_tim_timing_dev_t *)p_drv;
    amhw_mm32_tim_t          *p_hw_tim;

    if ((p_dev == NULL) || (chan != 0) || (p_count == NULL)) {
        return -AM_EINVAL;
    }

    p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 禁止定时器产生中断 */
    amhw_mm32_tim_int_disable(p_hw_tim, AMHW_MM32_TIM_UIE);

    /* 设置自动重装寄存器的值 */
    amhw_mm32_tim_arr_set(p_hw_tim, *((uint32_t *)p_count));

    /* 计数器清0 */
    amhw_mm32_tim_count_set(p_hw_tim, 0);

    /* 产生更新事件，重新初始化Prescaler计数器 及Repetition计数器 */
    amhw_mm32_tim_egr_set(p_hw_tim, AMHW_MM32_TIM_UG);

    /* 更新定时器时会产生更新事件,清除标志位 */
    amhw_mm32_tim_status_flg_clr(p_hw_tim, AMHW_MM32_TIM_UG);

    if (p_dev->pfn_callback != NULL) {

        /* 允许更新中断 */
        amhw_mm32_tim_int_enable(p_hw_tim, AMHW_MM32_TIM_UIE);
    }

    /* 使能定时器TIM允许计数 */
    amhw_mm32_tim_enable(p_hw_tim);

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_callback_set (void     *p_drv,
                                      uint8_t   chan,
                                      void    (*pfn_callback)(void *),
                                      void     *p_arg)
{
    am_mm32_tim_timing_dev_t *p_dev = (am_mm32_tim_timing_dev_t *)p_drv;
    amhw_mm32_tim_t          *p_hw_tim;

    if ((p_dev == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 不需要开启中断 */
    if (pfn_callback == NULL) {
        amhw_mm32_tim_int_disable(p_hw_tim, AMHW_MM32_TIM_UIE);
    } else {
        p_dev->pfn_callback = pfn_callback;
        p_dev->p_arg   = p_arg;
    }

    return AM_OK;
}

/*******************************************************************************
  Public functions
*******************************************************************************/

am_timer_handle_t am_mm32_tim_timing_init (
    am_mm32_tim_timing_dev_t             *p_dev,
    const am_mm32_tim_timing_devinfo_t  *p_devinfo)
{
    amhw_mm32_tim_t *p_hw_tim = NULL;

    if (NULL == p_dev || NULL == p_devinfo ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo          = p_devinfo;
    p_hw_tim                  = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_tim_timing_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev;

    p_dev->pfn_callback = NULL;
    p_dev->p_arg        = NULL;

    if ((p_devinfo->tim_type == AMHW_MM32_TIM_TYPE0) ||
        (p_devinfo->tim_type == AMHW_MM32_TIM_TYPE1))  {

        /* 边沿对齐模式 */
        amhw_mm32_tim_cms_set(p_hw_tim, 0);

        /* 向上计数 */
        amhw_mm32_tim_dir_set(p_hw_tim, 0);
    }

    /* 设置时钟分割:TDTS = Tck_tin */
    amhw_mm32_tim_ckd_set(p_hw_tim, 0);

    /* 允许更新事件 */
    amhw_mm32_tim_udis_enable(p_hw_tim);

    am_int_connect(p_dev->p_devinfo->inum, __tim_irq_handler, (void *)p_dev);

    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->timer_serv);
}

/******************************************************************************/
void am_mm32_tim_timing_deinit (am_timer_handle_t handle)
{

    am_mm32_tim_timing_dev_t *p_dev    = (am_mm32_tim_timing_dev_t *)handle;
    amhw_mm32_tim_t          *p_hw_tim = NULL;

    if (p_dev == NULL || p_dev->p_devinfo == NULL ) {
        return ;
    }

    p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 关闭更新中断 */
    amhw_mm32_tim_int_disable(p_hw_tim, AMHW_MM32_TIM_UIE);

    /* 关闭时钟，停止计数 */
    amhw_mm32_tim_disable(p_hw_tim);

    am_int_disconnect(p_dev->p_devinfo->inum, __tim_irq_handler, (void *)p_dev);
    am_int_disable(p_dev->p_devinfo->inum);

    p_dev->timer_serv.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
