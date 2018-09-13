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
 * \brief 通用LED设备管理
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_led.h"
#include "am_led_dev.h"
#include "am_int.h"

/*******************************************************************************
  Global variables
*******************************************************************************/

static am_led_dev_t *__gp_head;

/*******************************************************************************
  Local functions
*******************************************************************************/
static am_led_dev_t * __led_dev_find_with_id (int id)
{
    am_led_dev_t *p_cur   = __gp_head;

    int key = am_int_cpu_lock();

    while (p_cur != NULL) {

        if ((id >= p_cur->p_info->start_id) &&
            (id <= p_cur->p_info->end_id)) {

            break;
        }

        p_cur = p_cur->p_next;
    }

    am_int_cpu_unlock(key);

    return p_cur;
}

/******************************************************************************/
static int __led_dev_add (am_led_dev_t *p_dd)
{
    int key = am_int_cpu_lock();

    /* add the device to device list */
    p_dd->p_next = __gp_head;
    __gp_head    = p_dd;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
static int __led_dev_del (am_led_dev_t *p_dd)
{
    int            ret     = -AM_ENODEV;
    am_led_dev_t **pp_head = &__gp_head;
    am_led_dev_t  *p_prev  = AM_CONTAINER_OF(pp_head, am_led_dev_t, p_next);
    am_led_dev_t  *p_cur   = __gp_head;
  
    int key = am_int_cpu_lock();

    while (p_cur != NULL) {
        if (p_cur == p_dd) {

            p_prev->p_next = p_dd->p_next;
            p_dd->p_next   = NULL;
            ret            = AM_OK;
            break;
        }

        p_prev = p_cur;
        p_cur  = p_cur->p_next;
    }

    am_int_cpu_unlock(key);

    return ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/

int am_led_dev_lib_init (void)
{
    __gp_head = NULL;

    return AM_OK;
}

/******************************************************************************/
int am_led_dev_add (am_led_dev_t             *p_dev,
                    const am_led_servinfo_t  *p_info,
                    const am_led_drv_funcs_t *p_funcs,
                    void                     *p_cookie)
{
    if ((p_dev == NULL) || (p_funcs == NULL) || (p_info == NULL)) {
        return -AM_EINVAL;
    }

    if (__led_dev_find_with_id(p_info->start_id) != NULL) {
        return -AM_EPERM;
    }

    if (__led_dev_find_with_id(p_info->end_id) != NULL) {
        return -AM_EPERM;
    }

    p_dev->p_info   = p_info;
    p_dev->p_funcs  = p_funcs;
    p_dev->p_next   = NULL;
    p_dev->p_cookie = p_cookie;

    return __led_dev_add(p_dev);
}

/******************************************************************************/
int am_led_dev_del (am_led_dev_t *p_dd)
{
    if (p_dd == NULL) {
        return -AM_EINVAL;
    }

    return __led_dev_del(p_dd);
}

/*******************************************************************************
  The implement for standard interface (am_led.h)
*******************************************************************************/

int am_led_set (int led_id, am_bool_t state)
{
    am_led_dev_t *p_dev = __led_dev_find_with_id(led_id);

    if (p_dev == NULL) {
        return -AM_ENODEV;
    }

    if (p_dev->p_funcs->pfn_led_set) {
        return p_dev->p_funcs->pfn_led_set(p_dev->p_cookie, led_id, state);
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
int am_led_toggle (int led_id)
{
    am_led_dev_t *p_dev = __led_dev_find_with_id(led_id);

    if (p_dev == NULL) {
        return -AM_ENODEV;
    }

    if (p_dev->p_funcs->pfn_led_toggle) {
        return p_dev->p_funcs->pfn_led_toggle(p_dev->p_cookie, led_id);
    }

    return -AM_ENOTSUP;
}

/******************************************************************************/
int am_led_on (int led_id)
{
    return am_led_set(led_id, AM_TRUE);
}

/******************************************************************************/
int am_led_off (int led_id)
{
    return am_led_set(led_id, AM_FALSE);
}

/* end of file */
