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
 * \brief PIT 定时功能驱动层实现
 *
 * 1. PIT周期性中断定时器，本驱动实现其"定时"服务
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-18  sdy, first implementation.
 * \endinternal
 */

#include "am_fsl_pit_timing.h"
#include "am_int.h"
#include "am_clk.h"

/*******************************************************************************
  定时器驱动函数声明
*******************************************************************************/

static const am_timer_info_t *__pit_timing_info_get (void *p_drv);

static int __pit_timing_clkin_freq_get (void *p_drv, uint32_t *p_freq);

static int __pit_timing_prescale_set (void    *p_drv,
                                      uint8_t  chan,
                                      uint32_t prescale);

static int __pit_timing_prescale_get (void    *p_drv,
                                      uint8_t  chan,
                                      uint32_t *p_prescale);

static int __pit_timing_count_get (void     *p_drv,
                                   uint8_t   chan,
                                   void     *p_count);

static int __pit_timing_rollover_get (void     *p_drv,
                                      uint8_t   chan,
                                      void     *p_rollover);

static int __pit_timing_disable (void *p_drv, uint8_t chan);

static int __pit_timing_enable (void     *p_drv,
                                uint8_t   chan,
                                void     *p_count);

static int __pit_timing_callback_set (void    *p_drv,
                                      uint8_t  chan,
                                      void   (*pfn_callback)(void *),
                                      void    *p_arg);

static void __pit_irq_handler (void *p_arg);

static int __pit_timing_count_get64 (void     *p_drv,
                                     uint8_t   chan,
                                     uint64_t *p_count);
static int __pit_timing_rollover_get64 (void     *p_drv,
                                        uint8_t   chan,
                                        uint64_t *p_rollover);

static int __pit_timing_enable64 (void     *p_drv,
                                  uint8_t   chan,
                                  uint64_t  max_timer_count);

/*******************************************************************************
  全局变量
*******************************************************************************/

/* 定时器信息 */
static am_timer_info_t __g_pit_timing_info = {
    32,                                      /* 32位定时器               */
    2,                                       /* 双通道                   */
    AM_TIMER_CAN_INTERRUPT      |            /* 可以产生中断             */
    AM_TIMER_AUTO_RELOAD        |            /* 支持自动重载             */
    AM_TIMER_INTERMEDIATE_COUNT,             /* 当前计数器值可读         */
    0                                        /* 不支持预分频      */
};

/** \brief SCT for timing driver functions */
static const struct am_timer_drv_funcs __g_pit_timing_drv_funcs = {
    __pit_timing_info_get,
    __pit_timing_clkin_freq_get,
    __pit_timing_prescale_set,
    __pit_timing_prescale_get,
    __pit_timing_count_get,
    __pit_timing_rollover_get,
    __pit_timing_enable,
    __pit_timing_disable, 
    __pit_timing_callback_set,
};

/******************************************************************************/

void __pit_irq_handler (void *p_arg)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)p_arg;
    amhw_fsl_pit_t          *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    /*
     * 判断是否是定时器溢出中断
     */
    if (amhw_fsl_pit_timerout_flag_check(p_hw_pit, AMHW_FSL_PIT_CH(0))) {
        if (p_dev->pfn_ch1_callback != NULL) {
            p_dev->pfn_ch1_callback(p_dev->p_arg_ch1);
            amhw_fsl_pit_timerout_flag_clr(p_hw_pit, 0);
        }
    }
    if(amhw_fsl_pit_timerout_flag_check(p_hw_pit, AMHW_FSL_PIT_CH(1))) {
        if (p_dev->pfn_ch2_callback != NULL) {
            p_dev->pfn_ch2_callback(p_dev->p_arg_ch2);
            amhw_fsl_pit_timerout_flag_clr(p_hw_pit, 1);
        }
    }
}

/** \brief 获取PIT信息 */
static const am_timer_info_t * __pit_timing_info_get (void *p_drv)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)p_drv;

    if (p_dev->p_devinfo->flag == AM_FSL_PIT_TIMING_1_64BIT) {
        __g_pit_timing_info.counter_width = 64;
        __g_pit_timing_info.chan_num      = 1;
    }
    return &__g_pit_timing_info;
}

/** \brief 获取PIT时钟频率信息 */
static int __pit_timing_clkin_freq_get (void *p_drv, uint32_t *p_freq)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)p_drv;

    if ((p_dev == NULL) || (p_freq == NULL)) {
        return -AM_EINVAL;
    }

    /* 获取当前系统时钟频率，systick 使用的是系统时钟 */
    *p_freq =  am_clk_rate_get(p_dev->p_devinfo->clk_id);

    return AM_OK;
}

