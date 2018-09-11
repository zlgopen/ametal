/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief Timing driver implementation for SCT
 *
 * 1. The SCT can provide 3 service as follows,This driver implement 
 *    the timing function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The SCT resource used as follows:
 *    - The match 0 to save the period time,associate with event 0 to generate 
 *      interrupt.
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-14  tee, first implementation.
 * \endinternal
 */

#include "am_lpc_sct_timing.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_vdebug.h"
/*******************************************************************************
  Functions declaration
*******************************************************************************/

/* lpc5410x SCT for timing drv functions declaration */
//static int __sct_timing_info_get(void *p_drv, am_timer_info_t *p_info);

static const am_timer_info_t * __sct_timing_info_get (void *p_drv);

static int __sct_timing_clkin_freq_get (void *p_drv, uint32_t *p_freq);

static int __sct_timing_prescale_set (void    *p_drv, 
                                      uint8_t  chan, 
                                      uint32_t prescale);

static int __sct_timing_prescale_get (void    *p_drv, 
                                      uint8_t  chan, 
                                      uint32_t *prescale);
  
static int __sct_timing_count_get(void     *p_drv,
                                  uint8_t   chan,
                                  void     *p_count);

static int __sct_timing_rollover_get(void     *p_drv,
                                     uint8_t   chan,
                                     void     *p_rollover);
                                     
static int __sct_timing_disable(void *p_drv, uint8_t chan);

static int __sct_timing_enable(void     *p_drv, 
                               uint8_t   chan,
                               void     *max_sct_count);
    
static int __sct_timing_callback_set(void    *p_drv,
                                     uint8_t  chan,
                                     void   (*pfn_callback)(void *),
                                     void    *p_arg);
#if 0
static int  __sct_timing_connect(void *p_drv);
#endif

static void __sct_irq_handler(void *p_arg);
 
// 用户配置定时器模式
static amhw_lpc_sct_mode_t __g_mode = AMHW_LPC_SCT_MODE_SEPARATE_L;

/* 定时器信息 */
static const am_timer_info_t __g_sct_timing_info = {
    16,                                      /* 16位定时器               */
    2,                                       /* 2通道                   */
    AM_TIMER_CAN_INTERRUPT      |            /* 可以产生中断             */
    AM_TIMER_AUTO_RELOAD        |            /* 支持自动重载             */
    AM_TIMER_INTERMEDIATE_COUNT ,            /* 当前计数器值可读         */
    256                                      /* 预分频最大值：256      */
};


/** \brief SCT for timing driver functions */
static const struct am_timer_drv_funcs __g_sct_timing_drv_funcs = {
    __sct_timing_info_get,
    __sct_timing_clkin_freq_get,
    __sct_timing_prescale_set,
    __sct_timing_prescale_get,
    __sct_timing_count_get,
    __sct_timing_rollover_get,
    __sct_timing_enable,
    __sct_timing_disable,
    __sct_timing_callback_set,
};

/******************************************************************************/

/**
 * \brief Calculate 16 bits timer prescale(8-bit) and match value(16-bit) for a given count
 *
 * \param[in]  count   : number of clocks the 16bits timer count 
 * \param[out] p_match : pointer to hold the timer match value
 *
 * \return the 8-bit prescale value calculated
 */
uint32_t __prescale_and_match_cal (uint32_t count, uint32_t *p_match)
{
    uint32_t match, min_remainder, prescale;
    uint32_t result_quot, result_rem;
 
    min_remainder = count;
    prescale      = 1;
   *p_match       = 65536;

    /* 
     * the count is less than 65536, 
     * the prescale set to 1, and the match equal to count   
     */
    if (count <= 65536) {        
        prescale = 1;
       *p_match  = count;  
        return prescale;        
    }    
    
    /* check match value one by one until find a best one */
    for (match = 65536; match > 0; match--) {
        result_quot = count / match;
        result_rem  = count % match;
        
        /* 
         * because the width of prescale value is 8-bits, the result_quot maximum
         * value is 256, if beyond 256,the count is can't be reached.
         */
        if (result_quot <= 256) {       
            if (result_rem == 0) {   
            
                /* find a best prescale and match value */
                prescale = result_quot;
                *p_match = match;
                break;
                
            } else if (result_rem < min_remainder) {
            
                /* find a better prescale and match value */
                min_remainder = result_rem;
                prescale      = result_quot;
               *p_match       = match;
            }
        }
    }

    return prescale;
}

