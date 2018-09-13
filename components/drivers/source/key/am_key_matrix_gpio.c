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
 * \brief GPIO驱动型矩阵键盘驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-15  tee, first implementation.
 * \endinternal
 */
#include "am_key_matrix_gpio.h"
#include "am_gpio.h"

/*******************************************************************************
   Public Functions
*******************************************************************************/
am_key_matrix_t * am_key_matrix_gpio_init (
    am_key_matrix_gpio_t            *p_dev,
    const am_key_matrix_gpio_info_t *p_info)
{

    am_key_matrix_prepare_t  *p_prepare;
    am_key_matrix_read_t     *p_read;

    if ((p_dev == NULL) || (p_info == NULL)) {
        return NULL;
    }

    p_dev->p_info    = p_info;

    if (am_key_matrix_scan_mode_get(&p_info->base_info) == AM_KEY_MATRIX_SCAN_MODE_ROW) {

        p_prepare = am_key_matrix_prepare_gpio_init(&p_dev->prepare,
                                                    &p_info->base_info,
                                                     p_info->p_pins_row);

        p_read = am_key_matrix_read_gpio_init(&p_dev->read,
                                              &p_info->base_info,
                                               p_info->p_pins_col);
    } else {

        p_prepare = am_key_matrix_prepare_gpio_init(&p_dev->prepare,
                                                    &p_info->base_info,
                                                     p_info->p_pins_col);

        p_read = am_key_matrix_read_gpio_init(&p_dev->read,
                                              &p_info->base_info,
                                               p_info->p_pins_row);
    }

    if (am_key_matrix_init(&p_dev->isa,
                           &p_info->base_info,
                            p_prepare,
                            p_read) < 0) {
        return NULL;
    }

    return &(p_dev->isa);
}

/******************************************************************************/
int am_key_matrix_gpio_softimer_init (
    am_key_matrix_gpio_softimer_t            *p_dev,
    const am_key_matrix_gpio_softimer_info_t *p_info)
{
    am_key_matrix_t *p_key;

    if ((p_dev == NULL) || (p_info == NULL)) {
        return -AM_EINVAL;
    }

    p_key = am_key_matrix_gpio_init(&p_dev->key, &p_info->key_matrix_gpio_info);

    return am_key_matrix_softimer_init(&p_dev->isa,
                                       p_key,
                                       p_info->scan_interval_ms);
}
/* end of file */
