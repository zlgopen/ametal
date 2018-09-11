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
 * \brief RIT implementation for Standard Timer interface 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-17  tee, first implementation.
 * \endinternal
 */

#include "am_lpc_rit.h"
#include "am_clk.h"
#include "am_int.h"


/*******************************************************************************
* Functions declaration
*******************************************************************************/

/* lpc5410x RIT for timing drv functions declaration */
//static int __rit_info_get(void *p_drv, am_timer_info_t *p_info);
static const am_timer_info_t *__rit_info_get (void *p_drv);


static int __rit_clkin_freq_get (void *, uint32_t *);
static int __rit_prescale_set (void *, uint8_t, uint32_t );
static int __rit_prescale_get (void *, uint8_t, uint32_t *);

static int __rit_count_get64 (void     *p_drv,
                              uint8_t   chan,
                              void     *p_count);

static int __rit_rollover_get64 (void     *p_drv,
                                 uint8_t   chan,
                                 void     *p_rollover);
                                     
static int __rit_disable (void *p_drv, uint8_t chan);

static int __rit_enable64 (void     *p_drv,
                           uint8_t   chan,
                           void     *max_timer_count);
    
static int __rit_callback_set (void    *p_drv,
                               uint8_t  chan,
                               void   (*pfn_callback)(void *),
                               void    *p_arg);
#if 0
static int  __rit_connect(void *p_drv);
#endif

static void __timer_irq_handler(void *p_arg);
    
static const am_timer_info_t __g_rit_info = {
    48,
    1,
    AM_TIMER_CAN_INTERRUPT      |
    AM_TIMER_CANNOT_DISABLE     |
    AM_TIMER_INTERMEDIATE_COUNT ,
    0
};

/** \brief timer for timing driver functions */
static const struct am_timer_drv_funcs __g_rit_drv_funcs = {
    __rit_info_get,
	__rit_clkin_freq_get,
	__rit_prescale_set,
	__rit_prescale_get,
	__rit_count_get64,
	__rit_rollover_get64,
	__rit_enable64,
	__rit_disable,
	__rit_callback_set,
};

/******************************************************************************/