#if 0
int __sct_timing_connect (void *p_drv)
{
    am_lpc_sct_timing_dev_t *p_dev = (am_lpc_sct_timing_dev_t *)p_drv;
    
    am_int_connect(p_dev->p_devinfo->inum, __sct_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    
    return AM_OK;
}
#endif

void __sct_irq_handler (void *p_arg)
{
    am_lpc_sct_timing_dev_t *p_dev    = (am_lpc_sct_timing_dev_t *)p_arg;
    amhw_lpc_sct_t          *p_hw_sct = (amhw_lpc_sct_t *)(p_dev->p_devinfo->sct_regbase);

    /* 
     * Because use the event 0 to generate interrupt
     * So,Check if the event 0 interrupt 
     */
    if (amhw_lpc_sct_event_flag_chk(p_hw_sct, AMHW_LPC_SCT_EVT(0)) == AM_TRUE ) {
        
        if (p_dev->callback_info[0].pfn_callback != NULL) {
            p_dev->callback_info[0].pfn_callback(p_dev->callback_info[0].p_arg);
        }
        
        /* Clear the event 0 flag */
        amhw_lpc_sct_event_flag_clr(p_hw_sct, AMHW_LPC_SCT_EVT(0));  
    }
    
    if (amhw_lpc_sct_event_flag_chk(p_hw_sct, AMHW_LPC_SCT_EVT(1)) == AM_TRUE ) {
        
        if (p_dev->callback_info[1].pfn_callback != NULL) {
            p_dev->callback_info[1].pfn_callback(p_dev->callback_info[1].p_arg);
        }
        
        /* Clear the event 1 flag */
        amhw_lpc_sct_event_flag_clr(p_hw_sct, AMHW_LPC_SCT_EVT(1));  
    }
}

static const am_timer_info_t * __sct_timing_info_get (void *p_drv)
{
    return &__g_sct_timing_info;
}


static int __sct_timing_clkin_freq_get (void *p_drv, uint32_t *p_freq)
{
    am_lpc_sct_timing_dev_t *p_dev = (am_lpc_sct_timing_dev_t *)p_drv;

    if ((p_dev == NULL) || (p_freq == NULL)) {
        return -AM_EINVAL;
    }

    /* 获取当前系统时钟频率，systick 使用的是系统时钟 */
    *p_freq =  am_clk_rate_get(p_dev->p_devinfo->clk_id);

    return AM_OK;
}

static int __sct_timing_prescale_set (void    *p_drv, 
                                      uint8_t  chan, 
                                      uint32_t prescale)
{
    am_lpc_sct_timing_dev_t *p_dev = (am_lpc_sct_timing_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct ;

    if ((p_drv == NULL) || (chan != 0)) {
        return -AM_EINVAL;
    }
    
    p_hw_sct = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    
    if ((prescale != 0) && (prescale <= 65536)) {
 
        amhw_lpc_sct_prescale_set(p_hw_sct, __g_mode,  prescale);  /* 设置分频值 */

        return AM_OK;
    }

    return -AM_ENOTSUP;
}

static int __sct_timing_prescale_get (void    *p_drv, 
                                      uint8_t  chan, 
                                      uint32_t *p_prescale)
{
    am_lpc_sct_timing_dev_t *p_dev = (am_lpc_sct_timing_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct;

    if ((p_drv == NULL) || (chan != 0) || (p_prescale == NULL)) {
        return -AM_EINVAL;
    }
     
     p_hw_sct = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    // AMHW_LPC_SCT_MODE_SEPARATE_H  or  AMHW_LPC_SCT_MODE_SEPARATE_HL
     *p_prescale = amhw_lpc_sct_prescale_get (p_hw_sct, __g_mode);
    
     return AM_OK;  
}


/** \brief Get the SCT information */
//static int __sct_timing_info_get (void *p_drv, am_timer_info_t *p_info)
//{
//    am_lpc_sct_timing_dev_t *p_dev    = (am_lpc_sct_timing_dev_t *)p_drv;

//    /* Get the input frequency */
//    p_info->clk_frequency = am_clk_rate_get(p_dev->p_devinfo->clk_id);
//    p_info->max_frequency = p_info->clk_frequency;
//    
//    if (p_dev->p_devinfo->flag == AM_LPC_SCT_TIMING_1_32BIT) {
//        p_info->min_frequency = 1;                        /* The minimum freq is 1Hz */
//        
//        p_info->features  = AM_TIMER_SIZE(32)           | /* 32-bit SCT              */
//                            AM_TIMER_CHAN_NUM(1)        | /* support 1 channels      */
//                            AM_TIMER_CAN_INTERRUPT      | /* Can generate interrupt  */
//                            AM_TIMER_INTERMEDIATE_COUNT | /* can read now count      */
//                            AM_TIMER_AUTO_RELOAD;         /* Auto reload             */
//        
//    } else {
//        
//        /* 8-bit pre-scale,16-bit timer */
//        if (p_info->clk_frequency <= 0xFFFFFF) {
//            p_info->min_frequency = 1;
//        } else {
//            p_info->min_frequency = p_info->clk_frequency / 0xFFFFFF;
//            
//            if (p_info->min_frequency * 0xFFFFFF < p_info->clk_frequency) {
//                p_info->min_frequency += 1;
//            }
//        }
//            
//        p_info->features  = AM_TIMER_SIZE(16)           | /* 16-bit SCT               */
//                            AM_TIMER_CHAN_NUM(2)        | /* support 2 channels       */
//                            AM_TIMER_CAN_INTERRUPT      | /* Can generate interrupt   */
//                            AM_TIMER_INTERMEDIATE_COUNT | /* can read now count       */
//                            AM_TIMER_AUTO_RELOAD;         /* Auto reload              */        
//    
//    }
//    
//    return AM_OK;
//}

/** \brief Get the SCT Now counter value */ 
static int __sct_timing_count_get (void     *p_drv,
                                   uint8_t   chan,
                                   void     *p_count)
{
    am_lpc_sct_timing_dev_t *p_dev   = (am_lpc_sct_timing_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct = (amhw_lpc_sct_t *)(p_dev->p_devinfo->sct_regbase);
    
    if (p_dev->p_devinfo->flag == AM_LPC_SCT_TIMING_1_32BIT) {
        if (chan != 0) {
            return -AM_EINVAL;
        }

        *(uint32_t *)p_count = amhw_lpc_sct_count_get(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY);
        
    } else {
        if (chan == 0) {
            *(uint32_t *)p_count = amhw_lpc_sct_count_get(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_L);
        } else if (chan == 1) {
            *(uint32_t *)p_count = amhw_lpc_sct_count_get(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_H);
        }
        
        return -AM_EINVAL;
    }
    return AM_OK;
}

/** \brief The SCT roll over value get */
static int __sct_timing_rollover_get (void     *p_drv,
                                      uint8_t   chan,
                                      void *p_rollover)
{
    am_lpc_sct_timing_dev_t *p_dev    = (am_lpc_sct_timing_dev_t *)p_drv;
    

    if (p_dev->p_devinfo->flag == AM_LPC_SCT_TIMING_1_32BIT) {
        if (chan != 0) {
            return -AM_EINVAL;
        }

        *(uint32_t *)p_rollover = 0xFFFFFFFF / am_clk_rate_get(p_dev->p_devinfo->clk_id);
        
    } else {
        if (chan >= 2) {
            return -AM_EINVAL;
        }
        
        *(uint32_t *)p_rollover = 0xFFFFFF / am_clk_rate_get(p_dev->p_devinfo->clk_id);
    }
    
    return AM_OK;
}

/** \brief The SCT disable */
static int __sct_timing_disable (void *p_drv, uint8_t chan)
{
    am_lpc_sct_timing_dev_t *p_dev    = (am_lpc_sct_timing_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct = (amhw_lpc_sct_t *)(p_dev->p_devinfo->sct_regbase);
    
    if (p_dev->p_devinfo->flag == AM_LPC_SCT_TIMING_1_32BIT) {
        if (chan != 0) {
            return -AM_EINVAL;
        }
        /* Set the halt and stop bits,the SCT L counter is stop */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
        amhw_lpc_sct_count_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
        
    } else {
        if (chan == 0) {
            
            /* Set the halt and stop bits,the SCT L counter is stop */
            amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
            amhw_lpc_sct_count_set(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_L, 0);
            
        } else if (chan == 1) {
            
            /* Set the halt and stop bits,the SCT L counter is stop */
            amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
            amhw_lpc_sct_count_set(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_H, 0);
            
        }
        
        return -AM_EINVAL;
    }

    return AM_OK;
}

/** \brief The SCT enable and set the max count value */
static int __sct_timing_enable (void     *p_drv, 
                                uint8_t   chan,
                                void     *p_count)
{
    am_lpc_sct_timing_dev_t *p_dev    = (am_lpc_sct_timing_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct = (amhw_lpc_sct_t *)(p_dev->p_devinfo->sct_regbase);
    uint32_t            max_sct_count = *(uint32_t *)p_count;
  
    if (p_dev->p_devinfo->flag == AM_LPC_SCT_TIMING_1_32BIT) {
        if (chan != 0) {
            return -AM_EINVAL;
        }

        /* Before change the count value, halt the counter */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
        amhw_lpc_sct_count_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
        
        amhw_lpc_sct_mat_val_set(p_hw_sct, 
                                 AMHW_LPC_SCT_MODE_UNIFY, 
                                 AMHW_LPC_SCT_MAT(0), 
                                (uint32_t)max_sct_count);
        amhw_lpc_sct_mat_reload_val_set(p_hw_sct, 
                                        AMHW_LPC_SCT_MODE_UNIFY, 
                                        AMHW_LPC_SCT_MAT(0), 
                                       (uint32_t)max_sct_count);
        
        if (p_dev->callback_info[0].pfn_callback == NULL) {
            amhw_lpc_sct_event_int_disable(p_hw_sct, AMHW_LPC_SCT_EVT(0));
        } else {
            amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(0));
        }
        
        /* start counting */
        amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
        
    } else {
        
        uint8_t  prescale;
        uint32_t match;
        am_kprintf("2.count:%d \n", max_sct_count);
        /* 16bit timer + 8bit prescale support timing 0xFFFFFF counts*/
        if ((uint32_t)max_sct_count > 0xFFFFFF || chan >= 2) {
            return -AM_EINVAL;
        }
        
        prescale = __prescale_and_match_cal((uint32_t)max_sct_count, &match);
        
        if (chan == 0) {
            
            /* Before change the count value, halt the counter */
            amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
            amhw_lpc_sct_count_set(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_L, 0);
            
            amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_L, prescale);
            
            amhw_lpc_sct_mat_val_set(p_hw_sct, 
                                     AMHW_LPC_SCT_MODE_SEPARATE_L, 
                                     AMHW_LPC_SCT_MAT(0), 
                                     match);
            amhw_lpc_sct_mat_reload_val_set(p_hw_sct, 
                                            AMHW_LPC_SCT_MODE_SEPARATE_L,
                                            AMHW_LPC_SCT_MAT(0),
                                            match);
            
            if (p_dev->callback_info[0].pfn_callback == NULL) {
                amhw_lpc_sct_event_int_disable(p_hw_sct, AMHW_LPC_SCT_EVT(0));
            } else {
                amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(0));
            }
            
            /* start counting */
            amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
            
        } else if (chan == 1) {
            
            /* Before change the count value, halt the counter */
            amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_H | AMHW_LPC_SCT_CTRL_HALT_H);
            amhw_lpc_sct_count_set(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_H, 0);
            
            amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_H, prescale);
            
            amhw_lpc_sct_mat_val_set(p_hw_sct, 
                                 AMHW_LPC_SCT_MODE_SEPARATE_H, 
                                 AMHW_LPC_SCT_MAT(0), 
                                 match);
            amhw_lpc_sct_mat_reload_val_set(p_hw_sct, 
                                        AMHW_LPC_SCT_MODE_SEPARATE_H,
                                        AMHW_LPC_SCT_MAT(0),
                                        match);
            
            if (p_dev->callback_info[1].pfn_callback == NULL) {
                amhw_lpc_sct_event_int_disable(p_hw_sct, AMHW_LPC_SCT_EVT(1));
            } else {
                amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(1));
            }
            
            /* start counting */
            amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_H | AMHW_LPC_SCT_CTRL_HALT_H);
        }
    }

    return AM_OK;
}

