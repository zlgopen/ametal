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
 * \brief book`s app_mvc_bool_main driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */


#ifndef __APP_MVC_BOOL_MAIN_H__
#define __APP_MVC_BOOL_MAIN_H__

#pragma once
#include "ametal.h"
#include "am_led.h"
#include "am_input.h"
#include "am_zm516x.h"

//应用程序入口
int app_mvc_bool_main (int led_id, int key_code,am_zm516x_handle_t zm516x_handle);



#endif /* __APP_MVC_BOOL_MAIN_H__ */
/* end of file */


