/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief USBD cdc uart
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-21  adw, first implementation.
 * \endinternal
 */

#ifndef __AM_HC32F07X_USBD_CDC_VCOM_H
#define __AM_HC32F07X_USBD_CDC_VCOM_H

#include "am_usbd_cdc_vcom.h"

/**
 * \brief 初始化USB
 *
 * \param[in] p_dev     : 指向USB设备
 * \param[in] p_info    : 指向USB设备信息
 *
 * \return USB标准服务操作句柄。如果为 NULL，表明初始化失败。
 */
am_usbd_cdc_vcom_handle am_hc32f07x_usbd_cdc_vcom_init (am_usbd_cdc_vcom_t            *p_dev,
                                               const am_usbd_cdc_vcom_info_t *p_info,
                                               am_usbd_dev_t                 *p_usbd);

/**
 * \brief USB Device 去初始化
 *
 * \param[in] p_info : 指向USB设备信息
 */
void am_hc32f07x_usbd_cdc_vcom_deinit (am_usbd_cdc_vcom_t *p_dev);


am_usb_status_t am_hc32f07x_usbd_cdc_vcom_send(am_usbd_cdc_vcom_handle handle,
                                      uint8_t                *p_buff,
                                      uint8_t                 len);

// 接收回调函数
am_usb_status_t am_hc32f07x_usbd_cdc_vcom_recv_cb (am_usbd_cdc_vcom_handle     handle,
                                          pfn_cdc_vcom_recv_cb_t       pfn,
                                          void                       *p_arg);


#if defined(__cplusplus)
}
#endif


#endif /* __AM_USBD_CDC_VCOM_H */

/* end of file */
