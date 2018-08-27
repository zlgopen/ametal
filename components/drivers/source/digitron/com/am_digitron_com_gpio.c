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
 * \brief Î»ÂëÇý¶¯£¨GPIO£©
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_gpio.h"
#include "am_digitron_com_gpio.h"

/*******************************************************************************
  Local macro defines
*******************************************************************************/
#define __COM_IDX_INVALID  (-1)

/*******************************************************************************
  Local functions
*******************************************************************************/
static int __com_gpio_sel (void *p_cookie, int idx)
{
    int active_level;

    am_digitron_com_gpio_dev_t *p_dev = (am_digitron_com_gpio_dev_t *)p_cookie;

    if ((p_dev == NULL) || (idx >= p_dev->pin_num)) {
        return -AM_EINVAL;
    }

    if (p_dev->last_com == idx) {
        return AM_OK;
    }

    active_level = !am_digitron_com_active_low_get(p_dev->p_info);

    if (p_dev->last_com != __COM_IDX_INVALID) {
        am_gpio_set(p_dev->p_pins[p_dev->last_com], !active_level);
    }

    am_gpio_set(p_dev->p_pins[idx], active_level);

    p_dev->last_com = idx;

    return AM_OK;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
am_digitron_base_selector_com_t *am_digitron_com_gpio_init (
        am_digitron_com_gpio_dev_t        *p_dev,
        const am_digitron_base_info_t     *p_info,
        const int                         *p_pins)
{
    int i;

    if ((p_dev == NULL) || (p_pins == NULL)) {
        return NULL;
    }

    p_dev->p_info   = p_info;
    p_dev->last_com = __COM_IDX_INVALID;
    p_dev->p_pins   = p_pins;

    if (am_digitron_scan_mode_get(p_info) == AM_DIGITRON_SCAN_MODE_ROW) {
        p_dev->pin_num = am_digitron_num_row_get(p_info);
    } else {
        p_dev->pin_num = am_digitron_num_col_get(p_info);
    }

    if (am_digitron_com_active_low_get(p_info)) {
        for (i = 0; i < p_dev->pin_num; i++) {
            am_gpio_pin_cfg(p_pins[i], AM_GPIO_OUTPUT_INIT_HIGH);
        }
    } else {
        for (i = 0; i < p_dev->pin_num; i++) {
            am_gpio_pin_cfg(p_pins[i], AM_GPIO_OUTPUT_INIT_LOW);
        }
    }

    p_dev->selector.p_cookie     = p_dev;
    p_dev->selector.pfn_com_sel = __com_gpio_sel;

    return &(p_dev->selector);
}

/* end of file */
