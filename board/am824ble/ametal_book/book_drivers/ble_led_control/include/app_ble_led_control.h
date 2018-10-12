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
 * \brief book`s app_ble_led driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#ifndef __APP_BLE_IED_CONTROL_H
#define __APP_BLE_IED_CONTROL_H


#include "ametal.h"
#include "am_zlg9021.h"

int app_ble_led_control (am_zlg9021_handle_t zlg9021_handle);
                                                           
#endif /* __APP_BLE_IED_CONTROL_H */

/* end of file */
