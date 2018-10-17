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
 * \brief 通用矩阵键盘驱动接口（各具体驱动根据自身实现具体的键值读取函数）
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-15  tee, first implementation.
 * \endinternal
 */
#include "am_key_matrix.h"
#include "am_input.h"
#include "am_event_input_key.h"

/*******************************************************************************
   Local Functions
*******************************************************************************/

/* read one line */
static int __key_code_get (am_key_matrix_t *p_dev, int idx)
{
    /* code reversal row and col */
    int col;
    int off;
    
    if (am_key_matrix_scan_mode_get(p_dev->p_info) == AM_KEY_MATRIX_SCAN_MODE_ROW) {
        return am_key_matrix_code_get(p_dev->p_info, idx);
    }
    
    col = idx / p_dev->p_info->row;
    off = idx % p_dev->p_info->row;
    
    idx = off * p_dev->p_info->col + col;

    return am_key_matrix_code_get(p_dev->p_info, idx);
}

/******************************************************************************/

/* read change report */
static int __key_val_report (am_key_matrix_t *p_dev)
{
    uint32_t  key_change = p_dev->key_samp_cur ^ p_dev->key_final;
    uint8_t   key_num    = p_dev->p_info->col * p_dev->p_info->row;
    uint32_t  key_cur    = p_dev->key_samp_cur;
    am_bool_t active_low = am_key_matrix_active_low_get(p_dev->p_info);

    int key_code;
    int i;

    for (i = 0; i < key_num; i++) {
        if (key_change & (1u << i)) {

            key_code = __key_code_get(p_dev, i);       /* 状态变化的按键键码  */

            /* 按键激活电平与当前电平相同，按键按下，否则，按键释放 */
            if ((((key_cur & (1u << i)) == 0) && active_low) ||
                (((key_cur & (1u << i)) != 0) && !active_low)) {

                am_event_input_key_pressed(key_code);

            } else {
                am_event_input_key_released(key_code);
            }
        }
    }

    return AM_OK;
}

/******************************************************************************/

static void __key_matrix_process (am_key_matrix_t *p_dev)
{
    if (p_dev->key_samp_cur == p_dev->key_samp_last) {

        if (p_dev->key_samp_cur != p_dev->key_final) {

            __key_val_report(p_dev);

            p_dev->key_final = p_dev->key_samp_cur;
        }
    }

    p_dev->key_samp_last = p_dev->key_samp_cur;
}

/******************************************************************************/
int am_key_matrix_init (am_key_matrix_t                 *p_dev,
                        const am_key_matrix_base_info_t *p_info,
                        am_key_matrix_prepare_t         *p_prepare,
                        am_key_matrix_read_t            *p_read)
{

    if ((p_dev == NULL) || (p_info == NULL) || (p_read == NULL)) {
        return -AM_EINVAL;
    }

    /* todo : support more keys */
    if (p_info->row * p_info->col > 32) {
        return -AM_ENOTSUP;
    }

    p_dev->p_info     = p_info;
    p_dev->p_read     = p_read;
    p_dev->p_prepare  = p_prepare;

    if (am_key_matrix_active_low_get(p_info)) {
        p_dev->key_samp_last = p_dev->key_samp_cur = p_dev->key_final = 0xFFFFFFFF;
    } else {
        p_dev->key_samp_last = p_dev->key_samp_cur = p_dev->key_final = 0x00000000;
    }

    return AM_OK;
}

/******************************************************************************/
int am_key_matrix_scan (am_key_matrix_t *p_dev, int scan_idx)
{
    uint32_t  val = 0x00;
    uint8_t   scan_nums;
    am_bool_t scan_finish = AM_FALSE;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    if (am_key_matrix_scan_mode_get(p_dev->p_info) == AM_KEY_MATRIX_SCAN_MODE_ROW) {

        if (scan_idx >= p_dev->p_info->row) {
            return -AM_EINVAL;
        }

        if (scan_idx == (p_dev->p_info->row - 1)) {
            scan_finish = AM_TRUE;
        }

        scan_nums = p_dev->p_info->col;

    } else {

        if (scan_idx >= p_dev->p_info->col) {
            return -AM_EINVAL;
        }

        if (scan_idx == (p_dev->p_info->col - 1)) {
            scan_finish = AM_TRUE;
        }

        scan_nums = p_dev->p_info->row;
    }

    if (p_dev->p_prepare && p_dev->p_prepare->pfn_prepare) {
        p_dev->p_prepare->pfn_prepare(p_dev->p_prepare->p_cookie, scan_idx);
    }

    val = p_dev->p_read->pfn_val_read(p_dev->p_read->p_cookie, scan_idx);

    if (p_dev->p_prepare && p_dev->p_prepare->pfn_finish) {
        p_dev->p_prepare->pfn_finish(p_dev->p_prepare->p_cookie, scan_idx);
    }

    AM_BITS_SET(p_dev->key_samp_cur, scan_nums * scan_idx, scan_nums, val);

    /* scan finished, need report */
    if (scan_finish) {
        __key_matrix_process(p_dev);
    }

    return AM_OK;
}

/******************************************************************************/
int am_key_matrix_scan_idx_max_get (am_key_matrix_t *p_dev)
{
    if (p_dev == NULL) {
        return 0;
    }

    if (am_key_matrix_scan_mode_get(p_dev->p_info) == AM_KEY_MATRIX_SCAN_MODE_ROW) {
        return p_dev->p_info->row;
    } else {
        return p_dev->p_info->col;
    }
}

/* end of file */
