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
 * \brief LED标准接口实现（GPIO驱动）
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-25  fft, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_hc595.h"
#include "string.h"
#include "am_led_hc595.h"

/*******************************************************************************
  Local functions
*******************************************************************************/ 

static int __led_hc595_set (void *p_cookie, int led_id, am_bool_t state)
{
    am_led_hc595_dev_t *p_dev = (am_led_hc595_dev_t *)p_cookie;

    led_id = led_id - p_dev->p_info->serv_info.start_id;

    if (state ^ p_dev->p_info->active_low) {
        p_dev->p_info->p_buf[led_id >> 3] |= (1 << (led_id & 0x07));
    } else {
        p_dev->p_info->p_buf[led_id >> 3] &= ~(1 << (led_id & 0x07));
    }

    am_hc595_send(p_dev->handle,
                  p_dev->p_info->p_buf,
                  p_dev->p_info->hc595_num);

    return AM_OK;
}

/******************************************************************************/
static int __led_hc595_toggle (void *p_cookie, int led_id)
{
    am_led_hc595_dev_t *p_dev = (am_led_hc595_dev_t *)p_cookie;

    led_id = led_id - p_dev->p_info->serv_info.start_id;

    p_dev->p_info->p_buf[led_id >> 3] ^= (1 << (led_id & 0x07));

    am_hc595_send(p_dev->handle,
                  p_dev->p_info->p_buf,
                  p_dev->p_info->hc595_num);

    return AM_OK;
}

/******************************************************************************/

static const am_led_drv_funcs_t __g_led_hc595_drv_funcs = {
    __led_hc595_set,
    __led_hc595_toggle
};

/*******************************************************************************
  Public functions
*******************************************************************************/
int am_led_hc595_init (am_led_hc595_dev_t         *p_dev,
                       const am_led_hc595_info_t  *p_info,
                       am_hc595_handle_t           handle)
{
    if ((p_dev == NULL) || (p_info == NULL)) {
        return -AM_EINVAL;
    }

    p_dev->p_info = p_info;

    if (p_info->active_low) {
        memset(p_info->p_buf, 0xFF, p_info->hc595_num);
    } else {
        memset(p_info->p_buf, 0x00, p_info->hc595_num);
    }

    am_hc595_send(handle, p_info->p_buf, p_info->hc595_num);

    return am_led_dev_add(&p_dev->isa,
                          &p_info->serv_info,
                          &__g_led_hc595_drv_funcs,
                           p_dev);
}

/******************************************************************************/
int am_led_hc595_deinit (am_led_hc595_dev_t *p_dev)
{
    const am_led_hc595_info_t  *p_info;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_info = p_dev->p_info;

    if (p_info->active_low) {
        memset(p_info->p_buf, 0xFF, p_info->hc595_num);
    } else {
        memset(p_info->p_buf, 0x00, p_info->hc595_num);
    }

    am_hc595_send(p_dev->handle, p_info->p_buf, p_info->hc595_num);

    return am_led_dev_del(&p_dev->isa);
}

/* end of file */
