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
 * \brief TPM 定时功能驱动层实现
 *
 * 1. TPM支持提供如下三种标准服务，本驱动提供的是服务"定时"标准服务的驱动
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. 一个TPM模块，只有一个定时通道
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-13  sdy, first implementation.
 * \endinternal
 */

#include "am_fsl_tpm_timing.h"
#include "am_int.h"
#include "am_clk.h"

/*******************************************************************************
  定时器驱动函数声明
*******************************************************************************/

static const am_timer_info_t *__tpm_timing_info_get (void *p_drv);

static int __tmp_timing_clkin_freq_get (void *p_drv, uint32_t *p_freq);

static int __tpm_timing_count_get (void     *p_drv,
                                   uint8_t   chan,
                                   void     *p_count);

static int __tpm_timing_prescale_set (void    *p_drv,
                                      uint8_t  chan,
                                      uint32_t prescale);

static int __tpm_timing_prescale_get (void     *p_drv,
                                      uint8_t   chan,
                                      uint32_t *p_prescale);

static int __tpm_timing_rollover_get (void     *p_drv,
                                      uint8_t   chan,
                                      void     *p_rollover);

static int __tpm_timing_disable (void *p_drv, uint8_t chan);

static int __tpm_timing_enable (void     *p_drv,
                                uint8_t   chan,
                                void     *p_count);

static int __tpm_timing_callback_set (void    *p_drv,
                                      uint8_t  chan,
                                      void   (*pfn_callback)(void *),
                                      void    *p_arg);

static void __tpm_irq_handler (void *p_arg);

/*******************************************************************************
  全局变量
*******************************************************************************/

/* 定时器信息 */
static const am_timer_info_t __g_tpm_timing_info = {
    16,                                      /* 16位定时器               */
    1,                                       /* 单通道                   */
    AM_TIMER_CAN_INTERRUPT      |            /* 可以产生中断 */
    AM_TIMER_INTERMEDIATE_COUNT |            /* 可以实时读出计数值 */
    AM_TIMER_AUTO_RELOAD,                    /* 自动装载 */
    128                                      /* 预分频最大值：128      */
};

/** \brief TPM定时器驱动函数 */
static const struct am_timer_drv_funcs __g_tpm_timing_drv_funcs = {
    __tpm_timing_info_get,
    __tmp_timing_clkin_freq_get,
    __tpm_timing_prescale_set,
    __tpm_timing_prescale_get,
    __tpm_timing_count_get,
    __tpm_timing_rollover_get,
    __tpm_timing_enable,
    __tpm_timing_disable,
    __tpm_timing_callback_set,
};

/******************************************************************************/

void __tpm_irq_handler (void *p_arg)
{
    am_fsl_tpm_timing_dev_t *p_dev    = (am_fsl_tpm_timing_dev_t *)p_arg;
    amhw_fsl_tpm_t          *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;

    /*
     * 判断是否是定时器溢出中断
     */
    if (amhw_fsl_tpm_stat_flag_get(p_hw_tpm) & AMHW_FSL_TPM_STAT_OV_FLAG ) {

        if (p_dev->pfn_callback != NULL) {
            p_dev->pfn_callback(p_dev->p_arg);
        }

        /* 清除溢出标志 */
        amhw_fsl_tpm_stat_flag_clear(p_hw_tpm, AMHW_FSL_TPM_STAT_OV_FLAG);
    }
}

/** \brief 获取TPM信息 */
static const am_timer_info_t *__tpm_timing_info_get (void *p_drv)
{
    return &__g_tpm_timing_info;
}

/** \brief 获取TPM时钟频率信息 */
static int __tmp_timing_clkin_freq_get (void *p_drv, uint32_t *p_freq)
{
    am_fsl_tpm_timing_dev_t *p_dev    = (am_fsl_tpm_timing_dev_t *)p_drv;

    if ((p_dev == NULL) || (p_freq == NULL)) {
        return -AM_EINVAL;
    }

    /* 获取当前系统时钟频率，systick 使用的是系统时钟 */
    *p_freq =  am_clk_rate_get(p_dev->p_devinfo->clk_id);

    return AM_OK;
}

/**
 * \brief 获取TPM当前计数值
 */
static int __tpm_timing_count_get (void     *p_drv,
                                   uint8_t   chan,
                                   void     *p_count)
{
    am_fsl_tpm_timing_dev_t *p_dev    = (am_fsl_tpm_timing_dev_t *)p_drv;
    amhw_fsl_tpm_t          *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;

    *(uint32_t *)p_count = amhw_fsl_tpm_count_get(p_hw_tpm);

    return AM_OK;
}