/******************************************************************************/
static int __pit_timing_prescale_set (void *p_drv, uint8_t chan, uint32_t prescale)
{
    if (prescale == 1) {
        return AM_OK;
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
static int __pit_timing_prescale_get (void     *p_drv,
                                      uint8_t   chan,
                                      uint32_t *p_prescale)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)p_drv;

    if ((p_dev == NULL) || (p_prescale == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }

    /* 无分频器，分频值始终为1  */
    *p_prescale = 1;

    return AM_OK;
}

/**
 * \brief 获取PIT当前计数值
 */
static int __pit_timing_count_get (void     *p_drv,
                                   uint8_t   chan,
                                   void     *p_count)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)p_drv;
    amhw_fsl_pit_t          *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    if (p_dev->p_devinfo->flag == AM_FSL_PIT_TIMING_1_64BIT) {
        __pit_timing_count_get64(p_drv, chan, (uint64_t *)p_count);
        return AM_OK;
    }
    *(uint32_t *)p_count = amhw_fsl_pit_cval_get(p_hw_pit, AMHW_FSL_PIT_CH(chan));

    return AM_OK;
}

/**
 * \brief 获取PIT翻滚值
 */
static int __pit_timing_rollover_get (void     *p_drv,
                                      uint8_t   chan,
                                      void     *p_rollover)
{
    am_fsl_pit_timing_dev_t *p_dev = (am_fsl_pit_timing_dev_t *)p_drv;

    if (p_dev->p_devinfo->flag == AM_FSL_PIT_TIMING_1_64BIT) {
        __pit_timing_rollover_get64(p_drv, chan, (uint64_t *)p_rollover);
        return AM_OK;
    }

    p_dev = (am_fsl_pit_timing_dev_t *)p_drv;

    *(uint32_t *)p_rollover = 0xFFFFFFFFu / am_clk_rate_get(p_dev->p_devinfo->clk_id);

    return AM_OK;
}

/**
 * \brief PIT禁能
 */
static int __pit_timing_disable (void *p_drv, uint8_t chan)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)p_drv;
    amhw_fsl_pit_t          *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    /* 清零使能位 */
    amhw_fsl_pit_timer_ctrl_clear(p_hw_pit,
                                  AMHW_FSL_PIT_CH(chan),
                                  AMHW_FSL_PIT_TCTL_TMR_EN);

    return AM_OK;
}

/**
 * \brief PIT使能并设置计数值
 */
static int __pit_timing_enable (void     *p_drv,
                                uint8_t   chan,
                                void     *p_count)
{
    am_fsl_pit_timing_dev_t *p_dev         = (am_fsl_pit_timing_dev_t *)p_drv;
    amhw_fsl_pit_t          *p_hw_pit      = p_dev->p_devinfo->p_hw_pit;
    uint32_t                 max_pit_count = *(uint32_t *)p_count;

    /* 判断定时器位数是否为64位 */
    if (p_dev->p_devinfo->flag == AM_FSL_PIT_TIMING_1_64BIT) {
        __pit_timing_enable64(p_drv, chan, max_pit_count);
        return AM_OK;
    }

    amhw_fsl_pit_module_enable(p_hw_pit);           /* 打开PIT模块 */

    /* 设置装载值 */
    amhw_fsl_pit_ldval_set(p_hw_pit, AMHW_FSL_PIT_CH(chan), max_pit_count);

    /* 打开中断并使能通道 */
    amhw_fsl_pit_timer_ctrl_set(p_hw_pit,
                                AMHW_FSL_PIT_CH(chan),
                                AMHW_FSL_PIT_TCTL_TI_EN | AMHW_FSL_PIT_TCTL_TMR_EN);

    return AM_OK;
}

/**
 * \brief 设置中断回调函数
 */
