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
 * \brief PWM drivers implementation
 *
 * 1. The SCT can provide 3 service as follows,This driver implement 
 *    the PWM function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The SCT resource used as follows:
 *    - The match 0 to generate period.associated with event 0.
 *    - The match 1 to generate PWM channel 0 duty. output through SCT_OUT0
 *    - The match 2 to generate PWM channel 1 duty. output through SCT_OUT1
 *    - The match 3 to generate PWM channel 2 duty. output through SCT_OUT2
 *    - The match 4 to generate PWM channel 3 duty. output through SCT_OUT3
 *    - The match 5 to generate PWM channel 4 duty. output through SCT_OUT4
 *    - The match 6 to generate PWM channel 5 duty. output through SCT_OUT5
 *    - The match 7 to generate PWM channel 6 duty. output through SCT_OUT6
 *    - The match 8 to generate PWM channel 7 duty. output through SCT_OUT7
 *
 * \note One SCT output PWMs shared one period time,in other word,the PWMs
 *       frequency is same.
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#include "am_lpc_sct_pwm.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_clk.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

/** \brief Configure the PWM  */
static int __sct_pwm_config (void          *p_drv,
                             int            chan,
                             unsigned long  duty_ns,
                             unsigned long  period_ns);

/** \brief Enable the PWM OUTPUT */
static int __sct_pwm_enable(void *p_drv, int chan);

/** \brief Disable the PWM OUTPUT */
static int __sct_pwm_disable(void *p_drv, int chan);

/** \brief SCT for PWMs driver functions */
static const struct am_pwm_drv_funcs __g_sct_pwm_drv_funcs = {
    __sct_pwm_config,
    __sct_pwm_enable,
    __sct_pwm_disable,
};

/******************************************************************************/

/** \brief Configure the PWM  */
static int __sct_pwm_config (void          *p_drv,
                             int            chan,
                             unsigned long  duty_ns,
                             unsigned long  period_ns)
{
    am_lpc_sct_pwm_dev_t *p_dev    = (am_lpc_sct_pwm_dev_t *)p_drv;
    amhw_lpc_sct_t       *p_hw_sct = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    
    uint32_t clkfreq;
    uint32_t period_c, duty_c;

    if ((period_ns == 0) || (duty_ns > 4294967295UL) ||
        (period_ns > 4294967295UL) || (duty_ns > period_ns)) {
        return -AM_EINVAL;
    }

    /* The valid channel number is 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    clkfreq = am_clk_rate_get(p_dev->p_devinfo->clk_id);
     
    period_c = (uint64_t)(period_ns) * (clkfreq) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000;
 
    
    /* the period cycles at least one */
    if (period_c == 0) {
        period_c = 1;
    }
    
    /* Set the match0 register as period */
    if (amhw_lpc_sct_halt_check(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY) == AM_TRUE ) {
        amhw_lpc_sct_mat_val_set(p_hw_sct, 
                             AMHW_LPC_SCT_MODE_UNIFY, 
                             AMHW_LPC_SCT_MAT(0), 
                             period_c - 1);
    }
    
    amhw_lpc_sct_mat_reload_val_set(p_hw_sct, 
                                AMHW_LPC_SCT_MODE_UNIFY, 
                                AMHW_LPC_SCT_MAT(0), 
                                period_c - 1);
     
    /* The duty cycles at least one */
    if (duty_c == 0) {
        duty_c = 1;
    }
    
    /* Set the match (chan + 1) as duty */
    if (amhw_lpc_sct_halt_check(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY) == AM_TRUE ) {      
        amhw_lpc_sct_mat_val_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, chan + 1, duty_c - 1);
    }
    
    /* Set the match reload value */
    amhw_lpc_sct_mat_reload_val_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, chan + 1, duty_c - 1); 

    return AM_OK;
}

/** \brief Enable the PWM OUTPUT */
static int __sct_pwm_enable (void *p_drv, int chan)
{
    am_lpc_sct_pwm_dev_t    *p_dev      = (am_lpc_sct_pwm_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct   = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    am_lpc_sct_pwm_ioinfo_t *p_ioinfo   = p_dev->p_devinfo->p_ioinfo;
    uint8_t                match_num  = AMHW_LPC_SCT_MAT(chan+1);  
    uint8_t                event_num  = AMHW_LPC_SCT_EVT(chan+1);
    uint8_t                output_num = AMHW_LPC_SCT_OUT(chan);
    
    /* The valid channel number is 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /* Configure the match number register operate as match register */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                            AMHW_LPC_SCT_MODE_UNIFY,    /* the SCT mode is UNIFY(1 32-bit counter) */
                            match_num,
                            AMHW_LPC_SCT_MATCAP_MATCH); /* operate as match register               */  
    
    /* Use the event number same with match number */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                        event_num,
                        AMHW_LPC_SCT_EV_CTRL_MATCHSEL(match_num)  | 
                        AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH       | /* Uses the specified match only.    */
                        AMHW_LPC_SCT_EV_CTRL_STATELD_ADD          | /*  STATEV value is added into STATE */ 
                        AMHW_LPC_SCT_EV_CTRL_STATEV(0));            /* ADD 0,the state will not change   */    
    
    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].func);
    
    /* Enable the event happen in state 0 */
    amhw_lpc_sct_event_state_enable(p_hw_sct, event_num, AMHW_LPC_SCT_STATE(0));
    
    /* Event 0 Set the PWM output */
    amhw_lpc_sct_out_set_enable(p_hw_sct,output_num,AMHW_LPC_SCT_EVT(0));
    
    /* Event number clear the PWM output */
    amhw_lpc_sct_out_clr_enable(p_hw_sct, output_num, event_num);
    
    /* 
     * when the duty == period , the conflict will happen.
     * in this condition, the duty percent should be 100%, 
     * So set the output in-case of conflict.
     */
    amhw_lpc_sct_conflict_res_config(p_hw_sct, output_num, AMHW_LPC_SCT_RES_SET_OUTPUT);   

    /* Set and clear do not depend on direction */
    amhw_lpc_sct_output_dir_ctrl(p_hw_sct, output_num, AMHW_LPC_SCT_OUTPUTDIRCTRL_INDEPENDENT);
    
    /* Clear the halt and stop bits, the SCT is run */
    amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);

    return AM_OK;
}