#if 0
int __rit_connect (void *p_drv)
{
    am_lpc_rit_dev_t *p_dev = (am_lpc_rit_dev_t *)p_drv;
    
    am_int_connect(p_dev->p_devinfo->inum, __timer_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    
    return AM_OK;
}
#endif

void __timer_irq_handler (void *p_arg)
{
    am_lpc_rit_dev_t *p_dev    = (am_lpc_rit_dev_t *)p_arg;
    amhw_lpc_rit_t     *p_hw_rit =  (amhw_lpc_rit_t *)p_dev->p_devinfo->rit_regbase;

    /* 
     * Because use the match 0 to generate interrupt
     * So,Check if the match 0 interrupt 
     */
    if (amhw_lpc_rit_int_flag_check(p_hw_rit) == AM_TRUE) {
        
        if (p_dev->callback_info[0].pfn_callback != NULL) {
            p_dev->callback_info[0].pfn_callback(p_dev->callback_info[0].p_arg);
        }
        
        /* clear the match 0 interrupt flag */
        amhw_lpc_rit_int_flag_clr(p_hw_rit);
    }
}

static const am_timer_info_t * __rit_info_get (void *p_drv)
{
    return &__g_rit_info;
}



static int __rit_clkin_freq_get (void *p_drv, uint32_t *p_freq)
{
    am_lpc_rit_dev_t *p_dev = (am_lpc_rit_dev_t *)p_drv;

    if ((p_dev == NULL) || (p_freq == NULL)) {
        return -AM_EINVAL;
    }

    *p_freq =  am_clk_rate_get(p_dev->p_devinfo->clk_id);

    return AM_OK;
}

static int __rit_prescale_set (void    *p_drv,
                               uint8_t  chan,
                               uint32_t prescale)
{
    if (prescale == 1) {
       return AM_OK;
    }
    return AM_EINVAL;
}

static int __rit_prescale_get (void     *p_drv,
                               uint8_t   chan,
                               uint32_t *prescale)
{

    if ((p_drv == NULL) || (prescale == NULL)) {
        return -AM_EINVAL;
    }
    *prescale = 1;
     return AM_OK;
}

/** \brief Get the timer information */
//static int __rit_info_get (void *p_drv, am_timer_info_t *p_info)
//{
//    am_lpc_rit_dev_t *p_dev    = (am_lpc_rit_dev_t *)p_drv;
//
//    /* Get the input frequency */
//    p_info->clk_frequency = am_clk_rate_get(p_dev->p_devinfo->clk_id);
//    p_info->min_frequency = 1;                        /* The minimum freq is 1Hz     */
//    p_info->max_frequency = p_info->clk_frequency;
//
//    p_info->features  = AM_TIMER_SIZE(48)           | /* 48-bit timer             */
//                        AM_TIMER_CHAN_NUM(1)        | /* support 1 channels       */
//                        AM_TIMER_CAN_INTERRUPT      | /* Can generate interrupt   */
//                        AM_TIMER_INTERMEDIATE_COUNT | /* can read now count       */
//                        AM_TIMER_AUTO_RELOAD;         /* Auto reload              */
//
//    return AM_OK;
//}

/** \brief Get the timer Now counter value */ 
static int __rit_count_get64 (void       *p_drv,
                              uint8_t   chan,
                              void     *p_count)
{
    am_lpc_rit_dev_t *p_dev    = (am_lpc_rit_dev_t *)p_drv;
    amhw_lpc_rit_t   *p_hw_rit = (amhw_lpc_rit_t *)p_dev->p_devinfo->rit_regbase;
    
    if (chan != 0) {
        return -AM_EINVAL;
    }
    
    *(uint64_t *)p_count = amhw_lpc_rit_count_get(p_hw_rit);

    return AM_OK;
}

/** \brief The timer roll over value get */
static int __rit_rollover_get64 (void     *p_drv,
                                 uint8_t   chan,
                                 void     *p_rollover)
{
    am_lpc_rit_dev_t *p_dev    = (am_lpc_rit_dev_t *)p_drv;
    
    if (chan != 0) {
        return -AM_EINVAL;
    }
    
    *(uint64_t *)p_rollover = 0xFFFFFFFFFFFFull / am_clk_rate_get(p_dev->p_devinfo->clk_id);
    
    return AM_OK;
}

/** \brief The timer disable */
static int __rit_disable (void *p_drv, uint8_t chan)
{
    am_lpc_rit_dev_t *p_dev    = (am_lpc_rit_dev_t *)p_drv;
    amhw_lpc_rit_t     *p_hw_rit =  (amhw_lpc_rit_t *)p_dev->p_devinfo->rit_regbase;
    
    if (chan != 0) {
        return -AM_EINVAL;
    }
    
    amhw_lpc_rit_disable(p_hw_rit);
    
    amhw_lpc_rit_count_set(p_hw_rit, 0);
    
    /* clear the RIT interrupt flag */
    amhw_lpc_rit_int_flag_clr(p_hw_rit);

    return AM_OK;
}

/** \brief The timer enable and set the maximum count value */
static int __rit_enable64 (void     *p_drv, 
                           uint8_t   chan,
                           void     *p_count)
{
    am_lpc_rit_dev_t *p_dev    = (am_lpc_rit_dev_t *)p_drv;
    amhw_lpc_rit_t     *p_hw_rit =  (amhw_lpc_rit_t *)p_dev->p_devinfo->rit_regbase;
    
    uint64_t max_timer_count = *(uint64_t *)p_count;
    if (chan != 0) {
        return -AM_EINVAL;
    }
    
    amhw_lpc_rit_mask_set(p_hw_rit, 0);     /* No mask */
    amhw_lpc_rit_count_set(p_hw_rit, 0);
    amhw_lpc_rit_compval_set(p_hw_rit, max_timer_count - 1);
    amhw_lpc_rit_clear_enable(p_hw_rit);

    amhw_lpc_rit_enable(p_hw_rit);
    
    return AM_OK;
}

/** \brief set the function to be called on every interrupt */
static int __rit_callback_set (void   *p_drv,
                               uint8_t chan,
                               void  (*pfn_callback)(void *),
                               void   *p_arg)
{
    am_lpc_rit_dev_t *p_dev    = (am_lpc_rit_dev_t *)p_drv;
    amhw_lpc_rit_t     *p_hw_rit =  (amhw_lpc_rit_t *)p_dev->p_devinfo->rit_regbase;
    
    if (chan != 0) {
        return -AM_EINVAL;
    }
    
    p_dev->callback_info[0].pfn_callback = pfn_callback;
    p_dev->callback_info[0].p_arg   = p_arg;

    /* clear the RIT interrupt flag */
    amhw_lpc_rit_int_flag_clr(p_hw_rit);
 
    return AM_OK;
   
}

am_timer_handle_t am_lpc_rit_init (am_lpc_rit_dev_t           *p_dev,
                                 const am_lpc_rit_devinfo_t *p_devinfo)
{
    amhw_lpc_rit_t *p_hw_rit = NULL;

    if (p_devinfo == NULL || p_dev == NULL) {
        return NULL;
    }
    
    p_hw_rit                   = (amhw_lpc_rit_t *)p_devinfo->rit_regbase;
    p_dev->p_devinfo           = p_devinfo;
    p_dev->timer_serv.p_funcs  = (struct am_timer_drv_funcs *)&__g_rit_drv_funcs;
    p_dev->timer_serv.p_drv    = p_dev; 
    
    p_dev->callback_info[0].pfn_callback = NULL;
    p_dev->callback_info[0].p_arg        = NULL;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* disable the RIT,default(after reset) is enable */
    amhw_lpc_rit_disable(p_hw_rit);
    
    amhw_lpc_rit_clear_enable(p_hw_rit);
    
    /* 
     * Enable for debug,The timer is halted when the processor is halted for debugging 
     */
    amhw_lpc_rit_debug_enable(p_hw_rit);
    
    am_int_connect(p_dev->p_devinfo->inum, __timer_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->timer_serv);
}
                                     

void am_lpc_rit_deinit (am_lpc_rit_dev_t *p_dev)
{
    amhw_lpc_rit_t     *p_hw_rit =  (amhw_lpc_rit_t *)p_dev->p_devinfo->rit_regbase;
    
    if (p_dev == NULL) {
    	return;
    }

    p_dev->timer_serv.p_funcs  = NULL;
    p_dev->timer_serv.p_drv    = NULL; 
    
    /* disable the RIT */
    amhw_lpc_rit_disable(p_hw_rit);
    
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __timer_irq_handler, (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }
}

/* end of file */