/** \brief set the function to be called on every interrupt */
static int __sct_timing_callback_set (void     *p_drv,
                                      uint8_t   chan,
                                      void    (*pfn_callback)(void *),
                                      void     *p_arg)
{
    am_lpc_sct_timing_dev_t *p_dev    = (am_lpc_sct_timing_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct = (amhw_lpc_sct_t *)(p_dev->p_devinfo->sct_regbase);
    
    if (p_dev->p_devinfo->flag == AM_LPC_SCT_TIMING_1_32BIT) {
        if (chan != 0) {
            return -AM_EINVAL;
        }
        
        /* needn't interrupt, disable the event 0 interrupt */
        if (pfn_callback == NULL) {
            amhw_lpc_sct_event_int_disable(p_hw_sct, AMHW_LPC_SCT_EVT(0));
            
        } else {
            p_dev->callback_info[0].pfn_callback = pfn_callback;
            p_dev->callback_info[0].p_arg   = p_arg;
            
            amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(0));
        }
    } else {
        if (chan >= 2) {
            return -AM_EINVAL;
        }
        
        /* needn't interrupt, disable the event 0 interrupt */
        if (pfn_callback == NULL) {
            amhw_lpc_sct_event_int_disable(p_hw_sct, AMHW_LPC_SCT_EVT(chan));
            
        } else {
            
            p_dev->callback_info[chan].pfn_callback = pfn_callback;
            p_dev->callback_info[chan].p_arg   = p_arg;
            
            amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(chan));
        }
    }
    
    return AM_OK;
}

