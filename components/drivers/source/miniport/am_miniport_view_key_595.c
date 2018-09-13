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
 * \brief MiniPort-View 、 MiniPort-Key 和  MiniPort-595 联合使用
 *
 * 本驱动适合以下情况：
 * 1. 数码管段码使用GPIO驱动
 * 2. 数码管位码使用HC595驱动
 * 3. 矩阵键盘采用列扫描方式，列线与数码管位选线复用
 * 4. 矩阵键盘的键值读取使用GPIO读取（该GPIO与数码管独立）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_miniport_view_key_595.h"

/*******************************************************************************
  Local functions
*******************************************************************************/
static void __digitron_gpio_mutex (void *p_arg, int scan_idx)
{
    am_miniport_view_key_595_dev_t *p_dev = (am_miniport_view_key_595_dev_t *)p_arg;

    /* key scan */
    am_key_matrix_scan(&p_dev->key_dev, scan_idx);
}

/*******************************************************************************
  Public functions
*******************************************************************************/

int am_miniport_view_key_595_init (am_miniport_view_key_595_dev_t        *p_dev,
                                   const am_miniport_view_key_595_info_t *p_info,
                                   am_hc595_handle_t                      handle)
{
    am_key_matrix_read_t     *p_read;

    if ((p_dev == NULL) || (p_info == NULL)) {
        return -AM_EINVAL;
    }

    /* Only support col scan mode in this condition */
    if (am_key_matrix_scan_mode_get(&p_info->key_info) == AM_KEY_MATRIX_SCAN_MODE_ROW) {
        return -AM_EINVAL;
    }

    if (am_digitron_scan_hc595_gpio_init(&p_dev->scan_hc595_gpio_dev,
                                         &p_info->scan_info,
                                         handle) < 0) {
        return -AM_EINVAL;
    }

    p_read = am_key_matrix_read_gpio_init(&p_dev->read,
                                          &p_info->key_info,
                                           p_info->p_pins_row);

    if (am_key_matrix_init(&p_dev->key_dev,
                           &p_info->key_info,
                           NULL,              /* needn't prepare */
                           p_read) < 0) {
        return -AM_EINVAL;
    }

    p_dev->p_info = p_info;

    return am_digitron_scan_hc595_gpio_cb_set(&p_dev->scan_hc595_gpio_dev,
                                              __digitron_gpio_mutex,
                                              p_dev);
}

/******************************************************************************/
int am_miniport_view_key_595_deinit (am_miniport_view_key_595_dev_t *p_dev)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* 删除标准的数码管设备  */
    return am_digitron_scan_hc595_gpio_deinit(&p_dev->scan_hc595_gpio_dev);
}

/* end of file */
