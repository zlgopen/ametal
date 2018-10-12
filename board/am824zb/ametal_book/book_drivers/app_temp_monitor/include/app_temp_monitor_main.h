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
 * \brief book`s hc595 driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  ipk, first implementation
 * \endinternal
 */

#ifndef __APP_TEMP_MONITOR_MAIN_H__
#define __APP_TEMP_MONITOR_MAIN_H__


#include "ametal.h"
#include "am_zlg9021.h"
#include "am_lpc82x_inst_init.h"

#define   APP_KEY_CODE_SET   	KEY_0        // SET键编码
#define   APP_KEY_CODE_ADD   	KEY_1      	 // 加1键编码
#define   APP_KEY_CODE_LR    	KEY_2        // L/R键编码
#define   APP_KEY_CODE_DEC   	KEY_3        // 减1键编码
#define   APP_LED0_ID        	0            // LED0 ID号
#define   APP_LED1_ID        	1            // LED1 ID号
#define   APP_DIGITRON_ID   	0            // 数码管显示器ID号


int app_temp_monitor_main (am_zm516x_handle_t zm516x_handle,am_temp_handle_t temp_handle);
                                                           
#endif /* __APP_TEMP_MONITOR_MAIN_H__ */

/* end of file */