static void __sct_timing_1_32bit_init (amhw_lpc_sct_t *p_hw_sct)
{
    amhw_lpc_sct_config(p_hw_sct,
                        AMHW_LPC_SCT_CONFIG_32BIT_COUNTER  | /* the timer is operate as 1 32-bit timer */
                        AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK); /* the SCT clock is system clock          */
    
    /* Configure the match/capture reg 0 operate as match register */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                                AMHW_LPC_SCT_MODE_UNIFY,    /* the SCT mode is UNIFY(1 32-bit counter)  */
                                0,                      /* select the match/capture  0 to configure */
                                AMHW_LPC_SCT_MATCAP_MATCH); /* operate as match register                */
    
    /* CLear the match 0 and match reload value */
    amhw_lpc_sct_mat_val_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, AMHW_LPC_SCT_MAT(0), 0);
    amhw_lpc_sct_mat_reload_val_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, AMHW_LPC_SCT_MAT(0), 0);
    
    /* set the event 0 control */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            AMHW_LPC_SCT_EVT(0),                  /* event 0 */
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(0)    | /* the event associated with the match 0 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH | /* Uses the specified match only.        */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD    | /*  STATEV value is added into STATE     */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));      /* ADD 0,the state will not change       */
                   
    /* set the now state value is 0 */
    amhw_lpc_sct_state_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
    
    /* enable the event 0 happen in state 0 */
    amhw_lpc_sct_event_state_enable(p_hw_sct, AMHW_LPC_SCT_EVT(0), AMHW_LPC_SCT_STATE(0));
    
    /* enable the event 0 as the counter limit */
    amhw_lpc_sct_event_limit_enable(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, AMHW_LPC_SCT_EVT(0));
    
    /* enable the event 0 generate interrupt */
    amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(0));

    /* Set the CLRCTR bit to clear the counter to 0 */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);
    
    /* Set the prescaler value to 0. use the system clock,not prescale */
    amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
}


