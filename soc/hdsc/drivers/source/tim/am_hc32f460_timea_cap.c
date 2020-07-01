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
 * \brief TIM驱动，输入捕获驱动层实现
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-12  zp, first implementation
 * \endinternal
 */

#include "hc32f460_clk.h"
#include "am_hc32f460_timea_cap.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_clk.h"
#include "hc32f460_irq_handle.h"
#include "hc32f460_intctrl.h"
#include "am_vdebug.h"
/*******************************************************************************
* 私有定义
*******************************************************************************/
static uint16_t  __update_num = 0;

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief 捕获参数配置 */
static int __hc32f460_timea_cap_config (void              *p_cookie,
                                        int                chan,
                                        unsigned int       flags,
                                        am_cap_callback_t  pfn_callback,
                                        void              *p_arg);

/** \brief 使能捕获通道 */
static int __hc32f460_timea_cap_enable (void *p_drv, int chan);

/** \brief 禁能捕获通道 */
static int __hc32f460_timea_cap_disable (void *p_drv, int chan);

/** \brief 复位捕获通道计数值 */
static int __hc32f460_timea_cap_reset (void *p_drv, int chan);

static int __hc32f460_timea_cap_count_to_time (void         *p_drv,
                                               int           chan,
                                               unsigned int  count1,
                                               unsigned int  count2,
                                               unsigned int *p_time_ns);

static void __hc32f460_timea_cap_irq_handler (void *p_arg);

/** \brief 捕获服务驱动函数 */
static const struct am_cap_drv_funcs __g_tim_cap_drv_funcs = {
    __hc32f460_timea_cap_config,
    __hc32f460_timea_cap_enable,
    __hc32f460_timea_cap_disable,
    __hc32f460_timea_cap_reset,
    __hc32f460_timea_cap_count_to_time,
};

/******************************************************************************/

/** \brief 配置一个输入捕获通道 */
static int __hc32f460_timea_cap_config (void              *p_drv,
                                        int                chan,
                                        unsigned int       options,
                                        am_cap_callback_t  pfn_callback,
                                        void              *p_arg)
{
    am_hc32f460_timea_cap_dev_t *p_dev    = (am_hc32f460_timea_cap_dev_t *)p_drv;
    amhw_hc32f460_timea_t       *p_hw_tim = (amhw_hc32f460_timea_t *)p_dev->p_devinfo->tim_regbase;
    stc_timera_capture_init_t    stcTimeraCaptureInit;


    /* 有效通道号范围 0 ~ (chan_max_num - 1) */
    if (chan >= p_dev->chan_max) {
        return -AM_EINVAL;
    }

    /* 分频系数设置 */
    amhw_hc32f460_timea_mode_clkdiv_set(p_hw_tim, chan, p_dev->p_devinfo->clk_div);

    /*  选择上升沿触发 */
    if ((options & AM_CAP_TRIGGER_RISE) == AM_CAP_TRIGGER_RISE) {
        stcTimeraCaptureInit.enCapturePwmRisingEn = Enable;
        stcTimeraCaptureInit.enCapturePwmFallingEn = Disable;
    }

    /*  选择下降沿触发 */
    if ((options & AM_CAP_TRIGGER_FALL) == AM_CAP_TRIGGER_FALL) {
        stcTimeraCaptureInit.enCapturePwmRisingEn = Disable;
        stcTimeraCaptureInit.enCapturePwmFallingEn = Enable;
    }

    if((options & AM_CAP_TRIGGER_BOTH_EDGES) == AM_CAP_TRIGGER_BOTH_EDGES ) {
        stcTimeraCaptureInit.enCapturePwmRisingEn = Enable;
        stcTimeraCaptureInit.enCapturePwmFallingEn = Enable;
    }

    /* Configuration timera unit 1 capture structure */
    stcTimeraCaptureInit.enCaptureSpecifyEventEn = Disable;
    stcTimeraCaptureInit.enPwmClkDiv = TimeraFilterPclkDiv1;
    stcTimeraCaptureInit.enPwmFilterEn = Disable;
    stcTimeraCaptureInit.enCaptureTrigRisingEn = Disable;
    stcTimeraCaptureInit.enCaptureTrigFallingEn = Disable;
    stcTimeraCaptureInit.enTrigClkDiv = TimeraFilterPclkDiv1;
    stcTimeraCaptureInit.enTrigFilterEn = Disable;
    /* Enable channel 1 capture and interrupt */
    amhw_hc32f460_timea_capture_init(p_hw_tim,
                                     (timea_channel_t)chan,
                                     &stcTimeraCaptureInit);
    
    p_dev->callback_info[chan].callback_func = pfn_callback;
    p_dev->callback_info[chan].p_arg         = p_arg;

    return AM_OK;
}

