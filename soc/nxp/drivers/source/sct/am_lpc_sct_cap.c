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
 * \brief Capture driver implementation for SCT
 *
 * 1. The SCT can provide 3 service as follows,This driver implement 
 *    the capture function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The SCT resource used as follows:
 *    - The capture 0 to capture SCT input 0,associate with event 0
 *    - The capture 1 to capture SCT input 1,associate with event 1
 *    - The capture 2 to capture SCT input 2,associate with event 2
 *    - The capture 3 to capture SCT input 3,associate with event 3
 *    - The capture 4 to capture SCT input 4,associate with event 4
 *    - The capture 5 to capture SCT input 5,associate with event 5
 *    - The capture 6 to capture SCT input 6,associate with event 6
 *    - The capture 7 to capture SCT input 7,associate with event 7
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-14  tee, first implementation.
 * \endinternal
 */

#include "am_lpc_sct_cap.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_clk.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

/** \brief Configure the PWM  */
static int __sct_cap_config(void              *p_cookie,
                            int                chan,
                            unsigned int       flags,
                            am_cap_callback_t  pfn_callback,
                            void              *p_arg);

/** \brief Enable the PWM OUTPUT */
static int __sct_cap_enable(void *p_drv, int chan);

/** \brief Disable the PWM OUTPUT */
static int __sct_cap_disable(void *p_drv, int chan);
                                 
/** \brief Disable the capture channel */
static int __sct_cap_reset(void *p_drv, int chan);

static int __sct_cap_count_to_time (void         *p_drv,
                                    int           chan,
                                    unsigned int  count1,
                                    unsigned int  count2,
                                    unsigned int *p_time_ns);
                                    
static void __sct_cap_irq_handler(void *p_arg);

/** \brief SCT for PWMs driver functions */
static const struct am_cap_drv_funcs __g_sct_cap_drv_funcs = {
    __sct_cap_config,
    __sct_cap_enable,
    __sct_cap_disable,
    __sct_cap_reset,
    __sct_cap_count_to_time,
};

/******************************************************************************/

/** \brief Configure the PWM  */
static int __sct_cap_config (void              *p_drv,
                             int                chan,
                             unsigned int       options,
                             am_cap_callback_t  pfn_callback,
                             void              *p_arg)
{
    am_lpc_sct_cap_dev_t *p_dev    = (am_lpc_sct_cap_dev_t *)p_drv;
    amhw_lpc_sct_t       *p_hw_sct = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    uint32_t            iocond;
    
    /* The valid channel num is 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /*  Support rising edge */
    if ((options & AM_CAP_TRIGGER_RISE) == AM_CAP_TRIGGER_RISE) {
        iocond = AMHW_LPC_SCT_EV_CTRL_IOCOND_RISE;
    }
    
    if ((options & AM_CAP_TRIGGER_FALL) == AM_CAP_TRIGGER_FALL) {
        iocond = AMHW_LPC_SCT_EV_CTRL_IOCOND_FALL;
    }
    
    if ((options & AM_CAP_TRIGGER_BOTH_EDGES) == AM_CAP_TRIGGER_BOTH_EDGES) {
        return -AM_ENOTSUP; /* Not support both edge */
    }

    /* Configure the match number register operate as match register */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                                AMHW_LPC_SCT_MODE_UNIFY,      /* the SCT mode is UNIFY(1 32-bit counter) */
                                AMHW_LPC_SCT_MAT(chan),
                                AMHW_LPC_SCT_MATCAP_CAPTURE); /* operate as capture register             */
    
    /* Enable the event causes the CAPn _L (UNIFY = 0) register to be loaded */
    amhw_lpc_sct_cap_ctrl(p_hw_sct,
                          AMHW_LPC_SCT_MODE_UNIFY,
                          AMHW_LPC_SCT_CAP(chan),
                          AMHW_LPC_SCT_EVT(chan),
                          AM_TRUE);
    
    /*
     * Use the event number same with the channel number 
     * The event use only the IO condition,not use any match register 
     */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            AMHW_LPC_SCT_EVT(chan),
                            AMHW_LPC_SCT_EV_CTRL_OUTSEL_INPUT |
                            AMHW_LPC_SCT_EV_CTRL_IOSEL(chan)  |
                            iocond                            |
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_IO  | /* Uses the specified IO only */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD  | /* STATEV value is added into STATE     */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));    /* ADD 0,the state will not change      */
    
    p_dev->callback_info[chan].callback_func = pfn_callback;
    p_dev->callback_info[chan].p_arg    = p_arg;
    
    return AM_OK;
}

/** \brief Enable the PWM OUTPUT */
static int __sct_cap_enable (void *p_drv, int chan)
{
    am_lpc_sct_cap_dev_t    *p_dev    = (am_lpc_sct_cap_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    am_lpc_sct_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;
    
    /* The valid channel number is 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /* The channel 6 and channel 7 use the internal signal*/
    if (chan != 6 && chan != 7) {
        am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].func);
    }

    /* Enable the event0 happen in state 0 */
    amhw_lpc_sct_event_state_enable(p_hw_sct, AMHW_LPC_SCT_EVT(chan), AMHW_LPC_SCT_STATE(0));

    /* Enable the event0 generate interrupt */
    amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(chan));
    
    amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
    
    am_int_connect(p_dev->p_devinfo->inum, __sct_cap_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    
    return AM_OK;
}

