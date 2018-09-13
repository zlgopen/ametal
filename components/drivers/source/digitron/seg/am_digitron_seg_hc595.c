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
 * \brief ¶ÎÂëÇý¶¯£¨HC595£©
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_digitron_seg_hc595.h"

/*******************************************************************************
  Local functions
*******************************************************************************/
static int __seg_hc595_send (void   *p_cookie,
                             void   *p_seg,
                             uint8_t num_segment,
                             uint8_t num_digitron)
{
    am_digitron_seg_hc595_dev_t *p_dev = (am_digitron_seg_hc595_dev_t *)p_cookie;

    uint8_t *p_buf = (uint8_t *)p_seg;
    uint8_t  seg_bytes;
    int      i;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    seg_bytes = (num_segment + 7) / 8;

    for (i = 0; i < num_digitron; i++) {
    
        am_hc595_send(p_dev->handle, p_buf, seg_bytes);

        p_buf += seg_bytes;
    }

    am_hc595_enable(p_dev->handle);

    return AM_OK;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
am_digitron_base_sender_seg_t *am_digitron_seg_hc595_init (
        am_digitron_seg_hc595_dev_t     *p_dev,
        const am_digitron_base_info_t   *p_info,
        am_hc595_handle_t                handle)
{
    if ((p_dev == NULL) || (p_info == NULL) || (handle == NULL)) {
        return NULL;
    }

    p_dev->handle = handle;

    am_hc595_disable(handle);

    p_dev->sender.p_cookie     = p_dev;
    p_dev->sender.pfn_seg_send = __seg_hc595_send;

    return &(p_dev->sender);
}

/* end of file */