/* 
 * The event0 associated with match0 _L 16bits, _L 16bits timer
 * The event1 associated with match0 _H 16bits, _H 16bits timer
 */
static void __sct_timing_2_16bit_init(amhw_lpc_sct_t *p_hw_sct)
{
    amhw_lpc_sct_config(p_hw_sct,
                        AMHW_LPC_SCT_CONFIG_16BIT_COUNTER  | /* the timer is operate as 2 16-bit timer */
                        AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK); /* the SCT clock is system clock */

    /* configure the match/capture reg 0 _L / or _H 16bits operate as match register */
    amhw_lpc_sct_regmode_config(p_hw_sct, 
                                AMHW_LPC_SCT_MODE_SEPARATE_L,
                                AMHW_LPC_SCT_MAT(0),
                                AMHW_LPC_SCT_MATCAP_MATCH);
    
    amhw_lpc_sct_regmode_config(p_hw_sct, 
                                AMHW_LPC_SCT_MODE_SEPARATE_H,
                                AMHW_LPC_SCT_MAT(0),
                                AMHW_LPC_SCT_MATCAP_MATCH);
    
    /* The L timer use the event 0, set the event 1 control */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            AMHW_LPC_SCT_EVT(0),
                            AMHW_LPC_SCT_EV_CTRL_HEVENT_L       |
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(0)    | /* the event associated with the match 0 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH | /* Uses the specified match only.        */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD    | /*  STATEV value is added into STATE     */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));      /* ADD 0,the state will not change       */

    /* The H timer use the event 1, set the event 1 control */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            AMHW_LPC_SCT_EVT(1),
                            AMHW_LPC_SCT_EV_CTRL_HEVENT_H       |
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(0)    | /* the event associated with the match 0 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH | /* Uses the specified match only.        */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD    | /*  STATEV value is added into STATE     */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));      /* ADD 0,the state will not change       */

    /* set the now state value is 0 */
    amhw_lpc_sct_state_set(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_L, AMHW_LPC_SCT_STATE(0));
    amhw_lpc_sct_state_set(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_H, AMHW_LPC_SCT_STATE(0));
    
    /* enable the event happen in state 0 */
    amhw_lpc_sct_event_state_enable(p_hw_sct, AMHW_LPC_SCT_EVT(0), AMHW_LPC_SCT_STATE(0));
    amhw_lpc_sct_event_state_enable(p_hw_sct, AMHW_LPC_SCT_EVT(1), AMHW_LPC_SCT_STATE(0));

    /* enable the event as the counter limit */
    amhw_lpc_sct_event_limit_enable(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_L, AMHW_LPC_SCT_EVT(0));
    amhw_lpc_sct_event_limit_enable(p_hw_sct, AMHW_LPC_SCT_MODE_SEPARATE_H, AMHW_LPC_SCT_EVT(1));
    
    /* enable the event generate interrupt */
    amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(0));
    amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(1));

    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_H);
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);
}