/** \brief Disable the PWM OUTPUT */
static int __sct_pwm_disable (void *p_drv, int chan)
{
    am_lpc_sct_pwm_dev_t    *p_dev      = (am_lpc_sct_pwm_dev_t *)p_drv;
    amhw_lpc_sct_t          *p_hw_sct   = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;
    am_lpc_sct_pwm_ioinfo_t *p_ioinfo   = p_dev->p_devinfo->p_ioinfo;
    uint8_t                event_num  = AMHW_LPC_SCT_EVT(chan+1);
    uint8_t                output_num = AMHW_LPC_SCT_OUT(chan);
    
    int i;
    
    /* The valid channel number is 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }

    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].dfunc);
  
    /* Disable the event happen in state 0 */
    amhw_lpc_sct_event_state_disable(p_hw_sct, event_num, AMHW_LPC_SCT_STATE(0));
    
    /* Disable event 0 Set the PWM output     */
    amhw_lpc_sct_out_set_disable(p_hw_sct, output_num, AMHW_LPC_SCT_EVT(0));
    
    /* Disable event number clear the PWM output */
    amhw_lpc_sct_out_clr_disable(p_hw_sct, output_num, event_num);

    i = p_dev->p_devinfo->channels_num;
    
    for (i = p_dev->p_devinfo->channels_num - 1; i >= 0; i--) {
        if (p_hw_sct->out[i].clr != 0 || p_hw_sct->out[i].set != 0) {
            return AM_OK;
        }
    }
    
    /** If all channels disabled the PWM output,disable the SCT */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
   
    return AM_OK;
}

/** 
  * \brief Initialize the SCT to prepare for output PWMs
  *
  *  Initialize the SCT operate as 1-32bit timer,and enable MATCH0 for auto limit.
  *  
  * \param[in] p_hw_sct : Pointer to the SCT to be initialized for output PWMs
  * \return None
  */
static void __sct_pwm_init (amhw_lpc_sct_t *p_hw_sct)
{
    /* Set the halt and stop bits,the SCT is stop */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
    
    /* Initialize the match 0 operate as auto limit,act as the PWM's period */
    amhw_lpc_sct_config(p_hw_sct,
                    AMHW_LPC_SCT_CONFIG_32BIT_COUNTER  | /* the timer is operate as 1 32-bit timer */
                    AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK | /* the SCT clock is system clock  */          
                    AMHW_LPC_SCT_CONFIG_AUTOLIMIT_L );   /* the match 0 as auto limit  */
   
    /* Configure the mat-cap reg 0 operate as match register */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                            AMHW_LPC_SCT_MODE_UNIFY,
                            AMHW_LPC_SCT_MAT(0),
                            AMHW_LPC_SCT_MATCAP_MATCH);
                            
    amhw_lpc_sct_mat_val_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, AMHW_LPC_SCT_MAT(0), 0);
    
    /* Set the event 0 ctrl, event 0 for PWM0 */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            AMHW_LPC_SCT_EVT(0),                  /* event 0 */
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(0)    | /* the event associated with match 0 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH | /* Uses the specified match only.    */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD    | /*  STATEV value is added into STATE */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));      /* ADD 0,the state will not change   */

    /* Enable the event0 happen in state 0 */
    amhw_lpc_sct_event_state_enable(p_hw_sct, AMHW_LPC_SCT_EVT(0), AMHW_LPC_SCT_STATE(0));    

    /* Set the CLRCTR bit to clear the counter to 0 */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);
    
    /* Set the now state value is 0 */
    amhw_lpc_sct_state_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, AMHW_LPC_SCT_STATE(0));
    
    /* Set the prescale to 0. use the system clock,not prescale */
    amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
}

am_pwm_handle_t am_lpc_sct_pwm_init (am_lpc_sct_pwm_dev_t           *p_dev,
                                     const am_lpc_sct_pwm_devinfo_t *p_devinfo)
{
    amhw_lpc_sct_t  *p_hw_sct;
    
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }
    
    p_hw_sct                = (amhw_lpc_sct_t  *)p_devinfo->sct_regbase;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->pwm_serv.p_funcs = (struct am_pwm_drv_funcs *)&__g_sct_pwm_drv_funcs;
    p_dev->pwm_serv.p_drv   = p_dev;
    
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    __sct_pwm_init(p_hw_sct);

    return &(p_dev->pwm_serv);
}
                                     

void am_lpc_sct_pwm_deinit (am_lpc_sct_pwm_dev_t *p_dev)
{
    amhw_lpc_sct_t *p_hw_sct = (amhw_lpc_sct_t *)p_dev->p_devinfo->sct_regbase;

    if (p_dev == NULL) {
      return;
    }
 
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }
}

/* end of file */
