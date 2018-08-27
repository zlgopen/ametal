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
 * \brief Buzzer implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-15  tee, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_buzzer.h"
#include "am_buzzer_dev.h"
#include "am_delay.h"
#include "am_softimer.h"

/*******************************************************************************
  Global variable
*******************************************************************************/

static am_buzzer_dev_t  *__gp_buzzer_dev = NULL;

/*******************************************************************************
  Local functions
*******************************************************************************/ 
static void __beep_timer_callback (void *p_arg)
{
    am_softimer_stop((am_softimer_t *)p_arg);
    am_buzzer_off();
}

/*******************************************************************************
  public functions
*******************************************************************************/
int am_buzzer_dev_register (am_buzzer_dev_t             *p_dev,
                            const am_buzzer_drv_funcs_t *p_funcs,
                            void                        *p_cookie)
{
    if ((p_dev == NULL) || (p_funcs == NULL) || (p_funcs->pfn_buzzer_set == NULL)) {
        return -AM_EINVAL;
    }

    p_dev->p_funcs  = p_funcs;
    p_dev->p_cookie = p_cookie;

    __gp_buzzer_dev = p_dev;

    am_softimer_init(&p_dev->timer,__beep_timer_callback, &p_dev->timer);

    return AM_OK;
}

/******************************************************************************/
int am_buzzer_on (void)
{
    if ((NULL != __gp_buzzer_dev) && (NULL != __gp_buzzer_dev->p_funcs)) {
        return __gp_buzzer_dev->p_funcs->pfn_buzzer_set(__gp_buzzer_dev->p_cookie, AM_TRUE);
    }

    return -AM_ENODEV;
}

/******************************************************************************/
int am_buzzer_off (void)
{
    if ((NULL != __gp_buzzer_dev) && (NULL != __gp_buzzer_dev->p_funcs)) {
        return __gp_buzzer_dev->p_funcs->pfn_buzzer_set(__gp_buzzer_dev->p_cookie, AM_FALSE);
    }

    return -AM_ENODEV;
}

/******************************************************************************/
int am_buzzer_beep (uint32_t ms)
{
    int ret = am_buzzer_on();

    if (ret < 0) {
        return ret;
    }

    am_mdelay(ms);

    return am_buzzer_off();
}

/******************************************************************************/
int am_buzzer_beep_async (uint32_t ms)
{
    int ret = am_buzzer_on();

    if (ret < 0) {
        return ret;
    }

    am_softimer_start(&__gp_buzzer_dev->timer, ms);

    return AM_OK;
}

/* end of file */
