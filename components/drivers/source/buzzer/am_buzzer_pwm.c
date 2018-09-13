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
#include "am_buzzer_dev.h"
#include "am_buzzer_pwm.h"

/*******************************************************************************
  Global variable
*******************************************************************************/

typedef struct __buzzer_pwm_dev {
    am_buzzer_dev_t       isa;
    int                   chan;
    am_pwm_handle_t       handle;
} __buzzer_pwm_dev_t;

/* µ¥ÊµÀý */
static struct __buzzer_pwm_dev __g_buzzer_pwm_dev;

/*******************************************************************************
  private functions
*******************************************************************************/ 
static int __buzzer_pwm_set (void *p_cookie, am_bool_t on)
{
    struct __buzzer_pwm_dev *p_this = (struct __buzzer_pwm_dev *)p_cookie;

    if (p_this != NULL && p_this->handle != NULL) {
        if (on) {
            return am_pwm_enable(p_this->handle, p_this->chan);
        } else {
            return am_pwm_disable(p_this->handle, p_this->chan);
        }
    }

    return -AM_ENODEV;
}

/*******************************************************************************
  Driver functions
*******************************************************************************/
static const am_buzzer_drv_funcs_t __g_buzzer_pwm_drv_funcs = {
    __buzzer_pwm_set,
};

/*******************************************************************************
  public functions
*******************************************************************************/

/**
 * \brief Initialize the Buzzer on the am_board.c
 */ 
int am_buzzer_pwm_init (am_pwm_handle_t pwm_handle,
                        int             chan,
                        unsigned int    duty_ns,
                        unsigned int    period_ns)
{

    if (pwm_handle == NULL) {
        return -AM_EINVAL;
    }

    __g_buzzer_pwm_dev.chan   = chan;
    __g_buzzer_pwm_dev.handle = pwm_handle;

    am_buzzer_dev_register(&__g_buzzer_pwm_dev.isa,
                           &__g_buzzer_pwm_drv_funcs,
                           &__g_buzzer_pwm_dev);

    return am_pwm_config(pwm_handle, chan, duty_ns, period_ns);
}

/* end of file */
