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
#include "am_hc32f460_tim4_timing.h"
#include "am_clk.h"
#include "hc32f460_irq_handle.h"
#include "hc32f460_intctrl.h"
#include "am_vdebug.h"
/*******************************************************************************
  定时器驱动函数声明
*******************************************************************************/

static const am_timer_info_t * __tim4_timing_info_get (void *p_drv);

static int __tim4_timing_clkin_freq_get (void *, uint32_t *);
static int __tim4_timing_prescale_set (void *, uint8_t, uint32_t );
static int __tim4_timing_prescale_get (void *, uint8_t, uint32_t *);
static int __tim4_timing_count_get (void *, uint8_t, void *);
static int __tim4_timing_rollover_get (void *, uint8_t, void *);
static int __tim4_timing_disable (void *, uint8_t);
static int __tim4_timing_enable (void *, uint8_t, void *);
static int __tim4_timing_callback_set (void *, uint8_t, void (*)(void *), void *);

static void __tim4_irq_handler (void *p_arg);

#define TMR4X_GCMUH    (1 << 0)
#define TMR4X_GCMUL    (1 << 1)
#define TMR4X_GCMVH    (1 << 2)
#define TMR4X_GCMVL    (1 << 3)
#define TMR4X_GCMWH    (1 << 4)
#define TMR4X_GCMWL    (1 << 5)
#define TMR4X_GOVF     (1 << 6)
#define TMR4X_GUDF     (1 << 7)
#define TMR4X_RLOU     (1 << 8)
#define TMR4X_RLOV     (1 << 9)
#define TMR4X_RLOW     (1 << 10)


/*******************************************************************************
  全局变量
*******************************************************************************/

/* 定时器信息 */
static am_timer_info_t __g_tim4_timing_info;

/* TIM定时器驱动函数 */
static const struct am_timer_drv_funcs __g_tim4_timing_drv_funcs = {
    __tim4_timing_info_get,
    __tim4_timing_clkin_freq_get,
    __tim4_timing_prescale_set,
    __tim4_timing_prescale_get,
    __tim4_timing_count_get,
    __tim4_timing_rollover_get,
    __tim4_timing_enable,
    __tim4_timing_disable,
    __tim4_timing_callback_set,
};

/*******************************************************************************
  中断处理
*******************************************************************************/

void __tim4_irq_handler (void *p_arg)
{
    am_hc32f460_tim4_timing_dev_t *p_dev    = (am_hc32f460_tim4_timing_dev_t *)p_arg;
    amhw_hc32f460_tim4_t          *p_hw_tim = (amhw_hc32f460_tim4_t *)
                                               p_dev->p_devinfo->tim4_regbase;
                                           
    timer4_cnt_int_t flag = TIMER4_CNT_ZERO_MATCH_INT;                                          
    /*
     * 判断是否是定时器溢出中断
     */
    if (amhw_hc32f460_tim4_int_flag_check(p_hw_tim, flag) == AM_TRUE ) {

        if (p_dev->pfn_callback != NULL) {
            p_dev->pfn_callback(p_dev->p_arg);
        }

        /* 清除溢出标志 */
        amhw_hc32f460_tim4_int_flag_clr(p_hw_tim, flag);
    }
}

/*******************************************************************************
  标准定时器驱动实现
*******************************************************************************/

static const am_timer_info_t * __tim4_timing_info_get (void *p_drv)
{
    return &__g_tim4_timing_info;
}

/******************************************************************************/
static int __tim4_timing_clkin_freq_get (void *p_drv, uint32_t *p_freq)
{
    am_hc32f460_tim4_timing_dev_t *p_dev = (am_hc32f460_tim4_timing_dev_t *)p_drv;

    if ((p_dev == NULL) || (p_freq == NULL)) {
        return -AM_EINVAL;
    }
    
    *p_freq =  am_clk_rate_get(CLK_TIMER4_1);    
    return AM_OK;
}

