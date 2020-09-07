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
 * \note
 *
 * \internal
 * \par Modification history
 * - 1.00 
 * \endinternal
 */
#include "hc32f460_clk.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_hc32f460_tim_timing.h"
#include "am_clk.h"
#include "hc32f460_irq_handle.h"
#include "hc32f460_intctrl.h"
#include "am_vdebug.h"
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

#define TMR01_GCMA   (1 << 0)
#define TMR01_GCMB   (1 << 1)
#define TMR02_GCMA   (1 << 2)
#define TMR02_GCMB   (1 << 3)

/*******************************************************************************
  全局变量
*******************************************************************************/

/* 定时器信息 */
static am_timer_info_t __g_tim_timing_info;

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
    am_hc32f460_tim_timing_dev_t *p_dev    = (am_hc32f460_tim_timing_dev_t *)p_arg;
    amhw_hc32f460_tim_t          *p_hw_tim = (amhw_hc32f460_tim_t *)
                                           p_dev->p_devinfo->tim_regbase;
    uint8_t chan;
    chan = p_dev->p_devinfo->channel_num;
    /*
     * 判断是否是定时器溢出中断
     */
    if (amhw_hc32f460_tim_int_flag_check(p_hw_tim, chan) == AM_TRUE ) {

        if (p_dev->pfn_callback != NULL) {
            p_dev->pfn_callback(p_dev->p_arg);
        }

        /* 清除溢出标志 */
        amhw_hc32f460_tim_int_flag_clr(p_hw_tim, chan);
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
    am_hc32f460_tim_timing_dev_t *p_dev = (am_hc32f460_tim_timing_dev_t *)p_drv;

    if ((p_dev == NULL) || (p_freq == NULL)) {
        return -AM_EINVAL;
    }
    const am_hc32f460_tim_timing_devinfo_t  *p_devinfo = p_dev->p_devinfo;
    if (tim0_sync == p_devinfo->tim0_counter_mode)
    {
        if (tim0_pclk1 == p_devinfo->tim0_sync_clk_src)
        {
            *p_freq =  am_clk_rate_get(CLK_PCLK1);    
        }
        // TODO: tim0_Inside_hard_trig
        else
        {

        }
    }
    else
    {
        if (tim0_XTAL32 == p_devinfo->tim0_async_clk_src)
        {
            *p_freq =  am_clk_rate_get(CLK_XTAL32);    
        }
        else
        {
            *p_freq =  am_clk_rate_get(CLK_LRC);    
        }
    }
    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_prescale_set (void    *p_drv, 
                                      uint8_t  chan, 
                                      uint32_t prescale)
{
    am_hc32f460_tim_timing_dev_t *p_dev = (am_hc32f460_tim_timing_dev_t *)p_drv;
    amhw_hc32f460_tim_t          *p_hw_tim;
    amhw_hc32f460_tim_clkdiv_t    clkdiv;
    
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_hw_tim = (amhw_hc32f460_tim_t *)p_dev->p_devinfo->tim_regbase;

    if ((prescale != 0) && (prescale <= 1024)) {

        if(prescale <= 1) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV0;
        } else if(prescale <= 2) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV2;
        } else if(prescale <= 4) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV4;
        } else if(prescale <= 8) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV8;
        } else if(prescale <= 16) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV16;
        } else if(prescale <= 32) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV32;
        } else if(prescale <= 64) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV64;
        } else if(prescale <= 128) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV128;
        }else if(prescale <= 256) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV256;
        }else if(prescale <= 512) {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV512;
        }else {
            clkdiv = AMHW_HC32F460_TIM_CLK_DIV1024;
        }
        p_dev->prescale_in = 1 << clkdiv;
        p_dev->prescale_to = prescale;
        amhw_hc32f460_tim_mode_clkdiv_set(p_hw_tim, chan, clkdiv);

        return AM_OK;
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
static int __tim_timing_prescale_get (void     *p_drv, 
                                      uint8_t   chan, 
                                      uint32_t *p_prescale)
{

    am_hc32f460_tim_timing_dev_t *p_dev = (am_hc32f460_tim_timing_dev_t *)p_drv;
    amhw_hc32f460_tim_t          *p_hw_tim;

    uint16_t div = 1;

    if ((p_dev == NULL) || (p_prescale == NULL)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_hc32f460_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 获取分频值 */
    div = (uint16_t)amhw_hc32f460_tim_mode_clkdiv_get(p_hw_tim, chan);
    *p_prescale = 1 << div;

    p_dev->prescale_in = *p_prescale;

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_count_get (void *p_drv, uint8_t chan, void *p_count)
{
    am_hc32f460_tim_timing_dev_t *p_dev = (am_hc32f460_tim_timing_dev_t *)p_drv;
    amhw_hc32f460_tim_t          *p_hw_tim;

    if ((p_dev == NULL) || (p_count == NULL)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_hc32f460_tim_t *)p_dev->p_devinfo->tim_regbase;
    *(uint32_t *)p_count = amhw_hc32f460_tim_cnt16_count_get(p_hw_tim, chan);
    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_rollover_get (void *p_drv,uint8_t chan,void *p_rollover)
{
    am_hc32f460_tim_timing_dev_t *p_dev = (am_hc32f460_tim_timing_dev_t *)p_drv;
    amhw_hc32f460_tim_t          *p_hw_tim;

    if ((p_dev == NULL) || (p_rollover == NULL)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_hc32f460_tim_t *)p_dev->p_devinfo->tim_regbase;
    *(uint32_t *)p_rollover = 0xffff - amhw_hc32f460_tim_arr_count_get(p_hw_tim, chan);

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_disable (void *p_drv, uint8_t chan)
{
    am_hc32f460_tim_timing_dev_t *p_dev = (am_hc32f460_tim_timing_dev_t *)p_drv;
    amhw_hc32f460_tim_t          *p_hw_tim;
    
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
    
    p_hw_tim = (amhw_hc32f460_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 禁能更新中断 */
    amhw_hc32f460_tim_int_disable(p_hw_tim, chan);

    amhw_hc32f460_tim_disable(p_hw_tim, chan);

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_enable (void *p_drv, uint8_t chan, void *p_count)
{
    am_hc32f460_tim_timing_dev_t *p_dev = (am_hc32f460_tim_timing_dev_t *)p_drv;
    amhw_hc32f460_tim_t          *p_hw_tim;
    uint16_t                     count = *(uint16_t *)p_count;
    stc_tmr0_bconr_field_t       stc_bconr_tmp;

    if ((p_dev == NULL) || (p_count == NULL)) {
        return -AM_EINVAL;
    }
    
    const  am_hc32f460_tim_timing_devinfo_t  *p_devinfo = p_dev->p_devinfo;
    p_hw_tim = (amhw_hc32f460_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 禁止定时器产生中断 */
    amhw_hc32f460_tim_int_disable(p_hw_tim, chan);

    count = count * p_dev->prescale_to / p_dev->prescale_in;
    
    /* 只有在16位计数器模式下可设置重载寄存器的值 */
    amhw_hc32f460_tim_arr_count_set(p_hw_tim, chan, count);

    /* 设置计数器初值 CNT */
    amhw_hc32f460_tim_cnt16_count_set(p_hw_tim, chan, count);

    /* 计数时钟设置 */
    /* TMR0_U1_GCMA仅在异步模式下可用 */
    if(((amhw_hc32f460_tim_t *)HC32F460_TIMER0_1_BASE == p_hw_tim) &&
        (TIM0_CHANNEL_A == p_devinfo->channel_num))
    {
        if(tim0_async != p_devinfo->tim0_counter_mode)
        {
            while(1);
        }
    }
    
    stc_bconr_tmp = p_hw_tim->BCONR_f;
    
    switch(p_devinfo->channel_num)
    {
        case TIM0_CHANNEL_A:
            /*set timer counter mode*/
            stc_bconr_tmp.SYNSA = p_devinfo->tim0_counter_mode;
            switch(p_devinfo->tim0_counter_mode)
            {
                case tim0_sync:
                    stc_bconr_tmp.SYNCLKA = p_devinfo->tim0_sync_clk_src;
                    break;
                case tim0_async:
                    stc_bconr_tmp.ASYNCLKA = p_devinfo->tim0_async_clk_src;
                    break;
                default:
                    break;
            }
            /* Write BCONR register */
            p_hw_tim->BCONR_f = stc_bconr_tmp;    
            break;
    
        case TIM0_CHANNEL_B:
            /*set timer counter mode*/
            stc_bconr_tmp.SYNSB = p_devinfo->tim0_counter_mode;
            switch(p_devinfo->tim0_counter_mode)
            {
                case tim0_sync:
                    stc_bconr_tmp.SYNCLKB = p_devinfo->tim0_sync_clk_src;
                    break;
                case tim0_async:
                    stc_bconr_tmp.ASYNCLKB = p_devinfo->tim0_async_clk_src;
                    break;
                default:
                    break;
            }
            p_hw_tim->BCONR_f = stc_bconr_tmp;    
            break;
        default:
            break;
    }

    /* 更新定时器时会产生更新事件,清除标志位 */
    amhw_hc32f460_tim_int_flag_clr(p_hw_tim, chan);

    if (p_dev->pfn_callback != NULL) {

        /* 允许更新中断 */
        amhw_hc32f460_tim_int_enable(p_hw_tim, chan);
    }

    /* 使能定时器TIM允许计数 */
    amhw_hc32f460_tim_enable(p_hw_tim, chan);

    return AM_OK;
}

/******************************************************************************/
static int __tim_timing_callback_set (void     *p_drv,
                                      uint8_t   chan,
                                      void    (*pfn_callback)(void *),
                                      void     *p_arg)
{
    am_hc32f460_tim_timing_dev_t *p_dev = (am_hc32f460_tim_timing_dev_t *)p_drv;
    amhw_hc32f460_tim_t          *p_hw_tim;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_hc32f460_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 不需要开启中断 */
    if (pfn_callback == NULL) {
        amhw_hc32f460_tim_int_disable(p_hw_tim, chan);
    } else {
        p_dev->pfn_callback = pfn_callback;
        p_dev->p_arg   = p_arg;
    }

    return AM_OK;
}

/*******************************************************************************
  Public functions
*******************************************************************************/

am_timer_handle_t am_hc32f460_tim_timing_init (
    am_hc32f460_tim_timing_dev_t             *p_dev,
    const am_hc32f460_tim_timing_devinfo_t   *p_devinfo)
{
    amhw_hc32f460_tim_t *p_hw_tim = NULL;

    if (NULL == p_dev || NULL == p_devinfo ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo          = p_devinfo;
    p_hw_tim                  = (amhw_hc32f460_tim_t *)p_dev->p_devinfo->tim_regbase;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_tim_timing_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev;

    p_dev->pfn_callback = NULL;
    p_dev->p_arg        = NULL;

    /* 16位重载计数 ，可重载*/
    __g_tim_timing_info.counter_width = 16;

    __g_tim_timing_info.features =
        AM_TIMER_CAN_INTERRUPT   |  /* 可以产生中断 */
        AM_TIMER_AUTO_RELOAD     |  /* 支持自动重载 */
        AM_TIMER_INTERMEDIATE_COUNT;/* 读取当前计数器的值   */
    __g_tim_timing_info.prescaler = 0x7FF;
    __g_tim_timing_info.chan_num = 2;
    
    am_int_connect(p_dev->p_devinfo->inum, IRQ130_Handler, NULL);
    if (((amhw_hc32f460_tim_t *)HC32F460_TIMER0_1_BASE == p_hw_tim))
    {
        if (TIM0_CHANNEL_A == p_devinfo->channel_num)
        {
            amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, TMR01_GCMA);
        }
        if (TIM0_CHANNEL_B == p_devinfo->channel_num)
        {
            amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, TMR01_GCMB);
        }
    }

    if (((amhw_hc32f460_tim_t *)HC32F460_TIMER0_2_BASE == p_hw_tim))
    {
        if (TIM0_CHANNEL_A == p_devinfo->channel_num)
        {
            amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, TMR02_GCMA);
        }
        if (TIM0_CHANNEL_B == p_devinfo->channel_num)
        {
            amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, TMR02_GCMB);
        }
    }    
    am_int_enable(p_dev->p_devinfo->inum);
    return &(p_dev->timer_serv);
}

/******************************************************************************/
void am_hc32f460_tim_timing_deinit (am_timer_handle_t handle)
{

    am_hc32f460_tim_timing_dev_t *p_dev    = (am_hc32f460_tim_timing_dev_t *)handle;
    amhw_hc32f460_tim_t          *p_hw_tim = NULL;

    if (p_dev == NULL || p_dev->p_devinfo == NULL ) {
        return ;
    }
    const am_hc32f460_tim_timing_devinfo_t  *p_devinfo = p_dev->p_devinfo;
    p_hw_tim = (amhw_hc32f460_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 关闭更新中断 */
    amhw_hc32f460_tim_int_disable(p_hw_tim, p_devinfo->channel_num);

    /* 关闭定时器 */
    amhw_hc32f460_tim_disable(p_hw_tim, p_devinfo->channel_num);

//    am_int_disconnect(p_dev->p_devinfo->inum, IRQ130_Handler, (void *)p_dev);
//    am_int_disable(p_dev->p_devinfo->inum);

    p_dev->timer_serv.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

void Timer01GCMA_IrqHandler(void *p_arg)
{
    __tim_irq_handler(p_arg);
}

void Timer01GCMB_IrqHandler(void *p_arg)
{
    __tim_irq_handler(p_arg);
}


void Timer02GCMA_IrqHandler(void *p_arg)
{
    __tim_irq_handler(p_arg);
}

void Timer02GCMB_IrqHandler(void *p_arg)
{
    __tim_irq_handler(p_arg);
}
/* end of file */