am_timer_handle_t am_lpc_sct_timing_init (am_lpc_sct_timing_dev_t           *p_dev,
                                          const am_lpc_sct_timing_devinfo_t *p_devinfo)
{
    amhw_lpc_sct_t *p_hw_sct = NULL;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }
    

    p_dev->p_devinfo          = p_devinfo;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_sct_timing_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev; 

    p_hw_sct = (amhw_lpc_sct_t *)(p_devinfo->sct_regbase);

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* Operate as one 32-bit timer */
    if (p_devinfo->flag == AM_LPC_SCT_TIMING_1_32BIT) {
        __sct_timing_1_32bit_init(p_hw_sct);
        
        p_dev->callback_info[0].pfn_callback = NULL;
        p_dev->callback_info[0].p_arg        = NULL;
    } else {
        __sct_timing_2_16bit_init(p_hw_sct);
        
        p_dev->callback_info[0].pfn_callback = NULL;
        p_dev->callback_info[0].p_arg        = NULL;
    
        p_dev->callback_info[1].pfn_callback = NULL;
        p_dev->callback_info[1].p_arg        = NULL;
    }
    
    am_int_connect(p_dev->p_devinfo->inum, __sct_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->timer_serv);
}

void am_lpc_sct_timing_deinit (am_lpc_sct_timing_dev_t *p_dev)
{
    amhw_lpc_sct_t      *p_hw_sct = (amhw_lpc_sct_t *)(p_dev->p_devinfo->sct_regbase);

    if (p_dev == NULL) {
      return;
    }
 
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
    
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __sct_irq_handler, (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
