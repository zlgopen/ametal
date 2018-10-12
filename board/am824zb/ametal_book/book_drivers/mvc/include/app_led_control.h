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
 * \brief 
 *
 * \internal
 * \par Modification History
 * - 1.00 18-09-13  ipk, first implementation.
 * \endinternal
 */
#ifndef __APP_LED_CONTROL_H
#define __APP_LED_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif
	
#pragma once
#include "ametal.h"
#include "am_zm516x.h"

int app_led_control (am_zm516x_handle_t zm516x_handle, uint16_t my_addr, uint16_t dst_addr);


#ifdef __cplusplus
}
#endif

#endif /* __APP_LED_CONTROL_H */

/* end of file */