/** \brief Disable the PWM OUTPUT */
static int __sct_cap_disable (void *p_drv, int chan)
{
    am_lpc_sct_cap_dev_t    *p_dev    = (am_lpc_sct_cap_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    am_lpc_sct_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;
 
    int i;
    
    /* The valid channel number is 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /* The channel 6 and channel 7 use the internal signal*/
    if (chan != 6 && chan != 7) {
        am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].dfunc);
    }
    
    /* Disable the event happen in state 0 */
    amhw_lpc_sct_event_state_disable(p_hw_sct, AMHW_LPC_SCT_EVT(chan), AMHW_LPC_SCT_STATE(0));
    
    amhw_lpc_sct_event_int_disable(p_hw_sct, AMHW_LPC_SCT_EVT(chan));

    i = p_dev->p_devinfo->channels_num;
    
    for (i = p_dev->p_devinfo->channels_num - 1; i >= 0; i--) {
        if (p_hw_sct->event[i].state != 0) {  /* there are event to happen */
            return AM_OK;
        }
    }
    
    /** If all channels disabled the capture,disable the SCT */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
    
    return AM_OK;
}

/** \brief Disable the capture channel  */
static int __sct_cap_reset (void *p_drv, int chan)
{
    am_lpc_sct_cap_dev_t *p_dev    = (am_lpc_sct_cap_dev_t *)p_drv;
    amhw_lpc_sct_t       *p_hw_sct = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    
    if (amhw_lpc_sct_halt_check(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY) == AM_TRUE) {
    
        /* Clear the counter */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);
    } else {
    
        /* Set the halt bits,the SCT is stop */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_HALT_L);
        
        /* Clear the counter */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);
        
        amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_HALT_L);
    }
    
    return AM_OK;
}

static int __sct_cap_count_to_time (void         *p_drv,
                                    int           chan,
                                    unsigned int  count1,
                                    unsigned int  count2,
                                    unsigned int *p_time_ns)
{
    am_lpc_sct_cap_dev_t *p_dev    = (am_lpc_sct_cap_dev_t *)p_drv;
    uint32_t              clkfreq  = am_clk_rate_get(p_dev->p_devinfo->clk_id);
    uint32_t              count_err;
    uint64_t              time_ns;
    
    count_err = count2 - count1;

    /* convert the count err value to time  */
    time_ns = (uint64_t)1000000000 * (uint64_t)count_err / (uint64_t)clkfreq ;
    
    *p_time_ns = time_ns;
    
    return AM_OK;
}

static void __sct_cap_irq_handler (void *p_arg)
{
    am_lpc_sct_cap_dev_t *p_dev     = (am_lpc_sct_cap_dev_t *)p_arg;
    amhw_lpc_sct_t       *p_hw_sct  = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    uint8_t               chans_num = p_dev->p_devinfo->channels_num;
    
    uint8_t               i;
    am_cap_callback_t     callback_func;
    uint32_t              value;
    
    for (i = 0; i < chans_num; i++) {
        if (amhw_lpc_sct_event_flag_chk(p_hw_sct, AMHW_LPC_SCT_EVT(i)) == AM_TRUE ) {
            
            callback_func = p_dev->callback_info[i].callback_func;
            value = amhw_lpc_sct_cap_val_get(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, i);
            
            if (callback_func != NULL) {
                callback_func(p_dev->callback_info[i].p_arg, value);
            }
            
            /* Clear the event i flag */
            amhw_lpc_sct_event_flag_clr(p_hw_sct, AMHW_LPC_SCT_EVT(i));
            
        }
    }
}

/** 
  * \brief Initialize the SCT to prepare for output PWMs
  *
  *  Initialize the SCT operate as 1-32bit timer,and enable MATCH0 for auto limit.
  *  
  * \param[in] p_hw_sct : Pointer to the SCT to be initialized for output PWMs
  * \return None
  */
static  void __sct_cap_init (amhw_lpc_sct_t *p_hw_sct)
{
    /* Set the halt and stop bits,the SCT is stop */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
    
    amhw_lpc_sct_config(p_hw_sct,
                        AMHW_LPC_SCT_CONFIG_32BIT_COUNTER  |
                        AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK |
                        AMHW_LPC_SCT_CONFIG_INSYNC_ALL);
  
    /* Set the CLRCTR bit to clear the counter to 0 */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);
    
    /* Set the now state value is 0 */
    amhw_lpc_sct_state_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, AMHW_LPC_SCT_STATE(0));
    
    /* Set the prescale to 0. use the system clock,not prescale */
    amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
}

am_cap_handle_t am_lpc_sct_cap_init (am_lpc_sct_cap_dev_t           *p_dev,
                                     const am_lpc_sct_cap_devinfo_t *p_devinfo)
{
    amhw_lpc_sct_t *p_hw_sct;
    uint8_t    i;
    
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }
    
    /* The SCT support channels number is 8 */
    if (p_devinfo->channels_num > 8) {
        return NULL;
    }
    
    p_hw_sct                = (amhw_lpc_sct_t *)p_devinfo->sct_regbase;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->cap_serv.p_funcs = (struct am_cap_drv_funcs *)&__g_sct_cap_drv_funcs;
    p_dev->cap_serv.p_drv   = p_dev;
    
    for (i = 0; i < p_devinfo->channels_num; i++) {
        p_dev->callback_info[i].callback_func = NULL;
    }
    
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* Reset the SCT counter and pre-scale counts to 0 */
    __sct_cap_init(p_hw_sct);
    
    return &(p_dev->cap_serv);
}

void am_lpc_sct_cap_deinit (am_lpc_sct_cap_dev_t *p_dev)
{
    amhw_lpc_sct_t *p_hw_sct  = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;

    if (p_dev == NULL) {
      return;
    }
 
    /* Set the halt and stop bits,the SCT is stop */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
    
    am_int_disable(p_dev->p_devinfo->inum);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
