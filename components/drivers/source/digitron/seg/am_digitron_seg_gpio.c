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
 * \brief 段码驱动（GPIO）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_gpio.h"
#include "am_digitron_seg_gpio.h"

/*******************************************************************************
  Local functions
*******************************************************************************/
static int __seg_gpio_send (void   *p_cookie,
                            void   *p_seg,
                            uint8_t num_segment,
                            uint8_t num_digitron)
{
    int i, j, bit;

    am_digitron_seg_gpio_dev_t *p_dev = (am_digitron_seg_gpio_dev_t *)p_cookie;

    uint8_t  seg;
    uint8_t *p_cur_seg = (uint8_t *)p_seg;
    int      index     = 0;
    uint8_t  seg_bytes;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    seg_bytes = (num_segment + 7) / 8;
    index     = p_dev->pin_num - 1;

    for (i = 0; i < num_digitron; i++) {

        p_cur_seg = (uint8_t *)p_seg + i * seg_bytes;
    
        for (j = 0; j < seg_bytes; j++) {

            seg = p_cur_seg[j];

            for (bit = 0; bit < 8; bit++) {

                /* seg_active_low? 已经在上层处理  */
                if (AM_BIT_ISSET(seg, bit)) {
                    am_gpio_set(p_dev->p_pins[index--], 1);
                } else {
                    am_gpio_set(p_dev->p_pins[index--], 0);
                }
            }
        }
    }
    
    return AM_OK;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
am_digitron_base_sender_seg_t *am_digitron_seg_gpio_init (
        am_digitron_seg_gpio_dev_t     *p_dev,
        const am_digitron_base_info_t  *p_info,
        const int                      *p_pins)
{
    int i;

    if ((p_dev == NULL) || (p_info == NULL) || (p_pins == NULL)) {
        return NULL;
    }

    p_dev->p_info = p_info;
    p_dev->p_pins = p_pins;

    if (am_digitron_scan_mode_get(p_info) == AM_DIGITRON_SCAN_MODE_ROW) {
        p_dev->pin_num = am_digitron_num_col_get(p_info) *
                         am_digitron_num_segment_get(p_info);
    } else {
        p_dev->pin_num = am_digitron_num_row_get(p_info) *
                         am_digitron_num_segment_get(p_info);
    }

    if (am_digitron_seg_active_low_get(p_info)) {
        for (i = 0; i < p_dev->pin_num; i++) {
            am_gpio_pin_cfg(p_pins[i], AM_GPIO_OUTPUT_INIT_HIGH);
        }
    } else {
        for (i = 0; i < p_dev->pin_num; i++) {
            am_gpio_pin_cfg(p_pins[i], AM_GPIO_OUTPUT_INIT_LOW);
        }
    }

    p_dev->sender.p_cookie     = p_dev;
    p_dev->sender.pfn_seg_send = __seg_gpio_send;

    return &(p_dev->sender);
}

/* end of file */