/******************************************************************************/
static int __tim4_timing_prescale_set (void    *p_drv, 
                                       uint8_t  chan,
                                       uint32_t prescale)
{
    am_hc32f460_tim4_timing_dev_t *p_dev = (am_hc32f460_tim4_timing_dev_t *)p_drv;
    amhw_hc32f460_tim4_t          *p_hw_tim;
    amhw_hc32f460_tim4_clkdiv_t clkdiv;
    
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_hw_tim = (amhw_hc32f460_tim4_t *)p_dev->p_devinfo->tim4_regbase;

    if ((prescale != 0) && (prescale <= 1024)) {

        if(prescale <= 1) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV0;
        } else if(prescale <= 2) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV2;
        } else if(prescale <= 4) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV4;
        } else if(prescale <= 8) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV8;
        } else if(prescale <= 16) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV16;
        } else if(prescale <= 32) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV32;
        } else if(prescale <= 64) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV64;
        } else if(prescale <= 128) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV128;
        }else if(prescale <= 256) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV256;
        }else if(prescale <= 512) {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV512;
        }else {
            clkdiv = AMHW_HC32F460_TIM4_CLK_DIV1024;
        }
        p_dev->prescale_in = 1 << clkdiv;
        p_dev->prescale_to = prescale;
        amhw_hc32f460_tim4_mode_clkdiv_set(p_hw_tim, chan, clkdiv);

        return AM_OK;
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
static int __tim4_timing_prescale_get (void     *p_drv, 
                                       uint8_t   chan,
                                       uint32_t *p_prescale)
{

    am_hc32f460_tim4_timing_dev_t *p_dev = (am_hc32f460_tim4_timing_dev_t *)p_drv;
    amhw_hc32f460_tim4_t          *p_hw_tim;

    uint16_t div = 1;

    if ((p_dev == NULL) || (p_prescale == NULL)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_hc32f460_tim4_t *)p_dev->p_devinfo->tim4_regbase;

    /* 获取分频值 */
    div = (uint16_t)amhw_hc32f460_tim4_mode_clkdiv_get(p_hw_tim, chan);
    *p_prescale = 1 << div;

    p_dev->prescale_in = *p_prescale;

    return AM_OK;
}

/******************************************************************************/
static int __tim4_timing_count_get (void *p_drv, uint8_t chan, void *p_count)
{
    am_hc32f460_tim4_timing_dev_t *p_dev = (am_hc32f460_tim4_timing_dev_t *)p_drv;
    amhw_hc32f460_tim4_t          *p_hw_tim;

    if ((p_dev == NULL) || (p_count == NULL)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_hc32f460_tim4_t *)p_dev->p_devinfo->tim4_regbase;
    *(uint32_t *)p_count = amhw_hc32f460_tim4_cnt16_count_get(p_hw_tim, chan);
    return AM_OK;
}

/******************************************************************************/
static int __tim4_timing_rollover_get (void *p_drv,uint8_t chan,void *p_rollover)
{
    am_hc32f460_tim4_timing_dev_t *p_dev = (am_hc32f460_tim4_timing_dev_t *)p_drv;
    amhw_hc32f460_tim4_t          *p_hw_tim;

    if ((p_dev == NULL) || (p_rollover == NULL)) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_hc32f460_tim4_t *)p_dev->p_devinfo->tim4_regbase;
    *(uint32_t *)p_rollover = 0xffff - amhw_hc32f460_tim4_arr_count_get(p_hw_tim, chan);

    return AM_OK;
}

/******************************************************************************/
static int __tim4_timing_disable (void *p_drv, uint8_t chan)
{
    am_hc32f460_tim4_timing_dev_t *p_dev = (am_hc32f460_tim4_timing_dev_t *)p_drv;
    amhw_hc32f460_tim4_t          *p_hw_tim;
    
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
    
    p_hw_tim = (amhw_hc32f460_tim4_t *)p_dev->p_devinfo->tim4_regbase;

    /* 禁能中断 */
    amhw_hc32f460_tim4_intz_set(p_hw_tim, AM_FALSE);
    amhw_hc32f460_tim4_intp_set(p_hw_tim, AM_FALSE);
    amhw_hc32f460_tim4_disable(p_hw_tim, chan);

    return AM_OK;
}

