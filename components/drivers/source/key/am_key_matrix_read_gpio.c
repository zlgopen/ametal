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
 * \brief 矩阵键盘GPIO键值读取
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-15  tee, first implementation.
 * \endinternal
 */
#include "ametal.h" 
#include "am_gpio.h"
#include "am_key_matrix_read_gpio.h"


/*******************************************************************************
   Local Functions
*******************************************************************************/
static uint32_t __key_val_read (void *p_arg, int idx)
{
    am_key_matrix_read_gpio_t *p_dev = (am_key_matrix_read_gpio_t *)p_arg;

    int i;
    uint32_t val = 0x00000000;

    for (i = 0; i < p_dev->pin_num; i++) {
        if (am_gpio_get(p_dev->p_pins[i]) != 0) {
            val |= (1 << i);
        }
    }

    return val;
}

/*******************************************************************************
   Public Functions
*******************************************************************************/
am_key_matrix_read_t * am_key_matrix_read_gpio_init (
    am_key_matrix_read_gpio_t       *p_dev,
    const am_key_matrix_base_info_t *p_info,
    const int                       *p_pins)
{
    int i;
    int cfg_flags;

    if ((p_dev == NULL) || (p_info == NULL)) {
        return NULL;
    }

    p_dev->p_info = p_info;
    p_dev->p_pins = p_pins;

    if (am_key_matrix_scan_mode_get(p_info) == AM_KEY_MATRIX_SCAN_MODE_ROW) {
        p_dev->pin_num = am_key_matrix_num_col_get(p_info);
    } else {
        p_dev->pin_num = am_key_matrix_num_row_get(p_info);
    }

    cfg_flags = AM_GPIO_INPUT |
               (am_key_matrix_active_low_get(p_info) ?
                  AM_GPIO_PULLUP :
                  AM_GPIO_PULLDOWN);

    /* 将所有列线设置为输入状态 */
    for (i = 0; i < p_dev->pin_num; i++) {
        am_gpio_pin_cfg(p_pins[i], cfg_flags);
    }

    p_dev->isa.pfn_val_read = __key_val_read;
    p_dev->isa.p_cookie     = p_dev;

    return &(p_dev->isa);
}

/* end of file */
