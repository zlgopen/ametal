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
 * \brief TPM驱动，输入捕获驱动层实现
 *
 * 1. TPM支持提供如下三种标准服务，本驱动提供的是服务"捕获"标准服务的驱动;
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. TPM0有6个通道可作为捕获功能，TPM1和TPM2各有2个通道可作为捕获功能.
 *
 * \internal
 * \par Modification history
 * - 1.01 18-03-23  sdq, 改正分频后转换两次捕获值的时间值是实际时间的两倍的问题。
 * - 1.00 16-09-13  sdy, first implementation.
 * \endinternal
 */

#include "am_fsl_tpm_cap.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_int.h"

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief 捕获参数配置  */
static int __fsl_tpm_cap_config (void              *p_cookie,
                                 int                chan,
                                 unsigned int       flags,
                                 am_cap_callback_t  pfn_callback,
                                 void              *p_arg);

/** \brief 使能捕获通道 */
static int __fsl_tpm_cap_enable (void *p_drv, int chan);

/** \brief 禁能捕获通道 */
static int __fsl_tpm_cap_disable (void *p_drv, int chan);
                                 
/** \brief 复位捕获通道计数值*/
static int __fsl_tpm_cap_reset (void *p_drv, int chan);

static int __fsl_tpm_cap_count_to_time (void         *p_drv,
                                        int           chan,
                                        unsigned int  count1,
                                        unsigned int  count2,
                                        unsigned int *p_time_ns);
                                    
static void __fsl_tpm_cap_irq_handler (void *p_arg);

/** \brief 捕获服务驱动函数 */
static const struct am_cap_drv_funcs __g_tpm_cap_drv_funcs = {
    __fsl_tpm_cap_config,
    __fsl_tpm_cap_enable,
    __fsl_tpm_cap_disable,
    __fsl_tpm_cap_reset,
    __fsl_tpm_cap_count_to_time,
};

/******************************************************************************/

/** \brief 配置一个输入捕获通道  */
static int __fsl_tpm_cap_config (void              *p_drv,
                                 int                chan,
                                 unsigned int       options,
                                 am_cap_callback_t  pfn_callback,
                                 void              *p_arg)
{
    am_fsl_tpm_cap_dev_t *p_dev    = (am_fsl_tpm_cap_dev_t *)p_drv;
    amhw_fsl_tpm_t       *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    
    /* 有效通道号范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /*  选择上升/下降沿触发 */
    if ((options & AM_CAP_TRIGGER_BOTH_EDGES) == AM_CAP_TRIGGER_BOTH_EDGES) {
        amhw_fsl_tpm_ch_mode(p_hw_tpm, chan, AMHW_FSL_TPM_COUNTING_UP,
                             AMHW_FSL_TPM_CHSC_MS(0) | AMHW_FSL_TPM_CHSC_EL_SEL(3));
    }

    /*  选择上升沿触发 */
    if ((options & AM_CAP_TRIGGER_RISE) == AM_CAP_TRIGGER_RISE) {
        amhw_fsl_tpm_ch_mode(p_hw_tpm, chan, AMHW_FSL_TPM_COUNTING_UP,
                             AMHW_FSL_TPM_CHSC_MS(0) | AMHW_FSL_TPM_CHSC_EL_SEL(1));
    }
    
    /*  选择下降沿触发 */
    if ((options & AM_CAP_TRIGGER_FALL) == AM_CAP_TRIGGER_FALL) {
        amhw_fsl_tpm_ch_mode(p_hw_tpm, chan, AMHW_FSL_TPM_COUNTING_UP,
                             AMHW_FSL_TPM_CHSC_MS(0) | AMHW_FSL_TPM_CHSC_EL_SEL(2));
    }
    
    p_dev->callback_info[chan].callback_func = pfn_callback;
    p_dev->callback_info[chan].p_arg         = p_arg;
    
    return AM_OK;
}

/** \brief 捕获使能 */
static int __fsl_tpm_cap_enable (void *p_drv, int chan)
{
    am_fsl_tpm_cap_dev_t    *p_dev    = (am_fsl_tpm_cap_dev_t *)p_drv;
    amhw_fsl_tpm_t          *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    am_fsl_tpm_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;
    
    /* 有效通道号范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /* 引脚配置 */
    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].func);

    /* 使能通道中断 */
    amhw_fsl_tpm_ch_sc_set(p_hw_tpm, chan, AMHW_FSL_TPM_CHSC_IE);
    
    /* 中断连接并使能 */
    am_int_connect(p_dev->p_devinfo->inum, __fsl_tpm_cap_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    /* 打开时钟，启动计数 */
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_MODULE);
    
    return AM_OK;
}