/******************************************************************************/
static int __tim4_timing_enable (void *p_drv, uint8_t chan, void *p_count)
{
    am_hc32f460_tim4_timing_dev_t *p_dev = (am_hc32f460_tim4_timing_dev_t *)p_drv;
    amhw_hc32f460_tim4_t          *p_hw_tim;
    uint16_t                     count = *(uint16_t *)p_count;

    if ((p_dev == NULL) || (p_count == NULL)) {
        return -AM_EINVAL;
    }
    p_hw_tim = (amhw_hc32f460_tim4_t *)p_dev->p_devinfo->tim4_regbase;
    am_hc32f460_tim4_timing_devinfo_t  *p_devinfo = (am_hc32f460_tim4_timing_devinfo_t *)p_dev->p_devinfo;

    amhw_hc32f460_tim4_mode_set(p_hw_tim, p_devinfo->tim4_counter_mode);
    amhw_hc32f460_tim4_buf_set(p_hw_tim, AM_FALSE);    
    /* 默认时钟使用PCLK1 */
    amhw_hc32f460_tim4_ext_clk_set(p_hw_tim, AM_FALSE);    
    /* 中断使能 */    
    if (p_dev->pfn_callback != NULL) {
        amhw_hc32f460_tim4_intz_set(p_hw_tim, AM_TRUE);    
        amhw_hc32f460_tim4_intp_set(p_hw_tim, AM_FALSE);
    }
    /* set intterrupt mask times */
    amhw_hc32f460_tim4_zim_set(p_hw_tim, TIMER4_CNT_INT_MASK0);
    amhw_hc32f460_tim4_pim_set(p_hw_tim, TIMER4_CNT_INT_MASK0);

    count = count * p_dev->prescale_to / p_dev->prescale_in;
    
    /* 只有在16位计数器模式下可设置重载寄存器的值 */
    amhw_hc32f460_tim4_arr_count_set(p_hw_tim, chan, count);
    
    /* 更新定时器时会产生更新事件,清除标志位 */
    amhw_hc32f460_tim4_cnt_value_clr(p_hw_tim);

    /* 使能定时器TIM允许计数 */
    amhw_hc32f460_tim4_enable(p_hw_tim, chan);

    return AM_OK;
}

/******************************************************************************/
static int __tim4_timing_callback_set (void     *p_drv,
                                       uint8_t   chan,
                                       void    (*pfn_callback)(void *),
                                       void     *p_arg)
{
    am_hc32f460_tim4_timing_dev_t *p_dev = (am_hc32f460_tim4_timing_dev_t *)p_drv;
    amhw_hc32f460_tim4_t          *p_hw_tim;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
 
    p_hw_tim = (amhw_hc32f460_tim4_t *)p_dev->p_devinfo->tim4_regbase;

    /* 不需要开启中断 */
    if (pfn_callback == NULL) {
        amhw_hc32f460_tim4_intz_set(p_hw_tim, AM_FALSE);
        amhw_hc32f460_tim4_intp_set(p_hw_tim, AM_FALSE);
    } else {
        p_dev->pfn_callback = pfn_callback;
        p_dev->p_arg   = p_arg;
    }

    return AM_OK;
}

/*******************************************************************************
  Public functions
*******************************************************************************/

