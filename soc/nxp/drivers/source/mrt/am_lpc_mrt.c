/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2012 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief MRT(Multi-Rate Timer) drivers implementation
 *
 * The MRT is 24 bit timer with 4 channels
 *
 * \internal
 * \par History
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */
 

#include "am_lpc_mrt.h"
#include "am_int.h"
#include "am_clk.h"


/*******************************************************************************
* Functions declaration
*******************************************************************************/

/* LPC5410x MRT driver functions declaration */
static const am_timer_info_t * __mrt_info_get (void *p_drv);

static int __mrt_clkin_freq_get (void *, uint32_t *);
static int __mrt_prescale_set (void *, uint8_t, uint32_t );
static int __mrt_prescale_get (void *, uint8_t, uint32_t *);


static int __mrt_count_get(void *p_drv, uint8_t chan, void *p_count);

static int __mrt_rollover_get(void *p_drv, uint8_t chan, void *p_rollover);

static int __mrt_disable(void *p_drv, uint8_t chan);

static int __mrt_enable(void *p_drv, uint8_t chan, void  *max_timer_count);
    
static int __mrt_callback_set(void    *p_drv,
                              uint8_t  chan,
                              void   (*pfn_callback)(void *),
                              void    *p_arg);
#if 0
static int __mrt_connect(void *p_drv);
#endif
 
                              
/* 定时器信息 */
static const am_timer_info_t __g_mrt_timing_info = {
    24,                                      /* 24位定时器               */
    4,                                       /* 4通道                   */
    AM_TIMER_CAN_INTERRUPT      |            /* 可以产生中断             */
    AM_TIMER_AUTO_RELOAD        |            /* 支持自动重载             */
    AM_TIMER_INTERMEDIATE_COUNT ,            /* 当前计数器值可读         */
    0                                    /* 预分频最大值：65536      */
};

/** \brief sio driver functions */
static const struct am_timer_drv_funcs __g_mrt_drv_funcs = {
    __mrt_info_get,
    __mrt_clkin_freq_get,
    __mrt_prescale_set,
    __mrt_prescale_get,
    __mrt_count_get,
    __mrt_rollover_get,
    __mrt_enable,
    __mrt_disable,
    __mrt_callback_set,
};

/******************************************************************************/

