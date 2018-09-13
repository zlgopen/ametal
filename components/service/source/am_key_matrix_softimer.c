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
 * \brief 在标准矩阵键盘的基础上，添加软件定时器，实现自动扫描
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-15  tee, first implementation.
 * \endinternal
 */
#include "am_key_matrix_softimer.h"
#include "am_input.h"

/*******************************************************************************
   Local Functions
*******************************************************************************/

static void __key_matrix_timer_cb (void *p_arg)
{
    am_key_matrix_softimer_t *p_dev = (am_key_matrix_softimer_t *)p_arg;
    am_key_matrix_scan(p_dev->p_real, p_dev->scan_idx);
    p_dev->scan_idx++;

    if (p_dev->scan_idx >= am_key_matrix_scan_idx_max_get(p_dev->p_real)) {
        p_dev->scan_idx = 0;
    }
}

/*******************************************************************************
   Public Functions
*******************************************************************************/
int am_key_matrix_softimer_init (am_key_matrix_softimer_t *p_dev,
                                 am_key_matrix_t          *p_key,
                                 int                       scan_interval_ms)
{

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_dev->p_real   = p_key;
    p_dev->scan_idx = 0;

    am_softimer_init(&p_dev->timer, __key_matrix_timer_cb, p_dev);
    am_softimer_start(&p_dev->timer, scan_interval_ms);

    return AM_OK;
}

/******************************************************************************/
int am_key_matrix_softimer_deinit (am_key_matrix_softimer_t *p_dev)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    am_softimer_stop (&p_dev->timer);

    return AM_OK;
}

/* end of file */