/** \brief 捕获禁能 */
static int __fsl_tpm_cap_disable (void *p_drv, int chan)
{
    am_fsl_tpm_cap_dev_t    *p_dev    = (am_fsl_tpm_cap_dev_t *)p_drv;
    amhw_fsl_tpm_t          *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    am_fsl_tpm_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;
 
    /* 有效通道号范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /* 还原GPIO配置 */
    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].dfunc);
    
    amhw_fsl_tpm_ch_sc_clear(p_hw_tpm, chan, AMHW_FSL_TPM_CHSC_IE);

    /* 关闭时钟，停止计数 */
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_NONE);
    
    return AM_OK;
}

/**
  * \brief 复位捕获通道计数值
  */
static int __fsl_tpm_cap_reset (void *p_drv, int chan)
{
    am_fsl_tpm_cap_dev_t *p_dev    = (am_fsl_tpm_cap_dev_t *)p_drv;
    amhw_fsl_tpm_t       *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    
    /* 停止TPM计数 */
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_NONE);
    
    return AM_OK;
}

/**
  * \brief 转换两次捕获值为时间值
  */
static int __fsl_tpm_cap_count_to_time (void         *p_drv,
                                        int           chan,
                                        unsigned int  count1,
                                        unsigned int  count2,
                                        unsigned int *p_time_ns)
{
    am_fsl_tpm_cap_dev_t *p_dev    = (am_fsl_tpm_cap_dev_t *)p_drv;
    amhw_fsl_tpm_t       *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;

    uint32_t clkfreq = am_clk_rate_get(p_dev->p_devinfo->clk_id);
    uint32_t count_err;
    uint64_t time_ns;
    uint8_t  reg_pre, pre;

    reg_pre = (uint8_t)amhw_tpm_prescale_get(p_hw_tpm);
    pre = 1 << reg_pre;
    
    count_err = count2 - count1;

    /* 将两次读取值的差转换成时间  */
    time_ns = (uint64_t)1000000000 * (uint64_t)count_err * pre / (uint64_t)clkfreq;
    
    *p_time_ns = time_ns;
    
    return AM_OK;
}

/**
  * \brief 中断服务函数
  */
static void __fsl_tpm_cap_irq_handler (void *p_arg)
{
    am_fsl_tpm_cap_dev_t *p_dev     = (am_fsl_tpm_cap_dev_t *)p_arg;
    amhw_fsl_tpm_t       *p_hw_tpm  = p_dev->p_devinfo->p_hw_tpm;
    uint8_t               chans_num = p_dev->p_devinfo->channels_num;
    
    uint8_t             i;
    am_cap_callback_t   callback_func;
    uint32_t            value;
    
    for (i = 0; i < chans_num; i++) {
        if (amhw_fsl_tpm_stat_flag_get(p_hw_tpm) & (AMHW_FSL_TPM_STAT_CH0_FLAG << i) ) {
            
            callback_func = p_dev->callback_info[i].callback_func;
            value = amhw_fsl_tpm_ch_value_get(p_hw_tpm, i);
            
            if (callback_func != NULL) {
                callback_func(p_dev->callback_info[i].p_arg, value);
            }
            
            /* 清除通道i标志 */
            amhw_fsl_tpm_stat_flag_clear(p_hw_tpm, (AMHW_FSL_TPM_STAT_CH0_FLAG << i));
        }
    }
}

/** 
  * \brief 捕获初始化
  */
void __fsl_tpm_cap_init (amhw_fsl_tpm_t *p_hw_tpm)
{
    /* 清零计数器 */
    amhw_fsl_tpm_count_clear(p_hw_tpm);
    amhw_fsl_tpm_prescale_set(p_hw_tpm, AMHW_FSL_TPM_DIVIDED_BY_1);
}

am_cap_handle_t am_fsl_tpm_cap_init (am_fsl_tpm_cap_dev_t           *p_dev,
                                     const am_fsl_tpm_cap_devinfo_t *p_devinfo)
{
    amhw_fsl_tpm_t *p_hw_tpm;
    uint8_t    i;
    
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }
    
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_hw_tpm                = p_devinfo->p_hw_tpm;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->cap_serv.p_funcs = (struct am_cap_drv_funcs *)&__g_tpm_cap_drv_funcs;
    p_dev->cap_serv.p_drv   = p_dev;
    
    for (i = 0; i < p_devinfo->channels_num; i++) {
        p_dev->callback_info[i].callback_func = NULL;
    }
    
    /* 捕获初始化 */
    __fsl_tpm_cap_init(p_hw_tpm);
    
    return &(p_dev->cap_serv);
}

void am_fsl_tpm_cap_deinit (am_cap_handle_t handle)
{
 
    am_fsl_tpm_cap_dev_t *p_dev    = (am_fsl_tpm_cap_dev_t *)handle;
    amhw_fsl_tpm_t       *p_hw_tpm = NULL;

    if (p_dev == NULL || p_dev->p_devinfo == NULL ) {
        return ;
    }

    p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;

    /* 关闭TPM模块 */
    amhw_fsl_tpm_clock_mode(p_hw_tpm, AMHW_FSL_TPM_CLK_SRC_NONE);
    
    am_int_disable(p_dev->p_devinfo->inum);

    p_dev->cap_serv.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