/** \brief 捕获使能 */
static int __hc32f460_timea_cap_enable (void *p_drv, int chan)
{
    int i = 0, enable_flag = 0;
    am_hc32f460_timea_cap_dev_t    *p_dev    = (am_hc32f460_timea_cap_dev_t *)p_drv;
    amhw_hc32f460_timea_t          *p_hw_tim = (amhw_hc32f460_timea_t *)p_dev->p_devinfo->tim_regbase;
    am_hc32f460_timea_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;

    /* 有效通道号范围 0 ~ (chan_max_num - 1) */
    if (chan >= p_dev->chan_max) {
        return -AM_EINVAL;
    }

    /* 引脚配置 */
    for(i = 0; i < p_dev->p_devinfo->chan_max_num; i++) {
        if((p_ioinfo[i].channel) ==  chan){
            am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].func);
            enable_flag = 1;
            break;
        }
    }

    /* 输入通道号无效 */
    if(enable_flag == 0){
        return -AM_EINVAL;
    }

    am_int_connect(p_dev->p_devinfo->inum, IRQ136_Handler, NULL);
    if ((amhw_hc32f460_timea_t *)HC32F460_TMRA1_BASE == p_hw_tim)
    {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (TMRA_CMP) << 0);
    }else if ((amhw_hc32f460_timea_t *)HC32F460_TMRA2_BASE == p_hw_tim)
    {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (TMRA_CMP) << 3);
    }else if ((amhw_hc32f460_timea_t *)HC32F460_TMRA3_BASE == p_hw_tim)
    {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (TMRA_CMP) << 6);
    }else if ((amhw_hc32f460_timea_t *)HC32F460_TMRA4_BASE == p_hw_tim)
    {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (TMRA_CMP) << 9);
    }else if ((amhw_hc32f460_timea_t *)HC32F460_TMRA5_BASE == p_hw_tim)
    {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (TMRA_CMP) << 12);
    }else if ((amhw_hc32f460_timea_t *)HC32F460_TMRA6_BASE == p_hw_tim)
    {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (TMRA_CMP) << 16);
    }else {
        ;
    }       
    
    am_int_enable(p_dev->p_devinfo->inum);

    /* 设置自动重装寄存器的值 */
    amhw_hc32f460_timea_arr_count_set(p_hw_tim, chan, 0xFFFF - 1);


    /* 清零计数器 */
    amhw_hc32f460_timea_cnt16_count_set(p_hw_tim, 0);

    amhw_hc32f460_timea_int_enable(p_hw_tim, (timea_irq_type_t)chan);
    amhw_hc32f460_timea_int_enable(p_hw_tim, TIMERA_IRQ_OVERFLOW);

    amhw_hc32f460_timea_enable(p_hw_tim, chan);

    amhw_hc32f460_timea_int_flag_clr(p_hw_tim, (timea_flag_type_t)chan);
    amhw_hc32f460_timea_int_flag_clr(p_hw_tim, TIMEA_FLAG_OVERFLOW);
    
    return AM_OK;
}

/** \brief 捕获禁能 */
static int __hc32f460_timea_cap_disable (void *p_drv, int chan)
{
    int i = 0, disable_flag = 0;
    am_hc32f460_timea_cap_dev_t    *p_dev    = (am_hc32f460_timea_cap_dev_t *)p_drv;
    amhw_hc32f460_timea_t          *p_hw_tim = (amhw_hc32f460_timea_t *)p_dev->p_devinfo->tim_regbase;
    am_hc32f460_timea_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;

    /* 有效通道号范围 0 ~ (chan_max_num - 1) */
    if (chan >= p_dev->p_devinfo->chan_max_num) {
        return -AM_EINVAL;
    }

    /* 还原引脚配置 */
    for(i = 0; i < p_dev->p_devinfo->chan_max_num; i++) {
        if((p_ioinfo[i].channel) ==  chan){
            am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].dfunc);
            disable_flag = 1;
            break;
        }
    }

    /* 输入通道号无效 */
    if(disable_flag == 0){
        return -AM_EINVAL;
    }

    /* 断开NVIC中断回调函数 */
