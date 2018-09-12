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
 * \brief PWM driver implementation for standard timer
 *
 * 1. The standard timer can provide 3 service as follows,This driver implement 
 *    the PWM function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The four match register used as follows:
 *        - The match 3 to generate all PWMs period.
 *        - The match 0 to generate PWM channel 0 duty.
 *        - The match 1 to generate PWM channel 1 duty.
 *        - The match 2 to generate PWM channel 2 duty.
 *
 * \note One timer output PWMs shared one period time,in other word,the PWMs
 *       frequency are same. 
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#include "am_lpc_timer_pwm.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_clk.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

/** \brief Configure the PWM  */
static int __timer_pwm_config (void          *p_drv,
                               int            chan,
                               unsigned long  duty_ns,
                               unsigned long  period_ns);

/** \brief Enable the PWM OUTPUT */
static int __timer_pwm_enable(void *p_drv, int chan);

/** \brief Disable the PWM OUTPUT */
static int __timer_pwm_disable(void *p_drv, int chan);

/** \brief timer for PWMs driver functions */
static const struct am_pwm_drv_funcs __g_timer_pwm_drv_funcs = {
    __timer_pwm_config,
    __timer_pwm_enable,
    __timer_pwm_disable,
};

/******************************************************************************/

/** \brief Configure the PWM  */
static int __timer_pwm_config (void          *p_drv,
                               int            chan,
                               unsigned long  duty_ns,
                               unsigned long  period_ns)
{
    am_lpc_timer_pwm_dev_t *p_dev      = (am_lpc_timer_pwm_dev_t *)p_drv;
    amhw_lpc_timer_t       *p_hw_timer = (amhw_lpc_timer_t *)p_dev->p_devinfo->timer_regbase;
    
    uint32_t clkfreq;
    uint32_t period_c, low_c;

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
    low_c    = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000; 

    /* Low level time */
    low_c = period_c - low_c;
    
    /* Match 3 operate for period */
    amhw_lpc_timer_mat_val_set(p_hw_timer, 3, period_c - 1);  
     
    /* Match channel operate for low level time */
    amhw_lpc_timer_mat_val_set(p_hw_timer, chan, low_c);

    return AM_OK;
}

/** \brief Enable the PWM OUTPUT */
static int __timer_pwm_enable (void *p_drv, int chan)
{
    am_lpc_timer_pwm_dev_t     *p_dev      = (am_lpc_timer_pwm_dev_t *)p_drv;
    amhw_lpc_timer_t           *p_hw_timer = (amhw_lpc_timer_t *)p_dev->p_devinfo->timer_regbase;
    am_lpc_timer_pwm_ioinfo_t  *p_ioinfo   = p_dev->p_devinfo->p_ioinfo;
    
    /* The valid channel number is 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].func);
    
    amhw_lpc_timer_mat_pwm_enable(p_hw_timer, chan);
    
    /* The timer not enable*/
    if ((p_hw_timer->tcr & 0x01) != 0x01) {
        amhw_lpc_timer_enable(p_hw_timer);
    }
    
    return AM_OK;
}

/** \brief Disable the PWM OUTPUT */
static int __timer_pwm_disable (void *p_drv, int chan)
{
    am_lpc_timer_pwm_dev_t    *p_dev      = (am_lpc_timer_pwm_dev_t *)p_drv;
    amhw_lpc_timer_t          *p_hw_timer = (amhw_lpc_timer_t *)p_dev->p_devinfo->timer_regbase;
    am_lpc_timer_pwm_ioinfo_t *p_ioinfo   = p_dev->p_devinfo->p_ioinfo;
 
    /* The valid channel number is 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }

    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].dfunc);
  
    amhw_lpc_timer_mat_pwm_disable(p_hw_timer, chan);

    /* If all channels disabled the PWM output, disable the timer */
    if ((p_hw_timer->pwmc & 0x07) == 0) {
        amhw_lpc_timer_disable(p_hw_timer);
    }
    
    return AM_OK;
}

am_pwm_handle_t am_lpc_timer_pwm_init (am_lpc_timer_pwm_dev_t           *p_dev,
                                       const am_lpc_timer_pwm_devinfo_t *p_devinfo)
{
    amhw_lpc_timer_t *p_hw_timer;
    
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }
    
    p_hw_timer              = (amhw_lpc_timer_t *)p_devinfo->timer_regbase;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->pwm_serv.p_funcs = (struct am_pwm_drv_funcs *)&__g_timer_pwm_drv_funcs;
    p_dev->pwm_serv.p_drv   = p_dev;
    
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* Reset the timer counter and pre-scale counts to 0 */
    amhw_lpc_timer_reset(p_hw_timer);
    
    /* Set the timer pre-scale counts to 0   */
    amhw_lpc_timer_prescale_set(p_hw_timer, 0);
    
    /* clear the match 3 pending */
    amhw_lpc_timer_mat_pending_clr(p_hw_timer, 3);
    
    /* The match 3 operate for period,so the match channel reset the counter */
    amhw_lpc_timer_mat_reset_enable(p_hw_timer, 3);
    
    amhw_lpc_timer_disable(p_hw_timer);
    
    return &(p_dev->pwm_serv);
}
                                     

void am_lpc_timer_pwm_deinit (am_lpc_timer_pwm_dev_t *p_dev)
{
    amhw_lpc_timer_t *p_hw_timer = (amhw_lpc_timer_t *)p_dev->p_devinfo->timer_regbase;

    if (p_dev == NULL) {
    	return;
    }
 
    amhw_lpc_timer_disable(p_hw_timer);

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }
}

/* end of file */
