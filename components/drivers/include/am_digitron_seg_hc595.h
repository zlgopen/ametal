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
 * \brief 段码驱动（HC595）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DIGITRON_SEG_HC595_H
#define __AM_DIGITRON_SEG_HC595_H

#include "ametal.h"
#include "am_digitron_base.h"
#include "am_hc595.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name am_if_digitron_seg_hc595
 * @{
 */

/**
 * \brief HC595段码设备
 */
typedef struct am_digitron_seg_hc595 {
    am_digitron_base_sender_seg_t      sender;
    am_hc595_handle_t                  handle;
} am_digitron_seg_hc595_dev_t;

/**
 * \brief HC595段码发送器初始化
 *
 * \param[in] p_dev  : HC595段码发送器设备实例
 * \param[in] p_info : 数码管基础信息
 * \param[in] flags  : HC595数据发送标志，若Q0 与 'a'相连接，则 数据 LSB 应该先发送
 * \param[in] handle : 标准的HC595服务句柄
 *
 * \return 标准的段码发送器（可供数码管扫描器使用）
 */
am_digitron_base_sender_seg_t *am_digitron_seg_hc595_init (
        am_digitron_seg_hc595_dev_t     *p_dev,
        const am_digitron_base_info_t   *p_info,
        am_hc595_handle_t                handle);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DIGITRON_SEG_HC595_H */

/* end of file */