/******************************************************************************/
static int __tpm_timing_prescale_set (void    *p_drv,
                                      uint8_t  chan,
                                      uint32_t prescale)
{
    am_fsl_tpm_timing_dev_t *p_dev      = (am_fsl_tpm_timing_dev_t *)p_drv;
    uint8_t                  pre_reg    = 0;
    amhw_fsl_tpm_t          *p_hw_tpm;

    if ((p_drv == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }

    p_hw_tpm = (amhw_fsl_tpm_t *)p_dev->p_devinfo->p_hw_tpm;

    if ((prescale != 0) && (prescale <= 65536)) {

        /* 只支持分频大小1,2,4,8...128，求得大于分频数中最小的数2^n */
        for (pre_reg = 0; pre_reg < 8; pre_reg++) {
            if ((prescale >> pre_reg) == 1) {
                break;
            }
        }

        amhw_fsl_tpm_prescale_set(p_hw_tpm, (amhw_fsl_tpm_prescale_t)(pre_reg));

        return AM_OK;
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
static int __tpm_timing_prescale_get (void     *p_drv,
                                      uint8_t   chan,
                                      uint32_t *p_prescale)
{
    am_fsl_tpm_timing_dev_t *p_dev      = (am_fsl_tpm_timing_dev_t *)p_drv;
    amhw_fsl_tpm_t          *p_hw_tim;
    amhw_fsl_tpm_prescale_t  prescale;

    if ((p_dev == NULL) || (p_prescale == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }

    p_hw_tim = (amhw_fsl_tpm_t *)p_dev->p_devinfo->p_hw_tpm;

    /* 获取分频值 */
    prescale = amhw_tpm_prescale_get(p_hw_tim);

    *p_prescale = (uint32_t)1 << prescale;

    return AM_OK;
}

/**
 * \brief 获取TPM翻滚值
 */
static int __tpm_timing_rollover_get (void     *p_drv,
                                      uint8_t   chan,
                                      void     *p_rollover)
{
    am_fsl_tpm_timing_dev_t *p_dev    = (am_fsl_tpm_timing_dev_t *)p_drv;

    *(uint32_t *)p_rollover = 0xFFFF / am_clk_rate_get(p_dev->p_devinfo->clk_id);


    return AM_OK;
}

/**
 * \brief TPM禁能
 */
static int __tpm_timing_disable (void *p_drv, uint8_t chan)
{
    am_fsl_tpm_timing_dev_t *p_dev    = (am_fsl_tpm_timing_dev_t *)p_drv;
    amhw_fsl_tpm_t          *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;

    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_NONE);

    return AM_OK;
}

/**
 * \brief TPM使能并设置计数值
 */
static int __tpm_timing_enable (void     *p_drv,
                                uint8_t   chan,
                                void     *p_count)
{
    am_fsl_tpm_timing_dev_t *p_dev    = (am_fsl_tpm_timing_dev_t *)p_drv;
    amhw_fsl_tpm_t          *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;

    uint32_t match, temp;
    uint8_t  pre_real, pre_reg = 0;
    uint32_t tpm_count     = *(uint32_t *)p_count;

    /* 16位计数器最大值65535,分频器最大值128 */
    if (tpm_count > (0xFFFFu * 128)) {
        return -AM_EINVAL;
    }

    temp = tpm_count / 0xFFFFu + 1;

    /* 只支持分频大小1,2,4,8...128，求得大于分频数中最小的数2^n */
    for (pre_real = 1; pre_real < temp; ) {
        pre_reg++;                   /* 计算写入寄存器的分频值0,1,2,... */
        pre_real = pre_real << 1;           /* 分频数2^n */
    }
    match = tpm_count / pre_real ;

    if (pre_real > 1) {    /* 需要分频 */
        amhw_fsl_tpm_prescale_set(p_hw_tpm, (amhw_fsl_tpm_prescale_t)(pre_reg));
    }

    amhw_fsl_tpm_count_clear(p_hw_tpm);
    amhw_fsl_tpm_mod_set(p_hw_tpm, match - 1);
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_MODULE);

    return AM_OK;
}

/**
 * \brief 设置中断回调函数
 */
static int __tpm_timing_callback_set (void     *p_drv,
                                      uint8_t   chan,
                                      void    (*pfn_callback)(void *),
                                      void     *p_arg)
{
    am_fsl_tpm_timing_dev_t *p_dev    = (am_fsl_tpm_timing_dev_t *)p_drv;
    amhw_fsl_tpm_t          *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;


    /* 不需要开启中断 */
    if (pfn_callback == NULL) {
        amhw_fsl_tpm_ctrl_clear(p_hw_tpm, AMHW_FSL_TPM_SC_TOI_EN);

    } else {
        p_dev->pfn_callback = pfn_callback;
        p_dev->p_arg   = p_arg;

        amhw_fsl_tpm_ctrl_set(p_hw_tpm, AMHW_FSL_TPM_SC_TOI_EN);
    }

    return AM_OK;
}

/**
 * \brief TPM初始化
 */
am_timer_handle_t
am_fsl_tpm_timing_init (am_fsl_tpm_timing_dev_t           *p_dev,
                        const am_fsl_tpm_timing_devinfo_t *p_devinfo)
{

    if (NULL == p_dev || NULL == p_devinfo ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo          = p_devinfo;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_tpm_timing_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev;

    p_dev->pfn_callback = NULL;
    p_dev->p_arg        = NULL;

    am_int_connect(p_dev->p_devinfo->inum, __tpm_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->timer_serv);
}

/**
 * \brief TPM解初始化
 */
void am_fsl_tpm_timing_deinit (am_timer_handle_t handle)
{

    am_fsl_tpm_timing_dev_t *p_dev    = (am_fsl_tpm_timing_dev_t *)handle;
    amhw_fsl_tpm_t          *p_hw_tpm = NULL;

    if (p_dev == NULL || p_dev->p_devinfo == NULL ) {
        return ;
    }

    p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;

    /* 关闭时钟，停止计数 */
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_NONE);

    am_int_disable(p_dev->p_devinfo->inum);

    p_dev->timer_serv.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