static int __pit_timing_callback_set (void     *p_drv,
                                      uint8_t   chan,
                                      void    (*pfn_callback)(void *),
                                      void     *p_arg)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)p_drv;
    amhw_fsl_pit_t          *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;


    /* 不需要开启中断 */
    if (pfn_callback == NULL) {
        amhw_fsl_pit_timer_ctrl_clear(p_hw_pit,
                                      AMHW_FSL_PIT_CH(chan),
                                      AMHW_FSL_PIT_TCTL_TI_EN);
        return AM_OK;
    }

    if(chan == 0) {
        p_dev->pfn_ch1_callback = pfn_callback;
        p_dev->p_arg_ch1   = p_arg;
        if (p_dev->p_devinfo->flag == AM_FSL_PIT_TIMING_2_32BIT) {
            amhw_fsl_pit_timer_ctrl_set(p_hw_pit,
                                        AMHW_FSL_PIT_CH(chan),
                                        AMHW_FSL_PIT_TCTL_TI_EN);
        }
    } 
    
    if(chan == 1) {
        p_dev->pfn_ch2_callback = pfn_callback;
        p_dev->p_arg_ch2   = p_arg;
        if (p_dev->p_devinfo->flag == AM_FSL_PIT_TIMING_2_32BIT) {
            amhw_fsl_pit_timer_ctrl_set(p_hw_pit,
                                        AMHW_FSL_PIT_CH(chan),
                                        AMHW_FSL_PIT_TCTL_TI_EN);
        }
    }

    return AM_OK;
}

/**
 * \brief 64位定时器计数值获取
 */
static int __pit_timing_count_get64 (void     *p_drv,
                                     uint8_t   chan,
                                     uint64_t *p_count)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)p_drv;
    amhw_fsl_pit_t          *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    *p_count = amhw_fsl_pit_ltmr64h_get(p_hw_pit);         /* 必须先获取高32位 */
    *p_count = (*p_count << 32) | amhw_fsl_pit_ltmr64l_get(p_hw_pit);

    return AM_OK;
}

static int __pit_timing_rollover_get64 (void     *p_drv,
                                        uint8_t   chan,
                                        uint64_t *p_rollover)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)p_drv;

    *p_rollover = (~(uint64_t)0) / am_clk_rate_get(p_dev->p_devinfo->clk_id);

    return 0;
}

static int __pit_timing_enable64 (void    *p_drv,
                                  uint8_t  chan,
                                  uint64_t max_timer_count)
{
    am_fsl_pit_timing_dev_t *p_dev;
    amhw_fsl_pit_t          *p_hw_pit;

    uint32_t high_val, low_val;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_dev    = (am_fsl_pit_timing_dev_t *)p_drv;
    p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    high_val = max_timer_count >> 32;
    low_val  = max_timer_count / (high_val + 1) - 1;

    amhw_fsl_pit_module_enable(p_hw_pit);
    amhw_fsl_pit_ldval_set(p_hw_pit, 1, high_val);

    /* 使能连接模式，使能中断，开始计数 */
    amhw_fsl_pit_timer_ctrl_set(p_hw_pit, 1, AMHW_FSL_PIT_TCTL_MASK);

    amhw_fsl_pit_ldval_set(p_hw_pit, 0, low_val);
    
    amhw_fsl_pit_timer_ctrl_clear(
                        p_hw_pit,
                        0,
                        AMHW_FSL_PIT_TCTL_TI_EN | AMHW_FSL_PIT_TCTL_CHAIN_MODE);

    /* 不能开启中断 */
    amhw_fsl_pit_timer_ctrl_set(p_hw_pit, 0, AMHW_FSL_PIT_TCTL_TMR_EN);

    return AM_OK;
}

/**
 * \brief PIT初始化
 */
am_timer_handle_t am_fsl_pit_timing_init (am_fsl_pit_timing_dev_t           *p_dev,
                                          const am_fsl_pit_timing_devinfo_t *p_devinfo)
{
    if (NULL == p_dev || NULL == p_devinfo ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo          = p_devinfo;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_pit_timing_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev;

    p_dev->pfn_ch1_callback = NULL;
    p_dev->p_arg_ch1        = NULL;
    
    p_dev->pfn_ch2_callback = NULL;
    p_dev->p_arg_ch2        = NULL;

    if (p_devinfo->flag == AM_FSL_PIT_TIMING_2_32BIT) {

    } else {

    }

    am_int_connect(p_dev->p_devinfo->inum, __pit_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->timer_serv);
}

/**
 * \brief PIT解初始化
 */
void am_fsl_pit_timing_deinit (am_timer_handle_t handle)
{
    am_fsl_pit_timing_dev_t *p_dev    = (am_fsl_pit_timing_dev_t *)handle;
    amhw_fsl_pit_t          *p_hw_pit = NULL;

    if (p_dev == NULL || p_dev->p_devinfo == NULL ) {
        return ;
    }

    p_hw_pit = p_dev->p_devinfo->p_hw_pit;

    /* 关闭PIT模块 */
    amhw_fsl_pit_module_disable(p_hw_pit);

    am_int_disable(p_dev->p_devinfo->inum);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