am_timer_handle_t am_hc32f460_tim4_timing_init (
    am_hc32f460_tim4_timing_dev_t             *p_dev,
    const am_hc32f460_tim4_timing_devinfo_t   *p_devinfo)
{
    amhw_hc32f460_tim4_t *p_hw_tim = NULL;

    if (NULL == p_dev || NULL == p_devinfo ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo          = p_devinfo;
    p_hw_tim                  = (amhw_hc32f460_tim4_t *)p_dev->p_devinfo->tim4_regbase;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_tim4_timing_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev;

    p_dev->pfn_callback = NULL;
    p_dev->p_arg        = NULL;

    /* 16位重载计数 ，可重载*/
    __g_tim4_timing_info.counter_width = 16;

    __g_tim4_timing_info.features =
        AM_TIMER_CAN_INTERRUPT   |  /* 可以产生中断 */
        AM_TIMER_AUTO_RELOAD     |  /* 支持自动重载 */
        AM_TIMER_INTERMEDIATE_COUNT;/* 读取当前计数器的值   */
    __g_tim4_timing_info.prescaler = 0x7FF;
    __g_tim4_timing_info.chan_num = 1;

    /* Set default value */
    p_hw_tim->CCSR = (uint16_t)0x0050u;
    p_hw_tim->CNTR = (uint16_t)0x0000u;
    p_hw_tim->CPSR = (uint16_t)0xFFFFu;
    p_hw_tim->CVPR = (uint16_t)0x0000u;

    amhw_hc32f460_tim4_disable(p_hw_tim, 0);

    if ((amhw_hc32f460_tim4_t *)HC32F460_TMR41_BASE == p_hw_tim)
    {    
        am_int_connect(p_dev->p_devinfo->inum, IRQ138_Handler, NULL);
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (TMR4X_GUDF) << 0);
    }else if ((amhw_hc32f460_tim4_t *)HC32F460_TMR42_BASE == p_hw_tim)
    {    
        am_int_connect(p_dev->p_devinfo->inum, IRQ138_Handler, NULL);
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (TMR4X_GUDF) << 16);
    }else if ((amhw_hc32f460_tim4_t *)HC32F460_TMR43_BASE == p_hw_tim)
    {
        am_int_connect(p_dev->p_devinfo->inum, IRQ139_Handler, NULL);    
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (TMR4X_GUDF) << 0);
    }else {
        ;
    }        
    am_int_enable(p_dev->p_devinfo->inum);
    return &(p_dev->timer_serv);
}

/******************************************************************************/
void am_hc32f460_tim4_timing_deinit (am_timer_handle_t handle)
{

    am_hc32f460_tim4_timing_dev_t *p_dev    = (am_hc32f460_tim4_timing_dev_t *)handle;
    amhw_hc32f460_tim4_t          *p_hw_tim = NULL;

    if (p_dev == NULL || p_dev->p_devinfo == NULL ) {
        return ;
    }
    p_hw_tim = (amhw_hc32f460_tim4_t *)p_dev->p_devinfo->tim4_regbase;

    /* 关闭更新中断 */
    amhw_hc32f460_tim4_intz_set(p_hw_tim, AM_FALSE);
    amhw_hc32f460_tim4_intz_set(p_hw_tim, AM_FALSE);
    /* 关闭定时器 */
    amhw_hc32f460_tim4_disable(p_hw_tim, 0);

//    am_int_disconnect(p_dev->p_devinfo->inum, IRQ130_Handler, (void *)p_dev);
//    am_int_disable(p_dev->p_devinfo->inum);

    p_dev->timer_serv.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

void Timer41GCMUH_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer41GCMUL_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}
void Timer41GCMVH_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer41GCMVL_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer41GCMWH_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer41GCMWL_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer41GOV_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer41GUD_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer41ReloadU_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer41ReloadV_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer41ReloadW_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42GCMUH_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42GCMUL_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}
void Timer42GCMVH_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42GCMVL_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42GCMWH_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42GCMWL_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42GOV_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42GUD_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42ReloadU_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42ReloadV_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer42ReloadW_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43GCMUH_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43GCMUL_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}
void Timer43GCMVH_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43GCMVL_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43GCMWH_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43GCMWL_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43GOV_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43GUD_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43ReloadU_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43ReloadV_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

void Timer43ReloadW_IrqHandler(void *p_arg)
{
    __tim4_irq_handler(p_arg);
}

/* end of file */