//    am_int_disconnect(p_dev->p_devinfo->inum, __hc32f460_timea_cap_irq_handler, (void *)p_dev);
//    am_int_disable(p_dev->p_devinfo->inum);

    /* 禁止中断 */
    amhw_hc32f460_timea_int_disable(p_hw_tim, (timea_irq_type_t)chan);
    amhw_hc32f460_timea_int_disable(p_hw_tim, TIMERA_IRQ_OVERFLOW);


    /* 禁能定时器TIM允许计数 */
    amhw_hc32f460_timea_disable(p_hw_tim, chan);

    return AM_OK;
}

/**
  * \brief 复位捕获通道计数值
  */
static int __hc32f460_timea_cap_reset (void *p_drv, int chan)
{
    am_hc32f460_timea_cap_dev_t *p_dev    = (am_hc32f460_timea_cap_dev_t *)p_drv;
    amhw_hc32f460_timea_t       *p_hw_tim = (amhw_hc32f460_timea_t *)p_dev->p_devinfo->tim_regbase;

    /* 禁能定时器TIM允许计数 */
    amhw_hc32f460_timea_disable(p_hw_tim, chan);

    /* 清零计数器 */
    amhw_hc32f460_timea_cnt16_count_set(p_hw_tim, 0);

    /* 使能定时器 */
    amhw_hc32f460_timea_enable(p_hw_tim, chan);

    /* 清除中断标志 */
    amhw_hc32f460_timea_int_all_flag_clr(p_hw_tim);
    return AM_OK;
}

/**
  * \brief 转换两次捕获值为时间值
  */
static int __hc32f460_timea_cap_count_to_time (void         *p_drv,
                                               int           chan,
                                               unsigned int  count1,
                                               unsigned int  count2,
                                               unsigned int *p_time_ns)
{
    am_hc32f460_timea_cap_dev_t *p_dev    = (am_hc32f460_timea_cap_dev_t *)p_drv;
    amhw_hc32f460_timea_t       *p_hw_tim = (amhw_hc32f460_timea_t *)p_dev->p_devinfo->tim_regbase;

    uint32_t count_err;
    uint64_t time_ns;
    uint16_t reg_pre, pre;

    uint32_t clkfreq  = am_clk_rate_get(CLK_TIMERA_1);

    reg_pre = (uint16_t)amhw_hc32f460_timea_mode_clkdiv_get(p_hw_tim, chan);

    pre = 1 << reg_pre;

    count_err =  count2 - count1;

    /* 将两次读取值的差转换成时间 */
    time_ns = (uint64_t)1000000000 * (uint64_t)count_err * pre / (uint64_t)clkfreq;

    *p_time_ns = time_ns;

    return AM_OK;
}

/**
  * \brief 中断服务函数
  */
static void __hc32f460_timea_cap_irq_handler (void *p_arg)
{
    am_hc32f460_timea_cap_dev_t *p_dev     = (am_hc32f460_timea_cap_dev_t *)p_arg;
    amhw_hc32f460_timea_t       *p_hw_tim  = (amhw_hc32f460_timea_t *)p_dev->p_devinfo->tim_regbase;

    uint8_t           i = 0;
    am_cap_callback_t callback_func;
    uint32_t          value;
    if ((amhw_hc32f460_timea_int_flag_check(p_hw_tim,
             TIMEA_FLAG_OVERFLOW)) == AM_TRUE) {
        __update_num++;

        amhw_hc32f460_timea_int_flag_clr(p_hw_tim,
                                            TIMEA_FLAG_OVERFLOW);
    }            

    for(i = 0; i < p_dev->chan_max; i++) {
        if ((amhw_hc32f460_timea_int_flag_check(p_hw_tim,(timea_flag_type_t)i)) == AM_TRUE) {
                     
            callback_func = p_dev->callback_info[i].callback_func;
            value = amhw_hc32f460_timea_get_compare_value(p_hw_tim, (timea_channel_t)i) + (__update_num << 16ul);


            if (callback_func != NULL) {
                callback_func(p_dev->callback_info[i].p_arg, value);
            }     

            amhw_hc32f460_timea_int_flag_clr(p_hw_tim, (timea_flag_type_t)i);
        }
    }

                                       
}