#if 0
int __mrt_connect (void *p_drv)
{
    am_lpc_mrt_dev_t *p_dev = (am_lpc_mrt_dev_t *)p_drv;

    am_int_connect(p_dev->p_devinfo->inum, __mrt_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}
#endif

/** 
 * \brief Multi-Rate Timer interrupt request handler
 * \param[in]  p_arg  The user data setting in am_int_connect().
 * \return None
  */
static void __mrt_irq_handler (void *p_arg)
{
    am_lpc_mrt_dev_t *p_dev    = (am_lpc_mrt_dev_t *)p_arg;
    amhw_lpc_mrt_t   *p_hw_mrt = (amhw_lpc_mrt_t *)p_dev->p_devinfo->mrt_regbase;
    
    uint8_t i = 0;
    uint8_t channel_nums = p_dev->p_devinfo->channel_nums;
    
    for (i = 0; i < channel_nums; i++) {
        
        if (amhw_lpc_mrt_int_pending_get(p_hw_mrt, AMHW_LPC_MRT_CH(i)) == AM_TRUE) {

            if (p_dev->callback_info[i].pfn_callback != NULL) {
                p_dev->callback_info[i].pfn_callback(p_dev->callback_info[i].p_arg);
            }
            
            /* clear the interrupt pending flag */
            amhw_lpc_mrt_int_pending_clr(p_hw_mrt, AMHW_LPC_MRT_CH(i));
        }
    }
}


static int __mrt_clkin_freq_get (void *p_drv, uint32_t *p_freq)
{
    am_lpc_mrt_dev_t *p_dev        = (am_lpc_mrt_dev_t *)p_drv;
    
      if ((p_dev == NULL) || (p_freq == NULL)) {
        return -AM_EINVAL;
    }
      
    /* Get the input frequency */
    *p_freq = am_clk_rate_get(p_dev->p_devinfo->clk_id);

    return AM_OK;
}

static int __mrt_prescale_set (void    *p_drv, 
                                        uint8_t  chan, 
                                        uint32_t prescale)
{
    if (prescale == 1) {
       return AM_OK;
    }
    return AM_EINVAL;
}

static int __mrt_prescale_get (void     *p_drv, 
                               uint8_t   chan, 
                               uint32_t *prescale)
{
  
    if ((p_drv == NULL) || (prescale == NULL)) {
        return -AM_EINVAL;
    }
    *prescale = 1;
     return AM_OK; 
}

static const am_timer_info_t * __mrt_info_get (void *p_drv)
{
    return &__g_mrt_timing_info;
}
/** 
 * \brief Get the timer information 
 */
//static int __mrt_info_get (void *p_drv, am_timer_info_t *p_info)
//{
//    am_lpc_mrt_dev_t *p_dev        = (am_lpc_mrt_dev_t *)p_drv;
//    uint8_t         channel_nums = p_dev->p_devinfo->channel_nums;
//    
//    /* Get the input frequency */
//    p_info->clk_frequency = am_clk_rate_get(p_dev->p_devinfo->clk_id);
//    
//    if (p_info->clk_frequency <= 0xFFFFFF) {
//        p_info->min_frequency = 1;
//    } else {
//        p_info->min_frequency = p_info->clk_frequency / 0xFFFFFF;
//        
//        if (p_info->min_frequency * 0xFFFFFF < p_info->clk_frequency) {
//            p_info->min_frequency += 1;
//        }
//    }
//    
//    p_info->max_frequency = p_info->clk_frequency;
//    
//    p_info->features      = AM_TIMER_SIZE(24)               | /* 24-bit timer             */
//                            AM_TIMER_CHAN_NUM(channel_nums) | /* support 4 channels       */
//                            AM_TIMER_CAN_INTERRUPT          | /* Can generate interrupt   */
//                            AM_TIMER_INTERMEDIATE_COUNT     | /* can read now count       */
//                            AM_TIMER_AUTO_RELOAD;             /* Auto reload              */
//    
//    return AM_OK;
//}

/** \brief Get the timer Now counter value */ 
static int __mrt_count_get (void *p_drv, uint8_t chan, void *p_count)
{
    am_lpc_mrt_dev_t *p_dev        = (am_lpc_mrt_dev_t *)p_drv;
    amhw_lpc_mrt_t   *p_hw_mrt     = (amhw_lpc_mrt_t *)p_dev->p_devinfo->mrt_regbase;
    uint8_t         channel_nums = p_dev->p_devinfo->channel_nums;
    uint32_t        tmp;
    
    if (chan >= channel_nums) {
        return -AM_EINVAL;
    }
    
    /* Holds the current timer value of the down-counter */
    tmp = amhw_lpc_mrt_chan_count_get(p_hw_mrt, chan);
    
    /* Convert the value to as up-counter */
    *(uint32_t *)p_count = amhw_lpc_mrt_intval_get(p_hw_mrt, chan) - tmp;
    
    return AM_OK;
}

/** \brief The timer roll over value get */
static int __mrt_rollover_get (void *p_drv, uint8_t chan, void *p_rollover)
{
    am_lpc_mrt_dev_t *p_dev        = (am_lpc_mrt_dev_t *)p_drv;
    uint8_t         channel_nums = p_dev->p_devinfo->channel_nums;
    
    if (chan >= channel_nums) {
        return -AM_EINVAL;
    }
    
    /* 24-bit max value is 0x00FFFFFF */
    *(uint32_t *)p_rollover = AMHW_LPC_MRT_INTVAL_MAX / am_clk_rate_get(p_dev->p_devinfo->clk_id);
    
    return AM_OK;
}

/** \brief The timer disable */
static int __mrt_disable (void *p_drv, uint8_t chan)
{
    am_lpc_mrt_dev_t *p_dev        = (am_lpc_mrt_dev_t *)p_drv;
    amhw_lpc_mrt_t   *p_hw_mrt     = (amhw_lpc_mrt_t *)p_dev->p_devinfo->mrt_regbase;
    uint8_t         channel_nums = p_dev->p_devinfo->channel_nums;
    
    if (chan >= channel_nums) {
        return -AM_EINVAL;
    }
    
    amhw_lpc_mrt_intval_set(p_hw_mrt, chan, 0, AM_TRUE);
    
    amhw_lpc_mrt_int_flag_clr(p_hw_mrt, chan);
    amhw_lpc_mrt_int_pending_clr(p_hw_mrt, chan);
    
    amhw_lpc_mrt_int_disable(p_hw_mrt, chan);
    
    return AM_OK;
}

/** \brief The timer enable and set the maximum count value */
static int __mrt_enable (void *p_drv, uint8_t chan, void  *max_timer_count)
{
    am_lpc_mrt_dev_t *p_dev       = (am_lpc_mrt_dev_t *)p_drv;
    amhw_lpc_mrt_t   *p_hw_mrt    = (amhw_lpc_mrt_t *)p_dev->p_devinfo->mrt_regbase;
    uint8_t         channel_nums  = p_dev->p_devinfo->channel_nums;
    
    if (chan >= channel_nums) {
        return -AM_EINVAL;
    }
    amhw_lpc_mrt_int_disable(p_hw_mrt, chan);
    amhw_lpc_mrt_intval_set(p_hw_mrt, chan, *((uint32_t *)max_timer_count), AM_TRUE);
    
    if (p_dev->callback_info[chan].pfn_callback == NULL) {
        amhw_lpc_mrt_int_disable(p_hw_mrt, chan);
    } else {
        amhw_lpc_mrt_int_enable(p_hw_mrt, chan);
    }
    
    return AM_OK;
}

/** \brief set the function to be called on every interrupt */
static int __mrt_callback_set (void   *p_drv,
                               uint8_t chan,
                               void   (*pfn_callback)(void *),
                               void   *p_arg)
{
    am_lpc_mrt_dev_t *p_dev        = (am_lpc_mrt_dev_t *)p_drv;
    amhw_lpc_mrt_t   *p_hw_mrt     = (amhw_lpc_mrt_t *)p_dev->p_devinfo->mrt_regbase;
    uint8_t         channel_nums = p_dev->p_devinfo->channel_nums;
    
    if (chan >= channel_nums) {
        return -AM_EINVAL;
    }
    
    /* needn't interrupt, close the channel interrupt */
    if (pfn_callback == NULL) {
        amhw_lpc_mrt_int_disable(p_hw_mrt, chan);
        
    } else {
        p_dev->callback_info[chan].pfn_callback = pfn_callback;
        p_dev->callback_info[chan].p_arg   = p_arg;
        
        amhw_lpc_mrt_int_enable(p_hw_mrt, chan);
    }

    /* Interrupt connection */
    am_int_connect(p_dev->p_devinfo->inum, __mrt_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}


am_timer_handle_t am_lpc_mrt_init (am_lpc_mrt_dev_t           *p_dev,
                                   const am_lpc_mrt_devinfo_t *p_devinfo)
{
    amhw_lpc_mrt_t *p_hw_mrt;
    uint8_t     i;
    uint8_t     channel_nums;
    
    if (p_devinfo == NULL || p_dev == NULL) {
        return NULL;
    }

    channel_nums = p_devinfo->channel_nums;
    
    /* The max channel nus is AMHW_LPC_MRT_CHANNELS_NUM */
    if (channel_nums > AMHW_LPC_MRT_CHANNELS_NUM) {
        return NULL;
    }
    
    p_hw_mrt                  = (amhw_lpc_mrt_t *)p_devinfo->mrt_regbase;
    p_dev->p_devinfo          = p_devinfo;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_mrt_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev; 
    
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    for (i = 0; i < channel_nums; i++) {
        
        p_dev->callback_info[i].pfn_callback = NULL;
        p_dev->callback_info[i].p_arg        = NULL;

        amhw_lpc_mrt_mode_set(p_hw_mrt, i, AMHW_LPC_MRT_MODE_REPEAT);
    }   

    return &(p_dev->timer_serv);
}
                                     

void am_lpc_mrt_deinit (am_lpc_mrt_dev_t *p_dev)
{

    amhw_lpc_mrt_t *p_hw_mrt = NULL;
    uint8_t     i;
    uint8_t     channel_nums;
    
    if (p_dev == NULL) {
      return;
    }

    channel_nums = p_dev->p_devinfo->channel_nums;

    p_hw_mrt = (amhw_lpc_mrt_t *)p_dev->p_devinfo->mrt_regbase;
 
    p_dev->timer_serv.p_funcs = NULL;
    p_dev->timer_serv.p_drv   = NULL; 
    
    for (i = 0; i < channel_nums; i++) {
        
        p_dev->callback_info[i].pfn_callback = NULL;
        p_dev->callback_info[i].p_arg        = NULL;
        
        amhw_lpc_mrt_intval_set(p_hw_mrt, i, 0, AM_TRUE);

        amhw_lpc_mrt_int_flag_clr(p_hw_mrt, i);
        amhw_lpc_mrt_int_pending_clr(p_hw_mrt, i);

        amhw_lpc_mrt_int_disable(p_hw_mrt, i);
    }   
    
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __mrt_irq_handler, (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