/**
  * \brief 捕获初始化
  */
void __hc32f460_timea_cap_init (amhw_hc32f460_timea_t       *p_hw_tim,
                                am_hc32f460_timea_cap_dev_t *p_dev)
{
    /* 设置定时器模式2 */
    amhw_hc32f460_timea_mode_set(p_hw_tim, timea_count_mode_sawtooth_wave);

    /* 向上计数 */
    amhw_hc32f460_timea_dir_set(p_hw_tim, timea_count_dir_up);

    /* Unit 1 sync startup invalid */
    if(((amhw_hc32f460_timea_t *)HC32F460_TMRA1_BASE == p_hw_tim) &&
       (1 == p_dev->p_devinfo->sync_startup_en))
    {
        while(1);
    }
    
    p_hw_tim->BCSTR_f.SYNST = p_dev->p_devinfo->sync_startup_en;

    __update_num = 0;
}

am_cap_handle_t am_hc32f460_timea_cap_init (am_hc32f460_timea_cap_dev_t           *p_dev,
                                            const am_hc32f460_timea_cap_devinfo_t *p_devinfo)
{
    amhw_hc32f460_timea_t *p_hw_tim = NULL;
    uint8_t    i;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->chan_max = 8;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo        = p_devinfo;
    p_hw_tim                = (amhw_hc32f460_timea_t *)p_dev->p_devinfo->tim_regbase;
    p_dev->cap_serv.p_funcs = (struct am_cap_drv_funcs *)&__g_tim_cap_drv_funcs;
    p_dev->cap_serv.p_drv   = p_dev;

    for (i = 0; i < p_devinfo->chan_max_num; i++) {
        p_dev->callback_info[i].callback_func = NULL;
    }

    /* 捕获初始化 */
    __hc32f460_timea_cap_init(p_hw_tim, p_dev);

    return &(p_dev->cap_serv);
}

void am_hc32f460_timea_cap_deinit (am_cap_handle_t handle)
{
    uint8_t i = 0;

    am_hc32f460_timea_cap_dev_t *p_dev    = (am_hc32f460_timea_cap_dev_t *)handle;
    amhw_hc32f460_timea_t       *p_hw_tim = NULL;

    am_hc32f460_timea_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;

    if (p_dev == NULL || p_dev->p_devinfo == NULL ) {
        return ;
    }

    p_hw_tim     = (amhw_hc32f460_timea_t *)p_dev->p_devinfo->tim_regbase;

    /* 清零计数器 */
    amhw_hc32f460_timea_cnt16_count_set(p_hw_tim, 0);

    __update_num = 0;

    /* 关闭TIM模块 */
    amhw_hc32f460_timea_disable(p_hw_tim, 0);

    am_int_disable(p_dev->p_devinfo->inum);

    p_dev->cap_serv.p_drv = NULL;

    /* 还原GPIO配置 */
    for (i = 0; i <p_dev->p_devinfo->chan_max_num; i++) {

        am_gpio_pin_cfg(p_ioinfo[i].gpio, p_ioinfo[i].dfunc);
    }

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

void TimerA1CMP_IrqHandler(void *p_arg)
{
    __hc32f460_timea_cap_irq_handler(p_arg);
}

void TimerA2CMP_IrqHandler(void *p_arg)
{
    __hc32f460_timea_cap_irq_handler(p_arg);
}

void TimerA3CMP_IrqHandler(void *p_arg)
{
    __hc32f460_timea_cap_irq_handler(p_arg);
}

void TimerA4CMP_IrqHandler(void *p_arg)
{
    __hc32f460_timea_cap_irq_handler(p_arg);
}

void TimerA5CMP_IrqHandler(void *p_arg)
{
    __hc32f460_timea_cap_irq_handler(p_arg);
}

void TimerA6CMP_IrqHandler(void *p_arg)
{
    __hc32f460_timea_cap_irq_handler(p_arg);
}

/* end of file */
